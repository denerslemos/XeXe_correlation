#include "call_libraries.h"  // call libraries from ROOT and C++
#include "function_definition.h" // function definition
#define coscutmix 0.99996
#define dptcutmix 0.04

void MixEvents(bool use_centrality, int centrality_or_ntrkoff_int, int nEvt_to_mix, std::vector<int> ev_centrality, std::vector<int> ev_multiplicity, std::vector<double> vtx_z_vec, float vzcut, std::vector<std::vector<ROOT::Math::PtEtaPhiMVector>> Track_Vector, std::vector<std::vector<int>> Track_Chg_Vector, std::vector<std::vector<double>> Track_Eff_Vector, THnSparseD *histo_SS, THnSparseD *histo_SS3D, THnSparseD *histo_OS, THnSparseD *histo_OS3D, bool docostdptcut, bool do_hbt3d, bool dogamovcorrection, int systematic, TH1I* NeventsAss){

   int aux_n_evts = (int) vtx_z_vec.size(); // total number of events
   int tenper = round(0.1*aux_n_evts);   

   cout << "Running ... " << endl;
   cout << "Total # of events: " << aux_n_evts << endl; 
   
   for(int nevt_trg = 0; nevt_trg < aux_n_evts; nevt_trg++){ // first loop over all events

      if(nevt_trg != 0 && (nevt_trg % tenper) == 0){cout << nevt_trg << " out of " << aux_n_evts << endl;} // % processed

      std::vector<ROOT::Math::PtEtaPhiMVector> Trk_nevt_trg_vec = Track_Vector[nevt_trg]; // track 4-vector for each trigger event
      std::vector<int> Trk_chg_nevt_trg_vec = Track_Chg_Vector[nevt_trg]; // track charge vector for each trigger event
      std::vector<double> Trk_eff_nevt_trg_vec = Track_Eff_Vector[nevt_trg]; // track efficiency vector for each trigger event
      int nMix_nevt_trg = Trk_nevt_trg_vec.size(); // track vector size for triggers

      int assloop_start;
      float wrap_evnt = (float) 0.7 * aux_n_evts;
      if( nevt_trg < round(wrap_evnt) ){ assloop_start = nevt_trg + 1; } else { assloop_start = 0; }

      int n_associated = 0; // counter used to find the number to mix
      
      for (int nevt_assoc = assloop_start; nevt_assoc < aux_n_evts; nevt_assoc++){

         if(nevt_trg == nevt_assoc) continue; // avoid same event
         if(use_centrality){ if(fabs(ev_centrality[nevt_trg] - ev_centrality[nevt_assoc]) > centrality_or_ntrkoff_int) continue;
         }else{ if(fabs(ev_multiplicity[nevt_trg] - ev_multiplicity[nevt_assoc]) > centrality_or_ntrkoff_int) continue; }
         
         if(fabs(vtx_z_vec[nevt_trg]) > 8.0 && fabs(vtx_z_vec[nevt_trg]) < 10.0) vzcut = 2.0 * vzcut;
         if(fabs(vtx_z_vec[nevt_trg]) > 10.0) vzcut = 5.0 * vzcut;
         if(fabs(vtx_z_vec[nevt_trg] - vtx_z_vec[nevt_assoc]) > vzcut) continue; // vz matching

         std::vector<ROOT::Math::PtEtaPhiMVector> Track_nevt_ass_vec = Track_Vector[nevt_assoc]; // track 4-vector for each associate event
	 std::vector<int> Trk_chg_nevt_ass_vec = Track_Chg_Vector[nevt_assoc]; // track charge vector for each associate event
	 std::vector<double> Trk_eff_nevt_ass_vec = Track_Eff_Vector[nevt_assoc]; // track efficiency vector for each associate event
         int nMix_nevt_ass = (int)Track_nevt_ass_vec.size();  // track vector size for associate

         n_associated = n_associated + 1; // if pass the requirements sum 1
      	 if(n_associated > nEvt_to_mix)break;   
         // loop and fill correlation histograms
	 	 for (int ipair = 0; ipair < (nMix_nevt_trg * nMix_nevt_ass); ipair++){ // start loop over tracks

	        	int imix = ipair / nMix_nevt_ass;
    	    		int iimix = ipair % nMix_nevt_ass;
    	    
			double eff_trk_imix = Trk_eff_nevt_trg_vec[imix];
			double eff_trk_iimix = Trk_eff_nevt_ass_vec[iimix];
			double tot_eff = eff_trk_imix * eff_trk_iimix;
			if(fabs(Trk_nevt_trg_vec[imix].Eta() - Track_nevt_ass_vec[iimix].Eta()) == 0 && fabs(Trk_nevt_trg_vec[imix].Phi() - Track_nevt_ass_vec[iimix].Phi()) == 0) continue;
			if(docostdptcut){ if(splitcomb(Trk_nevt_trg_vec[imix],Track_nevt_ass_vec[iimix],coscutmix,dptcutmix)) continue; }
        		ROOT::Math::PtEtaPhiMVector psum2 = Trk_nevt_trg_vec[imix] + Track_nevt_ass_vec[iimix];
        		double kt = (psum2.Pt())/2.;
			double qinv = GetQ(Trk_nevt_trg_vec[imix],Track_nevt_ass_vec[iimix]);
			double qlong = GetQlongLCMS(Trk_nevt_trg_vec[imix],Track_nevt_ass_vec[iimix]);
			double qout = GetQout(Trk_nevt_trg_vec[imix],Track_nevt_ass_vec[iimix]);
			double qside = GetQside(Trk_nevt_trg_vec[imix],Track_nevt_ass_vec[iimix]);
			double x_2pc_hbt[3]={qinv, kt, (double)ev_centrality[nevt_trg]}; 
			double x_2pc_hbt_3D[5]={qlong, qout, qside, kt, (double)ev_centrality[nevt_trg]}; 
			double coulomb_ss = 1.0;
			double coulomb_os = 1.0;
			if(dogamovcorrection){
				coulomb_ss = CoulombSS(qinv,systematic);
				coulomb_os = CoulombOS(qinv,systematic);		
			}
			if(Trk_chg_nevt_trg_vec[imix]*Trk_chg_nevt_ass_vec[iimix] > 0){
				histo_SS->Fill(x_2pc_hbt,coulomb_ss*tot_eff);
				if(do_hbt3d) histo_SS3D->Fill(x_2pc_hbt_3D,coulomb_ss*tot_eff);
			}else{
				histo_OS->Fill(x_2pc_hbt,coulomb_os*tot_eff);			
				if(do_hbt3d) histo_OS3D->Fill(x_2pc_hbt_3D,coulomb_os*tot_eff);			
			}						
         } // end of correlation loop
      if(n_associated == nEvt_to_mix)break;
      } // end of associate loop
      NeventsAss->Fill(n_associated);
   } // end of all events loop
}
