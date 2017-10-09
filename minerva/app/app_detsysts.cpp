#include <DetectorSystematics.h>
#include <FileIO.h>

#include <stdio.h>
#include <iostream>

#include <TMatrixD.h>
#include <TFile.h>

using std::cout;
using std::endl;

// int main(int argc, char const *argv[])
int main()
{
	std::string in_file = "ReducedNTuple_TransverseAnalysis_noMggcut.root"; 
	std::string in_tree = "CCProtonPi0";
	std::string out_file = "DetSyst_Test.root";
	bool verbose = true;

	// Do we want something to overtake the nominal number of toys?
	// WTf do I mean here??? IDIOT
	// Ah the number of toys is the systname_sz

	DetectorSystematics * syst = new DetectorSystematics(100, verbose);

	// Ozgur's signal:
	// In ten's of MeV
	double lowMass = 60.;
	double higMass = 200.;
	double maxMass = 500.;

	int nlowMass_bins = (int)lowMass/10.;
	int nsigMass_bins = (int)(higMass - lowMass)/10.;
	int nhigMass_bins = (int)(maxMass - higMass)/10.;

	syst->AddSample("pi0LowMass", nlowMass_bins, 0.,      lowMass);
	syst->AddSample("signal",  	 nsigMass_bins, lowMass, higMass);
	syst->AddSample("pi0HigMass", nhigMass_bins, higMass, maxMass);
	
	// Both his sidebands:
	// syst->AddSample("sideband",  100, 10000., 20000.);

	FileIO reader(in_file, in_tree);

	// TFile * ofile = FileIO::MakeOutFile(out_file);
	// ofile->cd();
	// // reader.SetupLLNTuple();

	syst->AddVertErrorBand("Flux_BeamFocus", reader.mc_wgt_Flux_BeamFocus_sz);
	syst->AddVertErrorBand("ppfx1_Total", reader.mc_wgt_ppfx1_Total_sz);

	cout << "reader.GetEntries() = " << reader.GetEntries() << endl;

	// Int_t entries = reader.GetEntries();
	// Int_t entries = reader.GetEntries();
	Int_t loop_size = 10;
	// reader.SetMaxEntries(loop_size);

	for(Int_t i = 0; i < loop_size; i++){
		// syst->GetReady();
		reader.GetEntry(i);
		cout << "reader.pi0_invMass = " << reader.pi0_invMass << endl;
		// Think of something a litte simpler that hold the var in fill sample and then fills the
		// var in fill Vert/Lat error. May be problematic?

		// Want to make sure only one sample is filled in each interation
		if(0. < reader.pi0_invMass && reader.pi0_invMass <= lowMass){
			syst->FillSample("pi0LowMass", reader.pi0_invMass, reader.wgt);
			// syst->FillVertErrorBand("pi0LowMass", "Flux_BeamFocus", reader.pi0_invMass, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			// syst->FillVertErrorBand("pi0LowMass", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			// reader.SetSample(0);
		}
		else if(lowMass < reader.pi0_invMass && reader.pi0_invMass < higMass){
			syst->FillSample("signal", reader.pi0_invMass, reader.wgt);
			// syst->FillVertErrorBand("signal", "Flux_BeamFocus", reader.pi0_invMass, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			// syst->FillVertErrorBand("signal", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			// reader.SetSample(1);
		}
		else if(higMass <= reader.pi0_invMass && reader.pi0_invMass < maxMass){
			syst->FillSample("pi0HigMass", reader.pi0_invMass, reader.wgt);
			// syst->FillVertErrorBand("pi0HigMass", "Flux_BeamFocus", reader.pi0_invMass, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			// syst->FillVertErrorBand("pi0HigMass", "ppfx1_Total", reader.pi0_invMass, reader.mc_wgt_ppfx1_Total, reader.wgt);
			// reader.SetSample(2);
		}
		else{
			cout << "Warning Bad Range: " << reader.pi0_invMass << endl;
			// reader.SetSample(3);
		}
		// cout << "Now filling tree." << endl;
		// reader.Fill();
	}

	// reader.Write();

	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 
	cout << "Make Covariance Matrix" << endl;	
	TMatrixD cov = syst->GetCovMatrix();
	delete syst;
	// cov.Print();
	// cov.Write("detsyst");

	// ofile->Close();
	// delete ofile;

	return 1;
}


// #9  0x000000000042b86d in FileIOBase::~FileIOBase (this=0x7fffa8f76e60, __in_chrg=<value optimized out>) at src/fileio/FileIOBase.cpp:66
// #10 0x000000000041729e in FileIO::~FileIO (this=0x7fffa8f76e60, __in_chrg=<value optimized out>) at src/fileio/FileIO.h:12
