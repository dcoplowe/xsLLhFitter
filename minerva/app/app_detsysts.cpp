#include <DetectorSystematics.h>
#include <FileIO.h>

#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

// int main(int argc, char const *argv[])
int main()
{
	std::string in_file = "ReducedNTuple_TransverseAnalysis.root"; 
	std::string in_tree = "CCProtonPi0";
	std::string out_file = "DetSyst_Test.root";
	bool verbose = true;

	// Do we want something to overtake the nominal number of toys?

	DetectorSystematics syst(100, verbose);

	// Ozgur's signal:
	syst.AddSample("signal",  100, 0., 10000.);
	// Both his sidebands:
	syst.AddSample("sideband",  100, 10000., 20000.);

	FileIO reader(in_file, in_tree);

	syst.AddVertErrorBand("Flux_BeamFocus", reader.mc_wgt_Flux_BeamFocus_sz);
	syst.AddVertErrorBand("ppfx1_Total", reader.mc_wgt_ppfx1_Total_sz);
	// syst.AddLatErrorBand("MINOS Energy error");

	cout << "reader.GetEntries() = " << reader.GetEntries() << endl;

	// Want to fill the different samples given some 
	for(Int_t i = 0; i < 10; i++){
		syst.GetReady();

		reader.GetEntry(i);
		cout << "reader.muon_E = " << reader.muon_E << endl;

		// Think of something a litte simpler that hold the var in fill sample and then fills the
		// var in fill Vert/Lat error. May be problematic?

		// Want to make sure only one sample is filled in each interation
		if(reader.muon_E < 10000.){
			syst.FillSample("signal", reader.muon_E, reader.wgt);
			syst.FillVertErrorBand("signal", "Flux_BeamFocus", reader.muon_E, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			syst.FillVertErrorBand("signal", "ppfx1_Total", reader.muon_E, reader.mc_wgt_ppfx1_Total, reader.wgt);
		}
		else{
			syst.FillSample("sideband", reader.muon_E, reader.wgt);
			syst.FillVertErrorBand("sideband", "Flux_BeamFocus", reader.muon_E, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
			syst.FillVertErrorBand("sideband", "ppfx1_Total", reader.muon_E, reader.mc_wgt_ppfx1_Total, reader.wgt);
		} 
	}

	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 



	// syst.MakeCovarianceMatrix();

	return 1;
}