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

    float rate_gyr_x = 0.0;
    float rate_gyr_y = 0.0;
    float rate_gyr_z = 0.0;

    float gyroXangle = 0.0;
    float gyroYangle = 0.0;
    float gyroZangle = 0.0;
    float AccYangle = 0.0;
    float AccXangle = 0.0;
    float AccZangle = 0.0;
    float CFangleX = 0.0;
    float CFangleY = 0.0;

    float CFangleX_bias = 0.0;
    float CFangleY_bias = 0.0;
    float CFangleX_biasAverage = 0.0;
    float CFangleY_biasAverage = 0.0;
    int biasLoopCount = 0;

    FILE *theFile;


    struct timeval tvBegin, tvEnd, tvDiff;
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

        //convert to degrees/second
        rate_gyr_x = (float) gyrRaw[0] * G_GAIN;
        rate_gyr_y = (float) gyrRaw[1]  * G_GAIN;
        rate_gyr_z = (float) gyrRaw[2]  * G_GAIN;

        //Calculate the angles from the gyro
        gyroXangle+=rate_gyr_x*DT;
        gyroYangle+=rate_gyr_y*DT;
        gyroZangle+=rate_gyr_z*DT;

        //Convert Accelerometer values to degrees
        AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
        AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;

        //Change the rotation value of the accelerometer to -/+ 180 and move the
        //Y axis '0' point to up. (Code depends on how IMU is mounted)
        //If IMU is upside down
        
        if (AccXangle >180)
        {
           AccXangle -= (float)360.0;
        }

        AccYangle-=90;
        if (AccYangle >180)
        {
           AccYangle -= (float)360.0;
        }

        //If IMU is up the correct way, use these lines
        /*
        AccXangle -= (float)180.0;
        if (AccYangle > 90)
        {
            AccYangle -= (float)270;
        }
        else
        {
            AccYangle += (float)90;
        }
        */

        //Complementary filter used to combine the accelerometer and gyro values.
        CFangleX = AA * (CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle;
        CFangleY = AA * (CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle;

        biasLoopCount++;
        if (biasLoopCount >= 400 && biasLoopCount < 600)
        {
            CFangleX_bias += CFangleX;
            CFangleY_bias += CFangleY;
        }
        else if (biasLoopCount == 601)
        {
            CFangleX_biasAverage = CFangleX_bias/(200);
            CFangleY_biasAverage = CFangleY_bias/(200);
        }


        //Each loop should be at least 20ms - imu can only read at this rate
        while(myMillis() - loopStartInt < (DT*1000))
        {
            usleep(100); //arg in microsec
        }

        //gyroXangle, gyroYangle, AccXangle, AccYangle, real CFangleX, real CFangleY,
        //(time from beginning)
        fprintf(theFile, "%7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %d\n",
            gyroXangle, gyroYangle, AccXangle, AccYangle,
            CFangleX - CFangleX_biasAverage, CFangleY - CFangleY_biasAverage,
            (myMillis() - beginInt));
        //printf("%d: %7.3f %7.3f\n", biasLoopCount, CFangleX - CFangleX_biasAverage, CFangleY - CFangleY_biasAverage);

        
}

    // fclose file that was opened
    return fclose(theFile);
}

