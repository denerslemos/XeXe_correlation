#include "call_libraries.h"  // call libraries from ROOT and C++

// Obtain weight from tracking efficiency correction - default used by CMS
// it returns a efficiency correction to be applied in histograms as -> Fill(variable,this_weight)

bool checkBounds(double pt, double eta){
  if( TMath::Abs(eta) > 2.4 ){return false;}
  if( pt < 0 || pt > 500 ){return false;}
  return true;
}

// inputs are 2D histograms: reff2D for efficiency, rfak2D for fakes, rsec2D for secondary (decays), rmul2D for multiple reconstruction
// pT and eta are the transverse momentum and pseudorapidity of the track (considering a 2D histogram where X is eta axis and Y pT axis)
double getTrkCorrWeight(TFile *trkeff_file, int centrality, double pT, double eta){
  if( !checkBounds(pT, eta) ) return 0;
  double factor = 1.0;
  TH2 *eff_factor = nullptr; 
  // eff / (1 - fake rate)
  if(centrality <= 20){trkeff_file->GetObject("rTotalEff3D_0_10", eff_factor);
  }else if (centrality > 20 && centrality <= 60){trkeff_file->GetObject("rTotalEff3D_10_30", eff_factor);
  }else if (centrality > 60 && centrality <= 100){trkeff_file->GetObject("rTotalEff3D_30_50", eff_factor);
  }else if (centrality > 100 && centrality <= 140){trkeff_file->GetObject("rTotalEff3D_50_70", eff_factor); 
  }else if (centrality > 140){trkeff_file->GetObject("rTotalEff3D_70_100", eff_factor);}
  double eff = eff_factor->GetBinContent( eff_factor->GetXaxis()->FindBin(eta),eff_factor->GetYaxis()->FindBin(pT) );
  factor = (1. / eff); //only efficiency
  delete eff_factor;
  eff_factor = nullptr;
  return factor;
}
