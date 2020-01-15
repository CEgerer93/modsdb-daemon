#!/usr/bin/python

import sys,optparse


from myxml import *


usage = "usage: %prog [options] "
parser = optparse.OptionParser(usage) ;

parser.add_option("-q", "--pzi", type="int", default=0,
                  help='Set initial z-momentum (default 0)')

parser.add_option("-p", "--pzf", type="int", default=0,
                  help='Set final z-momentum (default 0)')

parser.add_option("-t", "--qsq", type="int", default=0,
                  help='Set momentum transfer (default 0)')

parser.add_option("-r", "--srcSm", type="string", default="S",
                  help='Set the srource smearing (default S)')

parser.add_option("-n", "--snkSm", type="string", default="S",
                  help='Set the sink smearing (default S)')

parser.add_option("-o", "--srcOp", type="string", default="g5",
                  help='Set the source operator (default g5)')

parser.add_option("-k", "--snkOp", type="string", default="g5",
                  help='Set the sink operator (default g5)')

parser.add_option("-f", "--g1", type="int", default=1,
                  help='Set the first current insertion (default 1)')

parser.add_option("-g", "--g2", type="int", default=1,
                  help='Set the second current insertion (default 1)')

parser.add_option("-x", "--xsep", type="int", default=0,
                  help='Set range of x-separations of currents (default 0)')

parser.add_option("-y", "--ysep", type="int", default=0,
                  help='Set range of y-separations of currents (default 0)')

parser.add_option("-z", "--zsep", type="int", default=1,
                  help='Set range of z-separations of currents (default 1)')

parser.add_option("-c", "--ccmass", type="int", default=0,
                  help='Set the current-current propagator mass (default 0)')


(options, args) = parser.parse_args()

srcOp="pion"+"-"+options.srcOp
srcSpin=0
srcSmear=options.srcSm
srcLor=[]
snkOp="pion"+"-"+options.snkOp
snkSpin=0
snkSmear=options.snkSm
snkLor=[options.ccmass, options.g1, options.g2, options.xsep, options.ysep, options.zsep]
# snkLor=[cc_mass,g1,g2,x_rel_sep,y_rel_sep,z_rel_sep]

mom_f=[0,0,options.pzf]
mom_i=[0,0,options.pzi]
q_sq=options.qsq
mass="m-0.2350_m-0.2350"
# mass="m-0.2350_m-0.2350_m-0.2350"
#mass="m-0.2800_m-0.2800_m-0.2800"
#mass="m-0.3025_m-0.3025_m-0.3025"

gamma=0
links=""
quark=1
dt=666
# ensem="random_string"
ensem=""


# for c in range(0,4): # loop over the cc_mass
#     for x in range(-options.xsep,options.xsep+1):
#         for y in range(-options.ysep,options.ysep+1):
#             for z in range(-options.zsep,options.zsep+1):
                
                
xml = MyXML()
xml.writeHeader()
xml.openTag("Keys")
xml.openTag("elem")
xml.writeTag("num_vecs",3)
xml.writeTag("src_name",srcOp)
xml.writeTag("src_smear",srcSmear)
xml.writeListTag("src_lorentz",srcLor)
xml.writeTag("src_spin",srcSpin)
xml.writeTag("snk_name",snkOp)
xml.writeTag("snk_smear",snkSmear)
xml.writeListTag("snk_lorentz",snkLor)
xml.writeTag("snk_spin",snkSpin)
xml.openTag("PiPf")
xml.writeListTag("p_f",mom_f)
xml.writeListTag("p_i",mom_i)
xml.writeTag("q_sq",q_sq)
xml.closeTag()
xml.writeTag("gamma",gamma)
xml.writeTag("links",links)
xml.writeTag("quark",quark)
xml.writeTag("dt",dt)
xml.writeTag("mass",mass)
xml.writeTag("ensemble",ensem)
xml.closeTag()
xml.closeTag()

# change snk_lorentz
# change p_i/p_f
