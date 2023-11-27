#include "call_libraries.h"  // call libraries from ROOT and C++

//define vectors for mixing
// reco jet  + reco track
std::vector<int> centrality_vector;
std::vector<int> multiplicity_vector;
std::vector<double> vz_vector;
std::vector<std::vector<ROOT::Math::PtEtaPhiMVector>> track_4vector;
std::vector<std::vector<double>> track_weights_vector;
std::vector<std::vector<int>> track_charge_vector;

std::vector<int> centrality_vector_gen;
std::vector<int> multiplicity_vector_gen;
std::vector<double> vz_vector_gen;
std::vector<std::vector<ROOT::Math::PtEtaPhiMVector>> track_4vector_gen;
std::vector<std::vector<double>> track_weights_vector_gen;
std::vector<std::vector<int>> track_charge_vector_gen;

// define the bins
// qinv
const int nQBins = 400;   // number of qinv bins
const double minQ = 0.0;  // minimum qinv
const double maxQ = 4.0;  // maximumm qinv

// q3D
const int nQBins3D = 100;   // number of q3D bins
const double minQ3D = 0.0;  // minimum q3D
const double maxQ3D = 2.0;  // maximumm q3D

//kT
const int nKtBins = 4; // number of average transverse momentum bins
double KtBins[nKtBins+1] = {0.1,0.3,0.5,0.7,1.0}; 

// centrality
const int nCentBins = 4; // number of centrality bins
double CentBins[nCentBins+1] = {0.0,20.0,60.0,100.0,140.0}; // 0-10%, 10-30%, 30-50%, 50-70% (0.5 in 0.5%)

// Event histograms
TH1I *Nevents = new TH1I("Nevents", "Nevents", 10, 0, 10);
TH1D *centrality_beforefilters = new TH1D("centrality_beforefilters", "centrality_beforefilters", 100, 0.0, 200.0);
TH1D *centrality = new TH1D("centrality", "centrality", 100, 0.0, 200.0);
TH1D *vzhist = new TH1D("vzhist", "vzhist", 80, -20., 20.);
TH1D *multiplicity = new TH1D("multiplicity", "multiplicity", 400, 0.0, 4000.0);
TH2D *MultVSCent = new TH2D("MultVSCent", "MultVSCent", 200, 0.0, 4000.0, 100, 0.0, 200.0);

//histograms before selection
TH1D *dxyoversigmadxy_beforeselection = new TH1D("dxyoversigmadxy_beforeselection", "dxyoversigmadxy_beforeselection", 100, -6.0, 6.0);
TH1D *dzoversigmadz_beforeselection = new TH1D("dzoversigmadz_beforeselection", "dzoversigmadz_beforeselection", 100, -6.0, 6.0);
TH1D *ptresolution_beforeselection = new TH1D("ptresolution_beforeselection", "ptresolution_beforeselection", 50, 0.0, 0.25);
TH1D *chi2overNDFoverNLayer_beforeselection = new TH1D("chi2overNDFoverNLayer_beforeselection", "chi2overNDFoverNLayer_beforeselection", 100, 0.0, 0.5);
TH1D *nhits_beforeselection = new TH1D("nhits_beforeselection", "nhits_beforeselection", 60, 0.0, 60.0);
TH1D *npixelhit_beforeselection = new TH1D("npixelhit_beforeselection", "npixelhit_beforeselection", 5, 0.0, 5.0);

//histograms after selections
TH1D *dxyoversigmadxy = new TH1D("dxyoversigmadxy", "dxyoversigmadxy", 100, -6.0, 6.0);
TH1D *dzoversigmadz = new TH1D("dzoversigmadz", "dzoversigmadz", 100, -6.0, 6.0);
TH1D *ptresolution = new TH1D("ptresolution", "ptresolution", 50, 0.0, 0.25);
TH1D *chi2overNDFoverNLayer = new TH1D("chi2overNDFoverNLayer", "chi2overNDFoverNLayer", 100, 0.0, 0.5);
TH1D *nhits = new TH1D("nhits", "nhits", 60, 0.0, 60.0);
TH1D *npixelhit = new TH1D("npixelhit", "npixelhit", 5, 0.0, 5.0);


// Track/Particle histograms
// Axis : 0 -> track pT, 1 -> trk eta, 2 -> trk phi, 3 -> trk charge, 4 -> centrality bin
int	bins_trk[5]      =   { 200   ,  25  ,   32		      , 4   , nCentBins};
double xmin_trk[5]   =   { 0.0   , -2.5 ,   -3.2  		  , -2.0, CentBins[0]};
double xmax_trk[5]   =   { 50.0  ,  2.5 ,   3.2  		  ,  2.0, CentBins[nCentBins+1]};
// --> Reco
THnSparseD *hist_reco_trk_beforeselection = new THnSparseD("hist_reco_trk_beforeselection", "hist_reco_trk_beforeselection", 5, bins_trk, xmin_trk, xmax_trk);
THnSparseD *hist_reco_trk = new THnSparseD("hist_reco_trk", "hist_reco_trk", 5, bins_trk, xmin_trk, xmax_trk);
THnSparseD *hist_reco_trk_corr = new THnSparseD("hist_reco_trk_corr", "hist_reco_trk_corr", 5, bins_trk, xmin_trk, xmax_trk);
// --> Gen
THnSparseD *hist_gen_trk = new THnSparseD("hist_gen_trk", "hist_gen_trk", 5, bins_trk, xmin_trk, xmax_trk);

// HBT histograms for tests
// Axis : 0 -> qinv, 1 -> kT, 2 -> centrality bin
int	bins_qinv[5]      =   { nQBins , nKtBins  		   ,   nCentBins};
double xmin_qinv[5]   =   { minQ   , KtBins[0] 		   ,   CentBins[0]};
double xmax_qinv[5]   =   { maxQ   , KtBins[nKtBins+1] ,   CentBins[nCentBins+1]};
THnSparseD *hist_qinv_SS = new THnSparseD("hist_qinv_SS", "hist_qinv_SS", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_INV = new THnSparseD("hist_qinv_SS_INV", "hist_qinv_SS_INV", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_ROT = new THnSparseD("hist_qinv_SS_ROT", "hist_qinv_SS_ROT", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_MIX = new THnSparseD("hist_qinv_SS_MIX", "hist_qinv_SS_MIX", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS = new THnSparseD("hist_qinv_OS", "hist_qinv_OS", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_INV = new THnSparseD("hist_qinv_OS_INV", "hist_qinv_OS_INV", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_ROT = new THnSparseD("hist_qinv_OS_ROT", "hist_qinv_OS_ROT", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_MIX = new THnSparseD("hist_qinv_OS_MIX", "hist_qinv_OS_MIX", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_gen = new THnSparseD("hist_qinv_SS_gen", "hist_qinv_SS_gen", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_gen_INV = new THnSparseD("hist_qinv_SS_gen_INV", "hist_qinv_SS_gen_INV", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_gen_ROT = new THnSparseD("hist_qinv_SS_gen_ROT", "hist_qinv_SS_gen_ROT", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_SS_gen_MIX = new THnSparseD("hist_qinv_SS_gen_MIX", "hist_qinv_SS_gen_MIX", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_gen = new THnSparseD("hist_qinv_OS_gen", "hist_qinv_OS_gen", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_gen_INV = new THnSparseD("hist_qinv_OS_gen_INV", "hist_qinv_OS_gen_INV", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_gen_ROT = new THnSparseD("hist_qinv_OS_gen_ROT", "hist_qinv_OS_gen_ROT", 3, bins_qinv, xmin_qinv, xmax_qinv);
THnSparseD *hist_qinv_OS_gen_MIX = new THnSparseD("hist_qinv_OS_gen_MIX", "hist_qinv_OS_gen_MIX", 3, bins_qinv, xmin_qinv, xmax_qinv);

// HBT histograms for tests
// Axis : 0 -> qlong, 1 -> qout, 2 -> qside, 3 -> kT, 4 -> centrality bin
int	bins_q3D[5]      =   { nQBins3D, nQBins3D, nQBins3D,  nKtBins 		      , nCentBins};
double xmin_q3D[5]   =   { minQ3D  , minQ3D  , minQ3D  ,  KtBins[0] 		  , CentBins[0]};
double xmax_q3D[5]   =   { maxQ3D  , maxQ3D  , maxQ3D  ,  KtBins[nKtBins+1]   , CentBins[nCentBins+1]};
THnSparseD *hist_q3D_SS = new THnSparseD("hist_q3D_SS", "hist_q3D_SS", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_INV = new THnSparseD("hist_q3D_SS_INV", "hist_q3D_SS_INV", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_ROT = new THnSparseD("hist_q3D_SS_ROT", "hist_q3D_SS_ROT", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_MIX = new THnSparseD("hist_q3D_SS_MIX", "hist_q3D_SS_MIX", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS = new THnSparseD("hist_q3D_OS", "hist_q3D_OS", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_INV = new THnSparseD("hist_q3D_OS_INV", "hist_q3D_OS_INV", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_ROT = new THnSparseD("hist_q3D_OS_ROT", "hist_q3D_OS_ROT", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_MIX = new THnSparseD("hist_q3D_OS_MIX", "hist_q3D_OS_MIX", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_gen = new THnSparseD("hist_q3D_SS_gen", "hist_q3D_SS_gen", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_gen_INV = new THnSparseD("hist_q3D_SS_gen_INV", "hist_q3D_SS_gen_INV", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_gen_ROT = new THnSparseD("hist_q3D_SS_gen_ROT", "hist_q3D_SS_gen_ROT", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_SS_gen_MIX = new THnSparseD("hist_q3D_SS_gen_MIX", "hist_q3D_SS_gen_MIX", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_gen = new THnSparseD("hist_q3D_OS_gen", "hist_q3D_OS_gen", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_gen_INV = new THnSparseD("hist_q3D_OS_gen_INV", "hist_q3D_OS_gen_INV", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_gen_ROT = new THnSparseD("hist_q3D_OS_gen_ROT", "hist_q3D_OS_gen_ROT", 5, bins_q3D, xmin_q3D, xmax_q3D);
THnSparseD *hist_q3D_OS_gen_MIX = new THnSparseD("hist_q3D_OS_gen_MIX", "hist_q3D_OS_gen_MIX", 5, bins_q3D, xmin_q3D, xmax_q3D);

void sw2(){

	Nevents->Sumw2();
	centrality_beforefilters->Sumw2();
	centrality->Sumw2();
	vzhist->Sumw2();
	multiplicity->Sumw2();
	MultVSCent->Sumw2();
	ptresolution_beforeselection->Sumw2();
	dxyoversigmadxy_beforeselection->Sumw2();
	dzoversigmadz_beforeselection->Sumw2();
	chi2overNDFoverNLayer_beforeselection->Sumw2();
	nhits_beforeselection->Sumw2();
	npixelhit_beforeselection->Sumw2();
	hist_reco_trk_beforeselection->Sumw2();
	ptresolution->Sumw2();
	dxyoversigmadxy->Sumw2();
	dzoversigmadz->Sumw2();
	chi2overNDFoverNLayer->Sumw2();
	nhits->Sumw2();
	npixelhit->Sumw2();
	hist_reco_trk->Sumw2();
	hist_reco_trk_corr->Sumw2();
	hist_gen_trk->Sumw2();
	hist_qinv_SS->Sumw2();
	hist_qinv_SS_INV->Sumw2();
	hist_qinv_SS_ROT->Sumw2();
	hist_qinv_SS_MIX->Sumw2();
	hist_qinv_OS->Sumw2();
	hist_qinv_OS_INV->Sumw2();
	hist_qinv_OS_ROT->Sumw2();
	hist_qinv_OS_MIX->Sumw2();
	hist_q3D_SS->Sumw2();
	hist_q3D_SS_INV->Sumw2();
	hist_q3D_SS_ROT->Sumw2();
	hist_q3D_SS_MIX->Sumw2();
	hist_q3D_OS->Sumw2();
	hist_q3D_OS_INV->Sumw2();
	hist_q3D_OS_ROT->Sumw2();
	hist_q3D_OS_MIX->Sumw2();
	hist_qinv_SS_gen->Sumw2();
	hist_qinv_SS_gen_INV->Sumw2();
	hist_qinv_SS_gen_ROT->Sumw2();
	hist_qinv_SS_gen_MIX->Sumw2();
	hist_qinv_OS_gen->Sumw2();
	hist_qinv_OS_gen_INV->Sumw2();
	hist_qinv_OS_gen_ROT->Sumw2();
	hist_qinv_OS_gen_MIX->Sumw2();
	hist_q3D_SS_gen->Sumw2();
	hist_q3D_SS_gen_INV->Sumw2();
	hist_q3D_SS_gen_ROT->Sumw2();
	hist_q3D_SS_gen_MIX->Sumw2();
	hist_q3D_OS_gen->Sumw2();
	hist_q3D_OS_gen_INV->Sumw2();
	hist_q3D_OS_gen_ROT->Sumw2();
	hist_q3D_OS_gen_MIX->Sumw2();
		
	hist_reco_trk_beforeselection->GetAxis(4)->Set(bins_trk[4],CentBins);
	hist_reco_trk->GetAxis(4)->Set(bins_trk[4],CentBins);	
	hist_reco_trk_corr->GetAxis(4)->Set(bins_trk[4],CentBins);
	hist_gen_trk->GetAxis(4)->Set(bins_trk[4],CentBins);
	
	hist_qinv_SS->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_INV->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_ROT->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_MIX->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_INV->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_ROT->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_MIX->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_SS_INV->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_SS_ROT->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_SS_MIX->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_INV->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_ROT->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_MIX->GetAxis(2)->Set(bins_qinv[2],CentBins);

	hist_q3D_SS->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_INV->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_ROT->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_MIX->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_INV->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_ROT->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_MIX->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_SS_INV->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_SS_ROT->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_SS_MIX->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_INV->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_ROT->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_MIX->GetAxis(4)->Set(bins_q3D[4],CentBins);

	hist_qinv_SS_gen->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_gen_INV->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_gen_ROT->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_gen_MIX->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_gen->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_gen_INV->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_gen_ROT->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_OS_gen_MIX->GetAxis(1)->Set(bins_qinv[1],KtBins);
	hist_qinv_SS_gen->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_SS_gen_INV->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_SS_gen_ROT->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_SS_gen_MIX->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_gen->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_gen_INV->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_gen_ROT->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_qinv_OS_gen_MIX->GetAxis(2)->Set(bins_qinv[2],CentBins);
	hist_q3D_SS_gen->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_gen_INV->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_gen_ROT->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_gen_MIX->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_gen->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_gen_INV->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_gen_ROT->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_OS_gen_MIX->GetAxis(3)->Set(bins_q3D[3],KtBins);
	hist_q3D_SS_gen->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_SS_gen_INV->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_SS_gen_ROT->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_SS_gen_MIX->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_gen->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_gen_INV->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_gen_ROT->GetAxis(4)->Set(bins_q3D[4],CentBins);
	hist_q3D_OS_gen_MIX->GetAxis(4)->Set(bins_q3D[4],CentBins);
	
}

// histogram writting

void write_eventQA(){
	Nevents->Write();
	centrality_beforefilters->Write();
	centrality->Write();
	vzhist->Write();
	multiplicity->Write();
	MultVSCent->Write();
}

void write_trackQA(bool is_MC){
	// no track selection
	ptresolution_beforeselection->Write();
	dxyoversigmadxy_beforeselection->Write();
	dzoversigmadz_beforeselection->Write();
	chi2overNDFoverNLayer_beforeselection->Write();
	nhits_beforeselection->Write();
	npixelhit_beforeselection->Write();
	hist_reco_trk_beforeselection->Write();
	// with track selection
	ptresolution->Write();
	dxyoversigmadxy->Write();
	dzoversigmadz->Write();
	chi2overNDFoverNLayer->Write();
	nhits->Write();
	npixelhit->Write();
	// no track selection
	hist_reco_trk->Write();
	// applying the 
	hist_reco_trk_corr->Write();
	// for MC gen level
	if(is_MC) hist_gen_trk->Write();

}

void write_HBT1D(bool is_MC){

	hist_qinv_SS->Write();
	hist_qinv_SS_INV->Write();
	hist_qinv_SS_ROT->Write();
	hist_qinv_SS_MIX->Write();
	hist_qinv_OS->Write();
	hist_qinv_OS_INV->Write();
	hist_qinv_OS_ROT->Write();
	hist_qinv_OS_MIX->Write();
	
	if(is_MC){
		hist_qinv_SS_gen->Write();
		hist_qinv_SS_gen_INV->Write();
		hist_qinv_SS_gen_ROT->Write();
		hist_qinv_SS_gen_MIX->Write();
		hist_qinv_OS_gen->Write();
		hist_qinv_OS_gen_INV->Write();
		hist_qinv_OS_gen_MIX->Write();
	}

}

void write_HBT3D(bool is_MC){

	hist_q3D_SS->Write();
	hist_q3D_SS_INV->Write();
	hist_q3D_SS_ROT->Write();
	hist_q3D_SS_MIX->Write();
	hist_q3D_OS->Write();
	hist_q3D_OS_INV->Write();
	hist_q3D_OS_ROT->Write();
	hist_q3D_OS_MIX->Write();
	
	if(is_MC){
		hist_q3D_SS_gen->Write();
		hist_q3D_SS_gen_INV->Write();
		hist_q3D_SS_gen_ROT->Write();
		hist_q3D_SS_gen_MIX->Write();
		hist_q3D_OS_gen->Write();
		hist_q3D_OS_gen_INV->Write();
		hist_q3D_OS_gen_ROT->Write();
		hist_q3D_OS_gen_MIX->Write();
	}

}

