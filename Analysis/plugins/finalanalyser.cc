// class declaration
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "CondFormats/DataRecord/interface/L1TUtmTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "TLorentzVector.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

#include "TTree.h"
#include "TH1.h"
#include "TH2D.h"
#include "fastjet/Selector.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include <fastjet/GhostedAreaSpec.hh>
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
#include "fastjet/Selector.hh"
// correction lib
#include "/cvmfs/cms.cern.ch/slc7_amd64_gcc11/external/py3-correctionlib/2.1.0-d2a3f7d7a03ec004ef7327ef5e29e333/lib/python3.9/site-packages/correctionlib/include/correction.h"
using correction::CorrectionSet;

using namespace fastjet;
using namespace std;

class finalanalyser : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit finalanalyser(const edm::ParameterSet&);
      ~finalanalyser();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      bool applyJetID(const pat::Jet & jet, const std::string & level);
      int getNPU(edm::Handle <std::vector <PileupSummaryInfo> >  puInfo);
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;  

      edm::EDGetTokenT <edm::View<pat::MET>> metToken_ ;
      edm::EDGetTokenT<edm::View<pat::Electron>> tok_electrons_;
      edm::EDGetTokenT<pat::MuonCollection> tok_muons_;
      edm::EDGetTokenT <edm::View<pat::Jet>> tok_jet_;
      edm::EDGetTokenT<reco::GenJetCollection > genJetsToken;
      edm::EDGetTokenT <std::vector<PileupSummaryInfo> > pileupToken;
      edm::EDGetTokenT <double > rhoToken;
      edm::EDGetTokenT<reco::VertexCollection>tok_primaryVertices_;
      edm::EDGetTokenT<edm::TriggerResults> HLTriggerResults_;
      edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone> > triggerObjects_;
      edm::EDGetTokenT<edm::TriggerResults> metfilterspatLabel_;
      edm::EDGetTokenT<edm::TriggerResults>metfiltersrecoLabel_;
      edm::EDGetTokenT<std::vector< pat::MET> > puppimetToken_;
      edm::EDGetTokenT<std::vector<reco::Vertex> > verticesToken_;
      edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
      edm::EDGetTokenT <edm::View<reco::GenParticle>> prunedGenToken_;
      edm::EDGetTokenT<LHEEventProduct> lheInfoToken;
      edm::EDGetTokenT<GlobalAlgBlkBxCollection> l1AlgosToken;
      edm::ESGetToken<L1TUtmTriggerMenu, L1TUtmTriggerMenuRcd> l1GtMenuToken_;
      edm::EDGetToken l1GtToken_;
      bool is_MC_;
      bool is_signal_;

      //jet veto map 
      std::string vetoMapFile;
      TFile *f_vetomap;
      TH2D  *h_vetomap;

      vector<double> gen_V_pt, gen_V_eta, gen_V_phi, gen_V_en ;
      vector<int> gen_V_pdgid, gen_V_status;

      std::string  jec_jerc_file;
      std::string  jerc_corr_tag;
      std::string  jerc_reso_tag;
      std::string  jec_corr_l1_tag ;
      std::string  jec_corr_l2_tag ;
      std::string  jec_corr_l3_tag ;
      std::string  jec_corr_l2l3res_tag ;
      std::string  jec_corr_pnet_l2_tag;
      std::string  jec_corr_pnet_l2l3res_tag;
      std::string  jec_corr_pnet_unc_tag;
      std::string  jec_corr_unc_tag;

      std::shared_ptr<const correction::Correction> jec_corr_l1;
      std::shared_ptr<const correction::Correction> jec_corr_l2;
      std::shared_ptr<const correction::Correction> jec_corr_l3;
      std::shared_ptr<const correction::Correction> jec_corr_l2l3res;
      std::shared_ptr<const correction::Correction> jerc_corr; 
      std::shared_ptr<const correction::Correction> jerc_reso;
      std::shared_ptr<const correction::Correction> jec_corr_pnet_l2;
      std::shared_ptr<const correction::Correction> jec_corr_pnet_l2l3res;
      std::shared_ptr<const correction::Correction> jec_corr_pnet_unc;
      std::shared_ptr<const correction::Correction> jec_corr_unc;

      TTree* m_tree;
      TTree* m_tree2;
      unsigned long _run, _event, _lumi;
      Float_t _genWeight;
      Int_t nPV;
      Int_t nPU;
      Float_t             dxy_cut = 0.05;
      Float_t             dz_cut  = 0.1;

      Float_t             pu = -999.9;
      Float_t             rho = -999.9;
      Float_t             electron_pt = -999.9, electron_eta = -999.9, electron_phi = -999.9, electron_energy = -999.9;
      Float_t             muon_pt = -999.9, muon_eta = -999.9 , muon_phi = -999.9 , muon_energy = -999.9;
      Int_t               electron_charge = 0, muon_charge = 0;

      Int_t    njet;
      vector<double> jet_pt;
      vector<double> jet_eta;
      vector<double> jet_phi;
      vector<double> jet_en;
      vector<double> jet_mass;
      vector<double> jet_PNBTG;
      vector<double> jet_PNQGL;
      vector<double> jet_pt_raw;
      vector<double> jet_eta_raw;
      vector<double> jet_phi_raw;
      vector<double> jet_energy_raw;
      vector<double> jet_mass_raw;
      vector<double> jet_jec;
      vector<double> jet_jec_L2;
      vector<double> jet_jec_L3;
      vector<double> jet_jec_L2L3;
      vector<double> jet_jec_unc;
      vector<double> jet_jerc_sf;
      vector<double> jet_jerc_sf_up;
      vector<double> jet_jerc_sf_dw;
      vector<double> jet_jerc_reso;
      vector<double> jet_genmatch_pt;
      vector<double> jet_genmatch_eta;
      vector<double> jet_genmatch_phi;
      vector<double> jet_genmatch_mass;
      vector<double> jet_genmatch_energy;
      vector<Bool_t> jet_id;
      vector<int> jet_pflv;
      vector<int> jet_hflv;
      vector<double> jet_pnet_jec;
      vector<double> jet_pnet_jec_unc;
      vector<double> jet_pnet_ptcorr;
      vector<double> jet_pnet_ptnu;
      vector<double> jet_pnet_ptres;
      //HLT objects
      Int_t    nL1jet;
      vector<double> L1jet_pt;
      vector<double> L1jet_eta;
      vector<double> L1jet_phi;
      vector<double> L1jet_en;

      Int_t    nCalojet;
      vector<double> Calojet_pt;
      vector<double> Calojet_eta;
      vector<double> Calojet_phi;
      vector<double> Calojet_en;

      Int_t   nPxljet;
      vector<double> Pxljet_pt;
      vector<double> Pxljet_eta;
      vector<double> Pxljet_phi;
      vector<double> Pxljet_en;

      Int_t    nL3jet;
      vector<double> L3jet_pt;
      vector<double> L3jet_eta;
      vector<double> L3jet_phi;
      vector<double> L3jet_en;


      Int_t    nCalobjet;
      vector<double> Calobjet_pt;
      vector<double> Calobjet_eta;
      vector<double> Calobjet_phi;
      vector<double> Calobjet_en;

      Int_t    nL3b1jet;
      vector<double> L3b1jet_pt;
      vector<double> L3b1jet_eta;
      vector<double> L3b1jet_phi;
      vector<double> L3b1jet_en;

      Int_t    nL3b2jet;
      vector<double> L3b2jet_pt;
      vector<double> L3b2jet_eta;
      vector<double> L3b2jet_phi;
      vector<double> L3b2jet_en;
      //MET filters
      Bool_t Flag_goodVertices_;
      Bool_t Flag_globalSuperTightHalo2016Filter_;
      Bool_t Flag_EcalDeadCellTriggerPrimitiveFilter_;
      Bool_t Flag_BadPFMuonFilter_;
      Bool_t Flag_BadPFMuonDzFilter_;
      Bool_t Flag_hfNoisyHitsFilter_;
      Bool_t Flag_eeBadScFilter_;
      Bool_t Flag_ecalBadCalibFilter_;
      Bool_t event_veto_map = false;
      Float_t        HT = -999.9;
      Float_t        MET_pt;
      Float_t        MET_phi;

      Bool_t              HLT_PFJet40= false;
      Bool_t              HLT_PFJet60= false;
      Bool_t              HLT_PFJet80= false;

      Bool_t HLT_QuadPFJet103_88_75_15 = false;
      Bool_t HLT_QuadPFJet105_88_76_15 = false;
      Bool_t HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 = false;
      Bool_t HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2 = false;
      Bool_t HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1  = false;
      Bool_t HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2 = false;

      Bool_t HLT_PF60=false;
      Bool_t HLT_PF80=false;
      Bool_t HLT_DiPFJetAve80=false;

      Bool_t L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5 =false;
      Bool_t L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5 =false;
      Bool_t L1_HTT280er =false;
      Bool_t L1_HTT320er =false;
      Bool_t L1_SingleJet160er2p5 =false;
      Bool_t L1_SingleJet180 =false;
      Bool_t L1_SingleJet200 =false;

      int idx_L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5, idx_L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5, idx_L1_HTT280er, idx_L1_HTT320er, idx_L1_SingleJet160er2p5, idx_L1_SingleJet180, idx_L1_SingleJet200;

      double Generator_weight;
      float wgt_isr_up, wgt_isr_dn, wgt_fsr_up, wgt_fsr_dn;
      float wgt_scl_0, wgt_scl_1, wgt_scl_2, wgt_scl_3, wgt_scl_4, wgt_scl_5, wgt_scl_6, wgt_scl_7, wgt_scl_8;

      uint nPDFsets = 103;
      static const int nlhescalemax = 9;
      int nLHEScaleWeights;
      float LHEScaleWeights[nlhescalemax];

      static const int nlhepdfmax = 103; // be consistent with nPDFsets (nlhepdfmax should be >= nPDFsets)
      int nLHEPDFWeights;
      float LHEPDFWeights[nlhepdfmax];

      static const int nalpsmax = 3;
      int nLHEAlpsWeights;
      float LHEAlpsWeights[nalpsmax];

      static const int nlhepsmax = 8;
      int nLHEPSWeights;
      float LHEPSWeights[nlhepsmax];

      double LHE_weight;
      float Generator_qscale, Generator_x1, Generator_x2, Generator_xpdf1, Generator_xpdf2, Generator_scalePDF;
      int Generator_id1, Generator_id2;

      template<typename T>
      class PatRefPtSorter {
       public:
       bool operator()(const T& i, const T& j) const {
           return (i->pt() > j->pt());
        }
      };
      PatRefPtSorter<reco::GenJetRef>  genJetRefSorter;
};
struct JetIDVars_
{
  float NHF, NEMF, MUF, CHF, CEMF;
  int NumConst, NumNeutralParticle, CHM;
};

finalanalyser::finalanalyser(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   tok_electrons_          = consumes<edm::View<pat::Electron>> (iConfig.getParameter<edm::InputTag>("Electrons"));
   tok_muons_              = consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("Muons"));
   tok_jet_                = consumes<edm::View<pat::Jet>> (iConfig.getParameter<edm::InputTag>("Jets"));
   genJetsToken            = consumes<reco::GenJetCollection > (iConfig.getParameter<edm::InputTag>("genJets"));
   lheInfoToken            = consumes<LHEEventProduct> (iConfig.getParameter<edm::InputTag>("lheInfo"));
   pileupToken             = consumes<std::vector<PileupSummaryInfo> >(iConfig.getUntrackedParameter <edm::InputTag>("pileupInfo"));
   rhoToken                = consumes <double> (iConfig.getParameter <edm::InputTag>("labe_rho"));
   HLTriggerResults_       = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));
   triggerObjects_         = consumes<std::vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("objects"));
   tok_primaryVertices_    = consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"));
   metfilterspatLabel_     = consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag> ("metfilterspatLabel_"));
   metfiltersrecoLabel_    = consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag> ("metfiltersrecoLabel_"));
   puppimetToken_          = consumes<std::vector<pat::MET> > (iConfig.getParameter<edm::InputTag>("PuppiMet"));
   verticesToken_          = consumes<std::vector<reco::Vertex> > (iConfig.getParameter<edm::InputTag>("Vertices"));
   genInfoToken_           = consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("GenInf"));
   prunedGenToken_         = consumes <edm::View<reco::GenParticle>> (iConfig.getParameter<edm::InputTag>("pruned"));
   l1GtMenuToken_          = esConsumes<L1TUtmTriggerMenu, L1TUtmTriggerMenuRcd>();
   l1GtToken_              = consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("l1GtSrc"));
   is_MC_                  = iConfig.getParameter<bool> ("is_MC");
   is_signal_              = iConfig.getParameter<bool> ("is_signal");
   vetoMapFile             = iConfig.getParameter < std::string > ("vetoMapFile");
   jec_jerc_file           = iConfig.getParameter < std::string > ("jec_jerc_file");
   jerc_corr_tag           = iConfig.existsAs<std::string>("jerc_corr_tag") ? iConfig.getParameter<std::string>("jerc_corr_tag") : "";
   jerc_reso_tag           = iConfig.existsAs<std::string>("jerc_reso_tag") ? iConfig.getParameter<std::string>("jerc_reso_tag") : "";
   jec_corr_l1_tag              = iConfig.existsAs<std::string>("jec_corr_l1_tag") ? iConfig.getParameter<std::string>("jec_corr_l1_tag") : ""; 
   jec_corr_l2_tag              = iConfig.existsAs<std::string>("jec_corr_l2_tag") ? iConfig.getParameter<std::string>("jec_corr_l2_tag") : ""; 
   jec_corr_l3_tag              = iConfig.existsAs<std::string>("jec_corr_l3_tag") ? iConfig.getParameter<std::string>("jec_corr_l3_tag") : ""; 
   jec_corr_l2l3res_tag         = iConfig.existsAs<std::string>("jec_corr_l2l3res_tag") ? iConfig.getParameter<std::string>("jec_corr_l2l3res_tag") : "";
   jec_corr_unc_tag             = iConfig.existsAs<std::string>("jec_corr_unc_tag") ? iConfig.getParameter<std::string>("jec_corr_unc_tag") : "";
   jec_corr_pnet_l2_tag         = iConfig.existsAs<std::string>("jec_corr_pnet_l2_tag") ? iConfig.getParameter<std::string>("jec_corr_pnet_l2_tag") : "";
   jec_corr_pnet_l2l3res_tag    = iConfig.existsAs<std::string>("jec_corr_pnet_l2l3res_tag") ? iConfig.getParameter<std::string>("jec_corr_pnet_l2l3res_tag") : "";
   jec_corr_pnet_unc_tag        = iConfig.existsAs<std::string>("jec_corr_pnet_unc_tag") ? iConfig.getParameter<std::string>("jec_corr_pnet_unc_tag") : "";

   usesResource("TFileService");
   edm::Service<TFileService> fs;
   m_tree = fs->make<TTree>("tree", "");
   m_tree2 = fs->make<TTree>("allEvt", "");
   if(is_MC_ && is_signal_)
   {
      m_tree2 -> Branch ("nPU", &nPU);
      m_tree2 -> Branch ("weight", & _genWeight);
      m_tree2 -> Branch ("nPV", &nPV);
      m_tree2->Branch("wgt_isr_up", &wgt_isr_up, "wgt_isr_up/F");
      m_tree2->Branch("wgt_isr_dn", &wgt_isr_dn, "wgt_isr_dn/F");
      m_tree2->Branch("wgt_fsr_up", &wgt_fsr_up, "wgt_fsr_up/F");
      m_tree2->Branch("wgt_fsr_dn", &wgt_fsr_dn, "wgt_fsr_dn/F");
      //Total weight QCDscale
      m_tree2->Branch("wgt_scl_0", &wgt_scl_0, "wgt_scl_0/F");
      m_tree2->Branch("wgt_scl_1", &wgt_scl_1, "wgt_scl_1/F");
      m_tree2->Branch("wgt_scl_2", &wgt_scl_2, "wgt_scl_2/F");
      m_tree2->Branch("wgt_scl_3", &wgt_scl_3, "wgt_scl_3/F");
      m_tree2->Branch("wgt_scl_4", &wgt_scl_4, "wgt_scl_4/F");
      m_tree2->Branch("wgt_scl_5", &wgt_scl_5, "wgt_scl_5/F");
      m_tree2->Branch("wgt_scl_6", &wgt_scl_6, "wgt_scl_6/F");
      m_tree2->Branch("wgt_scl_7", &wgt_scl_7, "wgt_scl_7/F");
      m_tree2->Branch("wgt_scl_8", &wgt_scl_8, "wgt_scl_8/F");
   }
   //*****************************************************//
   m_tree -> Branch ("nPU", &nPU);
   m_tree -> Branch ("weight", & _genWeight);
   m_tree -> Branch ("nPV", &nPV);
   m_tree -> Branch ("rho", &rho);
   m_tree -> Branch ("run", &_run, "run/l");
   m_tree -> Branch ("event", &_event, "event/l");
   m_tree -> Branch ("lumi", &_lumi, "lumi/l");
   m_tree -> Branch("event_veto_map", &event_veto_map);
   if(is_MC_)
   {   
     m_tree->Branch("gen_V_pt",&gen_V_pt);
     m_tree->Branch("gen_V_eta",&gen_V_eta);
     m_tree->Branch("gen_V_phi",&gen_V_phi);
     m_tree->Branch("gen_V_en",&gen_V_en);
     m_tree->Branch("gen_V_pdgid",&gen_V_pdgid);
     m_tree->Branch("gen_V_status",&gen_V_status);
   }

   if(is_MC_ && is_signal_)
   {
	   m_tree->Branch("LHE_weight",&LHE_weight, "LHE_weight/D");
           m_tree->Branch("nLHEScaleWeights",&nLHEScaleWeights, "nLHEScaleWeights/I");
           m_tree->Branch("LHEScaleWeights",LHEScaleWeights,"LHEScaleWeights[nLHEScaleWeights]/F");
           m_tree->Branch("nLHEPDFWeights",&nLHEPDFWeights, "nLHEPDFWeights/I");
           m_tree->Branch("LHEPDFWeights",LHEPDFWeights,"LHEPDFWeights[nLHEPDFWeights]/F");
           m_tree->Branch("nLHEAlpsWeights",&nLHEAlpsWeights, "nLHEAlpsWeights/I");
           m_tree->Branch("LHEAlpsWeights",LHEAlpsWeights,"LHEAlpsWeights[nLHEAlpsWeights]/F");
           m_tree->Branch("nLHEPSWeights",&nLHEPSWeights, "nLHEPSWeights/I");
           m_tree->Branch("LHEPSWeights",LHEPSWeights,"LHEPSWeights[nLHEPSWeights]/F");
           //GEN
           m_tree->Branch("Generator_weight", &Generator_weight, "Generator_weight/D") ;
           m_tree->Branch("Generator_qscale",&Generator_qscale,"Generator_qscale/F");
           m_tree->Branch("Generator_x1",&Generator_x1,"Generator_x1/F");
           m_tree->Branch("Generator_x2",&Generator_x2,"Generator_x2/F");
           m_tree->Branch("Generator_xpdf1",&Generator_xpdf1,"Generator_xpdf1/F");
           m_tree->Branch("Generator_xpdf2",&Generator_xpdf2,"Generator_xpdf2/F");
           m_tree->Branch("Generator_id1",&Generator_id1,"Generator_id1/I");
           m_tree->Branch("Generator_id2",&Generator_id2,"Generator_id2/I");
           m_tree->Branch("Generator_scalePDF",&Generator_scalePDF,"Generator_scalePDF/F");
   }   
   m_tree -> Branch ("Flag_goodVertices", &Flag_goodVertices_);
   m_tree -> Branch ("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter_);
   m_tree -> Branch ("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter_);
   m_tree -> Branch ("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter_);
   m_tree -> Branch ("Flag_BadPFMuonDzFilter", &Flag_BadPFMuonDzFilter_);
   m_tree -> Branch ("Flag_hfNoisyHitsFilter", &Flag_hfNoisyHitsFilter_);
   m_tree -> Branch ("Flag_eeBadScFilter", &Flag_eeBadScFilter_);
   m_tree -> Branch ("Flag_ecalBadCalibFilter_", &Flag_ecalBadCalibFilter_);

   //now we are storing all the informations about the b-Jets

   m_tree -> Branch ("njet",         &njet);
   m_tree -> Branch ("jet_pt",       &jet_pt);
   m_tree -> Branch ("jet_eta",      &jet_eta);
   m_tree -> Branch ("jet_phi",      &jet_phi);
   m_tree -> Branch ("jet_en",       &jet_en);
   m_tree -> Branch ("jet_mass",     &jet_mass);
   m_tree -> Branch ("jet_pt_raw",   &jet_pt_raw);
   m_tree -> Branch ("jet_mass_raw", &jet_mass_raw );
   m_tree -> Branch ("jet_energy_raw", &jet_energy_raw );
   m_tree -> Branch ("jet_jec", &jet_jec );
   m_tree -> Branch ("jet_jec_L2", &jet_jec_L2 );
   m_tree -> Branch ("jet_jec_L3", &jet_jec_L3 );
   m_tree -> Branch ("jet_jec_L2L3", &jet_jec_L2L3 );
   m_tree -> Branch ("jet_jerc_sf", &jet_jerc_sf );
   m_tree -> Branch ("jet_jerc_sf_up", &jet_jerc_sf_up);
   m_tree -> Branch ("jet_jerc_sf_dw", &jet_jerc_sf_dw);

   m_tree -> Branch ("jet_jerc_reso", &jet_jerc_reso );
   m_tree -> Branch ("jet_genmatch_pt", &jet_genmatch_pt );
   m_tree -> Branch ("jet_genmatch_eta", &jet_genmatch_eta );
   m_tree -> Branch ("jet_genmatch_phi", &jet_genmatch_phi );
   m_tree -> Branch ("jet_genmatch_mass", &jet_genmatch_mass );
   m_tree -> Branch ("jet_genmatch_energy", &jet_genmatch_energy );
   m_tree -> Branch ("jet_PNBTG",    &jet_PNBTG);
   m_tree -> Branch ("jet_PNQGL",    &jet_PNQGL);
   m_tree -> Branch ("jet_id",    &jet_id);
   m_tree -> Branch ("jet_pflv",  &jet_pflv);
   m_tree -> Branch ("jet_hflv",  &jet_hflv);
   m_tree -> Branch ("jet_pnet_jec", &jet_pnet_jec);
   m_tree -> Branch ("jet_pnet_ptcorr", &jet_pnet_ptcorr);
   m_tree -> Branch ("jet_pnet_ptnu", &jet_pnet_ptnu);
   m_tree -> Branch ("jet_pnet_ptres", &jet_pnet_ptres);
   m_tree -> Branch ("jet_jec_unc", &jet_jec_unc);
   m_tree -> Branch ("jet_pnet_jec_unc", &jet_pnet_jec_unc);

   m_tree-> Branch ("nL1jet", &nL1jet);
   m_tree-> Branch ("L1jet_pt", &L1jet_pt);
   m_tree-> Branch ("L1jet_eta", &L1jet_eta);
   m_tree-> Branch ("L1jet_phi", &L1jet_phi);
   m_tree-> Branch ("L1jet_en", &L1jet_en);

   m_tree-> Branch ("nCalojet",    &nCalojet);
   m_tree-> Branch ("Calojet_pt",  &Calojet_pt);
   m_tree-> Branch ("Calojet_eta", &Calojet_eta);
   m_tree-> Branch ("Calojet_phi", &Calojet_phi);
   m_tree-> Branch ("Calojet_en",  &Calojet_en);   

   m_tree-> Branch ("nPxljet",    &nPxljet);
   m_tree-> Branch ("Pxljet_pt",  &Pxljet_pt);
   m_tree-> Branch ("Pxljet_eta", &Pxljet_eta);
   m_tree-> Branch ("Pxljet_phi", &Pxljet_phi);
   m_tree-> Branch ("Pxljet_en",  &Pxljet_en);  

   m_tree-> Branch ("nL3jet", &nL3jet);
   m_tree-> Branch ("L3jet_pt", &L3jet_pt);
   m_tree-> Branch ("L3jet_eta", &L3jet_eta);
   m_tree-> Branch ("L3jet_phi", &L3jet_phi);
   m_tree-> Branch ("L3jet_en", &L3jet_en);

   m_tree-> Branch ("nCalobjet",    &nCalobjet);
   m_tree-> Branch ("Calobjet_pt",  &Calobjet_pt);
   m_tree-> Branch ("Calobjet_eta", &Calobjet_eta);
   m_tree-> Branch ("Calobjet_phi", &Calobjet_phi);
   m_tree-> Branch ("Calobjet_en",  &Calobjet_en);

   m_tree-> Branch ("nL3b1jet",     &nL3b1jet);
   m_tree-> Branch ("L3b1jet_pt",   &L3b1jet_pt);
   m_tree-> Branch ("L3b1jet_eta",  &L3b1jet_eta);
   m_tree-> Branch ("L3b1jet_phi",  &L3b1jet_phi);
   m_tree-> Branch ("L3b1jet_en",   &L3b1jet_en);

   m_tree-> Branch ("nL3b2jet",     &nL3b2jet);
   m_tree-> Branch ("L3b2jet_pt",   &L3b2jet_pt);
   m_tree-> Branch ("L3b2jet_eta",  &L3b2jet_eta);
   m_tree-> Branch ("L3b2jet_phi",  &L3b2jet_phi);
   m_tree-> Branch ("L3b2jet_en",   &L3b2jet_en);
   //online b-tag object HLT level

   m_tree-> Branch ("MET_pt",  &MET_pt);
   m_tree-> Branch ("MET_phi", &MET_phi);

   m_tree-> Branch ("HLT_QuadPFJet103_88_75_15", &HLT_QuadPFJet103_88_75_15 );
   m_tree-> Branch ("HLT_QuadPFJet105_88_76_15", &HLT_QuadPFJet105_88_76_15 );
   m_tree-> Branch ("HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1", &HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 );
   m_tree-> Branch ("HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2", &HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2 );
   m_tree-> Branch ("HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1", &HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 );
   m_tree-> Branch ("HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2", &HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2 );

   //L1 seed information for the VBF trigger
   m_tree->Branch("L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5", &L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5);
   m_tree->Branch("L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5", &L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5);
   m_tree->Branch("L1_HTT280er", &L1_HTT280er);
   m_tree->Branch("L1_HTT320er", &L1_HTT320er);
   m_tree->Branch("L1_SingleJet160er2p5", &L1_SingleJet160er2p5);
   m_tree->Branch("L1_SingleJet180", &L1_SingleJet180);   
   m_tree->Branch("L1_SingleJet200", &L1_SingleJet200);

}

finalanalyser::~finalanalyser()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

void finalanalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
  using namespace edm;
  using namespace reco;
  using namespace pat;
  using namespace std;

     edm::Handle<edm::View<pat::Jet>> recjets;
     iEvent.getByToken(tok_jet_, recjets);

     edm::Handle<edm::View<pat::Electron>> electrons;
     iEvent.getByToken(tok_electrons_, electrons);

     edm::Handle<double> rhoInfo;
     iEvent.getByToken(rhoToken, rhoInfo);

     edm::Handle<std::vector<reco::Vertex>> vertices;
     iEvent.getByToken(tok_primaryVertices_, vertices);

     edm::Handle<pat::MuonCollection> muons;
     iEvent.getByToken(tok_muons_, muons);

     edm::Handle<reco::VertexCollection> primaryVertices;
     iEvent.getByToken(tok_primaryVertices_, primaryVertices);

     edm::Handle<edm::TriggerResults> METFilterResults;
     iEvent.getByToken(metfilterspatLabel_, METFilterResults);

     edm::Handle<reco::GenJetCollection> genJetsH;
     iEvent.getByToken(genJetsToken, genJetsH);

     if(!(METFilterResults.isValid())) iEvent.getByToken(metfiltersrecoLabel_, METFilterResults);

     //rho index
     edm::Handle<double> Rho_PF;
     iEvent.getByToken(rhoToken, Rho_PF);
     rho = * Rho_PF ;

     reco::Vertex vertex;
     if (primaryVertices.isValid()) {
           if(primaryVertices->size() > 0){
                   vertex = primaryVertices->at(0);
           }
      }

     //Printing the list of triggers with their prescale factors
     edm::Handle<edm::TriggerResults> HLTR;
     iEvent.getByToken(HLTriggerResults_, HLTR);
     //Number of primary vertices
     edm::Handle<std::vector<reco::Vertex> > theVertices;
     iEvent.getByToken(verticesToken_,theVertices) ;
     nPV = theVertices->size();
      //Variables related to the MC only
     if(is_MC_)
       {
         edm::Handle<GenEventInfoProduct> genEvtInfo;
         iEvent.getByToken(genInfoToken_, genEvtInfo);
         _genWeight=genEvtInfo->weight();
         edm::Handle<std::vector<PileupSummaryInfo>>  PupInfo;
         iEvent.getByToken(pileupToken, PupInfo);
         std::vector<PileupSummaryInfo>::const_iterator PVI;
         nPU = -1;
         for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI)
         {
            int BX = PVI->getBunchCrossing();
            if(BX == 0)
            {
               nPU = PVI->getTrueNumInteractions();
               continue;
            }
          }
	 //Alternative weight calculations only for the POWHEG samples
	 if(is_signal_)
	 {
            wgt_isr_up = 1.0, wgt_isr_dn = 1.0, wgt_fsr_up = 1.0, wgt_fsr_dn = 1.0; //ISR up/down
            wgt_scl_0  = 1.0, wgt_scl_1 = 1.0, wgt_scl_2 = 1.0, wgt_scl_3 = 1.0;  wgt_scl_4  = 1.0, wgt_scl_5 = 1.0, wgt_scl_6 = 1.0, wgt_scl_7 = 1.0, wgt_scl_8 = 1.0; //QCDsacle
	    edm::Handle<LHEEventProduct>lheeventinfo ;
	    iEvent.getByToken(lheInfoToken,lheeventinfo) ;
	    nLHEScaleWeights = 0;
            nLHEPDFWeights = 0;
            nLHEAlpsWeights = 0;	    
            if(lheeventinfo.isValid()){
		    LHE_weight = lheeventinfo->originalXWGTUP();
		    for ( unsigned int index = 0; index < lheeventinfo->weights().size(); ++index ) {
			    if(index<nlhescalemax && nLHEScaleWeights<nlhescalemax){
				    LHEScaleWeights[nLHEScaleWeights] = lheeventinfo->weights()[index].wgt/lheeventinfo->originalXWGTUP();
				    wgt_scl_0 = lheeventinfo->weights()[0].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_1 = lheeventinfo->weights()[1].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_2 = lheeventinfo->weights()[2].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_3 = lheeventinfo->weights()[3].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_4 = lheeventinfo->weights()[4].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_5 = lheeventinfo->weights()[5].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_6 = lheeventinfo->weights()[6].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_7 = lheeventinfo->weights()[7].wgt/lheeventinfo->originalXWGTUP();
                                    wgt_scl_8 = lheeventinfo->weights()[8].wgt/lheeventinfo->originalXWGTUP();
                                    nLHEScaleWeights++;
			    }
			    if(index>=nlhescalemax && index<(nlhescalemax+nPDFsets)  && nLHEPDFWeights<nlhepdfmax){
                                    LHEPDFWeights[nLHEPDFWeights] = lheeventinfo->weights()[index].wgt/lheeventinfo->originalXWGTUP();
                                    nLHEPDFWeights++;
                            }
                            if(index>=(nlhescalemax+nPDFsets) && index<(nlhescalemax+nPDFsets+nalpsmax) && nLHEAlpsWeights<nalpsmax){
                                    LHEAlpsWeights[nLHEAlpsWeights] = lheeventinfo->weights()[index].wgt/lheeventinfo->originalXWGTUP();
                                    nLHEAlpsWeights++;
                            }
                       }
	    }
            nLHEPSWeights = 8;
	    if (genEvtInfo.isValid()){
		    Generator_weight = genEvtInfo->weight();
                    Generator_qscale = genEvtInfo->qScale();
                    Generator_x1 = (*genEvtInfo->pdf()).x.first;
                    Generator_x2 = (*genEvtInfo->pdf()).x.second;
                    Generator_id1 = (*genEvtInfo->pdf()).id.first;
                    Generator_id2 = (*genEvtInfo->pdf()).id.second;
                    Generator_xpdf1 = (*genEvtInfo->pdf()).xPDF.first;
                    Generator_xpdf2 = (*genEvtInfo->pdf()).xPDF.second;
                    Generator_scalePDF = (*genEvtInfo->pdf()).scalePDF;
                    if(genEvtInfo->weights().size()>2){
			    LHEPSWeights[0] = genEvtInfo->weights()[2]/genEvtInfo->weights()[1];
                            LHEPSWeights[1] = genEvtInfo->weights()[3]/genEvtInfo->weights()[1];
                            LHEPSWeights[2] = genEvtInfo->weights()[4]/genEvtInfo->weights()[1];
                            LHEPSWeights[3] = genEvtInfo->weights()[5]/genEvtInfo->weights()[1];
                            LHEPSWeights[4] = genEvtInfo->weights()[24]/genEvtInfo->weights()[1];
                            LHEPSWeights[5] = genEvtInfo->weights()[25]/genEvtInfo->weights()[1];
                            LHEPSWeights[6] = genEvtInfo->weights()[26]/genEvtInfo->weights()[1];
                            LHEPSWeights[7] = genEvtInfo->weights()[27]/genEvtInfo->weights()[1];
                            wgt_isr_up = genEvtInfo->weights()[25]/genEvtInfo->weights()[1]; wgt_isr_dn = genEvtInfo->weights()[24]/genEvtInfo->weights()[1];
                            wgt_fsr_up = genEvtInfo->weights()[3] /genEvtInfo->weights()[1]; wgt_fsr_dn = genEvtInfo->weights()[2] /genEvtInfo->weights()[1];
                   }
             }
	     else
             {
                Generator_weight = Generator_qscale = Generator_x1 = Generator_x2 = Generator_id1 = Generator_id2 = Generator_xpdf1 = Generator_xpdf2 = Generator_scalePDF = -10000;
             }
	 }//is_signal 
       }//is_monte-carlo 
     else
       {
         _genWeight = 1.0;
         nPU = 1.0;
       }
      m_tree2->Fill();
      event_veto_map = false; 
      //**********************************************************// 
        const edm::TriggerNames &triggerNames_ = iEvent.triggerNames(*HLTR);
        string path_HLT_QuadPFJet103_88_75_15                =  "HLT_QuadPFJet103_88_75_15";
	string path_HLT_QuadPFJet105_88_76_15                =  "HLT_QuadPFJet105_88_76_15";
	string path_HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1                =  "HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1";
	string path_HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2                =  "HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2";
	string path_HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1                =  "HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1";
	string path_HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2                =  "HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2";


        HLT_QuadPFJet103_88_75_15 = false;
        HLT_QuadPFJet105_88_76_15 = false;
        HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 = false;
        HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2 = false;
        HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1  = false;
        HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2 = false;

	Int_t hsize = triggerNames_.size();
        for(int i = 0; i< hsize;i++)
        {
          if(triggerNames_.triggerName(i).find(path_HLT_QuadPFJet103_88_75_15) != string::npos)
	       {path_HLT_QuadPFJet103_88_75_15= triggerNames_.triggerName(i);}
	  else if(triggerNames_.triggerName(i).find(path_HLT_QuadPFJet105_88_76_15) != string::npos)
               {path_HLT_QuadPFJet105_88_76_15= triggerNames_.triggerName(i);}
	  else if(triggerNames_.triggerName(i).find(path_HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1) != string::npos)
               {path_HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1= triggerNames_.triggerName(i);}
	  else if(triggerNames_.triggerName(i).find(path_HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2) != string::npos)
               {path_HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2= triggerNames_.triggerName(i);}
	  else if(triggerNames_.triggerName(i).find(path_HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1) != string::npos)
               {path_HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1= triggerNames_.triggerName(i);}
	  else if(triggerNames_.triggerName(i).find(path_HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2) != string::npos)
               {path_HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2= triggerNames_.triggerName(i);}
	}//loop over trigger
      //Event trigger 

      Bool_t isHLT_QuadPFJet103_88_75_15  =  HLTR->accept(triggerNames_.triggerIndex(path_HLT_QuadPFJet103_88_75_15));
      Bool_t isHLT_QuadPFJet105_88_76_15  =  HLTR->accept(triggerNames_.triggerIndex(path_HLT_QuadPFJet105_88_76_15));
      Bool_t isHLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1  =  HLTR->accept(triggerNames_.triggerIndex(path_HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1));
      Bool_t isHLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2            =  HLTR->accept(triggerNames_.triggerIndex(path_HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2));
      Bool_t isHLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1  =  HLTR->accept(triggerNames_.triggerIndex(path_HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1));
      Bool_t isHLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2            =  HLTR->accept(triggerNames_.triggerIndex(path_HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2));

      //muon-eg trigger selection.	      
      HLT_QuadPFJet103_88_75_15                                  = isHLT_QuadPFJet103_88_75_15;
      HLT_QuadPFJet105_88_76_15                                  = isHLT_QuadPFJet105_88_76_15;
      HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 = isHLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1;
      HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2           = isHLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2;
      HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 = isHLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1;
      HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2           = isHLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2;
      if (HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 || HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2 || HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1 || HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2)
      {
      //Event informations
      _run = iEvent.id().run() ;
      _event = iEvent.id().event();
      _lumi = iEvent.luminosityBlock();
      //MET metfilters
      const edm::TriggerNames & metfilterName = iEvent.triggerNames(*METFilterResults);
      unsigned int goodVerticesIndex_ = metfilterName.triggerIndex("Flag_goodVertices");
      Flag_goodVertices_ = METFilterResults.product()->accept(goodVerticesIndex_);
      unsigned int globalSuperTightHalo2016FilterIndex_ = metfilterName.triggerIndex("Flag_globalSuperTightHalo2016Filter");
      Flag_globalSuperTightHalo2016Filter_ = METFilterResults.product()->accept(globalSuperTightHalo2016FilterIndex_);
      unsigned int EcalDeadCellTriggerPrimitiveFilterIndex_ = metfilterName.triggerIndex("Flag_EcalDeadCellTriggerPrimitiveFilter");
      Flag_EcalDeadCellTriggerPrimitiveFilter_ = METFilterResults.product()->accept(EcalDeadCellTriggerPrimitiveFilterIndex_);
      unsigned int BadPFMuonFilterIndex_ = metfilterName.triggerIndex("Flag_BadPFMuonFilter");
      Flag_BadPFMuonFilter_ = METFilterResults.product()->accept(BadPFMuonFilterIndex_);
      unsigned int BadPFMuonFilterDzIndex_ = metfilterName.triggerIndex("Flag_BadPFMuonDzFilter");
      Flag_BadPFMuonDzFilter_ = METFilterResults.product()->accept(BadPFMuonFilterDzIndex_);
      unsigned int hfNoisyHitsIndex_ = metfilterName.triggerIndex("Flag_hfNoisyHitsFilter");
      Flag_hfNoisyHitsFilter_ = METFilterResults.product()->accept(hfNoisyHitsIndex_);
      unsigned int eeBadScFilterIndex_ = metfilterName.triggerIndex("Flag_eeBadScFilter");
      Flag_eeBadScFilter_ = METFilterResults.product()->accept(eeBadScFilterIndex_);
      unsigned int ecalBadCalibFilterIndex_ = metfilterName.triggerIndex("Flag_ecalBadCalibFilter");
      Flag_ecalBadCalibFilter_ = METFilterResults.product()->accept(ecalBadCalibFilterIndex_);
      //MET
      //PUPPI MET
      edm::Handle< vector<pat::MET> > ThePUPPIMET;
      iEvent.getByToken(puppimetToken_, ThePUPPIMET);
      const vector<pat::MET> *puppimetcol = ThePUPPIMET.product();
      const pat::MET *puppimet;
      puppimet = &(puppimetcol->front());
      MET_pt = puppimet->pt();
      MET_phi = puppimet->phi();


      // Level1 Trigger
      edm::ESHandle<L1TUtmTriggerMenu> menu;
      menu = iSetup.getHandle(l1GtMenuToken_);
      for(auto const & keyval: menu->getAlgorithmMap())
       {
          if(keyval.second.getName() == "L1_HTT280er")           			 idx_L1_HTT280er = keyval.second.getIndex();
          if(keyval.second.getName() == "L1_HTT320er")           			 idx_L1_HTT320er = keyval.second.getIndex();
	  if(keyval.second.getName() == "L1_SingleJet160er2p5" ) 			 idx_L1_SingleJet160er2p5 = keyval.second.getIndex();
	  if(keyval.second.getName() == "L1_SingleJet180" )      			 idx_L1_SingleJet180 = keyval.second.getIndex();
	  if(keyval.second.getName() == "L1_SingleJet200" )      			 idx_L1_SingleJet200 = keyval.second.getIndex();
	  if(keyval.second.getName() == "L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5" ) idx_L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5 = keyval.second.getIndex();
	  if(keyval.second.getName() == "L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5" )  idx_L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5 = keyval.second.getIndex();
       }
      //std::cout << "idx_L1 bit check: " << idx_L1_HTT280er << " | " <<   idx_L1_HTT320er << " | " <<        idx_L1_SingleJet160er2p5<< " | " << idx_L1_SingleJet180<< " | " <<      idx_L1_SingleJet200<< " | " <<      idx_L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5 << std::endl;
      
      Bool_t isL1_HTT280er = false;
      Bool_t isL1_HTT320er = false;
      Bool_t isL1_SingleJet160er2p5 = false;
      Bool_t isL1_SingleJet180 = false;
      Bool_t isL1_SingleJet200 = false;
      Bool_t isL1_TripleJet_100_80_70_DoubleJet_80_70_er2p5 = false;
      Bool_t isL1_TripleJet_95_75_65_DoubleJet_75_65_er2p5 = false;
      edm::Handle<BXVector<GlobalAlgBlk>> l1GtHandle;
      iEvent.getByToken(l1GtToken_, l1GtHandle);
      if(l1GtHandle.isValid()){
       int ibx = 0;
         for(auto itr = l1GtHandle->begin(ibx); itr != l1GtHandle->end(ibx); ++itr)
	 {
           if(itr->getAlgoDecisionFinal(idx_L1_HTT280er))          {isL1_HTT280er = true;}
           if(itr->getAlgoDecisionFinal(idx_L1_HTT320er))          {isL1_HTT320er = true;}
	   if(itr->getAlgoDecisionFinal(idx_L1_SingleJet160er2p5)) {isL1_SingleJet160er2p5 = true;}
	   if(itr->getAlgoDecisionFinal(idx_L1_SingleJet180))      {isL1_SingleJet180 = true;}
	   if(itr->getAlgoDecisionFinal(idx_L1_SingleJet200))      {isL1_SingleJet200 = true;}
	   if(itr->getAlgoDecisionFinal(idx_L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5)) {isL1_TripleJet_100_80_70_DoubleJet_80_70_er2p5 = true;}
	   if(itr->getAlgoDecisionFinal(idx_L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5)) {isL1_TripleJet_95_75_65_DoubleJet_75_65_er2p5 = true;}
	   //std::cout << "isL1 bit check: " << itr->getAlgoDecisionFinal(406) << " | " << itr->getAlgoDecisionFinal(407) << std::endl;
         }
      }

      L1_HTT280er= isL1_HTT280er;
      L1_HTT320er= isL1_HTT320er;
      L1_SingleJet160er2p5= isL1_SingleJet160er2p5;
      L1_SingleJet180= isL1_SingleJet180;
      L1_SingleJet200= isL1_SingleJet200;
      L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5 = isL1_TripleJet_95_75_65_DoubleJet_75_65_er2p5;
      L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5= isL1_TripleJet_100_80_70_DoubleJet_80_70_er2p5;


      //std::cout << "L1 bit check: " << L1_HTT280er << " | " <<   L1_HTT320er << " | " <<	L1_SingleJet160er2p5<< " | " <<	L1_SingleJet180<< " | " <<	L1_SingleJet200<< " | " <<	L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5 << std::endl;
        
      //The event veto map 
      event_veto_map = false;
      Bool_t jetveto_map = false;
      
      for(auto jref = recjets->begin(); jref != recjets->end(); jref++)
      {
        if (jref->pt() > 15.0 && (jref->neutralEmEnergyFraction() + jref->chargedEmEnergyFraction()) < 0.9 && applyJetID(*jref,"tight")){ 
           TLorentzVector jet4v;
           jet4v.SetPtEtaPhiM(jref->pt(),jref->eta(),jref->phi(),jref->mass());
           for (const pat::Muon &mu : *muons)
           {
    	       TLorentzVector muon4v; 
	       muon4v.SetPtEtaPhiM(mu.pt(), mu.eta(), mu.phi(), mu.mass());
	       if( muon4v.DeltaR(jet4v) > 0.2 ){
	       Int_t bin_eta = h_vetomap->GetXaxis() -> FindBin(jref -> eta());
	       Int_t bin_phi = h_vetomap->GetYaxis() -> FindBin(jref -> phi());
	       if(h_vetomap  and h_vetomap->GetBinContent(bin_eta, bin_phi) > 0 ) jetveto_map = true;
	   }
         }
      }    
     } 
     event_veto_map = jetveto_map;

     //Gen-V-pT for the Z and W pT 
     gen_V_pt     .clear();
     gen_V_eta    .clear();
     gen_V_phi    .clear();
     gen_V_en     .clear();
     gen_V_pdgid  .clear();
     gen_V_status .clear();
     if(is_MC_)
       {
		edm::Handle<edm::View<reco::GenParticle> > pruned;
		iEvent.getByToken(prunedGenToken_, pruned);
		for(size_t i=0; i<pruned->size();i++)
		{
		       const Candidate * part_pru = (&(*pruned)[i]);
		       if ((&(*pruned)[i]) -> isHardProcess()) {
			      if( fabs(part_pru->pdgId()) == 25 || fabs(part_pru->pdgId()) == 24 || fabs(part_pru->pdgId()) == 23 || fabs(part_pru->pdgId()) == 35  )
				  {
					 gen_V_pt     .push_back(part_pru->pt())  ;
					 gen_V_eta    .push_back(part_pru->y()) ;
					 gen_V_phi    .push_back(part_pru->phi()) ;
					 gen_V_en     .push_back(part_pru->energy()) ;
					 gen_V_pdgid  .push_back(part_pru->pdgId())  ;
					 gen_V_status .push_back(part_pru->status()) ;
				  }
		       }
		}
       }

      //electron selection
      vector<double> ele_pt;
      vector<double> ele_eta;
      vector<double> ele_phi;
      vector<double> ele_en;
      vector<bool> ele_id_T;
      vector<bool> ele_id_veto;
      vector<int>    ele_ch;
      vector<double> ele_dxy;
      vector<double> ele_dz;
      for (const pat::Electron &el : *electrons)
      {
        if (el.pt() < 10 || fabs(el.eta()) > 2.5) continue;
        reco::GsfTrackRef gsftrk1 = el.gsfTrack();
        Float_t el_dxy_ = gsftrk1->dxy(vertex.position());
        Float_t el_dz_  = gsftrk1->dz(vertex.position());
        //storing the electron informations
        ele_dxy.push_back(el_dxy_);
        ele_dz.push_back(el_dz_);
        ele_pt.push_back(el.pt());
        ele_eta.push_back(el.eta());
        ele_phi.push_back(el.phi());
        ele_en.push_back(el.energy());
        ele_ch.push_back(el.charge());
        ele_id_veto.push_back(el.electronID("cutBasedElectronID-RunIIIWinter22-V1-veto"));
     }
    
     //veto of loose electron
     bool extraele  = false;
     for(int pp = 0; pp < int(ele_pt.size()); pp++)
     {
       if(ele_id_veto.at(pp))
       {
         if( ( ele_eta.at(pp) < 1.445 && fabs(ele_dxy.at(pp)) < (dxy_cut) && fabs(ele_dz.at(pp)) < (dz_cut)) || ( ele_eta.at(pp) > 1.445 && fabs(ele_dxy.at(pp)) < (2.0*dxy_cut) && fabs(ele_dz.at(pp)) < (2.0* dz_cut)) )  
          {
            extraele = true;
            break;
          }
       }
     }
     //std::cout << "The electron status :  good electron "  << isGDele << " | loose electron " << extraele << std::endl;
     //**********************************************************//

     //muon selection
     vector<double> mu_pt;
     vector<double> mu_eta;
     vector<double> mu_phi;
     vector<double> mu_en;
     vector<bool>   mu_id_T;
     vector<bool>   mu_id_L;
     vector<double> mu_iso;
     vector<double> mu_dxy;
     vector<double> mu_dz;
     vector<int>    mu_ch;
     for (const pat::Muon &mu : *muons)
     {
        if (mu.pt() < 10 || fabs(mu.eta()) > 2.4) continue;
        Float_t mu_dxy_ = mu.muonBestTrack()->dxy(vertex.position());;
        Float_t mu_dz_  = mu.muonBestTrack()->dz(vertex.position());;
        Float_t Muon_pfiso = (mu.pfIsolationR04().sumChargedHadronPt + max(0., mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt))/mu.pt();
        //storing the muon informations
        mu_dxy.push_back(mu_dxy_);
        mu_dz.push_back(mu_dz_);
        mu_iso.push_back(Muon_pfiso);
        mu_pt.push_back(mu.pt());
        mu_eta.push_back(mu.eta());
        mu_phi.push_back(mu.phi());
        mu_en.push_back(mu.energy());
        mu_ch.push_back(mu.charge());
        mu_id_T.push_back(mu.passed(reco::Muon::CutBasedIdTight));
        mu_id_L.push_back(mu.passed(reco::Muon::CutBasedIdLoose));
     }
    
    //veto of loose muons
    bool extramu  = false;
    for(int pp = 0; pp < int(mu_pt.size()); pp++)
    {
      if(mu_id_L.at(pp) && mu_iso.at(pp) < 0.25 && mu_iso.at(pp) < 0.15 && fabs(mu_dxy.at(pp)) < 0.2 && fabs(mu_dz.at(pp)) < 0.5)
      {
        extramu = true;
        break;
      }
    }
    //  std::cout << "The mu status :  good muon "  << isGDmu << " | loose muon " << extramu << std::endl;
    //**********************************************************//
      std::vector<reco::GenJetRef> jetv_gen;   
      if(genJetsH.isValid()){
          for (auto jets_iter = genJetsH->begin(); jets_iter != genJetsH->end(); ++jets_iter) {                                                                                                   
          reco::GenJetRef jref (genJetsH, jets_iter - genJetsH->begin());                                                                                                                      
         jetv_gen.push_back(jref);                                                                                                                                                              
      }
      sort(jetv_gen.begin(), jetv_gen.end(), genJetRefSorter);
    }

    //The jet selection
    jet_pt.clear();
    jet_eta.clear();
    jet_phi.clear();
    jet_en.clear();
    jet_mass.clear();
    jet_id.clear();
    jet_PNBTG.clear();
    jet_PNQGL.clear();
    jet_pflv.clear();
    jet_hflv.clear();
    jet_pt_raw.clear();
    jet_mass_raw.clear();
    jet_energy_raw.clear();
    jet_jec.clear();
    jet_jec_L2.clear();
    jet_jec_L3.clear();
    jet_jec_L2L3.clear();
    jet_jerc_sf.clear();
    jet_jerc_sf_up.clear();
    jet_jerc_sf_dw.clear();
    jet_jerc_reso.clear();
    jet_genmatch_pt.clear();
    jet_genmatch_eta.clear();
    jet_genmatch_phi.clear();
    jet_genmatch_mass.clear();
    jet_genmatch_energy.clear();
    jet_pnet_jec.clear();
    jet_pnet_ptcorr.clear();
    jet_pnet_ptnu.clear();
    jet_pnet_ptres.clear();    
    jet_jec_unc.clear();
    jet_pnet_jec_unc.clear();
    njet = 0;
    HT = 0.0 ;

    for(auto jet = recjets->begin(); jet != recjets->end(); jet++)
      {
        if(jet->pt() < 25.0 || fabs(jet->eta()) > 4.7) continue;
        TLorentzVector recojet;
        recojet.SetPtEtaPhiM(jet->pt(),jet->eta(),jet->phi(),jet->mass());
        Bool_t jet_id_   =  applyJetID(*jet,"tight");
	jet_pnet_ptcorr.push_back(jet->bDiscriminator("pfParticleNetFromMiniAODAK4PuppiCentralJetTags:ptnu"));
        jet_pnet_ptnu.push_back(jet->bDiscriminator("pfParticleNetFromMiniAODAK4PuppiCentralJetTags:ptcorr"));
	jet_pnet_ptres.push_back( 0.5*(jet->bDiscriminator("pfParticleNetFromMiniAODAK4PuppiCentralJetTags:ptreshigh") - jet->bDiscriminator("pfParticleNetFromMiniAODAK4PuppiCentralJetTags:ptreslow")));
        Float_t jet_btag =  jet->bDiscriminator("pfParticleNetFromMiniAODAK4PuppiCentralDiscriminatorsJetTags:BvsAll") ;
	Float_t jet_qgtag =  jet->bDiscriminator("pfParticleNetFromMiniAODAK4PuppiCentralDiscriminatorsJetTags:QvsG");
	//preliminary variables
        jet_PNBTG.push_back(jet_btag);
        jet_PNQGL.push_back(jet_qgtag);
	//raw objects
        jet_pt_raw.push_back(jet->correctedJet("Uncorrected").pt());
        jet_mass_raw.push_back(jet->correctedJet("Uncorrected").mass());
        jet_energy_raw.push_back(jet->correctedJet("Uncorrected").energy());
	//Rederivation of the JEC
	float jet_jec_l1 = jec_corr_l1->evaluate({0.4,jet->eta(),jet_pt_raw.back(),rho});
        float jet_jec_l2 = jec_corr_l2->evaluate({jet->eta(),jet_pt_raw.back()*jet_jec_l1});
	float jet_jec_l3 = jec_corr_l3->evaluate({jet->eta(),jet_pt_raw.back()*jet_jec_l1*jet_jec_l2});
        float jet_jec_l2l3res = jec_corr_l2l3res->evaluate({jet->eta(),jet_pt_raw.back()*jet_jec_l1*jet_jec_l2*jet_jec_l3});
        //Rederivation of the pnet JEC
	float jet_jec_pnet_l2 = jec_corr_pnet_l2->evaluate({jet->eta(),jet_pt_raw.back()});
	float jet_jec_pnet_l2l3res = jec_corr_pnet_l2l3res->evaluate({jet->eta(),jet_pt_raw.back()*jet_jec_pnet_l2});

        jet_pnet_jec.push_back(jet_jec_pnet_l2*jet_jec_pnet_l2l3res);	        
	jet_jec.push_back(jet_jec_l1*jet_jec_l2*jet_jec_l3*jet_jec_l2l3res);
	jet_jec_L2.push_back(jet_jec_l2);
        jet_jec_L3.push_back(jet_jec_l3);
        jet_jec_L2L3.push_back(jet_jec_l2l3res);	
	//corrected jet
	TLorentzVector jet_raw; jet_raw.SetPtEtaPhiM(jet->correctedJet("Uncorrected").pt(),jet->eta(),jet->phi(),jet->correctedJet("Uncorrected").mass());
        TLorentzVector jet_corrected;
	jet_corrected = jet_raw * jet_jec_l1*jet_jec_l2*jet_jec_l3*jet_jec_l2l3res;
	jet_pt.push_back(jet_corrected.Pt());
        jet_eta.push_back(jet_corrected.Eta());
        jet_phi.push_back(jet_corrected.Phi());
        jet_en.push_back(jet_corrected.E());
	jet_mass.push_back(jet_corrected.M());
        jet_id.push_back(jet_id_);
        if(is_MC_)
        {
           jet_jec_unc.push_back(jec_corr_unc->evaluate({jet->eta(),jet_pt_raw.back()}) );
           jet_pnet_jec_unc.push_back(jec_corr_pnet_unc->evaluate({jet->eta(),jet_pt_raw.back()}) ); 	   
           jet_pflv.push_back(jet->hadronFlavour());
           jet_hflv.push_back(jet->partonFlavour());
	   jet_jerc_sf.push_back(jerc_corr->evaluate({jet->eta(),jet->pt(),"nom"}));	   
	   jet_jerc_sf_up.push_back(jerc_corr->evaluate({jet->eta(),jet->pt(),"up"}));
	   jet_jerc_sf_dw.push_back(jerc_corr->evaluate({jet->eta(),jet->pt(),"down"}));
	   double rp = jerc_reso->evaluate({jet->eta(),jet->pt(),rho});
	   jet_jerc_reso.push_back(rp);
	   int pos_matched = -1;
           float minDR = 0.4;
           for(size_t igen = 0; igen < jetv_gen.size(); igen++){
              if(reco::deltaR(jetv_gen[igen]->p4(),jet->p4()) < minDR && fabs(jetv_gen[igen]->pt() - jet->pt()) < 3*rp*jet->pt()){
                 pos_matched = igen;
                 minDR = reco::deltaR(jetv_gen[igen]->p4(),jet->p4());
              }
           }

           if(pos_matched >= 0){
                jet_genmatch_pt.push_back(jetv_gen[pos_matched]->pt());
      		jet_genmatch_eta.push_back(jetv_gen[pos_matched]->eta());
                jet_genmatch_phi.push_back(jetv_gen[pos_matched]->phi());
                jet_genmatch_mass.push_back(jetv_gen[pos_matched]->mass());
                jet_genmatch_energy.push_back(jetv_gen[pos_matched]->energy());
           }
           else{
                jet_genmatch_pt.push_back(0);
                jet_genmatch_eta.push_back(0);
                jet_genmatch_phi.push_back(0);
                jet_genmatch_mass.push_back(0);
                jet_genmatch_energy.push_back(0);
               }
        }
        else
	{
           jet_pflv.push_back(-1);
           jet_hflv.push_back(-1);
	   jet_jerc_sf.push_back(1);
	   jet_jerc_reso.push_back(0);
	   jet_genmatch_pt.push_back(0);
           jet_genmatch_eta.push_back(0);
           jet_genmatch_phi.push_back(0);
           jet_genmatch_mass.push_back(0);
           jet_genmatch_energy.push_back(0);
	   jet_jerc_sf_up.push_back(1);
	   jet_jerc_sf_dw.push_back(1);
	   jet_jec_unc.push_back(0);
	   jet_pnet_jec_unc.push_back(0);
	}

        njet++;
      }//jet

      nL1jet=0; nCalojet=0; nL3jet = 0; nPxljet = 0;
      L1jet_pt.clear(); L1jet_eta.clear(); L1jet_phi.clear(); L1jet_en.clear();
      Calojet_pt.clear(); Calojet_eta.clear(); Calojet_phi.clear(); Calojet_en.clear();
      Pxljet_pt.clear(); Pxljet_eta.clear(); Pxljet_phi.clear(); Pxljet_en.clear();
      L3jet_pt.clear(); L3jet_eta.clear(); L3jet_phi.clear(); L3jet_en.clear();

      nCalobjet = 0; nL3b1jet = 0; nL3b2jet = 0;
      Calobjet_pt.clear(); Calobjet_eta.clear(); Calobjet_phi.clear(); Calobjet_en.clear();
      L3b1jet_pt.clear(); L3b1jet_eta.clear(); L3b1jet_phi.clear(); L3b1jet_en.clear();
      L3b2jet_pt.clear(); L3b2jet_eta.clear(); L3b2jet_phi.clear(); L3b2jet_en.clear();

      //L1, CALO and HLT level jet selections
      edm::Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects;
      iEvent.getByToken(triggerObjects_, triggerObjects);
      const edm::TriggerNames &names_ = iEvent.triggerNames(*HLTR);
      for(pat::TriggerObjectStandAlone obj : *triggerObjects)
      {
          obj.unpackPathNames(names_);
	  obj.unpackFilterLabels(iEvent,*HLTR);
           //L1
           if(obj.collection()=="hltGtStage2Digis:Jet:HLT")
            {
              L1jet_pt.push_back(obj.pt());
              L1jet_eta.push_back(obj.eta());
              L1jet_phi.push_back(obj.phi());
              L1jet_en.push_back(obj.energy());
              nL1jet++;
            }
           //Calo
           if(obj.collection()=="hltAK4CaloJetsCorrectedIDPassed::HLT")
            {
              Calojet_pt.push_back(obj.pt());
              Calojet_eta.push_back(obj.eta());
              Calojet_phi.push_back(obj.phi());
              Calojet_en.push_back(obj.energy());
              nCalojet++;
            }
           //Pixel 
           if(obj.collection()=="hltAK4PixelOnlyPFJetsTightIDCorrected::HLT") 
           {
              Pxljet_pt.push_back(obj.pt());
              Pxljet_eta.push_back(obj.eta());
              Pxljet_phi.push_back(obj.phi());
              Pxljet_en.push_back(obj.energy());
              nPxljet++;
           }
           //L3-HLT
           if(obj.collection()=="hltAK4PFJetsLooseIDCorrected::HLT")
            {
              L3jet_pt.push_back(obj.pt());
              L3jet_eta.push_back(obj.eta());
              L3jet_phi.push_back(obj.phi());
              L3jet_en.push_back(obj.energy());
              nL3jet++;
            }            
	   //single and double b-tag HLT obj from Armen
	   //Calo b-tag HLT objects
	   
	   if(obj.collection()=="hltSelector8CentralJetsL1FastJet::HLT")     // Calo online BTag
           {
             if(obj.filterLabels().size()<1) continue;
             for(int iFilt=0; iFilt<(int)obj.filterLabels().size(); iFilt++)
             {
               if(obj.filterLabels()[iFilt]=="hltBTagCaloDeepCSV1p56Single")  // online Calo BTag > 0.4
               {
                  if(nCalobjet>5) continue;
                  Calobjet_pt.push_back(obj.pt());
		  Calobjet_eta.push_back(obj.eta());
		  Calobjet_phi.push_back(obj.phi());
		  Calobjet_en.push_back(obj.energy());
                  nCalobjet++;
               }
             }
           }
           //HLT b-tag obj
	   if(obj.collection()=="hltSelector6PFJets::HLT")                       // L3 online BTag
           {
              if(obj.filterLabels().size()<1) continue;
              for(int iFilt=0; iFilt<(int)obj.filterLabels().size(); iFilt++)
              {
                 if(obj.filterLabels()[iFilt]=="hltBTagPFDeepJet1p28Single6Jets")  // online L3 BTag > 0.43
                 {
                   if(nL3b1jet>5) continue;
		   L3b1jet_pt.push_back(obj.pt());
                   L3b1jet_eta.push_back(obj.eta());
                   L3b1jet_phi.push_back(obj.phi());
                   L3b1jet_en.push_back(obj.energy());
                   nL3b1jet++;
                 }
                if(obj.filterLabels()[iFilt]=="hltBTagPFDeepJet7p68Double6Jets")   // online L3 BTag > 0.16
                {
		   if(nL3b2jet>5) continue;
                   L3b2jet_pt.push_back(obj.pt());
                   L3b2jet_eta.push_back(obj.eta());
                   L3b2jet_phi.push_back(obj.phi());
                   L3b2jet_en.push_back(obj.energy());
                   nL3b2jet++;	
                }
              }
            }
      }
      //storing HLT results.
     
      if(njet > 3)
        {
          if(!extramu && !extraele )
          {
                m_tree->Fill();
          }
        }
      } //HLT selection 
        //**********storing the trigger results******************//
      //control trigger selection
  }

void finalanalyser::beginJob() {
   f_vetomap = TFile::Open(vetoMapFile.c_str());
   h_vetomap = (TH2D * ) f_vetomap -> Get("jetvetomap");   
   auto jet_correction_file = CorrectionSet::from_file(jec_jerc_file.c_str());
   jerc_corr = jet_correction_file->at(jerc_corr_tag);
   jerc_reso = jet_correction_file->at(jerc_reso_tag);
   jec_corr_l1 = jet_correction_file->at(jec_corr_l1_tag);
   jec_corr_l2 = jet_correction_file->at(jec_corr_l2_tag);	   
   jec_corr_l3 = jet_correction_file->at(jec_corr_l3_tag);  
   jec_corr_l2l3res = jet_correction_file->at(jec_corr_l2l3res_tag);
   jec_corr_pnet_l2 = jet_correction_file->at(jec_corr_pnet_l2_tag);
   jec_corr_pnet_l2l3res = jet_correction_file->at(jec_corr_pnet_l2l3res_tag);
   jec_corr_pnet_unc = jet_correction_file->at(jec_corr_pnet_unc_tag);
   jec_corr_unc = jet_correction_file->at(jec_corr_unc_tag);
  // ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
}

void finalanalyser::endJob() {
}

void
finalanalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
// to apply jet ID: https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVUL                                                                                                                             
bool finalanalyser::applyJetID(const pat::Jet & jet, const std::string & level){

  if(level != "tight" and level != "tightLepVeto")
    return true;

  double eta  = jet.eta();
  double nhf  = jet.neutralHadronEnergyFraction();
  double nemf = jet.neutralEmEnergyFraction();
  double chf  = jet.chargedHadronEnergyFraction();
  double muf  = jet.muonEnergyFraction();
  double cemf = jet.chargedEmEnergyFraction();
  int    np   = jet.chargedMultiplicity()+jet.neutralMultiplicity();
  int    chm  = jet.chargedMultiplicity();
  int    NumNeutralParticle = jet.neutralMultiplicity();
  int jetid  = 0;
  if (fabs(eta) <= 2.6 and (nhf < 0.99 and nemf < 0.90 and np > 1 and chf > 0.01 and chm > 0 and muf < 0.8 and cemf < 0.8)) jetid += 1;
  else if ( (fabs(eta)>2.6 and fabs(eta)<=2.7) and (cemf<0.8 and nemf<0.99 and muf <0.8 and nhf < 0.9 )) jetid += 1;
  else if ( (fabs(eta)>2.7 and fabs(eta)<=3.0) and (nhf < 0.9999)) jetid += 1;
  else if ( fabs(eta)>3.0 and ( nemf<0.90 and NumNeutralParticle>2 )) jetid += 1;

  if(level == "tight" and jetid > 0) return true;
  else if(level == "tightLepVeto" and jetid > 1) return true;
  else return false;

}

//define this as a plug-in
DEFINE_FWK_MODULE(finalanalyser);
