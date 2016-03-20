#########################################
# Name: Vansh Singh
# File: gpstest.py
# Date: 12/30/15
# Purpose: to output gps time data 
#########################################

import gps

session = gps.gps("localhost", "2947")
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)

while True:
    try:
        report = session.next()
        # to print full report data uncomment line below
        # print report

        if report['class'] == 'TPV':
            if hasattr(report, 'time'):
                print report.time

    except KeyError:
        pass
    except KeyboardInterrupt:
        quit()
    except StopIteration:
        session = None
        print "GPSD has terminated"

