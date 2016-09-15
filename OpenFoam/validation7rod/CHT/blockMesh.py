#!/usr/bin/env python
import os
import math

#######################################################################
## FUNCTION DEFINITIONS ###############################################
#######################################################################

def addpoint(p,z):
   if [p[0],p[1],z] not in pointlist: pointlist.append([p[0],p[1],z])
   return pointlist.index([p[0],p[1],z]);

def addarc(arc,z):
   arc = [addpoint(arc[0],z),addpoint(arc[1],z),[arc[2][0],arc[2][1],z]]
   if arc not in arclist: arclist.append(arc)
   return arclist.index(arc);

def getmidpoint(c,p1,p2):
   r     = math.sqrt( (p1[0]-c[0])**2 + (p1[1]-c[1])**2  )
   m     = [ (p1[0] + p2[0]) / 2 - c[0], (p1[1] + p2[1]) / 2 - c[1] ]
   root  = math.sqrt( m[0]**2 + m[1]**2 )
   m     = [ m[0]/ root    , m[1] / root]
   return  [ m[0]*r + c[0] , m[1]*r + c[1] ]; 

def createHexBlock(p1,p2,p3,p4,top,bot,points,grade):
    hexblocks.append([
        addpoint(p1,bot),addpoint(p2,bot),addpoint(p3,bot),addpoint(p4,bot),
        addpoint(p1,top),addpoint(p2,top),addpoint(p3,top),addpoint(p4,top),
        points,grade])
    return;

def createboundary(name,faces,h,type):
    listoffaces = []
    for f in faces:
        if len(f) == 2:
            for x in xrange(0,len(h)-1):
                listoffaces.append([addpoint(f[0],h[x]),addpoint(f[1],h[x]),addpoint(f[1],h[x+1]),addpoint(f[0],h[x+1])])
        elif len(f) == 4:
            listoffaces.append([addpoint(f[0],h),addpoint(f[1],h),addpoint(f[2],h),addpoint(f[3],h) ])
    boundlist.append([name,listoffaces,type])
    return [name,faces,type];

def writeblockmeshdict(pointlist,hexblocks,arclist,boundlist,mergepatches):
    blockMeshDict = open("system/blockMeshDict","w")
    
    # Write the heading
    blockMeshDict.write("FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       dictionary;\n    object      blockMeshDict;\n}\n")
    blockMeshDict.write("convertToMeters 0.001;\n")

    # Write down the vertices
    blockMeshDict.write( "\nvertices\n(\n")
    for point in pointlist: blockMeshDict.write( "    ("+repr(point[0])+" "+repr(point[1])+" "+repr(point[2])+") //"+repr(pointlist.index(point))+"\n")

    # Write down the blocks
    blockMeshDict.write( ");\n\nblocks\n(\n")
    for hex in hexblocks : 
       blockMeshDict.write( "  hex ( "+repr(hex[0])+" "+repr(hex[1])+" "+repr(hex[2])+" "+repr(hex[3])+" "+repr(hex[4])+" "+repr(hex[5])+" "+repr(hex[6])+" "+repr(hex[7])
           +" ) (" +repr(hex[8][0])+" "+repr(hex[8][1])+" "+repr(hex[8][2])+") simpleGrading ("+repr(hex[9][0])+" "+repr(hex[9][1])+" "+repr(hex[9][2])+")\n")
    
    # Write down the edges
    blockMeshDict.write( ");\n\nedges\n(\n") 
    for arc in arclist: 
       blockMeshDict.write("    arc "+repr(arc[0])+" "+repr(arc[1])+" ("+repr(arc[2][0])+" "+repr(arc[2][1])+" "+repr(arc[2][2])+")\n")
    
    # Write down the edges
    blockMeshDict.write( ");\n\nboundary\n(\n") 
    for b in boundlist:
        q = "" 
        for f in b[1]:
            q=q + "(" + repr(f[0]) + " " + repr(f[1]) + " " + repr(f[2]) + " " + repr(f[3]) + ")"
        blockMeshDict.write("    "+b[0]+" { type "+b[2]+"; faces ("+q+");}\n")

    #write down meged patches
    blockMeshDict.write( ");\n\nmergePatchPairs\n(\n")
    for m in mergepatches: 
       blockMeshDict.write( "(" + m[0] + " " + m[1] + ")\n")
    blockMeshDict.write( ");\n")
    blockMeshDict.close()
    return;

#######################################################################
## Script #############################################################
#######################################################################

pointlist, arclist, hexblocks, boundlist, mergepatches = ([] for i in range(5))
    
r      = 6                        # Radius of cylinders
R      = 54.5/2                   # Radius of outside cylinder
d      = 1                        # Boundary layer minimum thickness for outside cylinder
dg     = math.pi*30/180           # Degree of the mesh (45o)
pepdis = 32.5/2                   # Distance between 2 cylinders
l      = [200,0]                 # Levels of the Mesh
lf     = [[20],[1]]              # Mesh points and grading for the fluid mesh levels [50],[1]
ls     = [[20],[1]]              # Mesh points and grading for the solid mesh levels
lpi    = 36                       # number of mesh points per pi(180o)
expf   = [12,12]                  # Number of points in the mesh and their refinement [27,12]
exps   = [5,0.1]                  # Number of points in the solid mesh and their refinement   

#points for the center cylinder
c0 = [0,0]
a1 = [r,0]
a2 = [r*math.cos(dg), r*math.sin(dg)]

#points for the top cilinder
p0 = [pepdis,0]
p1 = [pepdis+r,0]
p2 = [pepdis+r*math.cos(dg), r*math.sin(dg)]
p3 = [pepdis, r]
p4 = [pepdis-r*math.cos(dg), r*math.sin(dg)]
p5 = [pepdis-r,0]

#grid points
s1 = [pepdis/2 , 0]
s2 = [pepdis/2 , pepdis/2*math.tan(dg)]
s3 = [pepdis , pepdis*math.tan(dg) ]
s4 = [pepdis*3/2 , pepdis/2*math.tan(dg)]
s5 = [pepdis*3/2 , 0]

#points for the outter wall
o1 = [R,0]
o2 = [ R *math.cos(dg),  R *math.sin(dg)]
o3 = [math.sqrt(R ** 2 - (pepdis/2*math.tan(dg)) ** 2),  pepdis/2*math.tan(dg)]

#Define Hexblocks input
for x in xrange(0,len(l)-1):
    createHexBlock(a1,s1,s2,a2,l[x],l[x+1], [expf[0],lpi/6,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer cen cilinder    

    createHexBlock(p1,s5,s4,p2,l[x],l[x+1], [expf[0],lpi/6,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer top cilinder
    createHexBlock(p2,s4,s3,p3,l[x],l[x+1], [expf[0],lpi/3,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer top cilinder
    createHexBlock(p3,s3,s2,p4,l[x],l[x+1], [expf[0],lpi/3,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer top cilinder
    createHexBlock(p4,s2,s1,p5,l[x],l[x+1], [expf[0],lpi/6,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer top cilinder

    createHexBlock(o3,s4,s5,o1,l[x],l[x+1], [expf[0],lpi/6,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer outter wall
    createHexBlock(o2,s3,s4,o3,l[x],l[x+1], [expf[0],lpi/3,lf[0][x]], [expf[1],1,lf[1][x]] )  # boundary layer outter wall


    createHexBlock(p0,p2,p3,p0,l[x],l[x+1], [exps[0],lpi/3,ls[0][x]], [exps[1],1,ls[1][x]])   # top cil
    createHexBlock(p0,p3,p4,p0,l[x],l[x+1], [exps[0],lpi/3,ls[0][x]], [exps[1],1,ls[1][x]])   # top cil
    createHexBlock(p0,p4,p5,p0,l[x],l[x+1], [exps[0],lpi/6,ls[0][x]], [exps[1],1,ls[1][x]])   # top cil
    createHexBlock(p0,p1,p2,p0,l[x],l[x+1], [exps[0],lpi/6,ls[0][x]], [exps[1],1,ls[1][x]])   # top cil
    createHexBlock(c0,a1,a2,c0,l[x],l[x+1], [exps[0],lpi/6,ls[0][x]], [exps[1],1,ls[1][x]])   # cen cil
    

#Define arcs
for z in l:
    addarc([a1,a2,getmidpoint(c0,a1,a2)],z)

    addarc([o1,o3,getmidpoint(c0,o1,o3)],z)
    addarc([o3,o2,getmidpoint(c0,o3,o2)],z)

    addarc([p1,p2,getmidpoint(p0,p1,p2)],z)
    addarc([p2,p3,getmidpoint(p0,p2,p3)],z)
    addarc([p3,p4,getmidpoint(p0,p3,p4)],z)
    addarc([p4,p5,getmidpoint(p0,p4,p5)],z)

createboundary("Outterwall",[[o2,o3],[o3,o1]],l,"patch")

createboundary("ciltoptop",[[p0,p1,p2,p0],[p0,p2,p3,p0],[p0,p3,p4,p0],[p0,p4,p5,p0]],l[0],"patch")
createboundary("cilbottop",[[p0,p2,p1,p0],[p0,p3,p2,p0],[p0,p4,p3,p0],[p0,p5,p4,p0]],l[-1],"patch")

createboundary("ciltopcen",[[c0,a1,a2,c0]],l[0],"patch")
createboundary("cilbotcen",[[c0,a2,a1,c0]],l[-1],"patch")

createboundary("symmetryleft",[[s1,a1],[p5,s1],[s5,p1],[o1,s5]],l,"symmetry")
createboundary("symmetryrigt",[[a2,s2],[s2,s3],[s3,o2]],l,"symmetry")

createboundary("symmetrytop1", [[p1,p0],[p0,p5]],l,"symmetry")

createboundary("symmetrycen1", [[a1,c0]],l,"symmetry" )
createboundary("symmetrycen2", [[c0,a2]],l,"symmetry" )

createboundary("Outlet",[[a1,s1,s2,a2],[p5,p4,s2,s1],[p4,p3,s3,s2],
   [p3,p2,s4,s3],[p2,p1,s5,s4],[s4,o3,o2,s3],[o1,o3,s4,s5]],l[0],"patch")

createboundary("Inlet",[[a2,s2,s1,a1],[s1,s2,p4,p5],[s2,s3,p3,p4],
    [s3,s4,p2,p3],[s4,s5,p1,p2],[s4,s3,o2,o3],[s5,s4,o3,o1]],l[-1],"patch")


writeblockmeshdict(pointlist,hexblocks,arclist,boundlist,mergepatches)
os.system("blockMesh")
os.system("checkMesh")
os.system("topoSet")
os.system("splitMeshRegions -cellZones -overwrite")
