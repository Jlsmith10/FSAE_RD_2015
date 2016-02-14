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

int mymillis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}

int main(int argc, char *argv[])
{
    int  accRaw[3];
    int  gyrRaw[3];

    int startInt  = mymillis();
    struct  timeval tvBegin, tvEnd,tvDiff;

    FILE *theFile;

    signal(SIGINT, INThandler);

    enableIMU();

    gettimeofday(&tvBegin, NULL);

    //get real time clock info -> fopen a file with the date as its name with "a"
    theFile = fopen("testFile", "a");
    
    //startInt = mymillis();

    while(1)//run until the button is pressed
    {
        startInt = mymillis();


        //read ACC and GYR data
        readACC(accRaw);
        readGYR(gyrRaw);

        //binary data printed
        fwrite(accRaw, sizeof(int), 3, theFile);
        fwrite(gyrRaw, sizeof(int), 3, theFile)
        //write the time????


        //Each loop should be at least 20ms.
        while(mymillis() - startInt < (DT*1000))
        {
            usleep(100); //arg in microsec
        }

        printf("Loop Time %d\t", mymillis()- startInt);
    }

    // fclose file that was opened
}

