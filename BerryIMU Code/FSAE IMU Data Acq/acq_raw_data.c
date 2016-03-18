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

int myMicro();

int myMicro()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    //tv.sec is the time from 1970 and tv_usec is microseconds which according
    //to the api are not to accurate
    return (tv.tv_sec)*1000000 + (tv.tv_usec);
}

/*int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}*/

int main(int argc, char *argv[])
{
    int  accRaw[3];
    int  gyrRaw[3];

    int startInt  = myMicro();
    struct  timeval tvBegin, tvEnd,tvDiff;

    FILE *theFile;

    signal(SIGINT, INThandler);

    enableIMU();

    gettimeofday(&tvBegin, NULL);

    //get REAL TIME CLOCK info -> fopen a file with the date as its name with "a"
    theFile = fopen("dataFiles/testFile", "w");
    
    //startInt = myMicro();

    while(1)//run until the button is pressed
    {
        startInt = myMicro();

        //read ACC and GYR data
        readACC(accRaw);
        readGYR(gyrRaw);

        //writing accRaw[0], accRaw[1], accRaw[2]
        //then gyrRaw[0], gyrRaw[1], gyrRaw[2]
        //then the time difference
        fprintf(theFile, "%d %d %d %d %d %d %d\n", accRaw[0], accRaw[1], accRaw[2], gyrRaw[0], gyrRaw[1], gyrRaw[2], (myMicro() - startInt));

        //Each loop should be at least 20ms.
        while(myMicro() - startInt < (DT*100000))
        {
            printf("%s", "sleeping");
            fflush(stdout);
            usleep(100); //arg in microsec
        }

    }

    // fclose file that was opened
}

