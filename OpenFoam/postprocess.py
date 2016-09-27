#!/usr/bin/env python
import os
import math

###########################

Y = []

for line in open("264/log2","r"):
        Y.append(math.log10(float(line)))


X  = range(len(Y))
Xo = range(len(Y))
Yo = range(len(Y))

Xo[0] = X[0]
Yo[0] = Y[0]

blockMeshDict = open("outputpp","w")

for x in range(0,3699):
	if( abs( Yo[-1] - Y[x] ) > 0.1 ):
		Xo.append(X[x])
		Yo.append(Y[x])
		blockMeshDict.write( "(" + repr(Xo[-1]) + "," + repr(Yo[-1]) + ")" )

blockMeshDict.write( "(" + repr(X[3700]) + "," + repr(Y[3700]) + ")" )