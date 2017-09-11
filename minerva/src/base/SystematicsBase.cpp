#ifndef __SYSTEMATICSBASE_CPP__
#define __SYSTEMATICSBASE_CPP__

#include <SystematicsBase.h>
#include <Sample.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

SystematicsBase::SystematicsBase(int n_universes, bool verbose) : m_Nuniverses(n_universes), m_verbose(verbose)
{
	cout << "SystematicsBase::SystematicsBase(int n_universes, bool verbose)" << endl;
	m_samples.clear();
}

SystematicsBase::SystematicsBase(bool verbose) :  m_Nuniverses(1), m_verbose(verbose)
{
	cout << "SystematicsBase::SystematicsBase(bool verbose)" << endl;
	m_samples.clear();
}

void SystematicsBase::AddSample(std::string name, Int_t nbins, Double_t x_low, Double_t x_high)
{
 	m_samples.push_back( new Sample(name, nbins, x_low, x_high) );
}

void SystematicsBase::AddSample(std::string name, Int_t nbins, Double_t * x_bins)
{
 	m_samples.push_back( new Sample(name, nbins, x_bins) );
}

SystematicsBase::~SystematicsBase()
{
	m_samples.clear();
}

#endif
