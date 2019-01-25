# -*- coding: utf-8 -*-

import math
import numpy as np
import matplotlib.pyplot as plt

# reading data
#p2 = np.genfromtxt("r_ankle.csv",delimiter="\t")
#p2 = np.genfromtxt("l_ankle.csv",delimiter="\t")
p2 = np.genfromtxt("r_knee.csv",delimiter="\t")
#p2 = np.genfromtxt("l_knee.csv",delimiter="\t")

# axis setting
yy,zz = [],[]
y = p2[0,:]
z = p2[:,0]
y = y[1:]
z = z[1:]

for num in range (len(y)):
    yy.append(y)
for num in range (len(z)):
    zz.append(z)
Y = np.array(yy)
Z = np.array(zz).T

# make multiarray data
p2 = np.delete(p2,0,1)
p2 = np.delete(p2,0,0)

# plot
#plt.contourf(Y,Z,p2,100)
#plt.contourf(Y,Z,p2,100,levels=[-15,-10,-5,0,5,10,15,20]) # ankle
plt.contourf(Y,Z,p2,100,levels=[0,5,10,15,20,25,30,35,40]) # knee
plt.xlabel('front muscle pressure[kPa]')
plt.ylabel('back muscle pressure[kPa]')
pp = plt.colorbar()
pp.set_label("Angle[deg]")
#plt.show()
#plt.savefig("r_ankle_5deg.png")
#plt.savefig("l_ankle_5deg.png")
plt.savefig("r_knee_5deg.png")
#plt.savefig("l_knee_5deg.png")
