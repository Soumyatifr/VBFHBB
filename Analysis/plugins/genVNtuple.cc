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
using namespace fastjet;
using namespace std;
//using namespace fastjet::contrib;
class genVNtuple : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit genVNtuple(const edm::ParameterSet&);
      ~genVNtuple();
      
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      int getNPU(edm::Handle <std::vector <PileupSummaryInfo> >  puInfo);
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      // ----------member data ---------------------------
       //edm::EDGetTokenT <reco::GenParticleCollection> genparticlesToken;
       edm::EDGetToken l1GtToken_;
       edm::EDGetTokenT <GenEventInfoProduct> genInfoToken_;
       edm::EDGetTokenT <edm::View<reco::GenParticle>> prunedGenToken_;


TTree* m_tree;

double weight;
vector<double> gen_V_pt, gen_V_eta, gen_V_phi, gen_V_en ;
vector<int> gen_V_pdgid, gen_V_status;

//******************************************************************//
};

genVNtuple::genVNtuple(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   genInfoToken_        = consumes <GenEventInfoProduct>  (iConfig.getParameter<edm::InputTag>("GenInf"));
   prunedGenToken_      = consumes <edm::View<reco::GenParticle>> (iConfig.getParameter<edm::InputTag>("pruned"));


usesResource("TFileService");   
edm::Service<TFileService> fs;
m_tree = fs->make<TTree>("tree", "");
//*********************Branches for GsfElectron***********************//
m_tree->Branch("weights",&weight );
m_tree->Branch("gen_V_pt",&gen_V_pt);
m_tree->Branch("gen_V_eta",&gen_V_eta);
m_tree->Branch("gen_V_phi",&gen_V_phi);
m_tree->Branch("gen_V_en",&gen_V_en);
m_tree->Branch("gen_V_pdgid",&gen_V_pdgid);
m_tree->Branch("gen_V_status",&gen_V_status);
}





genVNtuple::~genVNtuple()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

void genVNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
    using namespace edm;
    using namespace reco;
    using namespace pat;
    using namespace std;

    //generator event information    
    edm::Handle<GenEventInfoProduct> genEvtInfo;
    iEvent.getByToken(genInfoToken_, genEvtInfo);
    weight=genEvtInfo->weight();

    //Higgs and b-qarks information
    gen_V_pt     .clear(); 
    gen_V_eta    .clear(); 
    gen_V_phi    .clear(); 
    gen_V_en     .clear(); 
    gen_V_pdgid  .clear(); 
    gen_V_status .clear(); 

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
     m_tree->Fill();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
genVNtuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


//define this as a plug-in
DEFINE_FWK_MODULE(genVNtuple);
