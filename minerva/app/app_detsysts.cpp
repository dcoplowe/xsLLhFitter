#include <DetectorSystematics.h>
#include <FileIO.h>

#include <stdio.h>
#include <iostream>

#include <TMatrixD.h>
#include <TFile.h>
#include <DetError.h>

#include <TransverseTools.h>
#include <TTree.h>
#include <TVector3.h>

#include <Sample.h>

using std::cout;
using std::endl;
using std::string;

#ifndef kIniValue
#define kIniValue -999
#endif

// int main(int argc, char const *argv[])
int main()
{
	// string in_file = "ReducedNTuple_TransverseAnalysis_noMggcut.root";
	// string in_file = "ReducedNTuple_TransverseAnalysis_newSig_noMggcut_171017.root";
	// New file using latest ntuple:
	string in_file = "/minerva/data/users/dcoplowe/Analyse_Alice/MC/Reduce_newSIG_pi0Mcutn_FDy_prPIDSBn_prScorey_meSBn_101817.root";
	string in_tree = "CCProtonPi0";
	string out_file = "DetSyst_Test.root";
	bool verbose = false;

	DetectorSystematics * syst = new DetectorSystematics(100, verbose);

	// Ozgur's signal:
	// In ten's of MeV
	double lowMass = 60.;
	double higMass = 200.;
	double maxMass = 500.;

	double den = 10;
	int nlowMass_bins = (int)lowMass/den;
	int nsigMass_bins = (int)(higMass - lowMass)/den;
	int nhigMass_bins = (int)(maxMass - higMass)/den;

	syst->AddSample("pi0LowMass", nlowMass_bins, 0.,      lowMass);
	syst->AddSample("signal",  	 nsigMass_bins, lowMass, higMass);
	syst->AddSample("pi0HigMass", nhigMass_bins, higMass, maxMass);
	
	FileIO reader(in_file, in_tree);

	TFile * ofile = FileIO::MakeOutFile(out_file);

	// Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);
	ofile->cd();
	reader.SetupLLNTuple();

	// Add default systematic errors to ALL samples:
	syst->AddDefaults();

	// Add the errors:
	// For W we have the following errors to consider (from Ozgur's analysis):

	// Generic Systematics:
	// 1) Michel tagging uncertainty
	// 2) Muon Tracking (MINOS Uncertainty)
	// 3) Neutron response
	// 4) Pion response
	// 5) Proton tracking


	// syst->AddLatErrorBand("EM_EnergyScale", 500);

	// syst->

	// syst->AddVertErrorBand("ppfx1_Total", reader.mc_wgt_ppfx1_Total_sz);

	Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);
	for(Int_t i = 0; i < loop_size; i++){
		reader.GetEntry(i);
		// Think of something a litte simpler that hold the var in fill sample and then fills the
		// var in fill Vert/Lat error. May be problematic?
		DetError error(reader);
		DetError::Default def_err = error.GetDefaults();
		// cout << "reader.pi0_invMass = " << reader.pi0_invMass << " : EM Scale = ";

		// Want to make sure only one sample is filled in each interation
		if(0. < reader.pi0_invMass && reader.pi0_invMass <= lowMass){
			syst->FillSample("pi0LowMass", reader.pi0_invMass, reader.wgt);
			syst->FillDefaults("pi0LowMass", reader.pi0_invMass, def_err);
			// syst->FillLatErrorBand("pi0LowMass", "EM_EnergyScale", reader.pi0_invMass, em_scale);
			// syst->FillVertErrorBand("pi0LowMass", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			reader.SetSample(0);
		}
		else if(lowMass < reader.pi0_invMass && reader.pi0_invMass < higMass){
			syst->FillSample("signal", reader.pi0_invMass, reader.wgt);
			syst->FillDefaults("signal", reader.pi0_invMass, def_err);
			// syst->FillLatErrorBand("signal", "EM_EnergyScale", reader.pi0_invMass, em_scale);
			// syst->FillVertErrorBand("signal", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			reader.SetSample(1);
		}
		else if(higMass <= reader.pi0_invMass){
		// else if(higMass <= reader.pi0_invMass && reader.pi0_invMass < maxMass){
			syst->FillSample("pi0HigMass", reader.pi0_invMass, reader.wgt);
			syst->FillDefaults("pi0HigMass", reader.pi0_invMass, def_err);
			// syst->FillLatErrorBand("pi0HigMass", "EM_EnergyScale", reader.pi0_invMass, em_scale);
			// syst->FillVertErrorBand("pi0HigMass", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			reader.SetSample(2);
		}
		else{
			// cout << "Warning Bad Range: " << reader.pi0_invMass << endl;
			reader.SetSample(3);
		}
		// cout << "Now filling tree." << endl;
		reader.Fill();
	}

	reader.Write();
	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 
	cout << "Make Covariance Matrix" << endl;	
	TMatrixD cov = syst->GetCovMatrix();
	TMatrixD cov_pi0LowMass1 = syst->GetSample("pi0LowMass")->GetTotalErrorMatrix(false, true, false);
	TMatrixD cov_signal1 = syst->GetSample("signal")->GetTotalErrorMatrix(false, true, false);
	TMatrixD cov_pi0HigMass1 = syst->GetSample("pi0HigMass")->GetTotalErrorMatrix(false, true, false);

	// Get the individual sample covariane matrices: (true, false, false) <- Defualt for MnvH1D
	TMatrixD cov2 = syst->GetCovMatrix(true, false, false);
	TMatrixD cov_pi0LowMass2 = syst->GetSample("pi0LowMass")->GetTotalErrorMatrix(true, false, false);
	TMatrixD cov_signal2 = syst->GetSample("signal")->GetTotalErrorMatrix(true, false, false);
	TMatrixD cov_pi0HigMass2 = syst->GetSample("pi0HigMass")->GetTotalErrorMatrix(true, false, false);

	// // Need to find seg fault in this class...
	// // Seems to be some memory problem... :(
	// cov.Print();
	cov.Write("detsyst1");
	cov2.Write("detsyst2");
	cov_pi0LowMass1.Write("pi0LowMass1");
	cov_pi0LowMass2.Write("pi0LowMass2");
	cov_signal1.Write("signal1");
	cov_signal2.Write("signal2");
	cov_pi0HigMass1.Write("pi0HigMass1");
	cov_pi0HigMass2.Write("pi0HigMass2");

	// ofile->Close();
	// delete ofile;

	// delete syst;

	return 1;
}

// Old shit:
	// DetectorSystematics * syst = new DetectorSystematics(100, verbose);

	// FileIO reader(in_file, in_tree);
	// // cout << "FileIO reader(in_file, in_tree);" << endl;

	// TFile * ofile = FileIO::MakeOutFile(out_file);
	// ofile->cd();
	// TTree * tree = new TTree("temp","");

	// Double_t muon_P = kIniValue;
	// Double_t proton_P = kIniValue;
	// Double_t pion_P = kIniValue;

	// Double_t muon_pT = kIniValue;
	// Double_t proton_pT = kIniValue;
	// Double_t pion_pT = kIniValue;

	// tree->Branch("muon_P", &muon_P, "muon_P/D");
	// tree->Branch("proton_P", &proton_P, "proton_P/D");
	// tree->Branch("pion_P", &pion_P, "pion_P/D");

	// tree->Branch("muon_pT", &muon_pT, "muon_pT/D");
	// tree->Branch("proton_pT", &proton_pT, "proton_pT/D");
	// tree->Branch("pion_pT", &pion_pT, "pion_pT/D");

	// TransverseTools trans_tools;

	// Int_t loop_size = reader.GetEntries();
	// // reader.SetMaxEntries(loop_size);
	// for(Int_t i = 0; i < loop_size; i++){
	// 	reader.GetEntry(i);
 // 		if(reader.truth_isSignal != 1) continue;

 // 		if(reader.pi0_invMass < 60. && reader.pi0_invMass > 200.) continue;

	// 	const TVector3 * muon = new TVector3(reader.muon_px, reader.muon_py, reader.muon_pz);
	// 	muon_pT = trans_tools.GetPTRec(reader.CCProtonPi0_vtx, muon)->Mag();
	// 	delete muon;

	// 	const TVector3 * proton = new TVector3(reader.proton_px, reader.proton_py, reader.proton_pz);
	// 	proton_pT = trans_tools.GetPTRec(reader.CCProtonPi0_vtx, proton)->Mag();
	// 	delete proton;

	// 	const TVector3 * pion = new TVector3(reader.pi0_px, reader.pi0_py, reader.pi0_pz);
	// 	pion_pT = trans_tools.GetPTRec(reader.CCProtonPi0_vtx, pion)->Mag();
	// 	delete pion;

	// 	muon_P = reader.muon_P;
	// 	proton_P = reader.proton_P;
	// 	pion_P = reader.pi0_P;	
	// 	tree->Fill();	
	// }
	
	// ofile->cd();
	// tree->Write();
	
	// string var_name[6] = {"muon_P", "proton_P", "pion_P", "muon_pT", "proton_pT", "pion_pT"};
	// double var_low[6] = { 1200.,   450.,  250.,    0.,    0.,    0.};
	// double var_high[6] = { 7000., 1600., 4000., 1000., 1000., 1000.};
	// for(int i = 0; i < 6; i++){
	// 	double * bins = syst->GetOptBinning(tree, var_name[i], 3, var_low[i], var_high[i]);
	// }

	// ofile->Close();
	// delete ofile;
	// // delete syst;
	// return 1;