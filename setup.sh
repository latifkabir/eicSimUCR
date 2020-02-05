#!/bin/bash

export EIC_SIM_UCR=/star/u/kabir/pwg/eSimUCR
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$EIC_SIM_UCR/pythia8/lib:$EIC_SIM_UCR/eSimUCR/fastjet/lib
export PYTHIA8=$EIC_SIM_UCR/pythia8/
export PYTHIA8DATA=$EIC_SIM_UCR/pythia8/share/Pythia8/xmldoc/

source $EIC_SIM_UCR/root6/bin/thisroot.sh
