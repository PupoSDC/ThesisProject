#!/usr/bin/env python
import os
import math
blockMeshDict = open("system/blockMeshDict","w")

pointlist  = []
arclist    = []
hexblocks  = []
boundlist  = []
h = 400
inf = 0.000000000001

def addpoint(p):
   "adds point to list if does not exist"
   if p not in pointlist: pointlist.append(p)
   return pointlist.index(p);

def extrude(p):  
   return addpoint([p[0],p[1],-h]);

def extrudearcs():
	provlist = []
	for arc in arclist:	
		provlist.append( [extrude(pointlist[arc[0]]) , extrude(pointlist[arc[1]]) , [ arc[2][0],arc[2][1], -h ] ])
	arclist.extend(provlist)
	return;

def getmidpoint(c,p1,p2):
   r  = math.sqrt( (p1[0] - c[0]) ** 2 + (p1[1] - c[1]) ** 2  )
   m  = [ (p1[0] + p2[0]) / 2 - c[0], (p1[1] + p2[1]) / 2 - c[1], 0 ]
   root  = math.sqrt( m[0] ** 2 + m[1] ** 2 + m[2] ** 2)
   m  = [ m[0]/ root , m[1] / root, 0]
   return [m[0]*r + c[0] , m[1]*r + c[1],0]; 

def addarc(p1,p2,p3):
   P1n = addpoint(p1)
   P2n = addpoint(p2)
   if [P1n,P2n,p3] not in arclist:arclist.append([P1n,P2n,p3])
   return;

def createHexBlock(p1,p2,p3,p4,filler):
	hex = [extrude(p1),extrude(p2),extrude(p3),extrude(p4),addpoint(p1),addpoint(p2),addpoint(p3),addpoint(p4),filler]
	if hex not in hexblocks: hexblocks.append(hex)
	return;
def createboundary(name,faces,type):
	boundlist.append([name,faces,type]) 
	return boundlist.index([name,faces,type]);

def inverseboundary(name,bound):
	faces = []
	for b in bound[1]:
		faces.append([extrude(pointlist[b[3]]),extrude(pointlist[b[2]]),extrude(pointlist[b[1]]),extrude(pointlist[b[0]])])
	createboundary(name,faces,bound[2])
	return;

c0 = [0,0,0]
r = 3.175
R = 41.25
delta = 3
deltaO = 6
degree = 22.5
pepdis = 2*r*3.08

#points for the center cylinder
a2 = [c0[0]+r*math.cos(math.pi*degree/180),c0[1]+r*math.sin(math.pi*degree/180),0]
a1 = [a2[0],-1*a2[1],0]
a4 = [c0[0]+(r+delta)*math.cos(math.pi*degree/180),c0[1]+(r+delta)*math.sin(math.pi*degree/180),0]
a3 = [a4[0],-1*a4[1],0]
a5 = getmidpoint(c0,a3,a4)

#points for the top peperonni
p0 = [c0[0]+pepdis*math.cos(math.pi*degree/180), c0[1]-pepdis*math.sin(math.pi*degree/180),0]
p1 = [p0[0]-r*math.cos(math.pi*degree/180), p0[1]+r*math.sin(math.pi*degree/180),0]
p2 = [p0[0]+r*math.cos(math.pi*degree/180), p0[1]-r*math.sin(math.pi*degree/180),0]
p3 = [p0[0]+r*math.sin(math.pi*degree/180), p0[1]+r*math.cos(math.pi*degree/180),0]
p4 = [p0[0]-(r+delta)*math.cos(math.pi*degree/180), p0[1]+(r+delta)*math.sin(math.pi*degree/180),0]
p5 = [p0[0]+(r+delta)*math.cos(math.pi*degree/180), p0[1]-(r+delta)*math.sin(math.pi*degree/180),0]
p6 = [p0[0]+(r+delta-inf)*math.sin(math.pi*degree/180), p0[1]+(r+delta-inf)*math.cos(math.pi*degree/180),0]
p7 = getmidpoint(p0,p4,p6)
p8 = getmidpoint(p0,p6,p5)
p9 = [p0[0]-(r+delta+3)*math.cos(math.pi*degree/180), p0[1]+(r+delta+3)*math.sin(math.pi*degree/180),0]
pA = [c0[0]+pepdis-(r+delta+3),c0[1],0]
pB = [c0[0]+pepdis+(r+delta+3),c0[1],0]
pC = [p0[0]+(r+delta+3)*math.cos(math.pi*degree/180), p0[1]-(r+delta+3)*math.sin(math.pi*degree/180),0]
pD = [p9[0],p9[1]*-1,0]
pE = [pC[0],pC[1]*-1,0]

#points for the outter wall
o3 = [c0[0]+R*math.cos(math.pi*degree/180),c0[1]+R*math.sin(math.pi*degree/180),0]
o2 = [R,0,0]
o1 = [o3[0],-1*o3[1],0]

createHexBlock(a1,a3,a4,a2,"(10  4 40)  simpleGrading (12  1 1)") # boundary layer cil center

createHexBlock(p2,p5,p4,p1,"(10 16 40)  simpleGrading (12  1 1)") # boundary layer cil top
createHexBlock(p9,p4,p7,pA,"( 6  4 40)  simpleGrading ( 1  1 1)")
createHexBlock(p8,pB,pA,p7,"( 6  8 40)  simpleGrading ( 1  1 1)")
createHexBlock(p5,pC,pB,p8,"( 6  4 40)  simpleGrading ( 1  1 1)")

createHexBlock(a3,p9,pA,a5,"( 6  4 40)  simpleGrading ( 1  1 1)") # connect cil center to cil top

createHexBlock(pC,o1,o2,pB,"(10  4 40)  simpleGrading ( 1  1 1)") # connect cil top to outter wall

createHexBlock(a5,pA,pD,a4,"( 6 4 40)  simpleGrading ( 1  1 1)")
createHexBlock(pA,pB,pE,pD,"( 8 4 40)  simpleGrading ( 1  1 1)")
createHexBlock(pB,o2,o3,pE,"(10 4 40)  simpleGrading ( 1  1 1)")

addarc(a1,a2,getmidpoint(c0,a1,a2))
addarc(a3,a4,a5)
addarc(a3,a5,getmidpoint(c0,a3,a5))
addarc(a5,a4,getmidpoint(c0,a5,a4))

addarc(p1,p2,p3)					#Top Boundary layer arc
addarc(p4,p5,p6)					#Top Boundary layer out arc
addarc(p5,p8,getmidpoint(p0,p5,p8)) #
addarc(pC,pB,getmidpoint(c0,pC,pB))
addarc(p4,p7,getmidpoint(p0,p4,p7))
addarc(pA,p9,getmidpoint(c0,pA,p9))
addarc(p7,p8,p6)
addarc(pA,pD,getmidpoint(c0,pA,pD))
addarc(pB,pE,getmidpoint(c0,pB,pE))

addarc(o1,o2,getmidpoint(c0,o1,o2))
addarc(o2,o3,getmidpoint(c0,o2,o3))


'''
b = createboundary("Outlet",[
	[addpoint(a2),addpoint(a1),addpoint(a3),addpoint(a5)],
	[addpoint(p7),addpoint(p5),addpoint(d5),addpoint(d7)],
	[addpoint(p6),addpoint(p7),addpoint(d7),addpoint(d6)],
	[addpoint(a3),addpoint(p6),addpoint(d6),addpoint(a5)],
	[addpoint(p5),addpoint(o4),addpoint(o6),addpoint(d5)],
	[addpoint(p4),addpoint(p2),addpoint(p5),addpoint(p7)],
	[addpoint(p6),addpoint(p3),addpoint(p4),addpoint(p7)],
	[addpoint(d6),addpoint(d7),addpoint(d4),addpoint(d3)],
	[addpoint(d7),addpoint(d5),addpoint(d2),addpoint(d4)],
	[addpoint(o4),addpoint(o1),addpoint(o3),addpoint(o6)]],"patch")

inverseboundary("Inlet",boundlist[b])

createboundary("Outterwall",[[addpoint(o1),addpoint(o3),extrude(o3),extrude(o1)]],"patch");


createboundary("symmetryleft1", [
	[addpoint(a3),addpoint(a1),extrude(a1),extrude(a3)],
	[addpoint(p6),addpoint(a3),extrude(a3),extrude(p6)],
	[addpoint(p3),addpoint(p6),extrude(p6),extrude(p3)]],"symmetry" )

createboundary("symmetryleft2", [
	[addpoint(p5),addpoint(p2),extrude(p2),extrude(p5)],
	[addpoint(o4),addpoint(p5),extrude(p5),extrude(o4)],
	[addpoint(o1),addpoint(o4),extrude(o4),extrude(o1)]],"symmetry" )

createboundary("symmetryright1", [
	[addpoint(a2),addpoint(a5),extrude(a5),extrude(a2)],
	[addpoint(a5),addpoint(d6),extrude(d6),extrude(a5)],
	[addpoint(d6),addpoint(d3),extrude(d3),extrude(d6)]],"symmetry" )

createboundary("symmetryright2", [
	[addpoint(d2),addpoint(d5),extrude(d5),extrude(d2)],
	[addpoint(d5),addpoint(o6),extrude(o6),extrude(d5)],
	[addpoint(o6),addpoint(o3),extrude(o3),extrude(o6)]],"symmetry" )


# For jsut the fluid scenario!!
createboundary("cilcenterwallheated", [
	[addpoint(a1),addpoint(a2),extrude(a2),extrude(a1)]],"patch")

createboundary("ciltopwallheated", [
	[addpoint(p4),addpoint(p2),extrude(p2),extrude(p4)],
	[addpoint(p3),addpoint(p4),extrude(p4),extrude(p3)]],"patch")

'''


#extrudearcs()
blockMeshDict.write("FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       dictionary;\n    object      blockMeshDict;\n}\n")
blockMeshDict.write( "\nvertices\n(\n")
for point in pointlist: 
   blockMeshDict.write( "    (" + repr(point[0]) + " " + repr(point[1]) + " " + repr(point[2]) + ") //" + repr(pointlist.index(point)) + "\n")
blockMeshDict.write( ");\n")
blockMeshDict.write( "\nblocks\n(\n")
for hex in hexblocks : 
   blockMeshDict.write( "  hex ( " + repr(hex[0]) + " " + repr(hex[1]) + " " + repr(hex[2]) + " " + repr(hex[3]) + " " + repr(hex[4]) + " " + repr(hex[5]) + " " + repr(hex[6]) + " " + repr(hex[7]) + " )  " + hex[8] + "\n")
blockMeshDict.write( ");\n")
blockMeshDict.write( "\nedges\n(\n") 
for arc in arclist: 
   blockMeshDict.write( "    arc " +  repr(arc[0]) + " " +  repr(arc[1]) + " (" + repr(arc[2][0]) + " " + repr(arc[2][1]) + " " + repr(arc[2][2]) + ")\n")
blockMeshDict.write( ");\n")
blockMeshDict.write( "\nboundary\n(\n") 
for b in boundlist:
	q = "" 
	for f in b[1]:
		q = q + "(" + repr(f[0]) + " " + repr(f[1]) + " " + repr(f[2]) + " " + repr(f[3]) + ")"
   	blockMeshDict.write( "	" + b[0] + " { type " + b[2] + "; faces (" + q  +");}\n")
blockMeshDict.write( ");\n")
blockMeshDict.write( "\nmergePatchPairs();")

blockMeshDict.close()

import os
os.system("blockMesh")
