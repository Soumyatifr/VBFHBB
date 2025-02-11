import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("newPAT")
from PhysicsTools.PatAlgos.tools.helpers import getPatAlgosToolsTask
patAlgosToolsTask = getPatAlgosToolsTask(process)

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True)
)
isMC=True
isSinal=False

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.load("PhysicsTools.PatAlgos.slimming.slimmedAddPileupInfo_cfi")
process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
"/store/mc/Run3Summer22EEMiniAODv4/WtoLNu-2Jets_0J_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/MINIAODSIM/130X_mcRun3_2022_realistic_postEE_v6-v3/2540000/677063d1-002a-4bfa-a6ef-00abe58e61ce.root"
)
               )

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '130X_mcRun3_2022_realistic_postEE_v6','')


###########################    main analysers
process.mytuple = cms.EDAnalyzer(
    "genVNtuple",
    GenInf     = cms.InputTag("generator", "", "SIM"), #GEN-UL
    pruned     = cms.InputTag("prunedGenParticles"),
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string('output.root'))
process.p = cms.Path(
    process.mytuple,
    patAlgosToolsTask
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
