# XeXe HBT correlation code
This code is going to be used to measure the HBT correlations in 1 and 3D for charged hadrons in XeXe collisions using 2017 dataset.
## Instructions
The code was updated to work on LXPLUS9 in order to use more resources from the cluster in HTCondor. To login in LXPLUS9 use:
```
ssh username@lxplus9.cern.ch
```
Once logged into LXPLUS machines setup the CMSSW version as follows:
```
cmsrel CMSSW_13_0_5
cd CMSSW_13_0_5/src/
cmsenv
```
and work inside of the ```src``` folder.
The code was created to generate histograms as function of centrality or multiplicity.
- For centrality depentency we have the following bins: 0-10%, 10-30%, 30-50%, 50-70% and 70-100% (not a good idea to use given the higher EM contamination). This code takes a long time to run due the correlations between large number of tracks and also because of mixing. To get the centrality dependency, use:
```
git clone https://github.com/denerslemos/XeXe_correlation.git
```
You have to edit lines 7 and 11 of ```submit_cent.py```: outputfolder - output folder to store your own histograms; cmsswrepo - CMSSW repository CMSSW/src; pwdrepo - the repository where the code is; it is also possible to change the systematics by changing an integer as described in the beginning of the .C code.
To run the codes you just need to:
```
python3 submit_cent.py
```
this will submit all condor jobs and produce the root files in the output folder.
- For the multiplicity dependency, the same code can be used. I have used similar histograms as the ones used in previous PbPb analysis (FSQ-14-002) with bins from 10 to 250 (also matching pPb). This must run faster due the small amount of combinations. In this case you have to edit line 4 and 5 in ```correlation_XeXe.C```, just uncomment one and comment the other one.
Also, submit the condor jobs using: 
```
python3 submit_mult.py
```