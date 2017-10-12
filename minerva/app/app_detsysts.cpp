#include <DetectorSystematics.h>
#include <FileIO.h>

#include <stdio.h>
#include <iostream>

#include <TMatrixD.h>
#include <TFile.h>

using std::cout;
using std::endl;
using std::string;

// int main(int argc, char const *argv[])
int main()
{
	string in_file = "ReducedNTuple_TransverseAnalysis_noMggcut.root"; 
	string in_tree = "CCProtonPi0";
	string out_file = "DetSyst_Test.root";
	bool verbose = false;

	// Do we want something to overtake the nominal number of toys?
	// WTf do I mean here??? IDIOT
	// Ah the number of toys is the systname_sz

	DetectorSystematics * syst = new DetectorSystematics(100, verbose);

	// cout << "DetectorSystematics * syst = new DetectorSystematics(100, verbose);" << endl;

	// Ozgur's signal:
	// In ten's of MeV
	double lowMass = 60.;
	double higMass = 200.;
	double maxMass = 500.;

	double den = 15;
	int nlowMass_bins = (int)lowMass/den;
	int nsigMass_bins = (int)(higMass - lowMass)/den;
	int nhigMass_bins = (int)(maxMass - higMass)/den;

	syst->AddSample("pi0LowMass", nlowMass_bins, 0.,      lowMass);
	syst->AddSample("signal",  	 nsigMass_bins, lowMass, higMass);
	syst->AddSample("pi0HigMass", nhigMass_bins, higMass, maxMass);
	
	FileIO reader(in_file, in_tree);

	// cout << "FileIO reader(in_file, in_tree);" << endl;

	TFile * ofile = FileIO::MakeOutFile(out_file);

	// cout << "TFile * ofile = FileIO::MakeOutFile(out_file);" << endl;

	ofile->cd();
	reader.SetupLLNTuple();

	// cout << "reader.SetupLLNTuple();" << endl;
	syst->AddVertErrorBand("Flux_BeamFocus", reader.mc_wgt_Flux_BeamFocus_sz);
	// syst->AddVertErrorBand("ppfx1_Total", reader.mc_wgt_ppfx1_Total_sz);

	// cout << "reader.GetEntries() = " << reader.GetEntries() << endl;

	Int_t loop_size = reader.GetEntries();
	// reader.SetMaxEntries(loop_size);

	string var_name[3] = {"muon_P", "proton_P", "pi0_P"};
	double var_low[3] = { 1200., 450., 250. };
	double var_high[3] = { 7000., 1600., 4000. };

	for(int i = 0; i < 3; i++){
		double * bins = syst->GetOptBinning(reader.fChain, var_name[i], 3, var_low[i], var_high[i], "truth_isSignal == 1");
	}

	for(Int_t i = 0; i < loop_size; i++){
		reader.GetEntry(i);
		// cout << "reader.pi0_invMass = " << reader.pi0_invMass << endl;
		// Think of something a litte simpler that hold the var in fill sample and then fills the
		// var in fill Vert/Lat error. May be problematic?

		// Want to make sure only one sample is filled in each interation
		if(0. < reader.pi0_invMass && reader.pi0_invMass <= lowMass){
			syst->FillSample("pi0LowMass", reader.pi0_invMass, reader.wgt);
			syst->FillVertErrorBand("pi0LowMass", "Flux_BeamFocus", reader.pi0_invMass, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			// syst->FillVertErrorBand("pi0LowMass", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			reader.SetSample(0);
		}
		else if(lowMass < reader.pi0_invMass && reader.pi0_invMass < higMass){
			syst->FillSample("signal", reader.pi0_invMass, reader.wgt);
			syst->FillVertErrorBand("signal", "Flux_BeamFocus", reader.pi0_invMass, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			// syst->FillVertErrorBand("signal", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			reader.SetSample(1);
		}
		else if(higMass <= reader.pi0_invMass && reader.pi0_invMass < maxMass){
			syst->FillSample("pi0HigMass", reader.pi0_invMass, reader.wgt);
			syst->FillVertErrorBand("pi0HigMass", "Flux_BeamFocus", reader.pi0_invMass, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
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
	// // Need to find seg fault in this class...
	// // Seems to be some memory problem... :(
	// cov.Print();
	cov.Write("detsyst");

	ofile->Close();
	delete ofile;

	delete syst;

	return 1;
}
