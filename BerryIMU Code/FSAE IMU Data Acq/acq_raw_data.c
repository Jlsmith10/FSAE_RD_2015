#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "sensor.c"


#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant

#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846


void INThandler(int sig)
{
    signal(sig, SIG_IGN);
    exit(0);
}

int myMillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    //tv.sec is the time from 1970 and tv_usec is microseconds which according
    //to the api are not to accurate
    return (tv.tv_sec)*1000 + (tv.tv_usec)/1000;
}

int main(int argc, char *argv[])
{
    int  accRaw[3];
    int  gyrRaw[3];

    struct timeval tvBegin, tvEnd, tvDiff;

    FILE *theFile;

    signal(SIGINT, INThandler);

    enableIMU();

    gettimeofday(&tvBegin, NULL);

    time_t fileNameTimer = time(NULL);
    struct tm tm = *localtime(&fileNameTimer);
    
    char buf[28];//worst case date
    sprintf(buf, "dataFiles/");
    sprintf(buf, "%s%d-", buf, tm.tm_mon+1);
    sprintf(buf, "%s%d-", buf, tm.tm_mday);
    sprintf(buf, "%s%d-", buf, tm.tm_year+1900);
    sprintf(buf, "%s%d:", buf, tm.tm_hour);
    sprintf(buf, "%s%d:", buf, tm.tm_min);
    sprintf(buf, "%s%d", buf, tm.tm_sec);
    sprintf(buf, "%s.txt", buf);

    theFile = fopen(buf, "w");
    
    int loopStartInt  = myMillis();
    int beginInt = myMillis();

    while(1)//run until the button is pressed
    {
        loopStartInt = myMillis();

        //read ACC and GYR data
        readACC(accRaw);
        readGYR(gyrRaw);

        //Each loop should be at least 20ms - imu can only read at this sampling
        //rate apparently
        while(myMillis() - loopStartInt < (DT*1000))
        {
            usleep(100); //arg in microsec
        }

        //writing accRaw[0], accRaw[1], accRaw[2]
        //then gyrRaw[0], gyrRaw[1], gyrRaw[2]
        //then the time difference from before the while loop in milliseconds
        fprintf(theFile, "%d %d %d %d %d %d %d\n", accRaw[0], accRaw[1], accRaw[2], gyrRaw[0], gyrRaw[1], gyrRaw[2], (myMillis() - beginInt));

    }

    // fclose file that was opened
    return fclose(theFile);
}

