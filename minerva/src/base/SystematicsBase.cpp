#ifndef __SYSTEMATICSBASE_CPP__
#define __SYSTEMATICSBASE_CPP__

#include <SystematicsBase.h>

#include <Sample.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

SystematicsBase::SystematicsBase(const int n_universes, const bool verbose) : m_Nuniverses(n_universes), m_verbose(verbose),
	m_CurrentSample("") // , m_counter(-1)
{
	cout << "SystematicsBase::SystematicsBase(int n_universes, bool verbose)" << endl;
	m_samples.clear();
	// m_sample_map.clear();
}

SystematicsBase::SystematicsBase(const bool verbose) : m_Nuniverses(1), m_verbose(verbose), m_CurrentSample("")
{
	cout << "SystematicsBase::SystematicsBase(bool verbose)" << endl;
	m_samples.clear();
	// m_sample_map.clear();
}

void SystematicsBase::AddSample(const std::string &name, const int nbins, const double x_low, const double x_high)
{
	if(IsUniqueSample(name)){
		// m_samples.push_back( new Sample(name, nbins, x_low, x_high) );
		m_samples [ name ] = new Sample(name, nbins, x_low, x_high);
		// m_samples_map[ name ] = m_counter++;
	}
}

void SystematicsBase::AddSample(const std::string &name, const int nbins, const double * x_bins)
{
	if(IsUniqueSample(name)){
		// m_samples.push_back( new Sample(name, nbins, x_bins) );
		m_samples [ name ] = new Sample(name, nbins, x_bins);
		// m_samples_map[ name ] = m_counter++;
	}
}

SystematicsBase::~SystematicsBase()
{
	m_samples.clear();
}

bool SystematicsBase::IsUniqueSample(const std::string &name)
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
	m_value = -999.;
	m_wgt = -999.;
	std::map<std::string, Sample*>::iterator it = m_samples.find( name );
    if(it != m_samples.end()){
    	m_value = val;
    	m_wgt = weight;
    	it->second->Fill(m_value, m_wgt);
    	m_CurrentSample = it->first;
    	if(m_verbose) cout << "Filling Sample: " << it->first << " : " << val << endl;
    }
    else{
    	cout << __FILE__ << ":" << __LINE__ << " : ERROR Couldn't fill sample: \"" << name << ".\" Unable to find." << endl;
    	exit(0);
    }
}

// For simplicity fill samples with error bands:
bool SystematicsBase::FillLatErrorBand(const std::string& sam_name, const std::string& name, const double value, const std::vector<double>& shifts,
	const double cvweight, const bool fillcv, const double *weights)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillLatErrorBand(name, value, shifts, cvweight, fillcv, weights) ) pass = true;
	}
	return pass;
}

bool SystematicsBase::FillLatErrorBand(const std::string& sam_name, const std::string& name, const double value, const double * shifts, const double cvweight,
	const bool fillcv, const double *weights)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillLatErrorBand(name, value, shifts, cvweight, fillcv, weights) ) pass = true;
	}
	return pass;
}

bool SystematicsBase::FillLatErrorBand(const std::string& sam_name, const std::string& name, const double value, const double shiftDown, const double shiftUp,
	const double cvweight, const bool fillcv)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillLatErrorBand(name, value, shiftDown, shiftUp, cvweight, fillcv) ) pass = true;
	}
	return pass;
}

bool SystematicsBase::FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const std::vector<double>& weights,
	const double cvweight, double cvWeightFromMe)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillVertErrorBand(name, value, weights, cvweight, cvWeightFromMe) ) pass = true;
	}
	return pass;
}

bool SystematicsBase::FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const double * weights,
	const double cvweight, double cvWeightFromMe)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillVertErrorBand(name, value, weights, cvweight, cvWeightFromMe) ) pass = true;
	}
	return pass;
}

bool SystematicsBase::FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const double weightDown, const double weightUp,
	const double cvweight, double cvWeightFromMe)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillVertErrorBand(name, value, weightDown, weightUp, cvweight, cvWeightFromMe) ) pass = true;
	}
	return pass;
}

bool SystematicsBase::FillUncorrError(const std::string& sam_name, const std::string& name, const double value, const double err, const double cvweight)
{
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillUncorrError(name, value, err, cvweight) ) pass = true;
	}
	return pass;
}

#endif
