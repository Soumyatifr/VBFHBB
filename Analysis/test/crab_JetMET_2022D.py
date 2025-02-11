import os
from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'crab_JetMET_Run2022D'

config.General.workArea = 'crabworkarea_2022preEE'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'

config.JobType.psetName = 'run_modules_2022preEE_final.py'
config.JobType.inputFiles  = ['Summer22_23Sep2023_RunCD_v1.root','jet_jerc_2022preEE.json']
config.JobType.outputFiles = ['output.root']
config.Data.inputDataset = ''
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.lumiMask = 'Cert_Collisions2022_355100_362760_Golden.json'

config.Data.inputDataset = '/JetMET/Run2022D-19Dec2023-v1/MINIAOD'

config.Data.outLFNDirBase = '/store/user/mukherje/VBFHToBB/2022preEE/ver1/'
config.Site.storageSite   = 'T2_US_UCSD'

config.section_("Debug")
config.Debug.extraJDL = ['+CMS_ALLOW_OVERFLOW=False']
