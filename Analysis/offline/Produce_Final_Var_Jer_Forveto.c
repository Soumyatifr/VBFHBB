#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCut.h"
#include "TPaveText.h"
#include "TGaxis.h"
#include "TProfile.h"
#include "TCut.h"
#include <iostream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "correction.h"
#include <map>
using namespace std;
using correction::CorrectionSet;

float get_QCD_cor(TFile *file_SF, float pt, int pdgId, int choice = 1, float pt_min=100)
{
	// choice = 2-> taking cor from dark matter paper
	// choice = 1-> taking cor from CMS NLO samples
	// any other option-> taking cor from UHH files

	if(choice==0) { pt_min = 150; }

	char name[100];

	if(choice==1) { sprintf(name,"kFactor"); }
	else if(choice==2) {
		if      (pdgId==23){ sprintf(name,"kFactor"); }
		else if (pdgId==24){ sprintf(name,"kFactor"); }
		else { sprintf(name,"kfactor"); }
	}
	else { sprintf(name,"kfactor"); }

	//cout<<name<<endl;

	TH1F *h_qcd = (TH1F*)file_SF->Get(name);
	float cor = 1;
	if(pt>=pt_min){
		int pt_bin_id = h_qcd->GetXaxis()->FindBin(pt);
		cor = h_qcd->GetBinContent(pt_bin_id);
	}

	return cor;

}

double rew[100]= { 1.0,1.0,1.012,1.0152,1.0805,1.0043,1.0021,1.0098,2.8052,1.58579,0.940009,0.590456,0.357289,0.228586,0.178971,0.179546,0.204174,0.23343,0.256832,0.270224,0.28062,0.293677,0.320289,0.370999,0.439896,0.508875,0.562734,0.59917,0.625077,0.646724,0.663765,0.680047,0.692118,0.703977,0.716711,0.732862,0.756295,0.791354,0.837351,0.900965,0.981485,1.07731,1.18672,1.30742,1.42727,1.54666,1.65179,1.73941,1.80448,1.84491,1.85677,1.84475,1.81642,1.76972,1.72028,1.66507,1.62295,1.597,1.59949,1.62128,1.70122,1.80899,2.00402,2.25853,2.67455,3.26791,4.05272,5.32682,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


double PT0_Bins[]       ={0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 140, 160, 180, 200, 250, 300};
double SF_PT0_EtaReg1[] = {0.0, 0.0, 0.0, 0.0, 0.0, 11.0509, 0.0, 0.0, 1.25479, 2.03457, 1.47238, 0.55579, 1.18833, 1.01826, 1.24197, 1.71714, 3.27883, 4.93734, 5.65309, 5.32201, 3.29119, 1.74085, 1.31088, 1.14346, 1.07965, 1.03915, 1.02112, 1.00702, 1.00043, 1.00106, 0.99981, 1.00029, 1.00016};
double SF_PT0_EtaReg2[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 11.3251, 2.99658, 0.0, 0.0, 0.624112, 0.924899, 2.1248, 0.475138, 0.47411, 0.49557, 0.383104, 0.305908, 0.224528, 0.301616, 0.354611, 0.466372, 0.550506, 0.701743, 0.881962, 0.974212, 0.991039, 0.999178, 1.00023, 0.995803};
double SF_PT0_EtaReg3[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.3757, 0.0, 0.0, 34.5788, 0.0, 2.08671, 0.0, 0.25811, 0.14923, 0.0309905, 0.0, 0.0251673, 0.0, 0.0123536, 0.0184362, 0.0355507, 0.147387, 0.504572, 0.827283, 0.968024, 1.00002, 1.00005};
double SF_PT0_EtaReg4[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.53638, 0.183798, 0.314462, 0.30386, 0.491737, 0.550823, 0.704411, 0.827074, 0.960935, 0.995859, 1.00034, 1.0014, 1.05532, 1.00057};


double PT1_Bins[]       ={0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 140, 160, 180, 200, 250, 300};
double SF_PT1_EtaReg1[] = {0.0, 0.0, 0.0, 0.0, 2.13135, 2.78998, 0.527515, 2.56854, 0.767259, 1.69787, 1.75429, 1.04814, 1.99358, 2.47766, 3.43583, 4.43078, 3.87946, 2.22869, 1.35175, 1.13882, 1.05949, 1.02513, 1.0216, 1.01032, 1.0051, 1.00188, 1.00277, 1.00136, 0.999716, 1.00072, 0.999326, 1.00019, 1.00015};
double SF_PT1_EtaReg2[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.13138, 0.0, 0.0, 5.85005, 3.17611, 0.28415, 0.932973, 1.61781, 0.935322, 0.901248, 0.423986, 0.428084, 0.430644, 0.462794, 0.591475, 0.676163, 0.768372, 0.798523, 0.887473, 0.901272, 0.952396, 0.985096, 0.995563, 0.999258, 1.0003, 1.00021, 0.995785};
double SF_PT1_EtaReg3[] = {0.0, 0.0, 0.0, 0.0, 3.66116, 0.0, 0.0, 0.0, 0.0, 0.0, 4.24705, 0.0, 0.0, 2.97645, 0.0, 0.131532, 0.115802, 0.0263003, 0.0729371, 0.0249667, 0.0311853, 0.101818, 0.143213, 0.133253, 0.225446, 0.343509, 0.635904, 0.890706, 0.975111, 0.993753, 1.00002, 1.00005};
double SF_PT1_EtaReg4[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 31.7079, 0.0, 0.0, 0.316675, 0.378778, 0.469926, 0.493142, 0.607977, 0.767111, 0.849917, 0.906676, 0.951684, 0.965165, 0.995955, 0.993589, 0.995534, 1.00034, 1.0014, 1.05532, 1.00057};

double PT2_Bins[]       ={0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 140, 160, 180, 200, 250, 300};
double SF_PT2_EtaReg1[] = {0.0, 0.0, 0.0, 0.0, 1.82996, 1.58324, 0.38687, 2.03892, 1.79683, 1.77969, 2.54273, 2.46104, 3.2276, 3.57512, 2.63893, 1.51628, 1.13916, 1.03274, 1.01485, 1.00598, 1.00191, 1.0008, 0.999311, 1.00064, 0.999346, 1.00046, 1.00024, 1.00024, 0.9996, 1.00008, 1.00003, 1.00001, 1.00001};
double SF_PT2_EtaReg2[] = {0.0, 0.0, 0.0, 0.0, 2.65419, 1.20604, 0.956814, 0.0, 2.51623, 2.09578, 2.27987, 0.442801, 0.807847, 1.057, 0.686449, 0.591065, 0.664847, 0.764846, 0.84826, 0.903732, 0.947258, 0.972275, 0.986302, 0.986298, 0.991627, 0.994129, 0.997018, 0.999625, 1.00038, 1.00008, 1.0, 1.00001, 1.0};
double SF_PT2_EtaReg3[] = {0.0, 0.0, 0.0, 0.0, 0.0, 4.03612, 0.0, 3.5043, 0.0, 0.824209, 0.0, 1.74722, 0.538814, 0.38252, 0.40369, 0.198317, 0.135944, 0.23191, 0.338774, 0.385255, 0.454626, 0.48123, 0.654233, 0.745871, 0.765383, 0.843781, 0.88369, 0.956502, 0.991952, 1.00001, 1.00002, 1.0, 1.00001};
double SF_PT2_EtaReg4[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.14494, 0.430612, 0.718734, 0.606497, 0.6427, 0.781925, 0.834109, 0.929077, 0.948968, 1.00019, 0.993464, 1.0008, 1.0, 1.0006, 1.00018, 0.998571, 1.0, 1.00034, 1.0, 1.05532, 1.0};

double Pt0_Eff_Fact_1eta(double x) {
    int nbins = sizeof(PT0_Bins) / sizeof(PT0_Bins[0]);
    if (x < PT0_Bins[0])
        return 1;
    if (x >= PT0_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT0_Bins[PT_Interval] && x < PT0_Bins[PT_Interval + 1])
            return SF_PT0_EtaReg1[PT_Interval];
    }
    return 1;
}

double Pt0_Eff_Fact_2eta(double x) {
    int nbins = sizeof(PT0_Bins) / sizeof(PT0_Bins[0]);
    if (x < PT0_Bins[0])
        return 1;
    if (x >= PT0_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT0_Bins[PT_Interval] && x < PT0_Bins[PT_Interval + 1])
            return SF_PT0_EtaReg2[PT_Interval];
    }
    return 1;
}

double Pt0_Eff_Fact_3eta(double x) {
    int nbins = sizeof(PT0_Bins) / sizeof(PT0_Bins[0]);
    if (x < PT0_Bins[0])
        return 1;
    if (x >= PT0_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT0_Bins[PT_Interval] && x < PT0_Bins[PT_Interval + 1])
            return SF_PT0_EtaReg3[PT_Interval];
    }
    return 1;
}

double Pt0_Eff_Fact_4eta(double x) {
    int nbins = sizeof(PT0_Bins) / sizeof(PT0_Bins[0]);
    if (x < PT0_Bins[0])
        return 1;
    if (x >= PT0_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT0_Bins[PT_Interval] && x < PT0_Bins[PT_Interval + 1])
            return SF_PT0_EtaReg4[PT_Interval];
    }
    return 1;
}

double Pt1_Eff_Fact_1eta(double x) {
    int nbins = sizeof(PT1_Bins) / sizeof(PT1_Bins[0]);
    if (x < PT1_Bins[0])
        return 1;
    if (x >= PT1_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT1_Bins[PT_Interval] && x < PT1_Bins[PT_Interval + 1])
            return SF_PT1_EtaReg1[PT_Interval];
    }
    return 1;
}

double Pt1_Eff_Fact_2eta(double x) {
    int nbins = sizeof(PT1_Bins) / sizeof(PT1_Bins[0]);
    if (x < PT1_Bins[0])
        return 1;
    if (x >= PT1_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT1_Bins[PT_Interval] && x < PT1_Bins[PT_Interval + 1])
            return SF_PT1_EtaReg2[PT_Interval];
    }
    return 1;
}

double Pt1_Eff_Fact_3eta(double x) {
    int nbins = sizeof(PT1_Bins) / sizeof(PT1_Bins[0]);
    if (x < PT1_Bins[0])
        return 1;
    if (x >= PT1_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT1_Bins[PT_Interval] && x < PT1_Bins[PT_Interval + 1])
            return SF_PT1_EtaReg3[PT_Interval];
    }
    return 1;
}

double Pt1_Eff_Fact_4eta(double x) {
    int nbins = sizeof(PT1_Bins) / sizeof(PT1_Bins[0]);
    if (x < PT1_Bins[0])
        return 1;
    if (x >= PT1_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT1_Bins[PT_Interval] && x < PT1_Bins[PT_Interval + 1])
            return SF_PT1_EtaReg4[PT_Interval];
    }
    return 1;
}

double Pt2_Eff_Fact_1eta(double x) {
    int nbins = sizeof(PT2_Bins) / sizeof(PT2_Bins[0]);
    if (x < PT2_Bins[0])
        return 1;
    if (x >= PT2_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT2_Bins[PT_Interval] && x < PT2_Bins[PT_Interval + 1])
            return SF_PT2_EtaReg1[PT_Interval];
    }
    return 1;
}

double Pt2_Eff_Fact_2eta(double x) {
    int nbins = sizeof(PT2_Bins) / sizeof(PT2_Bins[0]);
    if (x < PT2_Bins[0])
        return 1;
    if (x >= PT2_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT2_Bins[PT_Interval] && x < PT2_Bins[PT_Interval + 1])
            return SF_PT2_EtaReg2[PT_Interval];
    }
    return 1;
}

double Pt2_Eff_Fact_3eta(double x) {
    int nbins = sizeof(PT2_Bins) / sizeof(PT2_Bins[0]);
    if (x < PT2_Bins[0])
        return 1;
    if (x >= PT2_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT2_Bins[PT_Interval] && x < PT2_Bins[PT_Interval + 1])
            return SF_PT2_EtaReg3[PT_Interval];
    }
    return 1;
}

double Pt2_Eff_Fact_4eta(double x) {
    int nbins = sizeof(PT2_Bins) / sizeof(PT2_Bins[0]);
    if (x < PT2_Bins[0])
        return 1;
    if (x >= PT2_Bins[nbins-1])
        return 1;
    for (int PT_Interval = 0; PT_Interval < nbins; PT_Interval++) {
        if (x >= PT2_Bins[PT_Interval] && x < PT2_Bins[PT_Interval + 1])
            return SF_PT2_EtaReg4[PT_Interval];
    }
    return 1;
}

double phi_Dist(double a, double b){
   if(fabs(a - b) > 3.14159265)
    return 6.2831853 - fabs(a - b);
   return fabs(a - b);
}
double jer_corrected_values(double reco_pt, double gen_pt, double resolution, double sf) {
    // Calculate the corrected value
    double correction = 0;
    if(gen_pt > 0 )
    {
       correction = (sf - 1.0) * (reco_pt - gen_pt) / reco_pt;
    }
    else 
    {
       correction = sqrt(std::max(0.,(sf*sf-1))) * resolution;
    }
    return correction;
}


void Produce_Final_Var_Jer_Forveto()
  {     
        Bool_t debug = false;
        //HLT Level cuts
	double PNet_medium_BTag = 0.2605;
     	double HLT_Mqq = 200.0;
	double HLT_dETAqq = 1.5;
    	double HLT_dPHIbb = 2.8; 
        Double_t lumi = 1.0;
	Long64_t ntotalhfill =0;
        Long64_t nvetohfill =0;
        double btag_weight_central;
	auto cset = CorrectionSet::from_file("btagging_2022postEE.json");
        cset->validate();
        auto cset_sf = cset->at("particleNet_shape");

	//***************************************************************************************//
        //                                     V-pT reweighting                                  //
        //***************************************************************************************//
        TFile *ZJets_cor_file;
        TFile *WJets_cor_file;
        ZJets_cor_file = new TFile("VJets_Cor/Z_Gen_pT_kFactor_NLOvsLO_13p6TeV.root","read");
        WJets_cor_file = new TFile("VJets_Cor/W_Gen_pT_kFactor_NLOvsLO_13p6TeV.root","read");

        map<string,pair<double,double>> Processes
        {
           #include "Info_Map/Map.txt"
        };

	for (pair<string, pair<double,double>> Proc : Processes)
	{
	std::cout << (TString)Proc.first << std::endl;
	TFile *file = TFile::Open("/eos/cms/store/group/phys_higgs/mukherje/VBFHBB/"+ (TString)Proc.first + ".root");
        TTree *mtree = (TTree*)file->Get("mytuple/tree");	  
        TFile *f = new TFile("/eos/cms/store/group/phys_higgs/mukherje/VBFHBB/NTuple/tree_"+ (TString)Proc.first + ".root", "RECREATE");
        TTree *Var_Tree = new TTree("tree","tree");
                  
	//Cut Flow Table
        TH1F* h = new TH1F("h", "", 12, 0.0, 12.0);
	//TTree *Var_Tree    = new TTree("Sig","Sig");
	ULong64_t T_event;
	Float_t weight_gen;
        Float_t T_weight , T_PUweight;
        Float_t T_pt0,T_pt1,T_pt2,T_pt3,T_pt4,T_pt5;
	Float_t T_eta0,T_eta1,T_eta2,T_eta3,T_eta4,T_eta5;
	Float_t T_en0,T_en1,T_en2,T_en3,T_en4,T_en5;
	Float_t T_phi0,T_phi1,T_phi2,T_phi3,T_phi4,T_phi5;
 	Float_t T_ptb1,T_reg_ptb1,T_etab1,T_phib1,T_btgb1,T_Ordb1 ,T_enb1, T_reg_enb1;
	Float_t T_ptb2,T_reg_ptb2,T_etab2,T_phib2,T_btgb2,T_Ordb2, T_enb2, T_reg_enb2;
  	Float_t T_ptq1,T_etaq1,T_phiq1,T_btgq1,T_qtgq1,T_enq1;
  	Float_t T_ptq2,T_etaq2,T_phiq2,T_btgq2,T_qtgq2 ,T_enq2;
  	Float_t T_mqq,T_dETAqq,T_dPHIqq,T_NJ_ingap,T_ptAll,T_pzAll,T_dETA_bb_qq,T_alphabb,T_alphaqq;
  	Float_t T_NJ_20,T_HTT_rest_20,T_E_rest_20,T_NJ_30,T_HTT_rest_30,T_E_rest_30;
  	Float_t T_zep,T_phiA_bb_qq,T_minR,T_dR_leadqH,T_dR_subleadqH,T_norm_pt,T_prod_pt,T_ptHtoM;
  	int T_bflv1, T_bflv2, T_qflv1, T_qflv2;
  	Float_t T_mbb;
  	Float_t T_reg_mbb;
	Float_t T_btag_weight_central;
        Float_t T_qglq1;
	Float_t T_qglq2;
	Float_t T_HLTweight;
	Float_t T_V_pTweight;
        Float_t T_xsec;
	Float_t T_sumwgt;
	// Event weight
	Var_Tree->Branch("T_xsec",                           &T_xsec,                       "T_xsec/F");
	Var_Tree->Branch("T_sumwgt",                         &T_sumwgt,                       "T_sumwgt/F");
	Var_Tree->Branch("T_event",                          &T_event,                      "T_event/L");
        Var_Tree->Branch("T_weight",                         &T_weight,                     "T_weight/F");
        Var_Tree->Branch("T_PUweight",                       &T_PUweight,                   "T_PUweight/F");
        Var_Tree->Branch("T_btag_weight_central",            &T_btag_weight_central,        "T_btag_weight_central/F");
        Var_Tree->Branch("T_HLTweight",                      &T_HLTweight,                  "T_HLTweight/F");
        Var_Tree->Branch("T_V_pTweight",                     &T_V_pTweight,                 "T_V_pTweight/F");

        // Leading Jets
  	Var_Tree->Branch("T_pt0",               &T_pt0,           "T_pt0/F");
  	Var_Tree->Branch("T_pt1",               &T_pt1,           "T_pt1/F");
  	Var_Tree->Branch("T_pt2",               &T_pt2,           "T_pt2/F");
  	Var_Tree->Branch("T_pt3",               &T_pt3,           "T_pt3/F");
	Var_Tree->Branch("T_pt4",               &T_pt4,           "T_pt4/F");
  	Var_Tree->Branch("T_pt5",               &T_pt5,           "T_pt5/F");

	Var_Tree->Branch("T_eta0",               &T_eta0,           "T_eta0/F");
        Var_Tree->Branch("T_eta1",               &T_eta1,           "T_eta1/F");
        Var_Tree->Branch("T_eta2",               &T_eta2,           "T_eta2/F");
        Var_Tree->Branch("T_eta3",               &T_eta3,           "T_eta3/F");
        Var_Tree->Branch("T_eta4",               &T_eta4,           "T_eta4/F");
        Var_Tree->Branch("T_eta5",               &T_eta5,           "T_eta5/F");

	Var_Tree->Branch("T_en0",               &T_en0,           "T_en0/F");
        Var_Tree->Branch("T_en1",               &T_en1,           "T_en1/F");
        Var_Tree->Branch("T_en2",               &T_en2,           "T_en2/F");
        Var_Tree->Branch("T_en3",               &T_en3,           "T_en3/F");
        Var_Tree->Branch("T_en4",               &T_en4,           "T_en4/F");
        Var_Tree->Branch("T_en5",               &T_en5,           "T_en5/F");

	Var_Tree->Branch("T_phi0",               &T_phi0,           "T_phi0/F");
        Var_Tree->Branch("T_phi1",               &T_phi1,           "T_phi1/F");
        Var_Tree->Branch("T_phi2",               &T_phi2,           "T_phi2/F");
        Var_Tree->Branch("T_phi3",               &T_phi3,           "T_phi3/F");
        Var_Tree->Branch("T_phi4",               &T_phi4,           "T_phi4/F");
        Var_Tree->Branch("T_phi5",               &T_phi5,           "T_phi5/F");

        //b-Jets variables
	Var_Tree->Branch("T_ptb1",              &T_ptb1,          "T_ptb1/F");
  	Var_Tree->Branch("T_ptb2",              &T_ptb2,          "T_ptb2/F");
	Var_Tree->Branch("T_reg_ptb1",              &T_reg_ptb1,          "T_reg_ptb1/F");
        Var_Tree->Branch("T_reg_ptb2",              &T_reg_ptb2,          "T_reg_ptb2/F");
  	Var_Tree->Branch("T_etab1",             &T_etab1,         "T_etab1/F");
  	Var_Tree->Branch("T_etab2",             &T_etab2,         "T_etab2/F");
  	Var_Tree->Branch("T_phib1",             &T_phib1,         "T_phib1/F");
  	Var_Tree->Branch("T_phib2",             &T_phib2,         "T_phib2/F");
	Var_Tree->Branch("T_enb1",              &T_enb1,         "T_enb1/F");
        Var_Tree->Branch("T_enb2",              &T_enb2,         "T_enb2/F");
        Var_Tree->Branch("T_reg_enb1",          &T_reg_enb1,     "T_reg_enb1/F");
        Var_Tree->Branch("T_reg_enb2",          &T_reg_enb2,     "T_reg_enb2/F");
  	Var_Tree->Branch("T_btgb1",             &T_btgb1,        "T_btgb1/F");
  	Var_Tree->Branch("T_btgb2",             &T_btgb2,        "T_btgb2/F");
  	Var_Tree->Branch("T_Ordb1",             &T_Ordb1,         "T_Ordb1/F");
  	Var_Tree->Branch("T_Ordb2",             &T_Ordb2,         "T_Ordb2/F");
        Var_Tree->Branch("T_bflv1",             &T_bflv1,         "T_bflv1/F");
  	Var_Tree->Branch("T_bflv2",             &T_bflv2,         "T_bflv2/F");
        Var_Tree->Branch("T_mbb",               &T_mbb,           "T_mbb/F");
        Var_Tree->Branch("T_reg_mbb",           &T_reg_mbb,       "T_reg_mbb/F");

        //VBF related variables
        Var_Tree->Branch("T_ptq1",              &T_ptq1,          "T_ptq1/F");
  	Var_Tree->Branch("T_ptq2",              &T_ptq2,          "T_ptq2/F");
  	Var_Tree->Branch("T_etaq1",             &T_etaq1,         "T_etaq1/F");
  	Var_Tree->Branch("T_etaq2",             &T_etaq2,         "T_etaq2/F");
  	Var_Tree->Branch("T_phiq1",             &T_phiq1,         "T_phiq1/F");
  	Var_Tree->Branch("T_phiq2",             &T_phiq2,         "T_phiq2/F");
	Var_Tree->Branch("T_enq1",              &T_enq1,          "T_enq1/F");
        Var_Tree->Branch("T_enq2",              &T_enq2,          "T_enq2/F");
        Var_Tree->Branch("T_qflv1",             &T_qflv1,         "T_qflv1/F"); //VBF quark information
        Var_Tree->Branch("T_qflv2",             &T_qflv2,         "T_qflv2/F"); //VBF quark information
	Var_Tree->Branch("T_qglq1",               &T_qglq1,           "T_qglq1/F");
        Var_Tree->Branch("T_qglq2",               &T_qglq2,           "T_qglq2/F");
        // Other reconstructed variables
  	Var_Tree->Branch("T_mqq",               &T_mqq,           "T_mqq/F");
  	Var_Tree->Branch("T_dETAqq",            &T_dETAqq,        "T_dETAmqq/F");
  	Var_Tree->Branch("T_dPHIqq",            &T_dPHIqq,        "T_dETAmqq/F");

  	Var_Tree->Branch("T_ptAll",             &T_ptAll,         "T_ptAll/F");
  	Var_Tree->Branch("T_pzAll",             &T_pzAll,         "T_pzAll/F");

  	Var_Tree->Branch("T_NJ_30",             &T_NJ_30,         "T_NJ_30/F");
  	Var_Tree->Branch("T_HTT_rest_30",       &T_HTT_rest_30,   "T_HTT_rest_30/F");
  	Var_Tree->Branch("T_E_rest_30",         &T_E_rest_30,     "T_E_rest_30/F");
  	Var_Tree->Branch("T_dETA_bb_qq",        &T_dETA_bb_qq,    "T_dETA_bb_qq/F");
  	Var_Tree->Branch("T_alphabb",           &T_alphabb,       "T_alpha/F");
  	Var_Tree->Branch("T_alphaqq",           &T_alphaqq,       "T_alphaqq/F");
        //Extra Variables.
 	Var_Tree->Branch("T_zep",               &T_zep,           "T_zep/F");
  	Var_Tree->Branch("T_phiA_bb_qq",        &T_phiA_bb_qq,    "T_phiA_bb_qq/F");
  	Var_Tree->Branch("T_minR",              &T_minR,          "T_minR/F");
  	Var_Tree->Branch("T_dR_leadqH",         &T_dR_leadqH,     "T_dR_leadqH/F");
  	Var_Tree->Branch("T_dR_subleadqH",      &T_dR_subleadqH,  "T_dR_subleadqH/F");
  	Var_Tree->Branch("T_norm_pt",           &T_norm_pt,       "T_norm_pt/F");
  	Var_Tree->Branch("T_prod_pt",           &T_prod_pt,       "T_prod_pt/F");
  	Var_Tree->Branch("T_ptHtoM",            &T_ptHtoM,        "T_ptHtoM/F");

        Int_t           nPU;
        Float_t         gen_weight = 1.0;
        Int_t           nPV;

	
        Float_t         rho;
        ULong64_t       run;
        ULong64_t       event;
        Bool_t          Flag_goodVertices;
        Bool_t          Flag_globalSuperTightHalo2016Filter;
        Bool_t          Flag_EcalDeadCellTriggerPrimitiveFilter;
        Bool_t          Flag_BadPFMuonFilter;
        Bool_t          Flag_BadPFMuonDzFilter;
        Bool_t          Flag_hfNoisyHitsFilter;
        Bool_t          Flag_eeBadScFilter;
        Bool_t          Flag_ecalBadCalibFilter_;
        Bool_t          event_veto_map;
	vector<double>  *gen_V_pt;
	vector<int>     *gen_V_pdgid;
        Int_t           njet;
	vector<double>  jet_Jerpt;
        vector<double>  jet_Jereta;
        vector<double>  jet_Jerphi;
        vector<double>  jet_Jeren;
        vector<double>  jet_Jermass;
        vector<double>  *jet_pt;
        vector<double>  *jet_eta;
        vector<double>  *jet_phi;
        vector<double>  *jet_en;
        vector<double>  *jet_mass;
        vector<double>  *jet_pt_raw;
        vector<double>  *jet_mass_raw;
        vector<double>  *jet_energy_raw;
        vector<double>  *jet_jec;
        vector<double>  *jet_jec_L2;
        vector<double>  *jet_jec_L3;
        vector<double>  *jet_jec_L2L3;
        vector<double>  *jet_jerc_sf;
        vector<double>  *jet_jerc_reso;
        vector<double>  *jet_genmatch_pt;
        vector<double>  *jet_genmatch_eta;
        vector<double>  *jet_genmatch_phi;
        vector<double>  *jet_genmatch_mass;
        vector<double>  *jet_genmatch_energy;
        vector<double>  *jet_PNBTG;
        vector<double>  *jet_PNQGL;
        vector<bool>    *jet_id;
        vector<int>     *jet_pflv;
        vector<int>     *jet_hflv;
        vector<double>  *jet_pnet_jec;
        vector<double>  *jet_pnet_ptcorr;
        vector<double>  *jet_pnet_ptnu;
        vector<double>  *jet_pnet_ptres;
        Float_t         HT;
        Int_t           nL1jet;
        vector<double>  *L1jet_pt;
        vector<double>  *L1jet_eta;
        vector<double>  *L1jet_phi;
        vector<double>  *L1jet_en;
        Int_t           nCalojet;
        vector<double>  *Calojet_pt;
        vector<double>  *Calojet_eta;
        vector<double>  *Calojet_phi;
        vector<double>  *Calojet_en;
        Int_t           nPxljet;
        vector<double>  *Pxljet_pt;
        vector<double>  *Pxljet_eta;
        vector<double>  *Pxljet_phi;
        vector<double>  *Pxljet_en;
        Int_t           nL3jet;
        vector<double>  *L3jet_pt;
        vector<double>  *L3jet_eta;
        vector<double>  *L3jet_phi;
        vector<double>  *L3jet_en;
        Float_t         MET_pt;
        Float_t         MET_phi;
        Bool_t          HLT_PF60;
        Bool_t          HLT_PF80;
        Bool_t          HLT_QuadPFJet103_88_75_15;
        Bool_t          L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5;
	Bool_t        HLT_QuadPFJet103_88_76_15;
        Bool_t        HLT_QuadPFJet105_88_76_15;
        Bool_t        HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1;
        Bool_t        HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2;
        Bool_t        HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1;
        Bool_t        HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2;
        // List of branches
        TBranch        *b_event_veto_map;
        TBranch        *b_nPU;   //!
        TBranch        *b_weight;   //!
        TBranch        *b_nPV;   //!
        TBranch        *b_rho;   //!
        TBranch        *b_run;   //!
        TBranch        *b_event;   //!
        TBranch        *b_Flag_goodVertices;   //!
        TBranch        *b_Flag_globalSuperTightHalo2016Filter;   //!
        TBranch        *b_Flag_EcalDeadCellTriggerPrimitiveFilter;   //!
        TBranch        *b_Flag_BadPFMuonFilter;   //!
        TBranch        *b_Flag_BadPFMuonDzFilter;   //!
        TBranch        *b_Flag_hfNoisyHitsFilter;   //!
        TBranch        *b_Flag_eeBadScFilter;   //!
        TBranch        *b_Flag_ecalBadCalibFilter_;   //!
	TBranch        *b_gen_V_pt;
	TBranch        *b_gen_V_pdgid;
        TBranch        *b_njet;   //!
        TBranch        *b_jet_pt;   //!
        TBranch        *b_jet_eta;   //!
        TBranch        *b_jet_phi;   //!
        TBranch        *b_jet_en;   //!
        TBranch        *b_jet_mass;   //!
        TBranch        *b_jet_pt_raw;   //!
        TBranch        *b_jet_mass_raw;   //!
        TBranch        *b_jet_energy_raw;   //!
        TBranch        *b_jet_jec;   //!
        TBranch        *b_jet_jec_L2;   //!
        TBranch        *b_jet_jec_L3;   //!
        TBranch        *b_jet_jec_L2L3;   //!
        TBranch        *b_jet_jerc_sf;   //!
        TBranch        *b_jet_jerc_reso;   //!
        TBranch        *b_jet_genmatch_pt;   //!
        TBranch        *b_jet_genmatch_eta;   //!
        TBranch        *b_jet_genmatch_phi;   //!
        TBranch        *b_jet_genmatch_mass;   //!
        TBranch        *b_jet_genmatch_energy;   //!
        TBranch        *b_jet_PNBTG;   //!
        TBranch        *b_jet_PNQGL;   //!
        TBranch        *b_jet_id;   //!
        TBranch        *b_jet_pflv;   //!
        TBranch        *b_jet_hflv;   //!
        TBranch        *b_jet_pnet_jec;   //!
        TBranch        *b_jet_pnet_ptcorr;   //!
        TBranch        *b_jet_pnet_ptnu;   //!
        TBranch        *b_jet_pnet_ptres;   //!
        TBranch        *b_HT;   //!
        TBranch        *b_nL1jet;   //!
        TBranch        *b_L1jet_pt;   //!
        TBranch        *b_L1jet_eta;   //!
        TBranch        *b_L1jet_phi;   //!
        TBranch        *b_L1jet_en;   //!
        TBranch        *b_nCalojet;   //!
        TBranch        *b_Calojet_pt;   //!
        TBranch        *b_Calojet_eta;   //!
        TBranch        *b_Calojet_phi;   //!
        TBranch        *b_Calojet_en;   //!
        TBranch        *b_nPxljet;   //!
        TBranch        *b_Pxljet_pt;   //!
        TBranch        *b_Pxljet_eta;   //!
        TBranch        *b_Pxljet_phi;   //!
        TBranch        *b_Pxljet_en;   //!
        TBranch        *b_nL3jet;   //!
        TBranch        *b_L3jet_pt;   //!
        TBranch        *b_L3jet_eta;   //!
        TBranch        *b_L3jet_phi;   //!
        TBranch        *b_L3jet_en;   //!
        TBranch        *b_MET_pt;   //!
        TBranch        *b_MET_phi;   //!
        TBranch        *b_HLT_PF60;   //!
        TBranch        *b_HLT_PF80;   //!
        TBranch        *b_HLT_QuadPFJet103_88_75_15;   //!
        TBranch        *b_L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5; //
        
     //  TBranch        *b_HLT_QuadPFJet103_88_76_15;	
	TBranch        *b_HLT_QuadPFJet105_88_76_15;
        TBranch        *b_HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1;
	TBranch        *b_HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2;
	TBranch        *b_HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1;
	TBranch        *b_HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2;

        // Set object pointer
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_en = 0;
   jet_mass = 0;
   jet_pt_raw = 0;
   jet_mass_raw = 0;
   jet_energy_raw = 0;
   jet_jec = 0;
   jet_jec_L2 = 0;
   jet_jec_L3 = 0;
   jet_jec_L2L3 = 0;
   jet_jerc_sf = 0;
   jet_jerc_reso = 0;
   jet_genmatch_pt = 0;
   jet_genmatch_eta = 0;
   jet_genmatch_phi = 0;
   jet_genmatch_mass = 0;
   jet_genmatch_energy = 0;
   jet_PNBTG = 0;
   jet_PNQGL = 0;
   jet_id = 0;
   jet_pflv = 0;
   jet_hflv = 0;
   jet_pnet_jec = 0;
   jet_pnet_ptcorr = 0;
   jet_pnet_ptnu = 0;
   jet_pnet_ptres = 0;
   L1jet_pt = 0;
   L1jet_eta = 0;
   L1jet_phi = 0;
   L1jet_en = 0;
   Calojet_pt = 0;
   Calojet_eta = 0;
   Calojet_phi = 0;
   Calojet_en = 0;
   Pxljet_pt = 0;
   Pxljet_eta = 0;
   Pxljet_phi = 0;
   Pxljet_en = 0;
   L3jet_pt = 0;
   L3jet_eta = 0;
   L3jet_phi = 0;
   L3jet_en = 0;
   gen_V_pdgid = 0,
   gen_V_pt = 0;
        mtree->SetBranchAddress("nPU", &nPU, &b_nPU);
	//if(kk==0)
	//{
         mtree->SetBranchAddress("weight", &gen_weight, &b_weight);
       // } 
        // std::cout<<" Set branch address"<<std::endl;
 	mtree->SetBranchAddress("event_veto_map", &event_veto_map, &b_event_veto_map);
        mtree->SetBranchAddress("nPV", &nPV, &b_nPV);
        mtree->SetBranchAddress("rho", &rho, &b_rho);
        mtree->SetBranchAddress("run", &run, &b_run);
        mtree->SetBranchAddress("event", &event, &b_event);
        mtree->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices, &b_Flag_goodVertices);
        mtree->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter, &b_Flag_globalSuperTightHalo2016Filter);
        mtree->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, &b_Flag_EcalDeadCellTriggerPrimitiveFilter);
        mtree->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, &b_Flag_BadPFMuonFilter);
        mtree->SetBranchAddress("Flag_BadPFMuonDzFilter", &Flag_BadPFMuonDzFilter, &b_Flag_BadPFMuonDzFilter);
        mtree->SetBranchAddress("Flag_hfNoisyHitsFilter", &Flag_hfNoisyHitsFilter, &b_Flag_hfNoisyHitsFilter);
        mtree->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter, &b_Flag_eeBadScFilter);
        mtree->SetBranchAddress("Flag_ecalBadCalibFilter_", &Flag_ecalBadCalibFilter_, &b_Flag_ecalBadCalibFilter_);

	if (((TString)Proc.first).Contains("Zto2Q") || ((TString)Proc.first).Contains("WJets"))
	{
	  mtree->SetBranchAddress("gen_V_pt", &gen_V_pt);
	  mtree->SetBranchAddress("gen_V_pdgid", &gen_V_pdgid);
	}
        mtree->SetBranchAddress("njet", &njet, &b_njet);
        mtree->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
        mtree->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
        mtree->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
        mtree->SetBranchAddress("jet_en", &jet_en, &b_jet_en);
        mtree->SetBranchAddress("jet_mass", &jet_mass, &b_jet_mass);
        mtree->SetBranchAddress("jet_pt_raw", &jet_pt_raw, &b_jet_pt_raw);
        mtree->SetBranchAddress("jet_mass_raw", &jet_mass_raw, &b_jet_mass_raw);
        mtree->SetBranchAddress("jet_energy_raw", &jet_energy_raw, &b_jet_energy_raw);
        mtree->SetBranchAddress("jet_jec", &jet_jec, &b_jet_jec);
        mtree->SetBranchAddress("jet_jec_L2", &jet_jec_L2, &b_jet_jec_L2);
        mtree->SetBranchAddress("jet_jec_L3", &jet_jec_L3, &b_jet_jec_L3);
        mtree->SetBranchAddress("jet_jec_L2L3", &jet_jec_L2L3, &b_jet_jec_L2L3);
        mtree->SetBranchAddress("jet_jerc_sf", &jet_jerc_sf, &b_jet_jerc_sf);
        mtree->SetBranchAddress("jet_jerc_reso", &jet_jerc_reso, &b_jet_jerc_reso);
        mtree->SetBranchAddress("jet_genmatch_pt", &jet_genmatch_pt, &b_jet_genmatch_pt);
        mtree->SetBranchAddress("jet_genmatch_eta", &jet_genmatch_eta, &b_jet_genmatch_eta);
        mtree->SetBranchAddress("jet_genmatch_phi", &jet_genmatch_phi, &b_jet_genmatch_phi);
        mtree->SetBranchAddress("jet_genmatch_mass", &jet_genmatch_mass, &b_jet_genmatch_mass);
        mtree->SetBranchAddress("jet_genmatch_energy", &jet_genmatch_energy, &b_jet_genmatch_energy);
        mtree->SetBranchAddress("jet_PNBTG", &jet_PNBTG, &b_jet_PNBTG);
        mtree->SetBranchAddress("jet_PNQGL", &jet_PNQGL, &b_jet_PNQGL);
        mtree->SetBranchAddress("jet_id", &jet_id, &b_jet_id);
        mtree->SetBranchAddress("jet_pflv", &jet_pflv, &b_jet_pflv);
        mtree->SetBranchAddress("jet_hflv", &jet_hflv, &b_jet_hflv);
        mtree->SetBranchAddress("jet_pnet_jec", &jet_pnet_jec, &b_jet_pnet_jec);
        mtree->SetBranchAddress("jet_pnet_ptcorr", &jet_pnet_ptcorr, &b_jet_pnet_ptcorr);
        mtree->SetBranchAddress("jet_pnet_ptnu", &jet_pnet_ptnu, &b_jet_pnet_ptnu);
        mtree->SetBranchAddress("jet_pnet_ptres", &jet_pnet_ptres, &b_jet_pnet_ptres);
        mtree->SetBranchAddress("nL1jet", &nL1jet, &b_nL1jet);
        mtree->SetBranchAddress("L1jet_pt", &L1jet_pt, &b_L1jet_pt);
        mtree->SetBranchAddress("L1jet_eta", &L1jet_eta, &b_L1jet_eta);
        mtree->SetBranchAddress("L1jet_phi", &L1jet_phi, &b_L1jet_phi);
        mtree->SetBranchAddress("L1jet_en", &L1jet_en, &b_L1jet_en);
        mtree->SetBranchAddress("nCalojet", &nCalojet, &b_nCalojet);
        mtree->SetBranchAddress("Calojet_pt", &Calojet_pt, &b_Calojet_pt);
        mtree->SetBranchAddress("Calojet_eta", &Calojet_eta, &b_Calojet_eta);
        mtree->SetBranchAddress("Calojet_phi", &Calojet_phi, &b_Calojet_phi);
        mtree->SetBranchAddress("Calojet_en", &Calojet_en, &b_Calojet_en);
        mtree->SetBranchAddress("nPxljet", &nPxljet, &b_nPxljet);
        mtree->SetBranchAddress("Pxljet_pt", &Pxljet_pt, &b_Pxljet_pt);
        mtree->SetBranchAddress("Pxljet_eta", &Pxljet_eta, &b_Pxljet_eta);
        mtree->SetBranchAddress("Pxljet_phi", &Pxljet_phi, &b_Pxljet_phi);
        mtree->SetBranchAddress("Pxljet_en", &Pxljet_en, &b_Pxljet_en);
        mtree->SetBranchAddress("nL3jet", &nL3jet, &b_nL3jet);
        mtree->SetBranchAddress("L3jet_pt", &L3jet_pt, &b_L3jet_pt);
        mtree->SetBranchAddress("L3jet_eta", &L3jet_eta, &b_L3jet_eta);
        mtree->SetBranchAddress("L3jet_phi", &L3jet_phi, &b_L3jet_phi);
        mtree->SetBranchAddress("L3jet_en", &L3jet_en, &b_L3jet_en);
        mtree->SetBranchAddress("MET_pt", &MET_pt, &b_MET_pt);
        mtree->SetBranchAddress("MET_phi", &MET_phi, &b_MET_phi);

        mtree->SetBranchAddress("HLT_QuadPFJet103_88_75_15", &HLT_QuadPFJet103_88_75_15, &b_HLT_QuadPFJet103_88_75_15);
        mtree->SetBranchAddress("L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5", &L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5, &b_L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5);
	mtree->SetBranchAddress("HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1", &HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1, &b_HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1);
	mtree->SetBranchAddress("HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2", &HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2, &b_HLT_QuadPFJet103_88_75_15_PFBTagDeepJet_1p3_VBF2);
	mtree->SetBranchAddress("HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1", &HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1, &b_HLT_QuadPFJet105_88_76_15_DoublePFBTagDeepJet_1p3_7p7_VBF1);
	mtree->SetBranchAddress("HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2", &HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2, &b_HLT_QuadPFJet105_88_76_15_PFBTagDeepJet_1p3_VBF2);
        Long64_t nn; 

	// if there var used to set the branch value is not initialised properly u will get problem here
	nn = mtree->GetEntries();
	
	std::cout<<"total number of entries     " << nn <<std::endl;
        for(Long64_t j =0; j < nn; j++)
          {     mtree->GetEntry(j);
		T_xsec = (double)Proc.second.first;
		T_sumwgt = (double)Proc.second.second;
                
	        //***************************************************************************************//
                //                                     V-pT reweighting                                  //
                //***************************************************************************************//
                T_V_pTweight = 1;
                if (((TString)Proc.first).Contains("Zto2Q")) 
		{		   
		   if(gen_V_pt->size() > 0)
		   {	 
		     Float_t GenV_pt = gen_V_pt->at(0);	
		     Float_t GenV_pdgId = fabs(gen_V_pdgid->at(0));
                     T_V_pTweight = get_QCD_cor(ZJets_cor_file,GenV_pt,GenV_pdgId,1);
		   }
		}
                if (((TString)Proc.first).Contains("WJets"))
                {
		  if(gen_V_pt->size() > 0)     
                  {	
		    Float_t GenV_pt = gen_V_pt->at(0) ; 
                    Float_t GenV_pdgId = fabs(gen_V_pdgid->at(0));	
                    T_V_pTweight = get_QCD_cor(WJets_cor_file,GenV_pt,GenV_pdgId,1);      
		  }
                }

		jet_Jerpt.clear();
                jet_Jereta.clear();
                jet_Jerphi.clear();
                jet_Jermass.clear();
                jet_Jeren.clear();

		//correcting the jets with the JER with the gen matched jets. 
		for (int nj = 0; nj < njet; nj++) {
			TLorentzVector jer_jet; // Use a local object instead of a pointer
			jer_jet.SetPtEtaPhiM(jet_pt->at(nj), jet_eta->at(nj), jet_phi->at(nj), jet_mass->at(nj));

			double rp = jet_jerc_reso->at(nj);
			double gaus_rp = gRandom->Gaus(0., rp);

			if (!((TString)Proc.first).Contains("Data")) {
			        jer_jet = jer_jet * (1 + jer_corrected_values(jer_jet.Pt(), jet_genmatch_pt->at(nj), gaus_rp, jet_jerc_sf->at(nj)));
			} 
			else {
			        jer_jet = jer_jet * 1.0; // This operation is redundant but works
			}

			jet_Jerpt.push_back(jer_jet.Pt());
			jet_Jereta.push_back(jer_jet.Eta());
			jet_Jerphi.push_back(jer_jet.Phi());
			jet_Jermass.push_back(jer_jet.M());
			jet_Jeren.push_back(jer_jet.E());
			//std::cout<<"------"<<jet_Jerpt.size()<<std::endl;
			//std::cout << jer_jet.Pt() << " " << jer_jet.Eta() << " " << jer_jet.Phi() << " " << jer_jet.M() << " " << jer_jet.E() << std::endl;
		}               
                //jet_pt->clear();
                //jet_pt = &jet_Jerpt;
                //std::cout<<"------------------------------"<<"njet"<<njet<<"size of jet"<< jet_Jerpt.size()<<"------"<<jet_pt->size()<<"--"<<"---"<<jet_pt->at(0)<<jet_Jerpt.at(0)<<std::endl;
		//std::cout<<"------"<<jet_Jerpt.size()<<std::endl;
                //continue;
		//double gaus_rp = gRandom->Gaus(0.,rp);  
		//mtree->GetEntry(j);
		h->Fill(0.5); //Total processed events
		ntotalhfill +=1 ;
		//if (j%20 !=0) continue; 
	        //if (j%10000 ==0) std::cout<<"Processed "<< j<<"/"<<nn << "  "<<gen_weight <<"events"<<std::endl;
 		//Jet veto map 
                if(event_veto_map) continue; 
                h->Fill(1.5);  //jet veto map
		nvetohfill += 1;
                if(!Flag_goodVertices || !Flag_globalSuperTightHalo2016Filter || !Flag_EcalDeadCellTriggerPrimitiveFilter || !Flag_BadPFMuonFilter || !Flag_BadPFMuonDzFilter || !Flag_eeBadScFilter || !Flag_ecalBadCalibFilter_ ) continue;
                h->Fill(2.5);  //noise filter
                ///std::cout << "basic fileters pass" << std::endl;
                //Tag condition 
                //HLT requeirments 
                if(!HLT_QuadPFJet103_88_75_15_DoublePFBTagDeepJet_1p3_7p7_VBF1) continue;  
                h->Fill(3.5);  //HLT filter
		//L1 unprescaled reqyuirements
		if(!L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5) continue;
                h->Fill(4.5);  //L1 filter
		//if(debug) std::cout << "trigger pass" << std::endl;
                //Tag jet selection pt > HLT + 50 GeV, |eta| < 2.5 , jet id  + L1 | CALO | HLT matching condition 
                if(  njet < 4 ) continue; 
		//if(debug) std::cout << "offline objects is there" << std::endl;
                //only considering those events where at least two offline and level one jets are there
                //probe jet conditions only for the absence of third jet 
		TLorentzVector tag1;
		TLorentzVector tag2;
		TLorentzVector tag3;
                //Leading jet selection
                
		if(!jet_id->at(0) || !jet_id->at(1) || !jet_id->at(2) || !jet_id->at(3)) continue; 
                if(fabs(jet_Jereta.at(0)) > 4.7 || fabs(jet_Jereta.at(1)) > 4.7 || fabs(jet_Jereta.at(2)) > 4.7 || fabs(jet_Jereta.at(3)) > 4.7)  continue;
                if(jet_Jerpt.at(0) < 108 || jet_Jerpt.at(1) < 93 || jet_Jerpt.at(2) < 80 || jet_Jerpt.at(3) < 30) continue; 
                tag1.SetPtEtaPhiE(jet_pt->at(0),jet_eta->at(0),jet_phi->at(0), jet_en->at(0));
                tag2.SetPtEtaPhiE(jet_pt->at(1),jet_eta->at(1),jet_phi->at(1), jet_en->at(1));
                tag3.SetPtEtaPhiE(jet_pt->at(2),jet_eta->at(2),jet_phi->at(2), jet_en->at(2));
                h->Fill(5.5);  // jet selection
                if(nL3jet < 3 || nCalojet < 3 || nL1jet < 3 ) continue; 
	        if(debug) std::cout << "Offline jets are okay" << std::endl;
                //HLT matching, online and offline jets....   
                vector<double> dr0L1;
                vector<double> dr0Calo;
                vector<double> dr0L3;
                vector<double> dr1L1;
                vector<double> dr1Calo;
                vector<double> dr1L3;
                vector<double> dr2L1;
                vector<double> dr2Calo;
                vector<double> dr2L3;

		//L1 jets
		for (int m=0;m<L1jet_pt->size();m++)
                {
			        TLorentzVector L1_jet;
				L1_jet.SetPtEtaPhiE(L1jet_pt->at(m), L1jet_eta->at(m), L1jet_phi->at(m), L1jet_en->at(m) );
				dr0L1.push_back(tag1.DeltaR(L1_jet));
				dr1L1.push_back(tag2.DeltaR(L1_jet));
				dr2L1.push_back(tag3.DeltaR(L1_jet));
                }
                //Calo jets
                for (int m=0;m<Calojet_pt->size();m++)
                {               
                                TLorentzVector Calo_jet;
                                Calo_jet.SetPtEtaPhiE(Calojet_pt->at(m), Calojet_eta->at(m), Calojet_phi->at(m), Calojet_en->at(m) );
                                dr0Calo.push_back(tag1.DeltaR(Calo_jet));
                                dr1Calo.push_back(tag2.DeltaR(Calo_jet));
                                dr2Calo.push_back(tag3.DeltaR(Calo_jet));
                }
                //HLT or L3 level jets
                for (int m=0;m<L3jet_pt->size();m++)
                {               
                                TLorentzVector L3_jet;
                                L3_jet.SetPtEtaPhiE(L3jet_pt->at(m), L3jet_eta->at(m), L3jet_phi->at(m), L3jet_en->at(m) );
                                dr0L3.push_back(tag1.DeltaR(L3_jet));
                                dr1L3.push_back(tag2.DeltaR(L3_jet));
                                dr2L3.push_back(tag3.DeltaR(L3_jet)); 
                }
		double L1_MatchedTo_0_pt=0;
                double L1_MatchedTo_1_pt=0;
                double L1_MatchedTo_2_pt=0;

                double Calo_MatchedTo_0_pt=0;
                double Calo_MatchedTo_1_pt=0;
                double Calo_MatchedTo_2_pt=0;

                double L3_MatchedTo_0_pt=0;
                double L3_MatchedTo_1_pt=0;
                double L3_MatchedTo_2_pt=0;
                double MaxDist0,MaxDist1,MaxDist2,MaxDist3;
		MaxDist0=MaxDist1=MaxDist2=MaxDist3=0.4;
		for(int n=0;n<L1jet_pt->size();n++)
                {
                   if(dr0L1[n] < MaxDist0) {L1_MatchedTo_0_pt  = L1jet_pt->at(n);  MaxDist0=dr0L1[n];}
                   if(dr1L1[n] < MaxDist1) {L1_MatchedTo_1_pt  = L1jet_pt->at(n);  MaxDist1=dr1L1[n];}
                   if(dr2L1[n] < MaxDist2) {L1_MatchedTo_2_pt  = L1jet_pt->at(n);  MaxDist2=dr2L1[n];}
                }
		MaxDist0=MaxDist1=MaxDist2=MaxDist3=0.4;
		for(int n=0;n<Calojet_pt->size();n++)
                {
                   if(dr0Calo[n] < MaxDist0) {Calo_MatchedTo_0_pt  = Calojet_pt->at(n);  MaxDist0=dr0Calo[n];}
                   if(dr1Calo[n] < MaxDist1) {Calo_MatchedTo_1_pt  = Calojet_pt->at(n);  MaxDist1=dr1Calo[n];}
                   if(dr2Calo[n] < MaxDist2) {Calo_MatchedTo_2_pt  = Calojet_pt->at(n);  MaxDist2=dr2Calo[n];}
                }
		for(int n=0;n<L3jet_pt->size();n++)
                {
                   if(dr0L3[n] < MaxDist0) {L3_MatchedTo_0_pt  = L3jet_pt->at(n);  MaxDist0=dr0L3[n];}
                   if(dr1L3[n] < MaxDist1) {L3_MatchedTo_1_pt  = L3jet_pt->at(n);  MaxDist1=dr1L3[n];}
                   if(dr2L3[n] < MaxDist2) {L3_MatchedTo_2_pt  = L3jet_pt->at(n);  MaxDist2=dr2L3[n];}
                }

 	        if(L1_MatchedTo_0_pt>=95 && L1_MatchedTo_1_pt >=75 && L1_MatchedTo_2_pt >=65 && Calo_MatchedTo_0_pt>=80 && Calo_MatchedTo_1_pt>=65 && Calo_MatchedTo_2_pt >=50 && L3_MatchedTo_0_pt>=103 && L3_MatchedTo_1_pt >= 88 && L3_MatchedTo_2_pt >=75 )
                {  
                h->Fill(6.5);  // HLT -matching selection        
		// std::cout<<"the final selection of leading four jet pass"<<std::endl;
		// ************************************************* All variables
  		double pt0,pt1,pt2,pt3,pt4,pt5;
		double eta0,eta1,eta2,eta3,eta4,eta5;
		double phi0,phi1,phi2,phi3,phi4,phi5;
		double en0,en1,en2,en3,en4,en5;
  		double etab1,phib1,enb1 ,btgb1,bcorrb1,bresob1;
  		double ptb2,etab2,phib2,enb2,btgb2,bcorrb2,bresob2;
  		double ptq1,etaq1,phiq1,enq1;
  		double ptq2,etaq2,phiq2,enq2;
  		double mqq,dETAqq,dPHIqq,ptAll,pzAll,HTT_rest_20,E_rest_20,HTT_rest_30,E_rest_30;
  		double mbb,reg_mbb;
  		double ptbg1,etabg1,phibg1,enbg1,DR1;
  		double ptbg2,etabg2,phibg2,enbg2,DR2;
  		int bflv1, bflv2;
		double qtgq1 ,qtgq2;
		double ptb1;


 		pt0=pt1=pt2=pt3=ptb1=etab1=phib1=enb1=btgb1=ptb2=etab2=phib2=enb2=btgb2=0;
  		ptq1=etaq1=phiq1=enq1=ptq2=etaq2=phiq2=enq2=0;
  		mqq=dETAqq=dPHIqq=ptAll=pzAll=HTT_rest_20=E_rest_20=HTT_rest_30=E_rest_30=mbb=reg_mbb=0;
  		ptbg1=etabg1=phibg1=enbg1=DR1=0;
 		ptbg2=etabg2=phibg2=enbg2=DR2=0;
 		bflv1=bflv2=999;
		pt4=pt5 = -999;
                en4 =en5 = -999;
		eta4 =eta5 = -999;
		phi4 =phi5 = -999;

                //***********************************************************************//
                //b-jet selection for the reconstructed Higgs candidate..... 
                //***********************************************************************//
                //b-tag SF 
		Int_t njet_bjet;
                if(njet < 6)   njet_bjet = njet;
                else njet_bjet = 6;
	        Float_t btag_weight_central = 1.0;
		if (!((TString)Proc.first).Contains("Data"))
		{
		   for(int nj =0;nj< njet_bjet; nj++){
			if(!jet_id->at(nj)) continue;
                        if (fabs(jet_Jereta.at(nj))>2.5) continue;
                        if (jet_Jerpt.at(nj) <  30.0) continue;
                        btag_weight_central = btag_weight_central * cset_sf->evaluate({"central",jet_pflv->at(nj), fabs(jet_Jereta.at(nj)), jet_Jerpt.at(nj), jet_PNBTG->at(nj)});
		   }
		}

		     T_btag_weight_central = btag_weight_central;
                     int Lorder = 10000;
		     int SubLorder = 10000;
		     double LBtag = 0.0;
		     double SubLBtag = 0.0;
		     double btag;

		     Float_t temp_btg = -999.9;
                     Int_t lead_ord = 999;

                     //Int_t njet_bjet;
                     //if(njet < 6)   njet_bjet = njet;
                    // else njet_bjet = 6;
		     //njet_bjet = 4;
                     for(int nj =0;nj< njet_bjet; nj++){
                         if(!jet_id->at(nj)) continue;
			 if (fabs(jet_Jereta.at(nj))>2.5) continue;
			 if (jet_Jerpt.at(nj) <  30.0) continue;
                         if(jet_PNBTG->at(nj) > temp_btg)
                             {
                               temp_btg = jet_PNBTG->at(nj);
                               LBtag  = temp_btg;
                               Lorder = nj;
                             }
                     }   
		     Float_t temp_btg_2 = -999.9;
                     for(int nj =0;nj< njet_bjet; nj++){
			 if(nj == Lorder) continue;    
                         if (!jet_id->at(nj)) continue;
                         if (fabs(jet_Jereta.at(nj))>2.5) continue;
                         if (jet_Jerpt.at(nj) <  30.0) continue;
                         if (jet_PNBTG->at(nj) > temp_btg_2 && jet_PNBTG->at(nj) < LBtag)
                             {
                               temp_btg_2 = jet_PNBTG->at(nj);
                               SubLBtag  = temp_btg_2;
                               SubLorder = nj;
                             }
                     }

		   if(Lorder == 10000 || SubLorder == 10000) continue;  
                   ptb1   = jet_Jerpt.at(Lorder);
                   etab1  = jet_Jereta.at(Lorder);
                   phib1  = jet_Jerphi.at(Lorder);
		   T_enb1 = jet_Jeren.at(Lorder);
                   btgb1  = jet_PNBTG->at(Lorder);
                   
                   // gen matching information for the b-tag jets......
                   ptbg1  = jet_genmatch_pt->at(Lorder);
     		   etabg1= jet_genmatch_phi->at(Lorder);
       		   phibg1= jet_genmatch_eta->at(Lorder);
       		   enbg1= jet_genmatch_energy->at(Lorder);
                   T_bflv1 = jet_pflv->at(Lorder);  // jet_pflv  : jet hadron flavour , name interchanged in an
		  // std::cout<<T_bflv1<<etab1<<ptb1<<btgb1<<std::endl;
		  // T_btag_sf1 = cset_sf->evaluate({"central",T_bflv1, fabs(etab1), ptb1, btgb1});

		   // SubLeading btag b2
		   ptb2 = jet_Jerpt.at(SubLorder);
		   etab2 = jet_Jereta.at(SubLorder);
                   phib2 = jet_Jerphi.at(SubLorder);
		   T_enb2 = jet_Jeren.at(SubLorder);
                   btgb2 = jet_PNBTG->at(SubLorder);

                   ptbg2=jet_genmatch_pt->at(SubLorder);
                   etabg2= jet_genmatch_phi->at(SubLorder);
                   phibg2= jet_genmatch_eta->at(SubLorder);
                   enbg2= jet_genmatch_energy->at(SubLorder);
		   T_bflv2 = jet_pflv->at(SubLorder);    
		  // std::cout<<T_bflv2<<etab2<<ptb2<<btgb2<<std::endl;
                  // T_btag_sf2 = cset_sf->evaluate({"central",T_bflv2, fabs(etab2), ptb2, btgb2});

                   T_ptb1=ptb1; T_etab1=etab1; T_phib1=phib1; T_btgb1=btgb1;  T_Ordb1=Lorder;
                   T_ptb2=ptb2; T_etab2=etab2; T_phib2=phib2; T_btgb2=btgb2;  T_Ordb2=SubLorder; 
                   
		   
                   //****************************************************************************
                   //   Application of b-jet energy regression 
                   //****************************************************************************
                   TLorentzVector b1_jet;
                   TLorentzVector b2_jet;
                   b1_jet.SetPtEtaPhiM(jet_Jerpt.at(Lorder),jet_Jereta.at(Lorder),jet_Jerphi.at(Lorder),jet_Jermass.at(Lorder));
                   b2_jet.SetPtEtaPhiM(jet_Jerpt.at(SubLorder),jet_Jereta.at(SubLorder),jet_Jerphi.at(SubLorder),jet_Jermass.at(SubLorder));
                   mbb = (b1_jet + b2_jet).M();
                   
                   TLorentzVector b_lead_reg_raw, b_sublead_reg_raw;
                   b_lead_reg_raw.SetPtEtaPhiM(jet_pt_raw->at(Lorder)*jet_pnet_ptcorr->at(Lorder)*jet_pnet_ptnu->at(Lorder),jet_Jereta.at(Lorder),jet_Jerphi.at(Lorder),jet_mass_raw->at(Lorder));
                   b_sublead_reg_raw.SetPtEtaPhiM(jet_pt_raw->at(SubLorder)*jet_pnet_ptcorr->at(SubLorder)*jet_pnet_ptnu->at(SubLorder),jet_Jereta.at(SubLorder),jet_Jerphi.at(SubLorder),jet_mass_raw->at(SubLorder));
                   //
                   TLorentzVector b_lead_reg, b_sublead_reg;
                   b_lead_reg = b_lead_reg_raw * jet_pnet_jec->at(Lorder);
		   b_sublead_reg = b_sublead_reg_raw * jet_pnet_jec->at(SubLorder);
		   if (!((TString)Proc.first).Contains("Data")) 
		   {
		       double rp = jet_jerc_reso->at(Lorder);
                       double gaus_rp = gRandom->Gaus(0., rp);
		       b_lead_reg  = b_lead_reg * (1 + jer_corrected_values(b_lead_reg.Pt(), jet_genmatch_pt->at(Lorder), gaus_rp, jet_jerc_sf->at(Lorder)));
                       rp = jet_jerc_reso->at(SubLorder);
                       gaus_rp = gRandom->Gaus(0., rp);
		       b_sublead_reg = b_sublead_reg * (1 + jer_corrected_values(b_sublead_reg.Pt(), jet_genmatch_pt->at(SubLorder), gaus_rp, jet_jerc_sf->at(SubLorder)));
		   }
		   TLorentzVector Higgs;
                   Higgs = b_lead_reg + b_sublead_reg;
                   reg_mbb = (b_lead_reg+b_sublead_reg).M();
		    
                   T_event = event;
		   T_reg_ptb1 = b_lead_reg.Pt();
		   T_reg_ptb2 = b_sublead_reg.Pt();
                   T_reg_enb1 = b_lead_reg.E();
		   T_reg_enb2 = b_sublead_reg.E();
                   T_mbb=mbb;
                   T_reg_mbb=reg_mbb;
		   T_ptHtoM  = Higgs.Pt()/T_reg_mbb;
		   // ********************************************** Mixing Angle bb
		   TLorentzVector pb1,pb2;
		   pb1 = b_lead_reg;
		   pb2 = b_sublead_reg;
                   TLorentzVector BB = b_lead_reg+b_sublead_reg;
                   pb1.Boost(-BB.BoostVector());
                   pb2.Boost(-BB.BoostVector());
                   TVector3 B1P(pb1.Px(),pb1.Py(),pb1.Pz());
                   TVector3 B2P(pb2.Px(),pb2.Py(),pb2.Pz());
                   TVector3 BBDirection(BB.Px(),BB.Py(),BB.Pz());
                   T_alphabb = min(B1P.Angle(BBDirection),B2P.Angle(BBDirection));
  		   double eta_Mean_bb = (etab1+etab2)/2;
                   if(debug) std::cout << "b-jet variable definiton" <<  std::endl;
                   //***********************************************************************//
                   //VBF-jet selection other than Higgs candiadte jets
                   //***********************************************************************//
		     double m_qq = -1.0;
		     int q1_order = 9999;
		     int q2_order = 9999;
		     bool hasvbfjet = false;
                     for (int q1_n=0 ;q1_n <njet-1 ; q1_n++){
			    // if (jet_id->at() !=1) continue;
			     for (int q2_n=q1_n +1;q2_n <njet ; q2_n++){
	                     if (jet_id->at(q2_n) !=1 || jet_id->at(q1_n) !=1 ) continue;
			     if (fabs(jet_Jereta.at(q2_n)) > 4.7 || fabs(jet_Jereta.at(q1_n)) > 4.7 ) continue;
			     if (jet_Jerpt.at(q2_n) < 30.0 || jet_Jerpt.at(q1_n) < 30.0) continue; 
			     if (q1_n ==Lorder || q1_n == SubLorder ||q2_n ==Lorder || q2_n == SubLorder) continue;
			     TLorentzVector q1_jet;
			     TLorentzVector q2_jet;

			     q1_jet.SetPtEtaPhiM(jet_Jerpt.at(q1_n),jet_Jereta.at(q1_n),jet_Jerphi.at(q1_n),jet_Jermass.at(q1_n));
                             q2_jet.SetPtEtaPhiM(jet_Jerpt.at(q2_n),jet_Jereta.at(q2_n),jet_Jerphi.at(q2_n),jet_Jermass.at(q2_n));
			     double m_q1q2 = (q1_jet + q2_jet).M();
			     if (m_q1q2 > m_qq){
				     q1_order = q1_n;
				     q2_order = q2_n;
				     m_qq = m_q1q2;
			     }
			     }
		   }

		   if (m_qq == -1.0) continue; //at least we have the two VBF jets. 
		   if (q1_order == 9999 || q2_order == 9999 || Lorder ==10000||SubLorder ==10000) continue; //at least we have two b-tag jets and two VBF jets. 
                   T_ptq1 = jet_Jerpt.at(q1_order);
		   T_etaq1 = jet_Jereta.at(q1_order);
		   T_phiq1 = jet_Jerphi.at(q1_order);
		   T_enq1 = jet_Jeren.at(q1_order);
		   T_qglq1 = jet_PNQGL->at(q1_order);
                   T_ptq2 = jet_Jerpt.at(q2_order);
		   T_etaq2 = jet_Jereta.at(q2_order);
		   T_phiq2 = jet_Jerphi.at(q2_order);
		   T_enq2 = jet_Jeren.at(q2_order);
		   T_qglq2 = jet_PNQGL->at(q2_order);
		   T_qflv1 = jet_pflv->at(q1_order);
		   T_qflv2 = jet_pflv->at(q2_order);
                   TLorentzVector q_lead,q_sublead;
		   q_lead.SetPtEtaPhiE(T_ptq1,T_etaq1,T_phiq1,T_enq1);
                   q_sublead.SetPtEtaPhiE(T_ptq2,T_etaq2,T_phiq2,T_enq2);

                   if(!((abs(T_etaq1) < 2.6 || abs(T_etaq1) > 3.1 || T_ptq1 > 50) && (abs(T_etaq2) < 2.6 || abs(T_etaq2) > 3.1 || T_ptq2  > 50))) continue;  
		   h->Fill(7.5); // forward veto jet
                   T_mqq = m_qq ;
                   T_dETAqq= fabs(T_etaq1-T_etaq2);
		   T_dPHIqq = phi_Dist(T_phiq1,T_phiq2);
                 
                   double pxb1 = T_ptb1*cos(T_phib1),  pxb2 = T_ptb2*cos(T_phib2),  pxq1 = T_ptq1*cos(T_phiq1),  pxq2 = T_ptq2*cos(T_phiq2);
                   double pyb1 = T_ptb1*sin(T_phib1),  pyb2 = T_ptb2*sin(T_phib2),  pyq1 = T_ptq1*sin(T_phiq1),  pyq2 = T_ptq2*sin(T_phiq2);
                   double pzb1 = T_ptb1*sinh(T_etab1), pzb2 = T_ptb2*sinh(T_etab2), pzq1 = T_ptq1*sinh(T_etaq1), pzq2 = T_ptq2*sinh(T_etaq2);

                   T_ptAll = sqrt((pxb1+pxb2+pxq1+pxq2)*(pxb1+pxb2+pxq1+pxq2) + (pyb1+pyb2+pyq1+pyq2)*(pyb1+pyb2+pyq1+pyq2));
                   T_pzAll = (pzb1+pzb2+pzq1+pzq2);  
		   double eta_Mean_qq = (T_etaq1+T_etaq2)/2;
                   T_dETA_bb_qq=eta_Mean_bb-eta_Mean_qq;
		   T_zep= ((b_lead_reg+b_sublead_reg).Eta() - (T_etaq1 + T_etaq2) * 0.5 );
		   float dRq1b1 = q_lead.DeltaR(b_lead_reg);
                   float dRq1b2 = q_lead.DeltaR(b_sublead_reg);
                   float dRq2b1 = q_sublead.DeltaR(b_lead_reg);
                   float dRq2b2 = q_sublead.DeltaR(b_sublead_reg);

                   T_minR = std::min( std::min( dRq1b1, dRq1b2 ), std::min( dRq2b1, dRq2b2 ) );
                   T_phiA_bb_qq = phi_Dist((b_lead_reg+b_sublead_reg).Phi(),(q_lead+q_sublead).Phi());

                   T_dR_leadqH = Higgs.DeltaR(q_lead);
                   T_dR_subleadqH = Higgs.DeltaR(q_sublead);
                   T_norm_pt = (b_lead_reg + b_sublead_reg + q_lead + q_sublead).Pt()/(b_lead_reg.Pt() + b_sublead_reg.Pt() + q_lead.Pt() + q_sublead.Pt());
                   T_prod_pt = ( b_lead_reg.Pt() * b_sublead_reg.Pt() )/ (q_lead.Pt() * q_sublead.Pt());

                   //********************************************** Mixing Angle qq
                   TLorentzVector pq1,pq2;
                   pq1.SetPxPyPzE(pxq1,pyq1,pzq1,T_enq1);
                   pq2.SetPxPyPzE(pxq2,pyq2,pzq2,T_enq2);
                   TLorentzVector QQ = pq1+pq2;
                   pq1.Boost(-QQ.BoostVector());
                   pq2.Boost(-QQ.BoostVector());

                   TVector3 QQDirection(QQ.Px(),QQ.Py(),QQ.Pz());
                   TVector3 Q1P(pq1.Px(),pq1.Py(),pq1.Pz());
                   TVector3 Q2P(pq2.Px(),pq2.Py(),pq2.Pz());
                   T_alphaqq = min(Q1P.Angle(QQDirection),Q2P.Angle(QQDirection));

		   if(debug) std::cout << "VBF variable definiton" <<  std::endl;
                   //std::cout<<"final variable assignment"<<std::endl;
                   // Now define the variable value

		   //Calculation of HLT SF
       		double TSF_pt0 = 1;
                double TSF_pt1 = 1;
                double TSF_pt2 = 1;
                double HLT_SF = 1;
		if (!((TString)Proc.first).Contains("Data"))
		{
                if (fabs(tag1.Eta()) < 1.4)
                             TSF_pt0 = Pt0_Eff_Fact_1eta(tag1.Pt());
                else if (fabs(tag1.Eta()) >= 1.4 && fabs(tag1.Eta()) < 2.5)
                            TSF_pt0 = Pt0_Eff_Fact_2eta(tag1.Pt());
                else if (fabs(tag1.Eta()) >= 2.5 && fabs(tag1.Eta()) < 3.0)
                            TSF_pt0 = Pt0_Eff_Fact_3eta(tag1.Pt());
                else        TSF_pt0 = Pt0_Eff_Fact_4eta(tag1.Pt());

		if (fabs(tag2.Eta()) < 1.4)
                             TSF_pt1 = Pt1_Eff_Fact_1eta(tag2.Pt());
                else if (fabs(tag2.Eta()) >= 1.4 && fabs(tag2.Eta()) < 2.5)
                            TSF_pt1 = Pt1_Eff_Fact_2eta(tag2.Pt());
                else if (fabs(tag2.Eta()) >= 2.5 && fabs(tag2.Eta()) < 3.0)
                            TSF_pt1 = Pt1_Eff_Fact_3eta(tag2.Pt());
                else TSF_pt1 = Pt1_Eff_Fact_4eta(tag2.Pt());

		if (fabs(tag3.Eta()) < 1.4)
                             TSF_pt2 = Pt2_Eff_Fact_1eta(tag3.Pt());
                else if (fabs(tag3.Eta()) >= 1.4 && fabs(tag3.Eta()) < 2.5)
                            TSF_pt2 = Pt2_Eff_Fact_2eta(tag3.Pt());
                else if (fabs(tag3.Eta()) >= 2.5 && fabs(tag3.Eta()) < 3.0)
                            TSF_pt2 = Pt2_Eff_Fact_3eta(tag3.Pt());
                else TSF_pt2 = Pt2_Eff_Fact_4eta(tag3.Pt());
                HLT_SF = TSF_pt0 * TSF_pt1 * TSF_pt2;
	        }

                   // if (j%10000 ==0) std::cout<< j<<"/"<<nn << "  "<< gen_weight  <<"events"<<std::endl;
		   Float_t weight;
		   weight = gen_weight*T_xsec/T_sumwgt;
		   //std::cout << lumi*1000.0 << "  " << gen_weight << "  " << rew[(int)nPU] << "  " << xsec[kk] << "  " << Br << "  " << HLT_SF << "  " << total_gen_weight[kk] << "  " << weight << std::endl;
                   if (((TString)Proc.first).Contains("Data")) weight =1;
		   T_weight = weight;
		   T_PUweight = rew[(int)nPU];
                   T_HLTweight = HLT_SF;
		   // if (j%10000 ==0) std::cout<< j<<"/"<<nn << " 2nd   "<< gen_weight  <<"events"<<std::endl; 
		   weight_gen = gen_weight;
		   // if (j%10000 ==0) std::cout<<"Processed "<< j<<"/"<<nn << "  "<<gen_weight << weight_gen  <<"events"<<std::endl;
		   // 6 leading jet pt , check the jet_id for 5th and 6th one 
		   T_pt0 = jet_Jerpt.at(0);
		   T_pt1 = jet_Jerpt.at(1);
                   T_pt2 = jet_Jerpt.at(2);
		   T_pt3 = jet_Jerpt.at(3);

		   T_phi0 = jet_Jerphi.at(0);
                   T_phi1 = jet_Jerphi.at(1);
                   T_phi2 = jet_Jerphi.at(2);
                   T_phi3 = jet_Jerphi.at(3);

		   T_eta0 = jet_Jereta.at(0);
                   T_eta1 = jet_Jereta.at(1);
                   T_eta2 = jet_Jereta.at(2);
                   T_eta3 = jet_Jereta.at(3);

		   T_en0 = jet_Jeren.at(0);
                   T_en1 = jet_Jeren.at(1);
                   T_en2 = jet_Jeren.at(2);
                   T_en3 = jet_Jeren.at(3);

                   //additiaon jet four vetor selections
		   T_pt4 = -10.0; T_eta4 = -10.0; T_en4 = -10.0; T_phi4= -10.0;
		   T_pt5 = -10.0; T_eta5 = -10.0; T_en5 = -10.0; T_phi5= -10.0;
		   if (njet > 4)
                   {
                           if(jet_id->at(4))
                           {
                                T_pt4 = jet_Jerpt.at(4);
			        T_eta4 = jet_Jereta.at(4);
				T_en4 = jet_Jeren.at(4);
				T_phi4 = jet_Jerphi.at(4);
                          }                                
                   }
                   if (njet > 5)
                   {
                           if(jet_id->at(5))
                           {
                                T_pt5  = jet_Jerpt.at(5);
			        T_eta5 = jet_Jereta.at(5);
				T_en5  = jet_Jeren.at(5);
				T_phi5 = jet_Jerphi.at(5);
                          }                                
                   }
                   if(debug) std::cout << "Additonal variable selection" <<  std::endl;                   
                   //Additional variable informations 
                   Float_t HTTer2p5 = 0.0;
		   Int_t   NJer2p5  = 0;
		   Int_t   Eer2p5   = 0.0;
		   for (int ww=0 ;ww <njet ; ww++){
                      if (jet_id->at(ww) !=1) continue;
                      if(fabs(jet_Jereta.at(ww) > 2.5)) continue; 
	              if(jet_Jerpt.at(ww) < 30.0) continue;
	       	          HTTer2p5 = HTTer2p5 + jet_Jerpt.at(ww);
			  Eer2p5   = Eer2p5 + jet_Jeren.at(ww);
			  NJer2p5++;
		   }  
		   if(fabs(T_etaq1) < 2.5) {HTTer2p5 = HTTer2p5 - T_ptq1; Eer2p5 = Eer2p5 - T_enq1; NJer2p5 = NJer2p5 - 1;}
		   if(fabs(T_etaq2) < 2.5) {HTTer2p5 = HTTer2p5 - T_ptq2; Eer2p5 = Eer2p5 - T_enq2; NJer2p5 = NJer2p5 - 1;}
                   if(fabs(T_etab1) < 2.5) {HTTer2p5 = HTTer2p5 - T_ptb1; Eer2p5 = Eer2p5 - T_enb1; NJer2p5 = NJer2p5 - 1;}
                   if(fabs(T_etab2) < 2.5) {HTTer2p5 = HTTer2p5 - T_ptb2; Eer2p5 = Eer2p5 - T_enb2; NJer2p5 = NJer2p5 - 1;}
                   T_NJ_30 = NJer2p5; T_HTT_rest_30 = HTTer2p5; T_E_rest_30 = Eer2p5; 
                
		   //*******************************************************************************//
		   //    Final selection that is going for the final training trees                 //
		   //*******************************************************************************//     
		   if( T_mqq > HLT_Mqq && T_dETAqq > HLT_dETAqq )
                   {
                        h->Fill(8.5);  // VBF -matching selection   
			if(debug) std::cout << "Offline VBF Okay" <<  std::endl;
                        if(fabs(b1_jet.DeltaPhi(b2_jet)) < HLT_dPHIbb )
                        {
			     if(debug) std::cout << "DeltaPhi-bb okay" <<  std::endl;
                             h->Fill(9.5);  // DeltaPhi(bb) -matching selection      
                             if( T_btgb1 > PNet_medium_BTag &&  T_btgb2 > PNet_medium_BTag ) 
                             {
				    if(debug) std::cout << "Tree filled" <<  std::endl;
                                    h->Fill(10.5);  // 2 medium b-tag selection   
                                    Var_Tree->Fill();  
                             }
                        }
                        
                   }  
                  }// HLT-matching conditions......
		}// Event loop
    std::cout<< "nEntries  "<< nn << "  ntotalfill Hist  " << ntotalhfill << "  nvetohfill   " << nvetohfill <<std::endl;
                  	
    f->Write();
    h->Write();
    f->Close();
    }//Kk_Proc loop
ZJets_cor_file->Close();
WJets_cor_file->Close();
}//End


         
