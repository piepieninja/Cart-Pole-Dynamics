import matplotlib.pyplot as plt
import numpy as np
import math
import csv

euler_t = []
euler_a = []
euler_x = []

mod_euler_t = []
mod_euler_a = []
mod_euler_x = []

# max_t = 1000
count = 0

with open('euler.csv', 'r') as csvFile:
    reader = csv.reader(csvFile)
    for row in reader:
        euler_t.append(float(row[0]))
        euler_a.append(float(row[1]))
        euler_x.append(float(row[2]))

count = 0
with open('modified_euler.csv', 'r') as csvFile:
    reader = csv.reader(csvFile)
    for row in reader:
        mod_euler_t.append(float(row[0]))
        mod_euler_a.append(float(row[1]))
        mod_euler_x.append(float(row[2]))

comp_thing_x = []
for i in range(0, len(euler_t)):
    comp_thing_x.append(abs(euler_x[i] - mod_euler_x[i]))

plt.plot(euler_t, euler_x, 'r')
plt.plot(mod_euler_t, mod_euler_x, 'g')
# plt.plot(euler_t,comp_thing_x, 'r')
plt.show()
# yee
