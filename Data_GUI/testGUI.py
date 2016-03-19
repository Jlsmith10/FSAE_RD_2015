import numpy as np
from numpy import *
import sys
import matplotlib.pyplot as plt

my_data = genfromtxt(sys.argv[1])


plt.title('Acceleration & Gyro Raw')

plt.subplot(2,3,1)
#second arg incorrect because loop time currently measured in between data points
plt.plot(my_data[:,6], my_data[:,0])
plt.xlabel('Time (microsecond)')
plt.ylabel('Raw Acc_X')

plt.subplot(2,3,2)
#second arg incorrect because loop time currently measured in between data points
plt.plot(my_data[:,6], my_data[:,1])
plt.xlabel('Time (microsecond)')
plt.ylabel('Raw Acc_Y')

plt.subplot(2,3,3)
#second arg incorrect because loop time currently measured in between data points
plt.plot(my_data[:,6], my_data[:,2])
plt.xlabel('Time (microsecond)')
plt.ylabel('Raw Acc_Z')

plt.subplot(2,3,4)
#second arg incorrect because loop time currently measured in between data points
plt.plot(my_data[:,6], my_data[:,3])
plt.xlabel('Time (microsecond)')
plt.ylabel('Raw Gyro_X')

plt.subplot(2,3,5)
#second arg incorrect because loop time currently measured in between data points
plt.plot(my_data[:,6], my_data[:,4])
plt.xlabel('Time (microsecond)')
plt.ylabel('Raw Gyro_Y')

plt.subplot(2,3,6)
#second arg incorrect because loop time currently measured in between data points
plt.plot(my_data[:,6], my_data[:,5])
plt.xlabel('Time (microsecond)')
plt.ylabel('Raw Gyro_Z')



#plt.plot(my_data)
plt.show()
