#ifndef __RUNDETSYST_CPP__
#define __RUNDETSYST_CPP__

#include <RunDetSyst.h>

#include <DetectorSystematics.h>
#include <FileIO.h>
#include <stdio.h>
#include <iostream>
#include <TMatrixD.h>
#include <TFile.h>
#include <DetError.h>
#include <TTree.h>
#include <TChain.h>
// #include <TVector3.h>
#include <Sample.h>
#include <ReadParam.h>
#include <DataInfo.h>

using std::cout;
using std::endl;
using std::string;

#ifndef kIniValue
#define kIniValue -999
#endif

void RunDetSyst::MakeMggDist()
{
	string in_file = ReadParam::GetParameterS("in_file", DataInfo::Opts::Run);
	string in_tree = ReadParam::GetParameterS("tree_name", DataInfo::Opts::Run);
	string out_file = ReadParam::GetParameterS("out_file", DataInfo::Opts::Run);
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

	syst->AddSample("pi0LowMass", nlowMass_bins, 0.,      lowMass, Sample::kUnder);
	syst->AddSample("signal",  	 nsigMass_bins, lowMass, higMass);
	syst->AddSample("pi0HigMass", nhigMass_bins, higMass, maxMass, Sample::kOver);
	
	FileIO reader(in_file, in_tree);

	TFile * ofile = FileIO::MakeOutFile(out_file);

	// Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);
	ofile->cd();
	reader.SetupLLNTuple();

	// Add default systematic errors to ALL samples:
	syst->AddDefaults();

	// Add specific variable shifts:
	syst->AddLatErrorBand("EMScale", 500);

	// Add the errors:
	// For W we have the following errors to consider (from Ozgur's analysis):

	// Generic Systematics:
	// 1) Michel tagging uncertainty
	// 2) Muon Tracking (MINOS Uncertainty)
	// 3) Neutron response
	// 4) Pion response
	// 5) Proton tracking

	Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);
	for(Int_t i = 0; i < loop_size; i++){
		reader.GetEntry(i);
		// Think of something a litte simpler that hold the var in fill sample and then fills the
		// var in fill Vert/Lat error. May be problematic?
		
		// cout << "reader.pi0_invMass = " << reader.pi0_invMass << " : EM Scale = ";

		// Need to check the effect of the lateral shifts for the cuts:
		// For each universe check if that that cuts were passed (if not passed 
		// we don't apply that variation to that universe (set it to zero)):
		// Deal with this via weights --> If the universe fails to pass cuts
		// set the weights to zero.

		// kEMScale = 0, kMuMom, kMuTheta, kPrBirks,
		// kPrMass, kPrBethBloch, kPrMEU

		string sam_name = "";
		// Want to make sure only one sample is filled in each interation
		if(reader.pi0_invMass < lowMass){
			sam_name = "pi0LowMass";
			reader.SetSample(0);
		}
		else if(lowMass <= reader.pi0_invMass && reader.pi0_invMass <= higMass){
			sam_name = "signal";
			reader.SetSample(1);
		}
		else if(higMass < reader.pi0_invMass){
		// else if(higMass <= reader.pi0_invMass && reader.pi0_invMass < maxMass){
			sam_name = "pi0HigMass";
			reader.SetSample(2);
		}
		else{
			cout << "Warning Bad Range: " << reader.pi0_invMass << endl;
			reader.SetSample(3);
		}

		if(!sam_name.empty()){
			syst->FillSample(sam_name, reader.pi0_invMass, reader.wgt);
			DetError error(reader);
			DetError::Default default_errors = error.GetDefaults();
			syst->FillDefaults(sam_name, reader.pi0_invMass, default_errors);

			// Now will with wgts and shifts:
			std::vector<double> EM_shifts = error.GetLinearEnergyShifts(reader.pi0_invMass);
			double * wgts = error.GetWgts(DetError::kEMScale);
			syst->FillLatErrorBand(sam_name, "EMScale", reader.pi0_invMass, EM_shifts, 1.0, true, wgts);
			delete [] wgts;
		}
		reader.Fill();
	}

	reader.Write();

	syst->MakeBinning();
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
	// return;
}

void RunDetSyst::MakeFullMgg()
{
	// string in_tree = ReadParam::GetParameterS("tree_name", DataInfo::Opts::Run);
	// // First make the samples:
	// string sig_file = ReadParam::GetParameterS("sig_file", DataInfo::Opts::Run);
	// string pid_file = ReadParam::GetParameterS("pid_file", DataInfo::Opts::Run);
	// string mic_file = ReadParam::GetParameterS("mic_file", DataInfo::Opts::Run);
	// string out_file = ReadParam::GetParameterS("full_file", DataInfo::Opts::Run);

	// // Ozgur's signal:
	// // In ten's of MeV
	// double lowMass = 60.;
	// double higMass = 200.;
	// double maxMass = 500.;

	// FileIO sig_reader(sig_file, in_tree);
	// TFile * sig_ofile = FileIO::MakeOutFile("sig_LLFormat.root");
	// sig_ofile->cd();
	// sig_reader.SetupLLNTuple();

	// for(Int_t i = 0; i < sig_reader.GetEntries(); i++){
	// 	sig_reader.GetEntry(i);
	// 	// Want to make sure only one sample is filled in each interation
	// 	if(sig_reader.pi0_invMass < lowMass) sig_reader.SetSample(0);
	// 	else if(lowMass <= sig_reader.pi0_invMass && sig_reader.pi0_invMass <= higMass){
	// 		sig_reader.SetSample(0);
	// 	}
	// 	else if(higMass < sig_reader.pi0_invMass){
	// 	// else if(higMass <= sig_reader.pi0_invMass && sig_reader.pi0_invMass < maxMass){
	// 		sig_reader.SetSample(1);
	// 	}
	// 	else{
	// 		sig_reader.SetSample(2);
	// 	}
	// 	sig_reader.Fill();
	// }

	// sig_ofile->Close();
	// delete sig_ofile;

	// FileIO pid_reader(pid_file, in_tree);
	// TFile * pid_ofile = FileIO::MakeOutFile("pid_LLFormat.root");
	// pid_ofile->cd();
	// pid_reader.SetupLLNTuple();

	// for(Int_t i = 0; i < pid_reader.GetEntries(); i++){
	// 	pid_reader.GetEntry(i);
	// 	pid_reader.SetSample(3);
	// 	pid_reader.Fill();
	// }

	// pid_ofile->Close();
	// delete pid_ofile;

	// FileIO mic_reader(mic_file, in_tree);
	// TFile * mic_ofile = FileIO::MakeOutFile("mic_LLFormat.root");
	// mic_ofile->cd();
	// mic_reader.SetupLLNTuple();

	// for(Int_t i = 0; i < mic_reader.GetEntries(); i++){
	// 	mic_reader.GetEntry(i);
	// 	mic_reader.SetSample(4);
	// 	mic_reader.Fill();
	// }

	// mic_ofile->Close();
	// delete mic_ofile;
	
	// TChain * merger = new TChain(in_tree.c_str());
	// merger->Add(sig_file.c_str());
	// merger->Add(pid_file.c_str());
	// merger->Add(mic_file.c_str());

	// TFile * merge_ofile = FileIO::MakeOutFile(out_file);
	// merge_ofile->cd();
	// TTree * tmerged = merger->CopyTree("");
	// tmerged->Write();
	// merge_ofile->Close();
	// delete merge_ofile;
	// delete merger;


	// // Now make output for covariance matrix:

	// double den = 10.;
	// int nlowMass_bins = (int)lowMass/den;
	// int nsigMass_bins = (int)(higMass - lowMass)/den;
	// int nhigMass_bins = (int)(maxMass - higMass)/den;

	// DetectorSystematics * syst = new DetectorSystematics(100, verbose);

	// syst->AddSample("pi0LowMass", nlowMass_bins, 0.,      lowMass, Sample::kUnder);
	// syst->AddSample("signal",  	 nsigMass_bins, lowMass, higMass);
	// syst->AddSample("pi0HigMass", nhigMass_bins, higMass, maxMass, Sample::kOver);

	// // Add the two sidebands used in ozgurs analysis:
	// syst->AddSample("ProtonPID", (nlowMass_bins + nsigMass_bins + nhigMass_bins), 0., maxMass, Sample::kBoth);
	// syst->AddSample("Michel",    (nmaxMass_bins + nsigMass_bins + nhigMass_bins), 0., maxMass, Sample::kBoth);	

	// FileIO reader(in_file, in_tree);

	// TFile * ofile = FileIO::MakeOutFile(out_file);

	// // Int_t loop_size = reader.GetEntries();
	// // reader.SetMaxEntries(loop_size);
	// ofile->cd();
	// reader.SetupLLNTuple();

	// // Add default systematic errors to ALL samples:
	// syst->AddDefaults();

	// // Add specific variable shifts:
	// syst->AddLatErrorBand("EMScale", 500);

	// // Add the errors:
	// // For W we have the following errors to consider (from Ozgur's analysis):

	// // Generic Systematics:
	// // 1) Michel tagging uncertainty
	// // 2) Muon Tracking (MINOS Uncertainty)
	// // 3) Neutron response
	// // 4) Pion response
	// // 5) Proton tracking

	// Int_t loop_size = reader.GetEntries();
	// // reader.SetMaxEntries(loop_size);
	// for(Int_t i = 0; i < loop_size; i++){
	// 	reader.GetEntry(i);
	// 	string sam_name = "";
	// 	// Want to make sure only one sample is filled in each interation
	// 	if(reader.pi0_invMass < lowMass){
	// 		sam_name = "pi0LowMass";
	// 		reader.SetSample(0);
	// 	}
	// 	else if(lowMass <= reader.pi0_invMass && reader.pi0_invMass <= higMass){
	// 		sam_name = "signal";
	// 		reader.SetSample(1);
	// 	}
	// 	else if(higMass < reader.pi0_invMass){
	// 	// else if(higMass <= reader.pi0_invMass && reader.pi0_invMass < maxMass){
	// 		sam_name = "pi0HigMass";
	// 		reader.SetSample(2);
	// 	}
	// 	else{
	// 		cout << "Warning Bad Range: " << reader.pi0_invMass << endl;
	// 		// reader.SetSample(3);
	// 	}

	// 	if(!sam_name.empty()){
	// 		syst->FillSample(sam_name, reader.pi0_invMass, reader.wgt);
	// 		DetError error(reader);
	// 		DetError::Default default_errors = error.GetDefaults();
	// 		syst->FillDefaults(sam_name, reader.pi0_invMass, default_errors);

	// 		// Now will with wgts and shifts:
	// 		std::vector<double> EM_shifts = error.GetLinearEnergyShifts(reader.pi0_invMass);
	// 		double * wgts = error.GetWgts(DetError::kEMScale);
	// 		syst->FillLatErrorBand(sam_name, "EMScale", reader.pi0_invMass, EM_shifts, 1.0, true, wgts);
	// 		delete [] wgts;
	// 	}
	// 	reader.Fill();
	// }

	// reader.Write();

	// syst->MakeBinning();
	// // In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// // 1) we want to add a variation to all samples 
	// cout << "Make Covariance Matrix" << endl;	
	// TMatrixD cov = syst->GetCovMatrix();
	// TMatrixD cov_pi0LowMass1 = syst->GetSample("pi0LowMass")->GetTotalErrorMatrix(false, true, false);
	// TMatrixD cov_signal1 = syst->GetSample("signal")->GetTotalErrorMatrix(false, true, false);
	// TMatrixD cov_pi0HigMass1 = syst->GetSample("pi0HigMass")->GetTotalErrorMatrix(false, true, false);
	// TMatrixD cov_ProtonPID1 = syst->GetSample("ProtonPID")->GetTotalErrorMatrix(false, true, false);
	// TMatrixD cov_Michel1 = syst->GetSample("Michel")->GetTotalErrorMatrix(false, true, false);

	// // Get the individual sample covariane matrices: (true, false, false) <- Defualt for MnvH1D
	// TMatrixD cov2 = syst->GetCovMatrix(true, false, false);
	// TMatrixD cov_pi0LowMass2 = syst->GetSample("pi0LowMass")->GetTotalErrorMatrix(true, false, false);
	// TMatrixD cov_signal2 = syst->GetSample("signal")->GetTotalErrorMatrix(true, false, false);
	// TMatrixD cov_pi0HigMass2 = syst->GetSample("pi0HigMass")->GetTotalErrorMatrix(true, false, false);
	// TMatrixD cov_ProtonPID2 = syst->GetSample("ProtonPID")->GetTotalErrorMatrix(true, true, false);
	// TMatrixD cov_Michel2 = syst->GetSample("Michel")->GetTotalErrorMatrix(true, true, false);

	// // // Need to find seg fault in this class...
	// // // Seems to be some memory problem... :(
	// // cov.Print();
	// cov.Write("detsyst1");
	// cov2.Write("detsyst2");
	// cov_pi0LowMass1.Write("pi0LowMass1");
	// cov_pi0LowMass2.Write("pi0LowMass2");
	// cov_signal1.Write("signal1");
	// cov_signal2.Write("signal2");
	// cov_pi0HigMass1.Write("pi0HigMass1");
	// cov_pi0HigMass2.Write("pi0HigMass2");

	// // ofile->Close();
	// // delete ofile;

	// // delete syst;

	// return 1;
}

void RunDetSyst::MakeFulldpTT()
{
	string in_file = ReadParam::GetParameterS("in_file", DataInfo::Opts::Run);
	string in_tree = ReadParam::GetParameterS("tree_name", DataInfo::Opts::Run);
	string out_file = ReadParam::GetParameterS("out_file", DataInfo::Opts::Run);
	bool verbose = false;

	DetectorSystematics * syst = new DetectorSystematics(100, verbose);

	syst->AddSample("dpTT", 19, -300., 300., Sample::kBoth);
	
	FileIO reader(in_file, in_tree);

	TFile * ofile = FileIO::MakeOutFile(out_file);
	ofile->cd();
	// Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);

	// Add default systematic errors to ALL samples:
	syst->AddDefaults();

	// Add specific variable shifts:
	syst->AddLatErrorBand("EMScale", 500);

	// Add the errors:
	// For W we have the following errors to consider (from Ozgur's analysis):

	// Generic Systematics:
	// 1) Michel tagging uncertainty
	// 2) Muon Tracking (MINOS Uncertainty)
	// 3) Neutron response
	// 4) Pion response
	// 5) Proton tracking

	TransverseTools trans;
	const TVector3 * prmom = new TVector3(0.,0.,0.);

	Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);
	for(Int_t i = 0; i < loop_size; i++){
		reader.GetEntry(i);

		if(reader.nProtonCandidates > 0){
			syst->FillSample("dpTT", reader.dpTT, reader.wgt);
			DetError error(reader);
			DetError::Default default_errors = error.GetDefaults();
			syst->FillDefaults("dpTT", reader.dpTT, default_errors);

			const TVector3 * mumom = TVector3(reader.muon_px, reader.muon_py, reader.muon_pz);
			const TVector3 * pimom = TVector3(reader.pi0_px, reader.pi0_py, reader.pi0_pz);

		    double pi_TT = trans.GetDPTTRec(reader.CCProtonPi0_vtx, mumom, prmom, pimom);// const;

		    std::vector<double> EM_shifts = error.GetLinearEnergyShifts(pi_TT);
		    double * wgts = error.GetWgts(DetError::kEMScale);

		    syst->FillLatErrorBand("dpTT", "EMScale", reader.dpTT, EM_shifts, 1.0, true, wgts);
		    delete [] wgts;


		    delete mumom;
		    delete pimom;
		}

	}

	syst->MakeBinning();
	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 
	cout << "Make Covariance Matrix" << endl;	
	TMatrixD cov = syst->GetCovMatrix();
	TMatrixD cov_pi0LowMass1 = syst->GetSample("dpTT")->GetTotalErrorMatrix(false, true, false);

	// Get the individual sample covariane matrices: (true, false, false) <- Defualt for MnvH1D
	TMatrixD cov2 = syst->GetCovMatrix(true, false, false);
	TMatrixD cov_pi0LowMass2 = syst->GetSample("dpTT")->GetTotalErrorMatrix(true, false, false);

	// // Need to find seg fault in this class...
	// // Seems to be some memory problem... :(
	// cov.Print();
	cov.Write("detsyst1");
	cov2.Write("detsyst2");
	cov_pi0LowMass1.Write("pi0LowMass1");
	cov_pi0LowMass2.Write("pi0LowMass2");

}

#endif