#!/usr/bin/env python

'''Add important stuff'''
import os.path
import optparse
import subprocess
import sys

''' Inputs for the skim code '''
usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-i', '--infiles', dest='infiles', help='input list of files (.txt)', default='', type='string')
parser.add_option('-o', '--outfiles', dest='outputfiles', help='output name', default='', type='string')
parser.add_option('-f', '--jobflav', dest='jobflavour', help='job flavour (espresso=20min, microcentury=1h, longlunch=2h, workday=8h, tomorrow=1d, testmatch=3d, nextweek=1w)', default='espresso', type='string')
parser.add_option('-c', '--ncpu', dest='numberofcpu', help='number of cpu requested (2GB per cpu)', default='1', type='int')
parser.add_option('-n', '--njobs', dest='numberofjobs', help='number of jobs to be submitted (integer)', default='1', type='int')
parser.add_option('-s', '--subfiles', dest='subfiles', help='HTCondor submission file', default='HTcondor_sub_data_', type='string')
parser.add_option('-u', '--uncertanties', dest='uncertanties', help='Systematic uncertainties number', default='0', type='int')
parser.add_option('-w', '--cmssw', dest='cmsswdir', help='CMSSW directory', default='$CMSSW_BASE/src', type='string')
parser.add_option('-p', '--pwd', dest='pwddir', help='Local directory', default='$(pwd)', type='string')

(opt, args) = parser.parse_args()
inFiles = opt.infiles
outFiles = opt.outputfiles
jobFlavour = opt.jobflavour
nCpu = opt.numberofcpu
nJobs = opt.numberofjobs
subFiles = opt.subfiles
uncerSys = opt.uncertanties
cmsswDir = opt.cmsswdir
pwdDir = opt.pwddir

''' Read list of files '''
listOfFiles = open(inFiles+'.txt', 'r')
Lines = listOfFiles.readlines()
print ("Number of files: "+str(len(Lines)))
print ("Number of jobs: "+str(nJobs))
ratio = 0
if(nJobs == 0):
	ratio = len(Lines)
else:
	ratio = len(Lines)/nJobs

if(ratio < 1):
        sys.exit("Number of jobs greated than number of files, please reduce the number of jobs")
ratioint = int(ratio)
print ("file/jobs: "+str(ratio)+"   --> closest integer: " +str(int(ratioint)))


''' Make .sh file automatically '''
script_content = """#!/bin/bash

echo "Setup CMSSW (ROOT version)"
cd """+cmsswDir+"""
eval `scramv1 runtime -sh`
cd """+pwdDir+"""
mkdir -p cond
echo "Submit skim jobs at "
echo PWD: $PWD

root -l -b -q "correlation_XeXe.C(\\"$1\\", \\"$2\\", $3, $4, $5, $6, $7, $8, $9, ${10}, ${11}, ${12})"
"""

# Save to a shell script file
with open("htsubmult.sh", "w") as f:
    f.write(script_content)

os.chmod("htsubmult.sh", 0o755)

''' Start the write submission file '''
fsubfile = open(subFiles+".sub", "w")
command_lines = '''universe   = vanilla
getenv     = True
executable = htsubmult.sh
+JobFlavour           = "'''+str(jobFlavour)+'''"
requirements = ((OpSysAndVer =?= "AlmaLinux9") && (CERNEnvironment =?= "qa"))
RequestCpus = '''+str(nCpu)+'''
'''

if(ratio == len(Lines)):
	temp = '''
log        = cond/'''+subFiles+'''.log
output     = cond/'''+subFiles+'''.out
error      = cond/'''+subFiles+'''.err
arguments = '''+inFiles+'''.txt '''+str(outFiles)+''' 0 0 0 10 5 2.0 0 0 1 '''+str(uncerSys)+'''
queue
'''

	command_lines += temp
elif(ratio != len(Lines)):	
	''' Loop over files '''
	for i in range(nJobs):
		outtempfiles = open(inFiles+"_part"+str(i)+".txt", "w")
		starti = i * ratioint
		endi = ( i + 1 ) * ratioint
		if( i == nJobs - 1 ):
			endi = len(Lines)
		for line in Lines[starti:endi]:
			outtempfiles.write(line)
		outtempfiles.close()
		temp = '''
log        = cond/'''+subFiles+'''_part_'''+str(i)+'''.log
output     = cond/'''+subFiles+'''_part_'''+str(i)+'''.out
error      = cond/'''+subFiles+'''_part_'''+str(i)+'''.err
arguments = '''+inFiles+'''_part'''+str(i)+'''.txt '''+str(outFiles)+'''_job_'''+str(i)+''' 0 0 0 10 5 2.0 0 0 1 '''+str(uncerSys)+'''
queue
'''
		command_lines += temp
elif(ratio == 1):
	i=0
	for line in Lines:
		outtempfiles = open(inFiles+"_part"+str(i)+".txt", "w")
		outtempfiles.write(line)
		outtempfiles.close()
		temp = '''
log        = cond/'''+subFiles+'''_part_'''+str(i)+'''.log
output     = cond/'''+subFiles+'''_part_'''+str(i)+'''.out
error      = cond/'''+subFiles+'''_part_'''+str(i)+'''.err
arguments = '''+inFiles+'''_part'''+str(i)+'''.txt '''+str(outFiles)+'''_job_'''+str(i)+''' 0 0 0 10 5 2.0 0 0 1 '''+str(uncerSys)+'''
queue
'''
		command_lines += temp
		i=i+1


fsubfile.write(command_lines)
fsubfile.close()
subprocess.call(["condor_submit", subFiles+".sub"])
