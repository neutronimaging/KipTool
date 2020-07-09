#!/usr/bin/env python

from subprocess import call
from math import fmod

srcmask="D:/P20170229_Snehota/04_evaluation/GR2_S2_6/recon/frame_{0:04}.tif"
dstmask="D:/P20170229_Snehota/04_evaluation/GR2_S2_6/filtered/fframe_{0:04}.tif"

# path to the application
kiptool="C:/Users/ander/source/repos/deployed/kiptool/KipToolCLI.exe"
cfgpath="C:/Users/ander/.imagingtools/CurrentKIPToolConfig.xml"

firstframe=0
lastframe=52
for i in range(firstframe,lastframe) :
    # set file mask for the slices
    srcname="image:srcfilemask="+srcmask.format(i)
    dstname="outimage:dstfilemask="+dstmask.format(i)

    # adjust the reconstruction angles to alternating between 0-180 and 180-360
    # call processing

    call([kiptool, "-f", cfgpath, srcname, dstname])