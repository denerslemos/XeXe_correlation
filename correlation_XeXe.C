#include "call_libraries.h" // call libraries from ROOT and C++
#include "read_tree.h" // read the TChains
#include "tracking_correction.h" // tracking correction
#include "define_histograms_cent.h" // histogram definition
//#include "define_histograms_mult.h" // histogram definition
#define pi_mass 0.1396

/*
--> Arguments
input_file: text file with a list of root input files: Forest or Skims
ouputfile: just a counting number to run on Condor
isMC: 0 for false --> data and > 0 for true --> MC
doquicktest: 0 for false and > 0 for true --> tests with 1000 events
domixing: 0 with mixing and 1 without mixing
Nmixevents: number of events to mix
mincentormult: minimum centrality of multiplicity in the mixing
minvz: minimum vertez between events in the mixing
hbt3d: 0 with 3D and 1 without 3D
gamov: 0 means no GAMOV Coulomb correction and > 0 means GAMOV is added
cent_bool: 0 to use centrality and different than 0 for multiplicity. For multiplicity we may need to change the bins in define_histograms.h
syst:systematic uncertainties
	--> 0: nominal
	--> 1: |vz| < 3
	--> 2: 3 < |vz| < 15
	--> 3: tracking tight
	--> 4: tracking loose
	--> 5: centrality up
	--> 6: centrality down
	--> 7: removal of duplication cut
	--> 8: removal of Npixel cut
	--> 9: gamov + 15% weight
	--> 10: gamov - 15% weight
	--> 11: Number of mix events from 10 to 20
	--> 12: Number of mix events from 10 to 5
	--> 13: Number of vz separation btw events from 2cm to 3cm
	--> 14: Number of vz separation btw events from 2cm to 1cm
	--> 15: Number of multiplicity/centrality separation btw events from 5 to 10 units
	--> 16: Number of multiplicity/centrality separation btw events from 5 to 3 units
*/
void correlation_XeXe(TString input_file, TString ouputfile, int isMC, int doquicktest, int domixing, int Nmixevents, int mincentormult, float minvz, int hbt3d, int gamov, int cent_bool, int syst){

	clock_t sec_start, sec_end;
	sec_start = clock(); // start timing measurement
	TDatime* date = new TDatime(); // to add date in the output file

	bool do_quicktest; if(doquicktest == 0){do_quicktest = false;}else{do_quicktest = true;}
	bool is_MC; if(isMC == 0){is_MC = false;}else{is_MC = true;}
	bool do_mixing; if(domixing == 0){do_mixing = true;}else{do_mixing = false;}
	bool do_hbt3d; if(hbt3d == 0){do_hbt3d = true;}else{do_hbt3d = false;}
	bool do_gamov; if(gamov == 0){do_gamov = false;}else{do_gamov = true;}
	bool use_centrality; if(cent_bool == 0){use_centrality = true;}else{use_centrality = false;}
	
	if(syst == 9 || syst == 10) do_gamov = true;
	
	bool dosplit = false;
	if(syst != 7) dosplit = true; 
	
	TString systematics = "nonapplied_nominal";
	if(syst == 0) systematics =  "nominal";
	if(syst == 1) systematics =  "vznarrow";
	if(syst == 2) systematics =  "vzwide";
	if(syst == 3) systematics =  "trktight";
	if(syst == 4) systematics =  "trkloose";
	if(syst == 5) systematics =  "centup";
	if(syst == 6) systematics =  "centdown";
	if(syst == 7) systematics =  "removeduplicatedcut";
	if(syst == 8) systematics =  "removeNpixelhitcut";
	if(syst == 9) systematics =  "gamovplus15";
	if(syst == 10) systematics =  "gamovminus15";
	if(syst == 11){ systematics =  "Nmix20"; Nmixevents = Nmixevents + 10; }
	if(syst == 12){ systematics =  "Nmix05"; Nmixevents = Nmixevents - 5; }
	if(syst == 13){ systematics =  "minvz3"; minvz = minvz + 1.0; }
	if(syst == 14){ systematics =  "minvz1"; minvz = minvz - 1.0; }
	if(syst == 15){ systematics =  "mincentormult10"; mincentormult = mincentormult + 5; }
	if(syst == 16){ systematics =  "mincentormult3"; mincentormult = mincentormult - 2; }
	
	if(use_centrality){ systematics +=  "_cent"; } else { systematics +=  "_Ntroff"; }
	
	// Track or particle efficiency file
	TFile *fileeff;
	fileeff = TFile::Open("efftables/XeXe_eff_table_94x_cent.root");
	if(syst != 1 && syst != 2 && syst != 3 && syst != 4) fileeff = TFile::Open("efftables/XeXe_eff_table_94x_cent.root");
	if(syst == 1) fileeff = TFile::Open("efftables/XeXe_eff_narrow_table_94x_cent.root");
	if(syst == 2) fileeff = TFile::Open("efftables/XeXe_eff_wide_table_94x_cent.root");
	if(syst == 3) fileeff = TFile::Open("efftables/XeXe_eff_tight_table_94x_cent.root");
	if(syst == 4) fileeff = TFile::Open("efftables/XeXe_eff_loose_table_94x_cent.root");

	TH2 *trkeff_file010 = nullptr; 
	fileeff->GetObject("rTotalEff3D_0_10", trkeff_file010);
	TH2 *trkeff_file1030 = nullptr; 
	fileeff->GetObject("rTotalEff3D_10_30", trkeff_file1030);
	TH2 *trkeff_file3050 = nullptr; 
	fileeff->GetObject("rTotalEff3D_30_50", trkeff_file3050);
	TH2 *trkeff_file5070 = nullptr; 
	fileeff->GetObject("rTotalEff3D_50_70", trkeff_file5070);
	TH2 *trkeff_file70100 = nullptr; 
	fileeff->GetObject("rTotalEff3D_70_100", trkeff_file70100);

	// Read the list of input file(s)
	fstream inputfile;
	inputfile.open(Form("%s",input_file.Data()), ios::in);
	if(!inputfile.is_open()){cout << "List of input files not founded!" << endl; return;}{cout << "List of input files founded! --> " << input_file.Data() << endl;}

	// Make a chain and a vector of file names
	std::vector<TString> file_name_vector;
	string file_chain;
	while(getline(inputfile, file_chain)){file_name_vector.push_back(file_chain.c_str());}
	inputfile.close();

	// Read the trees to be added in the Chain
	TChain *hea_tree = new TChain("hiEvtAnalyzer/HiTree"); // event quantities
	TChain *ski_tree = new TChain("skimanalysis/HltTree"); // event filters
	TChain *trk_tree = new TChain("ppTrack/trackTree"); // for tracking
	TChain *gen_tree;
	if(is_MC){gen_tree = new TChain("HiGenParticleAna/hi");} // MC gen particles
	// loop to add all the trees to the chain
	for (std::vector<TString>::iterator listIterator = file_name_vector.begin(); listIterator != file_name_vector.end(); listIterator++){
        TFile *testfile = TFile::Open(*listIterator,"READ");
		if(testfile && !testfile->IsZombie() && !testfile->TestBit(TFile::kRecovered)){ // safety against corrupted files
		cout << "Adding file " << *listIterator << " to the chains" << endl; // adding files to the chains for each step
		hea_tree->Add(*listIterator);
		ski_tree->Add(*listIterator);
		trk_tree->Add(*listIterator);
		if(is_MC){gen_tree->Add(*listIterator);}
		}else{cout << "File: " << *listIterator << " failed!" << endl;}		
	}
	file_name_vector.clear();	
	
	// Connect all chains
	hea_tree->AddFriend(ski_tree);
	hea_tree->AddFriend(trk_tree);
	if(is_MC){hea_tree->AddFriend(gen_tree);}
	
    // Read the desired branchs in the trees
	read_tree(hea_tree, is_MC); // access the tree informations
	
    // Use sumw2() to make sure about histogram uncertainties in ROOT
	sw2(); 

	int nevents = hea_tree->GetEntries(); // number of events
	cout << endl;
	cout << "Total number of events in those files: "<< nevents << endl;
	cout << endl;
	cout << "-------------------------------------------------" << endl;

	//define vectors for mixing
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

	double nev = (double)nevents;
	
	for(int i = 0; i < nevents; i++){

		hea_tree->GetEntry(i); // get events from ttree

		if(i != 0 && (i % 10) == 0){double alpha = (double)i; cout << " Running -> percentage: " << std::setprecision(3) << ((alpha / nev) * 100) << "%" << endl;} // % processed
		if(do_quicktest){if(i != 0 && i % 200 == 0 ) break;} // just for quick tests

		int cent;
		if(syst == 5){ cent = (int) (0.98 * (float)hiBin / 0.95);
		} else if(syst == 6){ cent = (int) (0.92 * (float)hiBin / 0.95);
		} else{ cent = (int) hiBin; }

		TH2 *trkeff_file = nullptr; 
		if(hiBin <= 20){trkeff_file = (TH2*)trkeff_file010->Clone();
		}else if (hiBin > 20 && hiBin <= 60){trkeff_file = (TH2*)trkeff_file1030->Clone();
		}else if (hiBin > 60 && hiBin <= 100){trkeff_file = (TH2*)trkeff_file3050->Clone();
		}else if (hiBin > 100 && hiBin <= 140){trkeff_file = (TH2*)trkeff_file5070->Clone();
		}else if (hiBin > 140){trkeff_file = (TH2*)trkeff_file70100->Clone();}
		
		int Ntroff = 0;
		if(!use_centrality) Ntroff = get_Ntrkoff(ntrk, trkpt, trketa, highpur, trkpterr, trkdcaxy, trkdcaxyerr, trkdcaz, trkdcazerr);
		
		centrality_beforefilters->Fill(cent);
		vzhist_beforefilters->Fill(vertexz);

		Nevents->Fill(0); // filled after each event cut		
		// Apply event filters
		if(PrimaryVertexFilter != 1) continue;
		Nevents->Fill(1); // filled after each event cut		
		if(BeamScrapingFilter != 1) continue;
		Nevents->Fill(2); // filled after each event cut		
		if(HfCoincFilter != 1) continue;
		Nevents->Fill(3); // filled after each event cut		

		if(syst == 1){ if(fabs(vertexz) > 3.0) continue; 
		} else if(syst == 2){ if( fabs(vertexz) > 15.0 || fabs(vertexz) < 3.0 ) continue; 
		} else{ if(fabs(vertexz) > 15.0) continue; }
		Nevents->Fill(4); // filled after each event cut		

		// Fill event histograms
		centrality->Fill(cent);
		vzhist->Fill(vertexz);

		if(use_centrality) if(hiBin > 180) continue; // remove 90% events due EM contamination
		if(!use_centrality && Ntroff < 10) continue; // just to remove MB track inneficiency		
		if(!use_centrality) if(Ntroff > 500) continue; // remove events with multiplicity > 250 to speed up the code
		Nevents->Fill(5); // filled after each event cut	

		// Vectors used for objects
		std::vector<ROOT::Math::PtEtaPhiMVector> tracks_reco;
		std::vector<double> track_weight_reco;
		std::vector<int> track_charge_reco;
		std::vector<ROOT::Math::PtEtaPhiMVector> tracks_gen;
		std::vector<double> track_weight_gen;
		std::vector<int> track_charge_gen;
		
		// ------------------- Reconstruction level (Data and MC) ----------------------------
		// Start loop over reco tracks (trksize is number of reco tracks)
		CheckNtrk->Fill(ntrk);

		for (int j = 0; j < ntrk; j++){ 
	
		    // Track QA array
			double x_reco_trk[5]={trkpt[j],trketa[j],trkphi[j],(double) trkcharge[j],(double) (use_centrality ? cent : Ntroff)}; 

			// Define some quantities		
			int NDF = (int) trkndof[j];
			int NLayer = (int) trknlayer[j];
			int NHits = (int) trknhits[j];
			int Npixelhit = (int) trkpixhits[j];

			// store quantities without selection
			hist_reco_trk_beforeselection->Fill(x_reco_trk);
			ptresolution_beforeselection->Fill(fabs(trkpterr[j]/trkpt[j]));
			dxyoversigmadxy_beforeselection->Fill((trkdcaxy[j]/trkdcaxyerr[j]));
			dzoversigmadz_beforeselection->Fill((trkdcaz[j]/trkdcazerr[j]));
			chi2overNDFoverNLayer_beforeselection->Fill((trkchi2[j]/(float)NDF)/(float)NLayer);
			nhits_beforeselection->Fill((float)NHits);
			npixelhit_beforeselection->Fill((float)Npixelhit);
			
			// Apply track selection (see read_tree.h to see what each variable means)
			if(trkpt[j] <= 0.2) continue;
			if(fabs(trketa[j]) > 2.4) continue;
			if(highpur[j] == false) continue;
			if(syst != 8) if (Npixelhit == 0) continue;

			if(syst == 3){
				if(fabs(trkpterr[j]/trkpt[j]) >= 0.05) continue;
				if(fabs(trkdcaxy[j]/trkdcaxyerr[j]) >= 2.0) continue;
				if(fabs(trkdcaz[j]/trkdcazerr[j]) >= 2.0) continue;
			}else if(syst == 4){
				if(fabs(trkpterr[j]/trkpt[j]) >= 0.1) continue;
				if(fabs(trkdcaxy[j]/trkdcaxyerr[j]) >= 5.0) continue;
				if(fabs(trkdcaz[j]/trkdcazerr[j]) >= 5.0) continue;
			}else{
				if(fabs(trkpterr[j]/trkpt[j]) >= 0.1) continue;
				if(fabs(trkdcaxy[j]/trkdcaxyerr[j]) >= 3.0) continue;
				if(fabs(trkdcaz[j]/trkdcazerr[j]) >= 3.0) continue;
			}

			// store quantities with selection
			ptresolution->Fill(fabs(trkpterr[j]/trkpt[j]));
			dxyoversigmadxy->Fill((trkdcaxy[j]/trkdcaxyerr[j]));
			dzoversigmadz->Fill((trkdcaz[j]/trkdcazerr[j]));
			chi2overNDFoverNLayer->Fill((trkchi2[j]/(float)NDF)/(float)NLayer);
			nhits->Fill((float)NHits);
			npixelhit->Fill((float)Npixelhit);
			hist_reco_trk->Fill(x_reco_trk);

			// Track efficiency correction
			double trk_weight = 1.0;
			trk_weight = trk_weight*getTrkCorrWeight(trkeff_file, trkpt[j], trketa[j]);
			hist_reco_trk_corr->Fill(x_reco_trk,trk_weight);
			
		    ROOT::Math::PtEtaPhiMVector TrackFourVector;
      		TrackFourVector.SetM(pi_mass);
      		TrackFourVector.SetEta(trketa[j]);
     		TrackFourVector.SetPhi(trkphi[j]);
     		TrackFourVector.SetPt(trkpt[j]);  
     		
     		tracks_reco.push_back(TrackFourVector);
			track_charge_reco.push_back(trkcharge[j]); 
			track_weight_reco.push_back(trk_weight); 

		} // End loop over tracks

		trkeff_file->Reset("ICESM"); // clear the efficiency histograms

		if(use_centrality) Ntroff = tracks_reco.size();	

		multiplicity->Fill(Ntroff);
		MultVSCent->Fill(Ntroff,cent);

		if(tracks_reco.size() > 1){
			Nevents->Fill(6); // filled after each event cut
			twoparticlecorrelation(tracks_reco, track_charge_reco, track_weight_reco, hist_pairSS_Mass, hist_dpt_cos_SS, hist_qinv_SS, hist_qinv_SS_INV, hist_qinv_SS_ROT, hist_q3D_SS, hist_q3D_SS_INV, hist_q3D_SS_ROT, hist_pairOS_Mass, hist_dpt_cos_OS, hist_qinv_OS, hist_qinv_OS_INV, hist_qinv_OS_ROT, hist_q3D_OS, hist_q3D_OS_INV, hist_q3D_OS_ROT, (use_centrality ? cent : Ntroff), dosplit, do_hbt3d, do_gamov, syst); // HBT correlations done at this step
			track_4vector.push_back(tracks_reco); // save 4 vector for mixing
			track_charge_vector.push_back(track_charge_reco); // save charge vector for mixing
			track_weights_vector.push_back(track_weight_reco); // save eff weight vector for mixing
			centrality_vector.push_back(cent); // save centrality vector for mixing
			multiplicity_vector.push_back(Ntroff); // save multiplicity vector for mixing
			vz_vector.push_back(vertexz); // save vz vector for mixing
		}
		
		tracks_reco.clear();
		track_weight_reco.clear();
		track_charge_reco.clear();

		// Generator level --> MC only
		if(is_MC){

			for(int j = 0; j < (int)gen_trkpt->size(); j++){ 
				// Kinematic and charge cuts
				if(fabs(gen_trketa->at(j)) > 2.4) continue;
				if(gen_trkpt->at(j) <= 0.2)continue;
				if(gen_trkchg->at(j) == 0) continue;
				// Track/particle QA histogram filling
				double x_gen_trk[5]={gen_trkpt->at(j), gen_trketa->at(j), gen_trkphi->at(j), (double) gen_trkchg->at(j), (double) (use_centrality ? cent : Ntroff) }; 	
		    	ROOT::Math::PtEtaPhiMVector TrackFourVectorGen;
      			TrackFourVectorGen.SetM(pi_mass);
      			TrackFourVectorGen.SetEta(gen_trketa->at(j));
     			TrackFourVectorGen.SetPhi(gen_trkphi->at(j));
     			TrackFourVectorGen.SetPt(gen_trkpt->at(j));  
     		
     			tracks_gen.push_back(TrackFourVectorGen);
				track_charge_gen.push_back(gen_trkchg->at(j)); 
				track_weight_gen.push_back(1.0); 						
			} // End loop over gen tracks
			
			mcmultiplicity->Fill(tracks_gen.size());

			if(tracks_gen.size() > 1){
				twoparticlecorrelation(tracks_gen, track_charge_gen, track_weight_gen, hist_pairSS_Mass_gen, hist_dpt_cos_SS_gen, hist_qinv_SS_gen, hist_qinv_SS_gen_INV, hist_qinv_SS_gen_ROT, hist_q3D_SS_gen, hist_q3D_SS_gen_INV, hist_q3D_SS_gen_ROT, hist_pairOS_Mass_gen, hist_dpt_cos_OS_gen, hist_qinv_OS_gen, hist_qinv_OS_gen_INV, hist_qinv_OS_gen_ROT, hist_q3D_OS_gen, hist_q3D_OS_gen_INV, hist_q3D_OS_gen_ROT, (use_centrality ? cent : Ntroff), dosplit, do_hbt3d, do_gamov, syst); // HBT correlations done at this step
				track_4vector_gen.push_back(tracks_gen); // save 4 vector for mixing
				track_charge_vector_gen.push_back(track_charge_gen); // save charge vector for mixing
				track_weights_vector_gen.push_back(track_weight_gen); // save eff weight vector for mixing
				centrality_vector_gen.push_back(cent); // save centrality vector for mixing
				multiplicity_vector_gen.push_back(Ntroff); // save multiplicity vector for mixing
				vz_vector_gen.push_back(vertexz); // save vz vector for mixing
			}

			tracks_gen.clear();
			track_weight_gen.clear();
			track_charge_gen.clear();

		} // End of MC IF statement
		
	} // End of event loop
	
	// do the mixing after the event selections
	if(do_mixing) cout << "Time for mixing" << endl;
	if(do_mixing) MixEvents(use_centrality, mincentormult, Nmixevents, centrality_vector, multiplicity_vector, vz_vector, minvz, track_4vector, track_charge_vector, track_weights_vector, hist_qinv_SS_MIX, hist_q3D_SS_MIX, hist_qinv_OS_MIX, hist_q3D_OS_MIX, dosplit, do_hbt3d, do_gamov, syst, NeventsAss);
	if(is_MC && do_mixing){MixEvents(use_centrality, mincentormult, Nmixevents, centrality_vector_gen, multiplicity_vector_gen, vz_vector_gen, minvz, track_4vector_gen, track_charge_vector_gen, track_weights_vector_gen, hist_qinv_SS_gen_MIX, hist_q3D_SS_gen_MIX, hist_qinv_OS_gen_MIX, hist_q3D_OS_gen_MIX, dosplit, do_hbt3d, do_gamov, syst, NeventsAssGEN);}

	
	// Output file name
	cout << endl;
	cout << "Writing histograms on ";
	cout << endl;

	// Make an output file
	string file_output = Form("%s_syst_%s_Nmix_%i_Mixint_%i_Vzint_%.f_on_%i", ouputfile.Data(), systematics.Data(), Nmixevents, mincentormult, minvz,date->GetDate()); // output file
	std::replace(file_output.begin(), file_output.end(), '.', 'p'); // replace . to p	

	// Open, write and close the output file
	TFile *MyFile = new TFile(Form("%s.root", file_output.c_str()), "RECREATE");
	if(MyFile->IsOpen()) cout << "output file: " << file_output.c_str() << ".root" << endl;
	MyFile->cd(); 

	// Write in different folders (see histogram_definition.h)
	// Control plots
	MyFile->mkdir("EventQA_histograms"); 
	MyFile->cd("EventQA_histograms"); 
	write_eventQA();

	MyFile->mkdir("TrackQA_histograms"); 
	MyFile->cd("TrackQA_histograms"); 
	write_trackQA(is_MC);

	MyFile->mkdir("HBT_1D"); 
	MyFile->cd("HBT_1D"); 
	write_HBT1D(is_MC);
	if(do_hbt3d){
		MyFile->mkdir("HBT_3D"); 
		MyFile->cd("HBT_3D"); 
		write_HBT3D(is_MC);
	}
	

	MyFile->Close();

	cout << endl;
	cout << "------------------------------------- DONE --------------------------------------" << endl;
	cout << endl;


	sec_end = clock(); // stop time counting
	cout << "========================================" << endl;
	cout << "Total running time: " << (double)(sec_end - sec_start) / CLOCKS_PER_SEC << " [s]" << endl;
	cout << "========================================" << endl;

}
