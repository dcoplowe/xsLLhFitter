#ifndef __DETECTORSYSTEMATICS_CPP__
#define __DETECTORSYSTEMATICS_CPP__

#include <DetectorSystematics.h>
#include <Sample.h>
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
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
			if( it->AddLatErrorBand(0, name, m_Nuniverses) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddLatErrorBand(0, name, m_Nuniverses)) counter++;

	return (counter == m_samples.size());
}

bool DetectorSystematics::AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base )
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddLatErrorBand(0, name, base) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddLatErrorBand(0, name, base)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddLatErrorBandAndFillWithCV( const std::string& name)
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddLatErrorBandAndFillWithCV(0, name, m_Nuniverses) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddLatErrorBandAndFillWithCV(0, name, m_Nuniverses)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddVertErrorBand( const std::string& name)
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddVertErrorBand(0, name, m_Nuniverses) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddVertErrorBand(0, name, m_Nuniverses)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base )
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddVertErrorBand(0, name, base) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddVertErrorBand(0, name, base)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddVertErrorBandAndFillWithCV( const std::string& name)
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddVertErrorBandAndFillWithCV(0, name, m_Nuniverses) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddVertErrorBandAndFillWithCV(0, name, m_Nuniverses)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddUncorrError( const std::string& name )
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddUncorrError(0, name) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddUncorrError(0, name)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent)
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddUncorrError(0, name, hist, errInContent) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddUncorrError(0, name, hist, errInContent)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddUncorrErrorAndFillWithCV( const std::string& name )
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddUncorrErrorAndFillWithCV(0, name) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddUncorrErrorAndFillWithCV(0, name)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddMissingErrorBandsAndFillWithCV( const MnvH1D& ref )
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddMissingErrorBandsAndFillWithCV(ref) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddMissingErrorBandsAndFillWithCV(ref)) counter++;
	
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddMissingErrorBandsAndFillWithCV( const MnvH2D& ref )
{
	size_t counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		if( it->AddMissingErrorBandsAndFillWithCV(ref) ) counter++;
	// for(size_t i = 0; i < m_samples.size(); i++) 
	// 	if(m_samples[i]->AddMissingErrorBandsAndFillWithCV(ref)) counter++;
	
	return (counter == m_samples.size());
}
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
#endif