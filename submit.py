#!/usr/bin/env python

'''Add important stuff'''
import os.path
import optparse

outputfolder = "/eos/user/d/ddesouza/XeXehistos"
inPut = 'XeXeMB'
systematics = 0

os.system("mkdir -p cond")
os.system("mkdir -p "+str(outputfolder)+"/"+str(inPut))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD1 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD1_ -f nextweek -c 4 -n 1 -s XeXe_PD1 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD2 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD2_ -f nextweek -c 4 -n 1 -s XeXe_PD2 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD3 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD3_ -f nextweek -c 4 -n 1 -s XeXe_PD3 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD4 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD4_ -f nextweek -c 4 -n 1 -s XeXe_PD4 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD5 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD5_ -f nextweek -c 4 -n 1 -s XeXe_PD5 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD6 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD6_ -f nextweek -c 4 -n 1 -s XeXe_PD6 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD7 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD7_ -f nextweek -c 4 -n 1 -s XeXe_PD7 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD8 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD8_ -f nextweek -c 4 -n 1 -s XeXe_PD8 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD9 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD9_ -f nextweek -c 4 -n 1 -s XeXe_PD9 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD10 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD10_ -f nextweek -c 4 -n 1 -s XeXe_PD10 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD11 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD11_ -f nextweek -c 4 -n 1 -s XeXe_PD11 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD12 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD12_ -f nextweek -c 4 -n 1 -s XeXe_PD12 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD13 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD13_ -f nextweek -c 4 -n 1 -s XeXe_PD13 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD14 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD14_ -f nextweek -c 4 -n 1 -s XeXe_PD14 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD15 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD15_ -f nextweek -c 4 -n 1 -s XeXe_PD15 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD16 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD16_ -f nextweek -c 4 -n 1 -s XeXe_PD16 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD17 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD17_ -f nextweek -c 4 -n 1 -s XeXe_PD17 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD18 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD18_ -f nextweek -c 4 -n 1 -s XeXe_PD18 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD19 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD19_ -f nextweek -c 4 -n 1 -s XeXe_PD19 "+str(systematics))
os.system("python HTCondor_submit_data.py -i inputdataset/XeXe_MB_PD20 -o "+str(outputfolder)+"/"+str(inPut)+"/XeXe_PD20_ -f nextweek -c 4 -n 1 -s XeXe_PD20 "+str(systematics))
