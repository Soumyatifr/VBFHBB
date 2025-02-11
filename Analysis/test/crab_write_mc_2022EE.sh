#!/bin/sh
production_tag=$1
config=$2
Dataset=$3
publication=$4
site=$5
DBS=$6

temp=crabfile_${1}.py

truncate -s 0 $temp

echo "import os
from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'crab_${production_tag}'
config.General.workArea = 'crabworkarea_2022EE'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '${config}'
config.JobType.inputFiles  = ['Summer22EE_23Sep2023_RunEFG_v1.root','jet_jerc_2022EE.json']
config.JobType.outputFiles = ['output.root']

config.Data.inputDataset = '$Dataset'
config.Data.inputDBS = '$DBS'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.outLFNDirBase = '/store/user/mukherje/VBFHToBB/2022EE/ver1/'

config.Site.storageSite = '$site'

config.section_(\"Debug\")
config.Debug.extraJDL = ['+CMS_ALLOW_OVERFLOW=False']
#config.Site.whitelist = [\"T2_IN_TIFR\"]" | cat >>$temp
