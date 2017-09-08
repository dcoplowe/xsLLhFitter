#ifndef __DETECTORSYSTEMATICS_CPP__
#define __DETECTORSYSTEMATICS_CPP__

#include <DetectorSystematics.h>

#include <iostream>

using std::cout;
using std::endl;

DetectorSystematics::DetectorSystematics(std::string in_filename, std::string in_treename, std::string out_filename, bool verbose) : 
	SystematicsBase(in_filename, in_treename, verbose)
{
	cout << "DetectorSystematics::DetectorSystematics()" << endl;
}

DetectorSystematics::~DetectorSystematics()
{

}

void DetectorSystematics::Run()
{
	cout << "DetectorSystematics::Run()" << endl;
}

#endif