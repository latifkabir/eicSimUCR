#!/bin/csh

# Filename: setup.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Aug 10 18:39:21 2019 (-0400)
# URL: jlab.org/~latif

set called=($_)
if ("$called" != "") then
   set scriptdir=$called[2]
   set UCR_SIM_HOME=`dirname $scriptdir`
   set UCR_SIM_HOME=`c\d $UCR_SIM_HOME && pwd`    # ensure absolute path
else
    set scriptdir=$1
    set UCR_SIM_HOME=$scriptdir
endif

setenv UCR_SIM_HOME "$UCR_SIM_HOME"
setenv EIC_SIM_UCR "/star/u/kabir/pwg/eSimUCR/"

if (!($?LD_LIBRARY_PATH)) then
    setenv LD_LIBRARY_PATH "${EIC_SIM_UCR}/pythia8/lib:${EIC_SIM_UCR}/eSimUCR/fastjet/lib:${UCR_SIM_HOME}/lib/"
else 
    setenv LD_LIBRARY_PATH "${EIC_SIM_UCR}/pythia8/lib:${EIC_SIM_UCR}/eSimUCR/fastjet/lib:${UCR_SIM_HOME}/lib/:${LD_LIBRARY_PATH}"
endif

setenv PYTHIA8 "${EIC_SIM_UCR}/pythia8/"
setenv PYTHIA8DATA "${EIC_SIM_UCR}/pythia8/share/Pythia8/xmldoc/"

if (-f ${EIC_SIM_UCR}/root6/bin/thisroot.csh) then
    source ${EIC_SIM_UCR}/root6/bin/thisroot.csh
endif
