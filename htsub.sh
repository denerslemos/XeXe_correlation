#!/bin/bash

echo "Setup CMSSW (Import ROOT version)"
cd /afs/cern.ch/user/d/ddesouza/XeXe/CMSSW_13_0_5/src
eval `scramv1 runtime -sh`
cd /afs/cern.ch/user/d/ddesouza/XeXe/CMSSW_13_0_5/src/XeXe_correlation/
mkdir -p cond
echo "Submit skim jobs at "
echo PWD: $PWD

root -l -b -q "correlation_XeXe.C(\"$1\", \"$2\", $3, $4, $5, $6, $7, $8, $9, ${10}, ${11}, ${12})"
