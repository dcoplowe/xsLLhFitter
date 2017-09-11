#include <DetectorSystematics.h>

#include <stdio.h>

// int main(int argc, char const *argv[])
int main()
{
	std::string in_file = "ReducedNTuple_TransverseAnalysis.root"; 
	std::string in_tree = "CCProtonPi0";
	std::string out_file = "DetSyst_Test.root";
	bool verbose = true;

	DetectorSystematics syst(10, verbose);

	syst.AddSample("signal", 29, -300., 300.);
	// syst.AddSample("Wgt1800", 29, 1800., 10000.);

	FileIO reader(in_file, in_tree);

	cout << "reader.GetEntries() = " << reader.GetEntries() << endl;

	// In order to produce a covariance matrix need to vary ALL systs in ALL samples
	// 1) we want to add a variation to all samples 



	// syst.MakeCovarianceMatrix();

	return 1;
}