#include <ModelSystematics.h>
#include <FileIO.h>

#include <stdio.h>
#include <iostream>
#include <KinematicCalculator.h>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char const *argv[])
{
	std::string in_file = "ReducedNTuple_TransverseAnalysis_noMggcut.root"; 
	std::string in_tree = "CCProtonPi0";
	std::string out_file = "ModSyst_Test.root";

	FileIO reader(in_file, in_tree);

	ModelSystematics syst(7);
	Int_t nInts = 5;

	// Ozgur's signal:
	// In ten's of MeV
	double lowMass = 60.;
	double higMass = 200.;
	double maxMass = 500.;

	int nlowMass_bins = (int)lowMass/10.;
	int nsigMass_bins = (int)(higMass - lowMass)/10.;
	int nhigMass_bins = (int)(maxMass - higMass)/10.;

	cout << "Adding samples" << endl;
	for(int i = 0; i < nInts; i++){
		string append = Form("_I%.1d", i);
		syst.AddSample( ("pi0LowMass" + append), nlowMass_bins, 0.,      lowMass);
		syst.AddSample( ("signal" + append), 	nsigMass_bins, lowMass, higMass);
		syst.AddSample( ("pi0HigMass" + append), nhigMass_bins, higMass, maxMass);		
	}

	cout << "Adding errors" << endl;
	// Set up all error bands for all samples
	// syst.AddVertErrorBand("AGKYxF1pi","_I0");
	// syst.AddVertErrorBand("AhtBY", "_I0");

	syst.AddVertErrorBand("NormCCRES","_I1");
	syst.AddVertErrorBand("Rvn1pi", "_I1");

	Int_t entries = reader.GetEntries();
	Int_t loop_size = 10;
	reader.SetMaxEntries(loop_size);

	for(Int_t i = 0; i < loop_size; i++){
		// syst.GetReady();
		reader.GetEntry(i);

		cout << " truth_ReconstructEvent = " << reader.truth_ReconstructEvent << endl;
		cout << " truth_isSignal = " << reader.truth_isSignal << endl;
		// make opening angle:
		double true_op_angle = KinCalc::OpeningAngle(reader.truth_gamma1_4P, reader.truth_gamma2_4P);
		double true_pi0_invMass = KinCalc::GetInvMass(reader.truth_gamma1_4P[3], reader.truth_gamma2_4P[3], true_op_angle);

		cout << "true_op_angle = " << true_op_angle << " true_pi0_invMass = " <<  true_pi0_invMass << endl;

		if(0. < true_pi0_invMass && true_pi0_invMass <= lowMass){
			if(reader.mc_intType == 1){
				// syst.FillSample("pi0LowMass_I0", true_pi0_invMass, reader.wgt);
				// syst.FillVertErrorBand("pi0LowMass_I0", "AGKYxF1pi", true_pi0_invMass, reader.truth_genie_wgt_AGKYxF1pi);
				// syst.FillVertErrorBand("pi0LowMass_I0", "AhtBY", true_pi0_invMass, reader.truth_genie_wgt_AhtBY);
			}
			else if(reader.mc_intType == 2){
				syst.FillSample("pi0LowMass_I1", true_pi0_invMass, reader.wgt);
				syst.FillVertErrorBand("pi0LowMass_I1", "NormCCRES", true_pi0_invMass, reader.truth_genie_wgt_NormCCRES);
				syst.FillVertErrorBand("pi0LowMass_I1", "Rvn1pi", true_pi0_invMass, reader.truth_genie_wgt_Rvn1pi);					
			}
		}
		else if(lowMass < true_pi0_invMass && true_pi0_invMass < higMass){
			if(reader.mc_intType == 1){
				// syst.FillSample("signal_I0", true_pi0_invMass, reader.wgt);
				// syst.FillVertErrorBand("signal_I0", "AGKYxF1pi", true_pi0_invMass, reader.truth_genie_wgt_AGKYxF1pi);
				// syst.FillVertErrorBand("signal_I0", "AhtBY", true_pi0_invMass, reader.truth_genie_wgt_AhtBY);
			}
			else if(reader.mc_intType == 2){
				syst.FillSample("signal_I1", true_pi0_invMass, reader.wgt);
				syst.FillVertErrorBand("signal_I1", "NormCCRES", true_pi0_invMass, reader.truth_genie_wgt_NormCCRES);
				syst.FillVertErrorBand("signal_I1", "Rvn1pi", true_pi0_invMass, reader.truth_genie_wgt_Rvn1pi);					
			}
		}
		else if(higMass <= true_pi0_invMass && true_pi0_invMass < maxMass){
			if(reader.mc_intType == 1){
				// syst.FillSample("pi0HigMas_I0", true_pi0_invMass, reader.wgt);
				// syst.FillVertErrorBand("pi0HigMas_I0", "AGKYxF1pi", true_pi0_invMass, reader.truth_genie_wgt_AGKYxF1pi);
				// syst.FillVertErrorBand("pi0HigMas_I0", "AhtBY", true_pi0_invMass, reader.truth_genie_wgt_AhtBY);
			}
			else if(reader.mc_intType == 2){
				syst.FillSample("pi0HigMas_I1", true_pi0_invMass, reader.wgt);
				syst.FillVertErrorBand("pi0HigMas_I1", "NormCCRES", true_pi0_invMass, reader.truth_genie_wgt_NormCCRES);
				syst.FillVertErrorBand("pi0HigMas_I1", "Rvn1pi", true_pi0_invMass, reader.truth_genie_wgt_Rvn1pi);					
			}
		}
		else{
			cout << "Warning Bad Range: " << true_pi0_invMass << endl;
		}
	}
	cout << "Finished Loop" << endl;
	
	// Now we want to produce the response functions:
	syst.BuildResponses(out_file);




	return 1;
}


	// The dream:
	// syst.AddSample(Form("pi0LowMass, i), nlowMass_bins, 0.,      lowMass);
	// but make: syst.AddSample(Form("pi0LowMass_I%.1d, i), nlowMass_bins, 0.,      lowMass);
	// syst.AddVertErrorBand("AGKYxF1pi", NN);
	// Add errors only to sample_INN
