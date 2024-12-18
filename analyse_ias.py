"""
This program writes a csv file containing the IAS values provided by the C program
"""

import sys
import csv

# Append to x, y ,z and t the values of ins, avgspeed, spac and freq outputed by the C model #
x=[]
y=[]
z=[]
t=[]
for s in sys.stdin:
    s=s[:-1]
    a, b, c, d = s.split(',')
    x.append(int(a))
    y.append(float(b))
    z.append(float(c))
    t.append(int(d))

# Write in pairs values of freq (t) and avgspeed (y) to a csv file #
with open('ias_data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    for u, v in zip(t, y):
        writer.writerow([u, v])
