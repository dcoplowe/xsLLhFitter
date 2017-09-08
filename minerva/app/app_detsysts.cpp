#include <DetectorSystematics.h>

#include <stdio.h>

int main(int argc, char const *argv[])
{
	std::string in_file = "ReducedNTuple_TransverseAnalysis.root"; 
	std::string in_tree = "CCProtonPi0";
	std::string out_file = "DetSyst_Test.root";
	bool verbose = true;

	DetectorSystematics syst(in_file, in_tree, out_file verbose);
	syst.Run();

	return 1;
}