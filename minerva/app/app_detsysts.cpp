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

	syst.AddSample("signal",  29, 0., 20000.);
	syst.AddSample("sideband",  29, -300., 300.);

	FileIO reader(in_file, in_tree);

	syst.AddVertErrorBand("Flux_BeamFocus", reader.mc_wgt_Flux_BeamFocus_sz);
	syst.AddVertErrorBand("ppfx1_Total", reader.mc_wgt_ppfx1_Total_sz);
	// syst.AddLatErrorBand("MINOS Energy error");

	cout << "reader.GetEntries() = " << reader.GetEntries() << endl;

	// Want to fill the different samples given some 
	for(Int_t i = 0; i < 10; i++){
		reader.GetEntry(i);
		cout << "reader.muon_E = " << reader.muon_E << endl;

		syst.FillSample("signal", reader.muon_E, reader.wgt );

		syst.FillVertErrorBand("Flux_BeamFocus", reader.muon_E, reader.mc_wgt_Flux_BeamFocus, reader.wgt);
     	syst.FillVertErrorBand("ppfx1_Total", reader.muon_E, reader.mc_wgt_ppfx1_Total, reader.wgt);
     	// syst.FillLatErrorBand("MINOS Energy error", reader.muon_E, CCInclusiveReco_sys_muon_energy_shift[0], CCInclusiveReco_sys_muon_energy_shift[1] )
		// mc_wgt_Flux_BeamFocus

	}

	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 



	// syst.MakeCovarianceMatrix();

	return 1;
}