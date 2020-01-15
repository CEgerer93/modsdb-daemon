#!/usr/bin/python

import sys,optparse


from myxml import *


usage = "usage: %prog [options] "
parser = optparse.OptionParser(usage) ;

parser.add_option("-b", "--beta", type="string", default="b6p1",
                  help='Set inverse coupling (default b6p1)')

parser.add_option("-e", "--ensem", type="string", default="XX",
                  help='Set ensemble (default 0.0)')

parser.add_option("-X", "--Nx", type="int", default=32,
                  help='Set the spatial dimension of ensemble (default 32)')

parser.add_option("-T", "--Nt", type="int", default=96,
                  help='Set the temporal dimension of ensemble (default 96)')

parser.add_option("-m", "--lm", type="string", default="0.05",
                  help='Set the light quark mass (default="0.05")')

parser.add_option("-s", "--src", type="int", default=0,
                  help='Set the source position (default 0)')

parser.add_option("-p", "--pzi", type="int", default=0,
                  help='Set initial z-momentum (default 0)')

parser.add_option("-q", "--pzf", type="int", default=0,
                  help='Set final z-momentum (default 0)')

parser.add_option("-t", "--qsq", type="int", default=0,
                  help='Set momentum transfer (default 0)')

parser.add_option("-g", "--g1", type="int", default=1,
                  help='Set the first current insertion (default 1)')

parser.add_option("-f", "--g2", type="int", default=1,
                  help='Set the second current insertion (default 1)')

parser.add_option("-x", "--xmin", type="int", default=0,
                  help='Set x-min of currents (default 0)')

parser.add_option("-u", "--xmax", type="int", default=0,
                  help='Set x-max of currents (default 0)')

parser.add_option("-y", "--ymin", type="int", default=0,
                  help='Set y-min of currents (default 0)')

parser.add_option("-v", "--ymax", type="int", default=0,
                  help='Set y-max of currents (default 0)')

parser.add_option("-z", "--zmin", type="int", default=0,
                  help='Set z-min of currents (default 0)')

parser.add_option("-w", "--zmax", type="int", default=1,
                  help='Set z-max of currents (default 1)')

parser.add_option("-c", "--ccmass", type="int", default=0,
                  help='Set the number of current-current propagator masses (default 0)')

# parser.add_option("-i", "--cfgmin", type="int", default=1000,
#                   help='Set the first cnfg (default 1000)')

# parser.add_option("-j", "--cfgmax", type="int", default=1000,
#                   help='Set the final cnfg (default 1000)')

# parser.add_option("-k", "--cfgstep", type="int", default=0,
#                   help='Set the trajectory step number (default 0)')




(options, args) = parser.parse_args()

# Set the lattice dimensions
# latDim="32 32 32 96"
latDim=str(options.Nx)+" "+str(options.Nx)+" "+str(options.Nx)+" "+str(options.Nt)

baseDir="/data/cpegerer/pionPDF"
beta=options.beta
ensem=options.ensem
lm=options.lm
src=options.src
momInStr=""
momOutStr=""

# Set the momentum strings for reading and writing to file
if options.pzi==-5:
    momInStr="pz-5"
    momOutStr="pz-5"
if options.pzi==-4:
    momInStr="pz-4"
    momOutStr="pz-4"
if options.pzi==-3:
    momInStr="pz-3"
    momOutStr="pz-3"
if options.pzi==-2:
    momInStr="pz-2"
    momOutStr="pz-2"
if options.pzi==-1 or options.pzi==0 or options.pzi==1:
    momInStr="pz01"
    if options.pzi==-1:
        momOutStr="pz-1"
    if options.pzi==0:
        momOutStr="pz0"
    if options.pzi==1:
        momOutStr="pz1"
# if options.pzi==-1:
#     momInStr="Pz1"
#     momOutStr="pz-1"
# if options.pzi==0:
#     momInStr="Pz0"
#     momOutStr="pz0"
# if options.pzi==1:
#     momInStr="Pz1pos"
#     momOutStr="pz+1"
if options.pzi==2:
    momInStr="pz2"
    momOutStr="pz2"
if options.pzi==3:
    momInStr="pz3"
    momOutStr="pz3"
if options.pzi==4:
    momInStr="pz4"
    momOutStr="pz4"
if options.pzi==5:
    momInStr="pz5"
    momOutStr="pz5"


# Set the input directory to search for mod file
Dir=baseDir+"/"+beta+"/"+ensem+"/freefield/pdf_m"+lm+"_db/"+str(src)
# Set the output name for the sdb
sdbname="/data/cpegerer/SDB_pion/"+beta+"/"+ensem+"/pdf_m"+lm+"_db/freefield/"+momOutStr+"/"+str(src)


mom_f=[0,0,options.pzf]
mom_i=[0,0,options.pzi]
q_sq=options.qsq

# gamma=[options.g1,options.g2]
# Original runs do not have 11-11 current...
# gamma=["1 1","2 2","4 4","1 13","2 14","14 14","13 13","11 11"]
# gamma=["1 1","2 2","4 4","1 13","2 14","14 14","13 13"] #b6p1 32x96
# gamma=["0 0","1 1","2 2","4 4","1 13","2 14","14 14","13 13","11 11","15 15"] #b6p1 24x64
# gamma=["0 0","1 1","2 2","4 4","1 13","2 14","14 14","13 13","11 11","15 15"] #b6p3 32x64

gamma=[]
gammas=[1,2,8,13,14,7]
for i in gammas:
    for j in gammas:
        gamma.append(str(i)+" "+str(j))


# Set the current-src times based on which momenta is specified
Times="5 7 8 9 10 11 12 14 15"
numT=9
# if options.pzi==0 or options.pzi==-1 or options.pzi==1:
#     Times="5 6 7 8 9 10 11 12" # for b6p1 lattices
#     numT=8 # for b6p1 lattices
# #    Times="5 6 7 8 9 10 11 12 13" # for b6p3 32x64 lattices
# #    numT=9 # for b6p3 32x64 lattices
# if options.pzi==-4 or options.pzi==-3 or options.pzi==-2 or options.pzi==2 or options.pzi==3 or options.pzi==4:
#     Times="3 4 5 6 7 8 9 10 11"
#     numT=9
# if options.pzi==-5 or options.pzi==5:
#     Times="2 3 4 5 6 7 8"
#     numT=7

# Set the separation of currents here
xmin=options.xmin
xmax=options.xmax
ymin=options.ymin
ymax=options.ymax
zmin=options.zmin
zmax=options.zmax


def elemblock2pt():
    # Specify current-source information
    xml.openTag("Jsrc_sep")
    xml.writeTag("num_t",numT)
    xml.writeTag("times",Times)
    xml.closeTag()
    # Set the desired separations to peek
    xml.openTag("Xi")
    xml.openTag("xsep")
    xml.writeTag("xmin",0)
    xml.writeTag("xmax",0)
    xml.closeTag()
    xml.openTag("ysep")
    xml.writeTag("ymin",0)
    xml.writeTag("ymax",0)
    xml.closeTag()
    xml.openTag("zsep")
    xml.writeTag("zmin",0)
    xml.writeTag("zmax",0)
    xml.closeTag()
    xml.closeTag()
    # Set the external momenta and momentum transfer
    xml.openTag("PiPf")
    xml.writeListTag("p_i",mom_i)
    xml.writeListTag("p_f",mom_f)
    xml.writeTag("q_sq",q_sq)
    xml.closeTag()
    # Set the currents to extract
    xml.writeTag("gams","-1 -1")
    xml.writeTag("ccm",-1)
    # Set the location and naming of source mod file
    xml.openTag("Mod")
    xml.writeTag("Dir",Dir)
    xml.writeTag("Name","freefield_2pt.mod")
    xml.writeTag("Npt","2mod")
    xml.closeTag()
    # Set the location and naming of resulting sdb file
    xml.openTag("sdb")
    xml.writeTag("name",sdbname+"/pion_2pt/pion_2pt.sdb")
    xml.closeTag()

def elemblock4pt(ccm,g):
    # Specify current-source information
    xml.openTag("Jsrc_sep")
    xml.writeTag("num_t",numT)
    xml.writeTag("times",Times)
    xml.closeTag()
    # Set the desired separations to peek
    xml.openTag("Xi")
    xml.openTag("xsep")
    xml.writeTag("xmin",xmin)
    xml.writeTag("xmax",xmax)
    xml.closeTag()
    xml.openTag("ysep")
    xml.writeTag("ymin",ymin)
    xml.writeTag("ymax",ymax)
    xml.closeTag()
    xml.openTag("zsep")
    xml.writeTag("zmin",zmin)
    xml.writeTag("zmax",zmax)
    xml.closeTag()
    xml.closeTag()
    # Set the external momenta and momentum transfer
    xml.openTag("PiPf")
    xml.writeListTag("p_i",mom_i)
    xml.writeListTag("p_f",mom_f)
    xml.writeTag("q_sq",q_sq)
    xml.closeTag()
    # Set the currents to extract
    xml.writeTag("gams",g)
    xml.writeTag("ccm",ccm)
    # Set the location and naming of source mod file
    xml.openTag("Mod")
    xml.writeTag("Dir",Dir)
    xml.writeTag("Name","freefield_4pt.mod")
    xml.writeTag("Npt","4mod")
    xml.closeTag()
    # Set the location and naming of resulting sdb file
    xml.openTag("sdb")
    xml.writeTag("name",sdbname+"/hxyz/pion_4pt.sdb")
    xml.closeTag()


xml = MyXML()
xml.writeHeader()
xml.openTag("Modsdb")
xml.writeTag("nrow",latDim)
xml.openTag("Params")

# Only one 2pt key to make
xml.openTag("elem")
elemblock2pt()
xml.closeTag()


# Loop over 4pt keys - only one config
for ccm in range(0,options.ccmass):
    # Loop over 8 currents
    for g in range(0,len(gamma)):
        
        xml.openTag("elem")
        elemblock4pt(ccm,gamma[g])
        xml.closeTag()

xml.closeTag() 
xml.closeTag()
