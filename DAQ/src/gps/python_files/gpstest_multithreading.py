#! /usr/bin/python
# Written by Dan Mandle http://dan.mandle.me Septmember 2012
# License: GPL 2.0


# Not currently working

import os
from gps import *
from time import *
import time
import threading

gpsd = None #global variable

class GpsPoller(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        global gpsd #bring it in scope
        gpsd = gps(mode=WATCH_ENABLE) # starting the stream of info
        self.current_value = None
        self.running = True #setting the thread running to true

    def run(self):
        global gpsd #bring it in scope
        while gpsd.running:
            gpsd.next() #this will continue to loop and grab EACH set of gpsd 
                        #info to clear the buffer

if __name__ == '__main__':
    gpsp= GpsPoller() #create the thread
    try:
        gpsp.start() #start it up
        while True:
            #It may take a second or two to get good data
            #print gpsd.fix.latitude, ', ', gpsd.fix.longitude,' Time: ',
            #gpsd.utc

            os.system('clear')

            print
            print " GPS reading"
            print "---------------------------------------"
            print "latitude     " , gpsd.fix.latitude
            print "longitude    " , gpsd.fix.longitude
            print "time utc     " , gpsd.utc,'+', gpsd.fix.time
            print "altitude (m)  " , gpsd.fix.altitude
            print "eps          " , gpsd.fix.eps
            print "epx          " , gpsd.fix.epx
            print "epv          " , gpsd.fix.epv
            print "ept          " , gpsd.fix.ept
            print "speed (m/s)  " , gpsd.fix.speed
            print "climb        " , gpsd.fix.climb
            print "track        " , gpsd.fix.track
            print "mode         " , gpsd.fix.mode
            print 
            print "sats         " , gpsd.satellites

            time.sleep(.25) #set to whatever

    except (KeyboardInterrupt, SystemExit): #when you press ctrl+c
        print "\nKilling Thread..."
        gpsp.running = False
        gpsp.join() # wait for the thread to finish the loop
        
    print "Done.\nExiting."
    