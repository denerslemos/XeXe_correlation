#include "call_libraries.h"  // call libraries from ROOT and C++
#define coscut 0.99996
#define dptcut 0.04

/*
Calculate q invariant
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
bool splitcomb(ROOT::Math::PtEtaPhiMVector &vec1,ROOT::Math::PtEtaPhiMVector &vec2, double cos_cut, double dpt_cut){
   bool issplit=false;
   Double_t cosa = TMath::Abs(vec1.Px()*vec2.Px() + vec1.Py()*vec2.Py() + vec1.Pz()*vec2.Pz())/(vec1.P()*vec2.P());
   Double_t deltapt = TMath::Abs(vec1.Pt() - vec2.Pt());
   if ( (cosa > cos_cut) && (deltapt < dpt_cut)) { issplit = true;}
   return issplit;
}

/*
Calculate q invariant
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQ(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   ROOT::Math::PtEtaPhiMVector Sum4V = p1+p2;
   Double_t q = Sum4V.M2() - 4.0*p1.mass()*p2.mass();
   return (  q > 0 ?  TMath::Sqrt(q) : -TMath::Sqrt(-q)  );
}

/*
Calculate q long in the LCMS
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQlongLCMS(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   Double_t num = 2*( (p1.Pz())*(p2.E()) - (p2.Pz())*(p1.E()) );
   Double_t den = TMath::Sqrt( (p1.E()+p2.E())*(p1.E()+p2.E()) - (p1.Pz()+p2.Pz())*(p1.Pz()+p2.Pz()) );
   Double_t qlongLCMS = 0.0;
   if(den != 0) qlongLCMS = fabs(num/den);
   return qlongLCMS;
}

/*
Calculate q out in the LCMS
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQout(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   TVector3 qT;
   qT.SetXYZ(p1.Px()-p2.Px(),p1.Py()-p2.Py(),0.0);
   TVector3 kT;
   kT.SetXYZ( (p1.Px()+p2.Px())/2.0 , (p1.Py()+p2.Py())/2.0 ,0.0);
   TVector3 qout;
   qout = qT.Dot(kT.Unit())*kT.Unit();
   Double_t absValue = qout.Mag();
   return absValue; 
}

/*
Calculate q side in the LCMS
--> Arguments
p1: particle 1 4-vector
p2: particle 2 4-vector
*/
float GetQside(ROOT::Math::PtEtaPhiMVector &p1, ROOT::Math::PtEtaPhiMVector &p2){
   TVector3 qT;
   qT.SetXYZ(p1.Px()-p2.Px(),p1.Py()-p2.Py(),0.0);
   TVector3 kT;
   kT.SetXYZ( (p1.Px()+p2.Px())/2.0 , (p1.Py()+p2.Py())/2.0 ,0.0);
   TVector3 qout;
   qout = qT.Dot(kT.Unit())*kT.Unit();
   TVector3 qsid;
   qsid = qT - qout;
   Double_t absValue = qsid.Mag();
   return absValue;
}

ROOT::Math::PtEtaPhiMVector InvertPVector( ROOT::Math::PtEtaPhiMVector &vec){
   ROOT::Math::PtEtaPhiMVector ovec = vec;
   ovec.SetPxPyPzE(-vec.Px(),-vec.Py(),-vec.Pz(),vec.E());
   return ovec;
}

ROOT::Math::PtEtaPhiMVector InvertXYVector( ROOT::Math::PtEtaPhiMVector &vec){
  ROOT::Math::PtEtaPhiMVector ovec = vec;
  ovec.SetXYZT(-vec.X(),-vec.Y(),vec.Z(),vec.T());
  return ovec;
}

/*
Measure the 2 particle correlation
--> Arguments
tracks: vector with track informations
tracks_charge: vector with track charge informations
tracks_weight: vector with track efficiency informations
*/
void twoparticlecorrelation(std::vector<ROOT::Math::PtEtaPhiMVector> tracks, std::vector<int> tracks_charge, std::vector<double> tracks_weight, THnSparse* histo_2pcorr_samesign,  THnSparse* histo_2pcorr_samesign_inverted,  THnSparse* histo_2pcorr_samesign_rotated, THnSparse* histo_2pcorr_samesign3D,  THnSparse* histo_2pcorr_samesign3D_inverted,  THnSparse* histo_2pcorr_samesign3D_rotated, THnSparse* histo_2pcorr_oppsign, THnSparse* histo_2pcorr_oppsign_inverted, THnSparse* histo_2pcorr_oppsign_rotated, THnSparse* histo_2pcorr_oppsign3D, THnSparse* histo_2pcorr_oppsign3D_inverted, THnSparse* histo_2pcorr_oppsign3D_rotated, int cent, bool docostdptcut){
	// get correlation histograms
	for (int a = 0; a < tracks.size(); a++){ // start loop over tracks
		double eff_trk_a = tracks_weight[a];
		for (int b = a+1; b < tracks.size(); b++){ // start loop over tracks+1
			double eff_trk_b = tracks_weight[b];
			double tot_eff = eff_trk_a*eff_trk_b;

			if(fabs(tracks[a].Eta() - tracks[b].Eta()) == 0 && fabs(tracks[a].Phi() - tracks[b].Phi()) == 0) continue;
			if(docostdptcut) splitcomb(tracks[a],tracks[b],coscut,dptcut);
			
        	ROOT::Math::PtEtaPhiMVector psum2 = tracks[a] + tracks[b];
        	double kt = (psum2.Pt())/2.;
        	ROOT::Math::PtEtaPhiMVector psum2_inverted = tracks[a] + InvertPVector(tracks[b]);
        	double kt_inverted = (psum2_inverted.Pt())/2.;
        	ROOT::Math::PtEtaPhiMVector psum2_rotated = tracks[a] + InvertXYVector(tracks[b]);
        	double kt_rotated = (psum2_rotated.Pt())/2.;

			ROOT::Math::PtEtaPhiMVector trackb_inverted = InvertPVector(tracks[b]);
			ROOT::Math::PtEtaPhiMVector trackb_rotated = InvertXYVector(tracks[b]);

			double qinv = GetQ(tracks[a],tracks[b]);
			double qinv_inverted = GetQ(tracks[a], trackb_inverted);
			double qinv_rotated = GetQ(tracks[a], trackb_rotated);
			double qlong = GetQlongLCMS(tracks[a],tracks[b]);
			double qlong_inverted = GetQlongLCMS(tracks[a], trackb_inverted);
			double qlong_rotated = GetQlongLCMS(tracks[a], trackb_rotated);
			double qout = GetQout(tracks[a],tracks[b]);
			double qout_inverted = GetQout(tracks[a], trackb_inverted);
			double qout_rotated = GetQout(tracks[a], trackb_rotated);
			double qside = GetQside(tracks[a],tracks[b]);
			double qside_inverted = GetQside(tracks[a], trackb_inverted);
			double qside_rotated = GetQside(tracks[a], trackb_rotated);
			double x_2pc_hbt[3]={qinv, kt, (double)cent}; 
			double x_2pc_hbt_inv[3]={qinv_inverted, kt_inverted, (double)cent}; 
			double x_2pc_hbt_rot[3]={qinv_rotated, kt_rotated, (double)cent}; 
			double x_2pc_hbt_3D[5]={qlong, qout, qside, kt, (double)cent}; 
			double x_2pc_hbt_3D_inv[5]={qlong_inverted, qout_inverted, qside_inverted, kt_inverted, (double)cent}; 
			double x_2pc_hbt_3D_rot[5]={qlong_rotated, qout_rotated, qside_rotated, kt_rotated, (double)cent}; 
			if(tracks_charge[a]*tracks_charge[b] > 0){
				histo_2pcorr_samesign->Fill(x_2pc_hbt,tot_eff);
				histo_2pcorr_samesign_inverted->Fill(x_2pc_hbt_inv,tot_eff);
				histo_2pcorr_samesign_rotated->Fill(x_2pc_hbt_rot,tot_eff);
				histo_2pcorr_samesign3D->Fill(x_2pc_hbt_3D,tot_eff);
				histo_2pcorr_samesign3D_inverted->Fill(x_2pc_hbt_3D_inv,tot_eff);
				histo_2pcorr_samesign3D_rotated->Fill(x_2pc_hbt_3D_rot,tot_eff);
			}else{
				histo_2pcorr_oppsign->Fill(x_2pc_hbt,tot_eff);			
				histo_2pcorr_oppsign_inverted->Fill(x_2pc_hbt_inv,tot_eff);			
				histo_2pcorr_oppsign_rotated->Fill(x_2pc_hbt_rot,tot_eff);			
				histo_2pcorr_oppsign3D->Fill(x_2pc_hbt_3D,tot_eff);			
				histo_2pcorr_oppsign3D_inverted->Fill(x_2pc_hbt_3D_inv,tot_eff);			
				histo_2pcorr_oppsign3D_rotated->Fill(x_2pc_hbt_3D_rot,tot_eff);			
			}
		} // b loop
	} // a loop
}
