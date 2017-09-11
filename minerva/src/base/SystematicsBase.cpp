#ifndef __SYSTEMATICSBASE_CPP__
#define __SYSTEMATICSBASE_CPP__

#include <SystematicsBase.h>
#include <Sample.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

SystematicsBase::SystematicsBase(const int n_universes, const bool verbose) : m_Nuniverses(n_universes), m_verbose(verbose) // , m_counter(-1)
{
	cout << "SystematicsBase::SystematicsBase(int n_universes, bool verbose)" << endl;
	m_samples.clear();
	// m_sample_map.clear();
}

SystematicsBase::SystematicsBase(const bool verbose) :  m_Nuniverses(1), m_verbose(verbose)
{
	cout << "SystematicsBase::SystematicsBase(bool verbose)" << endl;
	m_samples.clear();
	// m_sample_map.clear();
}

void SystematicsBase::AddSample(const std::string &name, const int nbins, const double x_low, const double x_high)
{
	if(IsUniqueName(name)){
		// m_samples.push_back( new Sample(name, nbins, x_low, x_high) );
		m_samples [ name ] = new Sample(name, nbins, x_low, x_high);
		// m_samples_map[ name ] = m_counter++;
	}
}

void SystematicsBase::AddSample(const std::string &name, const int nbins, const double * x_bins)
{
	if(IsUniqueName(name)){
		// m_samples.push_back( new Sample(name, nbins, x_bins) );
		m_samples [ name ] = new Sample(name, nbins, x_bins);
		// m_samples_map[ name ] = m_counter++;
	}
}

SystematicsBase::~SystematicsBase()
{
	m_samples.clear();
}

bool SystematicsBase::IsUniqueName(const std::string &name)
{
    std::map<std::string, Sample*>::iterator it = m_samples.find( name );
    if(it != m_samples.end()){
    	cout << __FILE__ << ":" << __LINE__ << " : ERROR Name must be unique, multiple \"" << name << "\" found." << endl;
    	exit(0); 
    	return false;
    }
    else return true;    	
}

void SystematicsBase::FillSample(const std::string &name, const double val, const double weight)
{
	std::map<std::string, Sample*>::iterator it = m_samples.find( name );
    if(it != m_samples.end()) it->second->Fill(val, weight);
    else{
    	cout << __FILE__ << ":" << __LINE__ << " : ERROR Couldn't fill sample: \"" << name << ".\" Unable to find." << endl;
    	exit(0);
    }
}

#endif
