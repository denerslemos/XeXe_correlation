#include "call_libraries.h"  // call libraries from ROOT and C++

// declare variables

// event quantities
float vertexz; // event z vertex
int hiBin; // event centrality
float hfplus; // event hf + deposit of energy
float hfminus; // event hf - deposit of energy
int PrimaryVertexFilter; 
int BeamScrapingFilter;
int HfCoincFilter;

// reco tracks
int ntrk;                 // number of track
float trkpt[99999];       // track pT
float trketa[99999];      // track eta
float trkphi[99999];      // track phi
float trkpterr[99999];    // track pT error (uncertainty)
float trkdcaxy[99999];    // track dxy impact parameter (transverse distance between primary vertex and collision - distance of closest approuch - DCA)
float trkdcaz[99999];     // track dz impact parameter (longitudinal distance between primary vertex and collision - distance of closest approuch - DCA)
float trkdcaxyerr[99999]; // track dxy error (uncertainty)
float trkdcazerr[99999];  // track dxy error (uncertainty)
float trkchi2[99999];     // track reconstruction chi2 of the fitting
float pfEcal[99999];      // particle flow energy deposit in ECAL
float pfHcal[99999];      // particle flow energy deposit in HCAL
float trkmva[99999];      // track mva for each step
int trkalgo[99999];       // track algorithm/step
unsigned char trkndof[99999];       // track number of degrees of freedom in the fitting 
int trkcharge[99999];     // track charge
unsigned char trknhits[99999];      // number of hits in the tracker
unsigned char trknlayer[99999];     // number of layers with measurement in the tracker
unsigned char trkpixhits[99999];// number of hits in the pixel detector
bool highpur[99999];      // tracker steps MVA selection

// gen tracks
std::vector<float> *gen_trkpt = 0;  // gen particle pT
std::vector<float> *gen_trketa = 0; // gen particle eta
std::vector<float> *gen_trkphi = 0; // gen particle phi
std::vector<int> *gen_trkchg = 0;   // gen particle charge
std::vector<int> *gen_trkpid = 0;   // gen particle pid

//All variables listed above are readed in the function bellow
/*
Function to read the Forest/Skim tree
Arguments ->  transfer quantities from trees to our variables
tree: input TChain from jet_analyzer.C file
is_MC: true -> MC; false -> Data
*/
void read_tree(TChain *tree, bool is_MC){

    tree->SetBranchStatus("*", 0); // disable all branches - this is important while reading big files

    // enable branches of interest -> see definition of each variables above
    // event quantities
    tree->SetBranchStatus("vz", 1);
    tree->SetBranchAddress("vz", &vertexz);
	tree->SetBranchStatus("hiBin", 1); 
	tree->SetBranchAddress("hiBin", &hiBin);
	tree->SetBranchStatus("hiHFplus", 1); 
	tree->SetBranchAddress("hiHFplus", &hfplus);
	tree->SetBranchStatus("hiHFminus", 1); 
	tree->SetBranchAddress("hiHFminus", &hfminus);
	tree->SetBranchStatus("pPAprimaryVertexFilter", 1); 
	tree->SetBranchAddress("pPAprimaryVertexFilter", &PrimaryVertexFilter);	
	tree->SetBranchStatus("pBeamScrapingFilter", 1); 
	tree->SetBranchAddress("pBeamScrapingFilter", &BeamScrapingFilter);
	tree->SetBranchStatus("phfCoincFilter3", 1);
	tree->SetBranchAddress("phfCoincFilter3", &HfCoincFilter);
	
    // track quantities
    tree->SetBranchStatus("nTrk", 1);
    tree->SetBranchStatus("trkPt", 1);
    tree->SetBranchStatus("trkEta", 1);
    tree->SetBranchStatus("trkPhi", 1);
    tree->SetBranchStatus("trkPtError", 1);
    tree->SetBranchStatus("trkDxy1", 1);
    tree->SetBranchStatus("trkDxyError1", 1);
    tree->SetBranchStatus("trkDz1", 1);
    tree->SetBranchStatus("trkDzError1", 1);
    tree->SetBranchStatus("trkChi2", 1);
    tree->SetBranchStatus("trkNdof", 1);
    tree->SetBranchStatus("trkCharge", 1);
    tree->SetBranchStatus("trkNHit", 1);
    tree->SetBranchStatus("trkNlayer", 1);
    tree->SetBranchStatus("trkNPixelHit", 1);
    tree->SetBranchStatus("highPurity", 1);
    tree->SetBranchStatus("pfEcal", 1);
    tree->SetBranchStatus("pfHcal", 1);

    tree->SetBranchAddress("nTrk", &ntrk);
    tree->SetBranchAddress("trkPt", &trkpt);
    tree->SetBranchAddress("trkEta", &trketa);
    tree->SetBranchAddress("trkPhi", &trkphi);
    tree->SetBranchAddress("trkPtError", &trkpterr);
    tree->SetBranchAddress("trkDxy1", &trkdcaxy);
    tree->SetBranchAddress("trkDxyError1", &trkdcaxyerr);
    tree->SetBranchAddress("trkDz1", &trkdcaz);
    tree->SetBranchAddress("trkDzError1", &trkdcazerr);
    tree->SetBranchAddress("trkChi2", &trkchi2);
    tree->SetBranchAddress("trkNdof", &trkndof);
    tree->SetBranchAddress("trkCharge", &trkcharge);
    tree->SetBranchAddress("trkNHit", &trknhits);
    tree->SetBranchAddress("trkNlayer", &trknlayer);
    tree->SetBranchAddress("trkNPixelHit", &trkpixhits);
    tree->SetBranchAddress("highPurity", &highpur);
    tree->SetBranchAddress("pfEcal", &pfEcal);
    tree->SetBranchAddress("pfHcal", &pfHcal);

    // gen particle quantities
    if(is_MC){
        tree->SetBranchStatus("pt", 1);
        tree->SetBranchStatus("eta", 1);
        tree->SetBranchStatus("phi", 1);
        tree->SetBranchStatus("chg", 1);
        tree->SetBranchStatus("pdg", 1);

        tree->SetBranchAddress("pt", &gen_trkpt);
        tree->SetBranchAddress("eta", &gen_trketa);
        tree->SetBranchAddress("phi", &gen_trkphi);
        tree->SetBranchAddress("chg", &gen_trkchg);
        tree->SetBranchAddress("pdg", &gen_trkpid);
    }

}
