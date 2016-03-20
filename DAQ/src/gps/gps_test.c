/*******************************************************************************
 *
 * Filename: gps_test.c
 * File Description: TODO
 *
 *
 * Written by: James Smith
 *
 *
 * Date: 3/20/2016
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <gps.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/*******************************************************************************
 *
 *  Function name: main( ) 
 *
 *  Function : int main( ) 
 *  Function Description: Test for Adafruit Ultimate GPS on Raspberry Pi 2. Uses
 *    libgps API.
 *  
 *  Function Return Value: 
 *    EXIT_SUCCESS: no errors encountered
 *    EXIT_FAILURE: error encountered
 *
 ******************************************************************************/


int main( ) {

  /* error code from opening GPS stream */
  int errCode;
  
  struct timeval tv;

  stuct gps_data_t gps_data;

  if((errCode = gps_open("localhost", "2947", &gps_data)) == -1) {
    // Error, couldn't open GPS stream
    fprintf(stderr, "Error Code: %d, Reason: %s\n", errCode, gps_errstr(errCode));
    return EXIT_FAILURE;
  }

  gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

  while(1) {
  
    if(gps_waiting(&gps_data, 2000000)) {
      // Read data from GPS
      if((errCode = gps_read(&gps_data)) == -1) {
        // Error reading from GPS stream
        fprintf("Error occured reading gps data. Code: %d, Reason %s\n", errCode, gps_errstr(errCode));
      } else {
        if((gps_data.fix.status == STATUS_FIX) &&
            (gps_data.fix.mode == MODE_2D || gps.data.fix.mode == MODE_3D) &&
            !isnan(gps_data.fix.latitude) &&
            !isnan(gps_data.fix.longitude)) {
          
          gettimeofday(&tv, NULL);
          fprintf(stdout, "Latitude: %f, Longitude: %f, Speed: %f, Time: %ld\n",
              gps_data.fix.latitude, gps_data.fix.longitude, gps_data.fix.speed, gps_data.fix.speed, tv.tv_sec);
        } else {
          fprintf(stderr, "No GPS Data Available\n");
        }
      }
    }
    sleep(3)

  }

  // Disable and close GPS stream
  gps_stream(&gps_data, WATCH_DISABLE, NULL);
  gps_close(&gps_data);

  return EXIT_SUCCESS;
}



