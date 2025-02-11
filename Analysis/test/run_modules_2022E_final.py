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
isMC=False
isSinal=False

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.load("PhysicsTools.PatAlgos.slimming.slimmedAddPileupInfo_cfi")
process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
"root://cms-xrd-global.cern.ch//store/data/Run2022F/JetMET/MINIAOD/19Dec2023-v2/2540000/01a88007-123e-41d9-bda4-e4b79f8742bd.root"
#"root://cms-xrd-global.cern.ch//store/data/Run2022F/JetMET/MINIAOD/22Sep2023-v2/2550000/0006ceef-7579-459e-b618-9e58213246bf.root"

)
               )

#######################################
### Evaluate the version of particleNet
#######################################
#From CMSSW releases >=13_0_X
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection, addJetCollection
_btagDiscriminators=[]
from RecoBTag.ONNXRuntime.pfParticleNetFromMiniAODAK4_cff import _pfParticleNetFromMiniAODAK4PuppiCentralJetTagsAll as pfParticleNetFromMiniAODAK4PuppiCentralJetTagsAll
_btagDiscriminators += pfParticleNetFromMiniAODAK4PuppiCentralJetTagsAll

print(_btagDiscriminators)

process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '130X_dataRun3_PromptAnalysis_v1','')


from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJetsPuppi'),
   pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
   svSource = cms.InputTag('slimmedSecondaryVertices'),
   jetCorrections = ('AK4PFPuppi', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual']), 'None'),
   btagDiscriminators = _btagDiscriminators,
   postfix = 'WithPNetInfo',
)

###########################    main analysers
process.mytuple = cms.EDAnalyzer(
    "finalanalyser",
    pileupInfo         = cms.untracked.InputTag("slimmedAddPileupInfo"),
    genJets            = cms.InputTag("slimmedGenJets"),
    packedGenParticles = cms.InputTag("packedGenParticles"),
    Jets               = cms.InputTag("selectedUpdatedPatJetsWithPNetInfo"),
    Electrons          = cms.InputTag("slimmedElectrons"),
    Muons              = cms.InputTag("slimmedMuons"),
    labe_rho           = cms.InputTag("fixedGridRhoFastjetAll"),
    vertices           = cms.InputTag("offlineSlimmedPrimaryVertices"),    
    PFMETs             = cms.InputTag("slimmedMETs"),
    PuppiMet           = cms.InputTag("slimmedMETsPuppi"),
    bits               = cms.InputTag("TriggerResults","","HLT"),
    objects            = cms.InputTag("slimmedPatTrigger"),
    metfilterspatLabel_  = cms.untracked.InputTag("TriggerResults::PAT"),
    metfiltersrecoLabel_ = cms.untracked.InputTag("TriggerResults::RECO"),
    GenInf               = cms.InputTag("generator", "", "SIM"),
    Vertices          = cms.InputTag("offlineSlimmedPrimaryVertices"),
    l1GtSrc           = cms.InputTag("gtStage2Digis"),
    vetoMapFile       = cms.string("Summer22EE_23Sep2023_RunEFG_v1.root"),
    jec_jerc_file     = cms.string("jet_jerc_2022EE.json"),
    jerc_corr_tag     = cms.string("Summer22EE_22Sep2023_JRV1_MC_ScaleFactor_AK4PFPuppi"),
    jerc_reso_tag     = cms.string("Summer22EE_22Sep2023_JRV1_MC_PtResolution_AK4PFPuppi"),
    jec_corr_l1_tag   = cms.string("Summer22EE_22Sep2023_RunE_V2_DATA_L1FastJet_AK4PFPuppi"),
    jec_corr_l2_tag   = cms.string("Summer22EE_22Sep2023_RunE_V2_DATA_L2Relative_AK4PFPuppi"),
    jec_corr_l3_tag   = cms.string("Summer22EE_22Sep2023_RunE_V2_DATA_L3Absolute_AK4PFPuppi"),
    jec_corr_l2l3res_tag = cms.string("Summer22EE_22Sep2023_RunE_V2_DATA_L2L3Residual_AK4PFPuppi"),
    jec_corr_pnet_l2_tag   = cms.string("Summer22EE_22Sep2023_RunE_V2_DATA_L2Relative_AK4PFPuppiPNetRegressionPlusNeutrino"),
    jec_corr_pnet_l2l3res_tag = cms.string("Summer22EE_22Sep2023_RunE_V2_DATA_L2L3Residual_AK4PFPuppiPNetRegressionPlusNeutrino"),
    jec_corr_unc_tag  = cms.string("Summer22EE_22Sep2023_V2_MC_Total_AK4PFPuppi"),
    jec_corr_pnet_unc_tag  = cms.string("Summer22EE_22Sep2023_V2_MC_Total_AK4PFPuppiPNetRegressionPlusNeutrino"),
    lheInfo           = cms.InputTag("externalLHEProducer"),
    is_MC = cms.bool(isMC),
    is_signal = cms.bool(isSinal)
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string('output.root'))
process.p = cms.Path(
    process.mytuple,
    patAlgosToolsTask
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

