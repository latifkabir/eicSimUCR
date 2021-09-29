#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Syntax: $0 <Job Number> <Number of Events>"
    exit
fi    

let jobNo=0+$1
nEvents=$2

PythiaCmdFile=/star/u/kabir/GIT/eicSimUCR/simJobs/temp/HardDiffraction_$jobNo.cmnd
OutName="eicDiffJetSim_${jobNo}.root" 
cp /star/u/kabir/GIT/eicSimUCR/simJobs/HardDiffraction_template.cmnd $PythiaCmdFile
sed -i "s/EVENTS/$nEvents/g" $PythiaCmdFile
sed -i "s/SEED/$jobNo/g" $PythiaCmdFile

source /star/u/kabir/GIT/eicSimUCR/setup.sh
#DelphesPythia8 /star/u/kabir/GIT/eicSimUCR/delphes/cards/delphes_card_EIC.tcl $PythiaCmdFile $OutName
DelphesPythia8 /star/u/kabir/GIT/eicSimUCR/delphes/cards/delphes_card_allsilicon_3T.tcl $PythiaCmdFile $OutName
