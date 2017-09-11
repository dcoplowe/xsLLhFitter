#ifndef __DETECTORSYSTEMATICS_CPP__
#define __DETECTORSYSTEMATICS_CPP__

#include <DetectorSystematics.h>

#include <iostream>
#include <TH1D.h>

using std::cout;
using std::endl;

DetectorSystematics::DetectorSystematics(bool verbose) : SystematicsBase(verbose)
{ 
	cout << "DetectorSystematics::DetectorSystematics(bool verbose)" << endl;
}

DetectorSystematics::DetectorSystematics(int n_universes, bool verbose) : SystematicsBase(n_universes, verbose) 
{
	cout << "DetectorSystematics::DetectorSystematics(int n_universes, bool verbose)" << endl;
}

DetectorSystematics::~DetectorSystematics()
{
	m_samples.clear();
}

void DetectorSystematics::Run()
{
	cout << "DetectorSystematics::Run()" << endl;
}

// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
bool DetectorSystematics::AddLatErrorBand( const std::string& name)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddLatErrorBand(name, m_Nuniverses)) counter++;

	return (counter == m_samples.size())
}

bool DetectorSystematics::AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base )
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddLatErrorBand(name, base)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddLatErrorBandAndFillWithCV( const std::string& name)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddLatErrorBandAndFillWithCV(name, m_Nuniverses)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddVertErrorBand( const std::string& name)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddVertErrorBand(name, m_Nuniverses)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base )
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddVertErrorBand(name, base)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddVertErrorBandAndFillWithCV( const std::string& name)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddVertErrorBandAndFillWithCV(name, m_Nuniverses)) counter++;
	
	return (counter == m_samples.size())
}


bool DetectorSystematics::AddUncorrError( const std::string& name )
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddUncorrError(name)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddUncorrError(name, hist, errInContent)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddUncorrErrorAndFillWithCV( const std::string& name )
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddUncorrErrorAndFillWithCV(name)) counter++;
	
	return (counter == m_samples.size())
}


bool DetectorSystematics::AddMissingErrorBandsAndFillWithCV( const MnvH1D& ref )
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddMissingErrorBandsAndFillWithCV(ref)) counter++;
	
	return (counter == m_samples.size())
}

bool DetectorSystematics::AddMissingErrorBandsAndFillWithCV( const MnvH2D& ref )
{
	size_t counter = 0;
	for(size_t i = 0; i < m_samples.size(); i++) 
		if(m_samples[i]->AddMissingErrorBandsAndFillWithCV(ref)) counter++;
	
	return (counter == m_samples.size())
}
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
#endif