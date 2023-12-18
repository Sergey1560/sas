#!/usr/bin/env python3

import sys
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime as dt
import csv
import matplotlib.ticker

from matplotlib.ticker import (MultipleLocator, FormatStrFormatter,AutoMinorLocator)

#Для файла 1310ms.csv
#angle_offset = 263.2 + 180
#Для файла 2620ms.csv
angle_offset = 265.9 + 180


if len(sys.argv) < 2:
    print("No filename argument")
    exit()

angle = []
speed = []
time =  []

idx = 0

with open(sys.argv[1], newline = '') as f:
    for row in csv.reader(f, delimiter = ';', quotechar = '"'):
        angle.append(float(row[0]) + angle_offset)
        speed.append(float(row[1]))        
        time.append(idx*10)
        idx = idx+1
        
dpi = 80
fig = plt.figure(dpi = dpi, figsize = (1024 / dpi, 768 / dpi) )
mpl.rcParams.update({'font.size': 10})


plt.title(sys.argv[1])
plt.xlabel("Count")
plt.ylabel("Val")

ax = plt.gca()
ax.yaxis.grid(True)
ax.xaxis.grid(True)

ax.xaxis.set_minor_locator(AutoMinorLocator())
ax.yaxis.set_minor_locator(AutoMinorLocator())
ax.tick_params(which='major', length=10, width=2)
ax.tick_params(which='minor', length=5, width=1)

locatory = matplotlib.ticker.MultipleLocator (base=0.1)
locatorx = matplotlib.ticker.MultipleLocator (base=1)

plt.plot(time, angle,label = 'Angle')  
plt.plot(time, speed,label = 'Speed')  

plt.legend(loc = 'upper right')

fig.savefig('graph.png')
