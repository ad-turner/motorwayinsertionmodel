"""
This program recieves the values provided by the C program
It also reads a csv file and turns it into a dictionnary for easy acces to the IAS values
It then successively plots I vs IBS, I vs IBS and IAS and then a smothened version of this last graph using gaussian_filter1d
Uncomment lines 41 49 and 57 to save the graphs as .png in the directory
"""

import sys
import matplotlib.pyplot as plt
import csv
import math
from scipy.ndimage import gaussian_filter1d

# Append to x, y ,z and r the values of ins, avgspeed, spac and freq outputed by the C model #
x=[]
y=[]
z=[]
r=[]
for s in sys.stdin:
    s=s[:-1]
    a, b, c, d = s.split(',')
    x.append(int(a))
    y.append(float(b))
    z.append(float(c))
    r.append(int(d))

# Create a dictionnary and write the values of ias_data.csv into it #
ias_dict = {}
with open('ias_data.csv', mode='r', newline='') as file:
    reader = csv.reader(file)
    for row in reader:
        u = int(row[0])
        v = row[1]
        ias_dict[u] = v

# Floor all values of z and put them in a new list #
z_floor = [math.floor(value) for value in z]

# Create a list of ias values corresponding one to one with the floored values of z #
ias = [float(ias_dict[i]) for i in z_floor]

# Plot I vs IBS #
plt.plot(x,y, label = "IBS")
plt.xlabel('Insertions per 10s')
plt.ylabel('Km per h')
plt.legend()
#plt.savefig('fig1.png')
plt.show()

# Plot I vs IBS and IAS #
plt.plot(x,y, label = "IBS")
plt.plot(x,ias, label = "IAS")
plt.xlabel('Insertions per 10s')
plt.ylabel('Km per h')
plt.legend()
#plt.savefig('fig2.png')
plt.show()

# Plot I vs IBS and IAS (smoothened) #
plt.plot(x,y, label = "IBS")
plt.plot(x,gaussian_filter1d(ias, sigma=1.5), label = "IAS")
plt.xlabel('Insertions per 10s')
plt.ylabel('Km per h')
plt.legend()
#plt.savefig('fig3.png')
plt.show()
