#ifndef __RUNDETSYST_CPP__
#define __RUNDETSYST_CPP__

#include <DetectorSystematics.h>
#include <FileIO.h>
#include <stdio.h>
#include <iostream>
#include <TMatrixD.h>
#include <TFile.h>
#include <DetError.h>
#include <TTree.h>
// #include <TVector3.h>
#include <Sample.h>
#include <ReadParam.h>

using std::cout;
using std::endl;
using std::string;

#ifndef kIniValue
#define kIniValue -999
#endif

RunDetSyst::MakeMggDist()
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

	return 1;



}

RunDetSyst::MakeFullMgg()
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

	// First make the samples:
	





	double den = 10;
	int nlowMass_bins = (int)lowMass/den;
	int nsigMass_bins = (int)(higMass - lowMass)/den;
	int nhigMass_bins = (int)(maxMass - higMass)/den;

	syst->AddSample("pi0LowMass", nlowMass_bins, 0.,      lowMass, Sample::kUnder);
	syst->AddSample("signal",  	 nsigMass_bins, lowMass, higMass);
	syst->AddSample("pi0HigMass", nhigMass_bins, higMass, maxMass, Sample::kOver);

	// Add the two sidebands used in ozgurs analysis:
	syst->AddSample("ProtonPID", (nlowMass_bins + nsigMass_bins + nhigMass_bins), 0., maxMass, Sample::kBoth);
	syst->AddSample("Michel",    (nmaxMass_bins + nsigMass_bins + nhigMass_bins), 0., maxMass, Sample::kBoth);	


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

	return 1;



}

#endif