#!/bin/bash

# Filename: setup.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Aug 10 18:39:21 2019 (-0400)
# URL: jlab.org/~latif

UCR_SIM_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export UCR_SIM_HOME
export EIC_SIM_UCR=/gpfs/mnt/gpfs01/star/pwg/kabir/eSimUCR/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EIC_SIM_UCR/pythia8/lib:$EIC_SIM_UCR/eSimUCR/fastjet/lib:$EIC_SIM_UCR/eic-smear/build/lib:$EIC_SIM_UCR/HepMC2/lib:$UCR_SIM_HOME/lib/
export PYTHIA8=$EIC_SIM_UCR/pythia8/
export PYTHIA8DATA=$EIC_SIM_UCR/pythia8/share/Pythia8/xmldoc/

if [ -f $EIC_SIM_UCR/root6/bin/thisroot.sh ]
then
    source $EIC_SIM_UCR/root6/bin/thisroot.sh
fi

export ROOT_INCLUDE_PATH=$EIC_SIM_UCR/eic-smear/include/:$EIC_SIM_UCR/HepMC2/include/HepMC/
