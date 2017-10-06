#include <ModelSystematics.h>
#include <FileIO.h>

#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
	std::string in_file = "ReducedNTuple_TransverseAnalysis.root"; 
	std::string in_tree = "CCProtonPi0";
	std::string out_file = "DetSyst_Test.root";

	FileIO reader(in_file, in_tree);

	ModelSystematics syst(reader.genie_wgt_n_shifts);
	Int_t nInts = 5;

	// Ozgur's signal:
	// In ten's of MeV
	double lowMass = 60.;
	double higMass = 200.;
	double maxMass = 500.;

	int nlowMass_bins = (int)lowMass/10.;
	int nsigMass_bins = (int)(higMass - lowMass)/10.;

	for(int i = 0; i < nInts; i++){
		syst.AddSample(Form("pi0LowMass_I%.1d", i), nlowMass_bins, 0.,      lowMass);
		syst.AddSample(Form("signal_I%.1d", i), nlowMass_bins, lowMass, higMass);
		syst.AddSample(Form("pi0HigMass_I%.1d", i), nhigMass_bins, higMass, maxMass);		
	}

	// Set up all error bands for all samples
	syst.AddVertErrorBand("AGKYxF1pi","_I0");
	syst.AddVertErrorBand("AhtBY", "_I0");

	syst.AddVertErrorBand("NormCCRES","_I1");
	syst.AddVertErrorBand("Rvn1pi", "_I1");

	Int_t entries = reader.GetEntries();
	Int_t per10 = (Int_t)entries/10;

	for(Int_t i = 0; i < 100; i++){
		// syst.GetReady();
		reader.GetEntry(i);

		if(0. < reader.truth_pi0_invMass && reader.truth_pi0_invMass < lowMass){
			if(reader.mc_IntType == 1){
				syst.FillSample("pi0LowMass_I0", reader.truth_pi0_invMass, reader.mc_wgt);
				syst.FillVertErrorBand("pi0LowMass_I0", "AGKYxF1pi", reader.truth_pi0_invMass, reader.truth_genie_wgt_AGKYxF1pi);
				syst.FillVertErrorBand("pi0LowMass_I0", "AhtBY", reader.truth_pi0_invMass, reader.truth_genie_wgt_AhtBY);
			}
			else if(reader.mc_IntType == 2){
				syst.FillSample("pi0LowMass_I1", reader.truth_pi0_invMass, reader.mc_wgt);
				syst.FillVertErrorBand("pi0LowMass_I1", "NormCCRES", reader.truth_pi0_invMass, reader.truth_genie_wgt_NormCCRES);
				syst.FillVertErrorBand("pi0LowMass_I1", "Rvn1pi", reader.truth_pi0_invMass, reader.truth_genie_wgt_Rvn1pi);					
			}
		}
		else if(lowMass < reader.truth_pi0_invMass && reader.truth_pi0_invMass < higMass){
			if(reader.mc_IntType == 1){
				syst.FillSample("signal_I0", reader.truth_pi0_invMass, reader.mc_wgt);
				syst.FillVertErrorBand("signal_I0", "AGKYxF1pi", reader.truth_pi0_invMass, reader.truth_genie_wgt_AGKYxF1pi);
				syst.FillVertErrorBand("signal_I0", "AhtBY", reader.truth_pi0_invMass, reader.truth_genie_wgt_AhtBY);
			}
			else if(reader.mc_IntType == 2){
				syst.FillSample("signal_I1", reader.truth_pi0_invMass, reader.mc_wgt);
				syst.FillVertErrorBand("signal_I1", "NormCCRES", reader.truth_pi0_invMass, reader.truth_genie_wgt_NormCCRES);
				syst.FillVertErrorBand("signal_I1", "Rvn1pi", reader.truth_pi0_invMass, reader.truth_genie_wgt_Rvn1pi);					
			}
		}
		else if(higMass < reader.truth_pi0_invMass && reader.truth_pi0_invMass < maxMass){
			if(reader.mc_IntType == 1){
				syst.FillSample("pi0HigMas_I0", reader.truth_pi0_invMass, reader.mc_wgt);
				syst.FillVertErrorBand("pi0HigMas_I0", "AGKYxF1pi", reader.truth_pi0_invMass, reader.truth_genie_wgt_AGKYxF1pi);
				syst.FillVertErrorBand("pi0HigMas_I0", "AhtBY", reader.truth_pi0_invMass, reader.truth_genie_wgt_AhtBY);
			}
			else if(reader.mc_IntType == 2){
				syst.FillSample("pi0HigMas_I1", reader.truth_pi0_invMass, reader.mc_wgt);
				syst.FillVertErrorBand("pi0HigMas_I1", "NormCCRES", reader.truth_pi0_invMass, reader.truth_genie_wgt_NormCCRES);
				syst.FillVertErrorBand("pi0HigMas_I1", "Rvn1pi", reader.truth_pi0_invMass, reader.truth_genie_wgt_Rvn1pi);					
			}
		}
		else{
			cout << "Warning Bad Range: " << reader.truth_pi0_invMass << endl;
		}
	}

	// Now we want to produce the response functions:


	return 1;
}

	// The dream:
	// syst.AddSample(Form("pi0LowMass, i), nlowMass_bins, 0.,      lowMass);
	// but make: syst.AddSample(Form("pi0LowMass_I%.1d, i), nlowMass_bins, 0.,      lowMass);
	// syst.AddVertErrorBand("AGKYxF1pi", NN);
	// Add errors only to sample_INN
