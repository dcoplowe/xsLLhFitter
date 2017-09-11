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

	DetectorSystematics syst(10, verbose);

	syst.AddSample("signal",  29, 0., 20000.);
	syst.AddSample("signal",  29, -300., 300.);
	double dpv = new [ 6 ];
	dpv[0] = 1800.;
	dpv[1] = 2200.;
	dpv[2] = 3000.;
	dpv[3] = 5000.;
	dpv[4] = 6000.;
	dpv[5] = 10000.;

	syst.AddSample("Wgt1800", 5, dpv);

	double ffffd[ 6 ] = { 1800., 2200., 3000., 5000., 6000., 10000. };
	syst.AddSample("Wgt1800", ffffd);


	FileIO reader(in_file, in_tree);

	cout << "reader.GetEntries() = " << reader.GetEntries() << endl;

	// Want to fill the different samples given some 
	for(Int_t i = 0; i < 10; i++){
		reader.GetEntry(i);
		cout << "reader.muon_E = " << reader.muon_E << endl;

		syst.FillSample("signal", reader.muon_E, reader.mc_cvweight_total );
	}

	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 



	// syst.MakeCovarianceMatrix();

	return 1;
}