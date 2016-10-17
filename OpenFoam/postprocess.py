#!/usr/bin/env python
import os
import math

###########################

K = ["Solving for Uz","Solving for p","DILUPBiCG:  Solving for h","DICPCG:  Solving for h"]
output = open("outputpp","w")

for bah in K:
	Y = []

	for line in open("log","r"):
		 if bah in line:
		 	Y.append(math.log10(float(line.split(' ')[8].replace(",", ""))))

	X  = range(len(Y))
	Xo = range(len(Y))
	Yo = range(len(Y))

	Xo[0] = X[0]
	Yo[0] = Y[0]

	output.write('\n \n' + bah + '\n \n')

	for x in range(0,len(Y)-1):
		if( abs( Yo[-1] - Y[x] ) > 0.2 ):
			Xo.append(X[x])
			Yo.append(Y[x])
			output.write( "(" + str(Xo[-1]) + "," + format(Yo[-1], '.2f') + ")" )

	output.write( "(" + repr(X[-1]) + "," + format(Y[-1], '.2f') + ")" )


K2 = ["max","min","avg"]

for b in range(0,len(K2)):

	Y = []

	for line in open("log","r"):
		 if "T   max/avg/min :" in line:
		 	Y.append(float(line.split(' ')[b+7].replace(",", "")))

	X  = range(len(Y))
	Xo = range(len(Y))
	Yo = range(len(Y))

	Xo[0] = X[0]
	Yo[0] = Y[0]

	output.write('\n \n' + "T " + ' ' +  K2[b]  + '\n \n')

	for x in range(0,len(Y)-1):
		if( abs( Yo[-1] - Y[x] ) > 0.1 ):
			Xo.append(X[x])
			Yo.append(Y[x])
			output.write( "(" + str(Xo[-1]) + "," + format(Yo[-1], '.2f') + ")" )

	output.write( "(" + repr(X[-1]) + "," + format(Y[-1], '.2f') + ")" )


for b in range(0,len(K2)):

	Y = []

	for line in open("log","r"):
		 if "U   max/avg/min" in line:
		 	Y.append(float(line.split(' ')[b+7].replace(",", "")))

	X  = range(len(Y))
	Xo = range(len(Y))
	Yo = range(len(Y))

	Xo[0] = X[0]
	Yo[0] = Y[0]

	output.write('\n \n' + "U " + ' ' + K2[b]  + '\n \n')

	for x in range(0,len(Y)-1):
		if( abs( Yo[-1] - Y[x] ) > 0.01 ):
			Xo.append(X[x])
			Yo.append(Y[x])
			output.write( "(" + str(Xo[-1]) + "," + format(Yo[-1], '.2f') + ")" )

	output.write( "(" + repr(X[-1]) + "," + format(Y[-1], '.2f') + ")" )


for b in range(0,len(K2)):

	Y = []

	for line in open("log","r"):
		 if "Prg max/avg/min :" in line:
		 	Y.append(float(line.split(' ')[b+5].replace(",", "")))

	X  = range(len(Y))
	Xo = range(len(Y))
	Yo = range(len(Y))

	Xo[0] = X[0]
	Yo[0] = Y[0]

	output.write('\n \n' + "Prg " + ' ' +  K2[b] + '\n \n')

	for x in range(0,len(Y)-1):
		if( abs( Yo[-1] - Y[x] ) > 0.01 ):
			Xo.append(X[x])
			Yo.append(Y[x])
			output.write( "(" + str(Xo[-1]) + "," + format(Yo[-1], '.2f') + ")" )

	output.write( "(" + repr(X[-1]) + "," + format(Y[-1], '.2f') + ")" )


Y = []

for line in open("log","r"):
	 if "EnergyBalance" in line:
	 	Y.append(float(line.split(' ')[3].replace(",", ""))*72)

X  = range(len(Y))
Xo = range(len(Y))
Yo = range(len(Y))

Xo[0] = X[0]
Yo[0] = Y[0]

output.write('\n \n' + "PEnergyBalance " + '\n \n')

for x in range(0,len(Y)-1):
	if( abs( Yo[-1] - Y[x] ) > 0.03 ):
		Xo.append(X[x])
		Yo.append(Y[x])
		output.write( "(" + str(Xo[-1]) + "," + format(Yo[-1], '.2f') + ")" )

output.write( "(" + repr(X[-1]) + "," + format(Y[-1], '.2f') + ")" )


Y = []

for line in open("log","r"):
	 if "fluid Outflow" in line:
	 	Y.append(float(line.split(' ')[7].replace(",", ""))*72*1000)

X  = range(len(Y))
Xo = range(len(Y))
Yo = range(len(Y))

Xo[0] = X[0]
Yo[0] = Y[0]

output.write('\n \n' + "fluid Outflow" + ' ' +  '\n \n')

for x in range(0,len(Y)-1):
	if( abs( Yo[-1] - Y[x] ) > 0.1 ):
		Xo.append(X[x])
		Yo.append(Y[x])
		output.write( "(" + str(Xo[-1]) + "," + format(Yo[-1], '.2f') + ")" )

output.write( "(" + repr(X[-1]) + "," + format(Y[-1], '.2f') + ")" )