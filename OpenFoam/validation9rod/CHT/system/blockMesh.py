import math

pointlist  = []
arclist    = []
hexblocks  = []
h = 876

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

def addarc(c,p1,p2):
   "adds arc to list if does not exist"
   addpoint(p1)
   addpoint(p2)
   r  = math.sqrt( (p1[0] - c[0]) ** 2 + (p1[1] - c[1]) ** 2  )
   m  = [ (p1[0] + p2[0]) / 2 - c[0], (p1[1] + p2[1]) / 2 - c[1], 0 ]
   root  = math.sqrt( m[0] ** 2 + m[1] ** 2 + m[2] ** 2)
   m  = [ m[0]/ root , m[1] / root, 0]
   p3 = [m[0]*r + c[0] , m[1]*r + c[1],0]
   a = [pointlist.index(p1),pointlist.index(p2), p3 ]
   if a not in arclist:arclist.append(a)
   return; 

def createHexBlock(p1,p2,p3,p4,filler):
	hex = [extrude(p1),extrude(p2),extrude(p3),extrude(p4),addpoint(p1),addpoint(p2),addpoint(p3),addpoint(p4),filler]
	if hex not in hexblocks: hexblocks.append(hex)
	return;


center = [0,0,0]
r = 3.175
R = 41.25
delta = 4
deltaO = 6
degree = 22.5
pepdis = 2*r*3.08

addpoint(center)

#points for the center cylinder
a2 = [center[0]+r*math.cos(math.pi*degree/180),center[1]+r*math.sin(math.pi*degree/180),0]
a1 = [a2[0],-1*a2[1],0]
addarc(center,a1,a2)

#points for the boundary layer center Cylinder
a5 = [center[0]+(r+delta)*math.cos(math.pi*degree/180),center[1]+(r+delta)*math.sin(math.pi*degree/180),0]
a4 = [(r+delta),0,0]
a3 = [a5[0],-1*a5[1],0]
addarc(center,a3,a4)
addarc(center,a4,a5)
addarc(center,a3,a4)
addarc(center,a3,a5)

#points for the outter wall
o3 = [center[0]+R*math.cos(math.pi*degree/180),center[1]+R*math.sin(math.pi*degree/180),0]
o2 = [R,0,0]
o1 = [o3[0],-1*o3[1],0]
addarc(center,o1,o3)

#points for the outter boundary wall
o6 = [center[0]+(R-deltaO)*math.cos(math.pi*degree/180),center[1]+(R-deltaO)*math.sin(math.pi*degree/180),0]
o5 = [R-deltaO, 0 , 0]
o4 = [o6[0],-1*o6[1],0]
addarc(center,o4,o6)

#points for the top peperonni
p1 = [center[0]+pepdis*math.cos(math.pi*degree/180), center[1]-pepdis*math.sin(math.pi*degree/180),0]
p2 = [p1[0] + r*math.cos(math.pi*degree/180),p1[1] - r*math.sin(math.pi*degree/180),0]
p3 = [p1[0] - r*math.cos(math.pi*degree/180),p1[1] + r*math.sin(math.pi*degree/180),0]
p4 = [p1[0] + r*math.sin(math.pi*degree/180),p1[1] + r*math.cos(math.pi*degree/180),0]
addarc(p1,p2,p4)
addarc(p1,p4,p3)

#points boundary layer top peperonni
p5 = [p1[0] + (r+delta)*math.cos(math.pi*degree/180),p1[1] - (r+delta)*math.sin(math.pi*degree/180),0]
p6 = [p1[0] - (r+delta)*math.cos(math.pi*degree/180),p1[1] + (r+delta)*math.sin(math.pi*degree/180),0]
p7 = [p1[0] + (r+delta)*math.sin(math.pi*degree/180),p1[1] + (r+delta)*math.cos(math.pi*degree/180),0]
addarc(p1,p5,p7)
addarc(p1,p7,p6)

#points for the diagonal peperonni
pepdisd =  math.sqrt( pepdis ** 2 + (pepdis / 2) ** 2 )
d1 = [center[0]+pepdisd*math.cos(math.pi*degree/180), center[1]+pepdisd*math.sin(math.pi*degree/180),0]
d2 = [d1[0] + r*math.cos(math.pi*degree/180),d1[1] + r*math.sin(math.pi*degree/180),0]
d3 = [d1[0] - r*math.cos(math.pi*degree/180),d1[1] - r*math.sin(math.pi*degree/180),0]
d4 = [d1[0] + r*math.sin(math.pi*degree/180),d1[1] - r*math.cos(math.pi*degree/180),0]
addarc(d1,d2,d4)
addarc(d1,d4,d3)

#points boundary layer top peperonni
d5 = [d1[0] + (r+delta)*math.cos(math.pi*degree/180),d1[1] + (r+delta)*math.sin(math.pi*degree/180),0]
d6 = [d1[0] - (r+delta)*math.cos(math.pi*degree/180),d1[1] - (r+delta)*math.sin(math.pi*degree/180),0]
d7 = [d1[0] + (r+delta)*math.sin(math.pi*degree/180),d1[1] - (r+delta)*math.cos(math.pi*degree/180),0]
addarc(d1,d6,d7)
addarc(d1,d7,d5)

boundary = "( 20  9 100)  simpleGrading ( 12 1 1)"
solid 	 = "( 3 9 100)  simpleGrading (0.02 1 1)"

createHexBlock(center,a1,a2,center,solid) # center cil
createHexBlock(a1,a3,a5,a2,boundary) 		# boundary wall for center cil
createHexBlock(o3,o6,o4,o1,boundary)			# bounder for outter wall
createHexBlock(p1,p4,p3,p1,solid)			# 1/2 cil top
createHexBlock(p1,p2,p4,p1,solid)			# 1/2 cil top
createHexBlock(p2,p5,p7,p4,boundary)			# boundary layer cil top
createHexBlock(p4,p7,p6,p3,boundary)			# boundary layer cil top
createHexBlock(d1,d4,d2,d1,solid)			# 1/2 cil diag
createHexBlock(d1,d3,d4,d1,solid)			# 1/2 cil diag
createHexBlock(d4,d7,d5,d2,boundary)			# boundary layer cil top
createHexBlock(d3,d6,d7,d4,boundary)			# boundary layer cil top

extrudearcs()
print ""
print ""
print "vertices"
print "("  
# Print points
for point in pointlist: 
   print "    (" + repr(point[0]) + " " + repr(point[1]) + " " + repr(point[2]) + ") //" + repr(pointlist.index(point)) + ""
print ");"
print ""
print "blocks"
print "("  
for hex in hexblocks : 
   print "  hex ( " + repr(hex[0]) + " " + repr(hex[1]) + " " + repr(hex[2]) + " " + repr(hex[3]) + " " + repr(hex[4]) + " " + repr(hex[5]) + " " + repr(hex[6]) + " " + repr(hex[7]) + " )  " + hex[8] + ""
print ");"
print ""
print "edges"
print "("  
for arc in arclist: 
   print "    arc " +  repr(arc[0]) + " " +  repr(arc[1]) + " (" + repr(arc[2][0]) + " " + repr(arc[2][1]) + " " + repr(arc[2][2]) + ")"
print ");"
print ""
