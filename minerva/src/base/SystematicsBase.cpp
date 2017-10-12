#ifndef __SYSTEMATICSBASE_CPP__
#define __SYSTEMATICSBASE_CPP__

#include <SystematicsBase.h>

#include <Sample.h>
#include <iostream>
#include <TTree.h>

using std::string;
using std::cout;
using std::endl;

SystematicsBase::SystematicsBase(const int n_universes, const bool verbose) : m_Nuniverses(n_universes), m_verbose(verbose),
	m_Nsamples(0), m_CurrentSample("") // , m_counter(-1)
{
	m_samples.clear();
	Sample::Verbose(m_verbose);
}

SystematicsBase::SystematicsBase(const bool verbose) : m_Nuniverses(1), m_verbose(verbose), m_Nsamples(0), m_CurrentSample("")
{
	Sample::Verbose(m_verbose);
	m_samples.clear();
}

void SystematicsBase::AddSample(const std::string &name, const int nbins, const double x_low, const double x_high)
{
	if(IsUniqueSample(name)){
		m_samples [ name ] = new Sample(name, nbins, x_low, x_high);
		m_Nsamples++;
	}
}

void SystematicsBase::AddSample(const std::string &name, const int nbins, const double * x_bins)
{
	if(IsUniqueSample(name)){
		// m_samples.push_back( new Sample(name, nbins, x_bins) );
		m_samples [ name ] = new Sample(name, nbins, x_bins);
		m_Nsamples++;
		// m_samples_map[ name ] = m_counter++;
	}
}

SystematicsBase::~SystematicsBase()
{
	// cout << "SystematicsBase::~SystematicsBase() 1)" << endl;
	m_samples.clear();
	// cout << "SystematicsBase::~SystematicsBase() 2)" << endl;

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
	if(m_verbose) cout << "Filling vertical Error Band: " << name << " for sample " << sam_name;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillVertErrorBand(name, value, weights, cvweight, cvWeightFromMe) ) pass = true;
	}

	if(m_verbose){
		if(pass) cout << " Success" << endl;
		else cout << endl;
	}
	return pass;
}

bool SystematicsBase::FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const double * weights,
	const double cvweight, double cvWeightFromMe)
{
	if(m_verbose) cout << "Filling vertical Error Band: " << name << " for sample " << sam_name;
	bool pass = false;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillVertErrorBand(name, value, weights, cvweight, cvWeightFromMe) ) pass = true;
	}

	if(m_verbose){
		if(pass) cout << " Success" << endl;
		else cout << endl;
	}
	return pass;
}

bool SystematicsBase::FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const double weightDown, const double weightUp,
	const double cvweight, double cvWeightFromMe)
{
	bool pass = false;
	if(m_verbose)cout << "Filling vertical Error Band: " << name << " for sample " << sam_name;
	std::map<std::string,Sample*>::iterator it = m_samples.find( sam_name );
	if(it != m_samples.end()){
		if( it->second->FillVertErrorBand(name, value, weightDown, weightUp, cvweight, cvWeightFromMe) ) pass = true;
	}

	if(m_verbose){
		if(pass) cout << " Success" << endl;
		else cout << endl;
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

double * SystematicsBase::GetOptBinning(TTree * intree, const std::string &var_name, const int x_nbins, const double x_min, const double x_max,
    const string &cuts, const double precision)
{
	// Default presision should be within sqrt( N(x_high - x_low)/nbins )

    double * binning = new double [ x_nbins + 1 ];
    binning[ 0 ] = x_min;
    binning[ x_nbins ] = x_max;

    int integral = GetEntriesInRange(intree, var_name, x_min, x_max, cuts);
    int dentry = (int)integral/x_nbins;
    double ave_bin = (double)(x_max - x_min)/x_nbins;

    cout << "Entries =  " << integral << " : Entries per bin = " << dentry << " p/m " << (int)(dentry*precision) << endl;
    cout << "Starting ave bin size " << ave_bin << endl;

    double * tot_bin = new double [ x_nbins ];

    for(int i = 1; i < x_nbins; i++){
        // first using starting point:
        double start = x_min + i*ave_bin;
        cout << "Finding element " << i << ": Using " << start << " as initial value." << endl;
        double low = binning[ (i - 1) ];

        int entries = GetEntriesInRange(intree, var_name, low, start, cuts);
        double delta = (double)(1. - (double)entries/(double)dentry);
        cout << "entries/dentry = " << entries << "/" << dentry << " Starting delta = " << delta << endl;
   		double value = start;
   		double sign = (delta < 0.) ? -1. : 1.;
        if(TMath::Abs(delta) > precision){
        	for(int m = 0; m < 10; m++){
        		for(int n = 0; n < 10; n++){
        			for(int o = 0; o < 10; o++){
        				// This is good up to in stats of 1e6.
        				value = start*(1. + sign*( m*0.001 + n*0.01 + o*0.1 ) );
        				entries = GetEntriesInRange(intree, var_name, low, value, cuts);
        				double delta = (double)(1. - (double)entries/(double)dentry);
        				cout << "For " << low << " <= " << var_name << " <= " << value << " : " << entries << " (" << dentry << ") Delta = " << delta << endl;
        				if(TMath::Abs(delta) < precision) break;
        			}
        			if(TMath::Abs(delta) < precision) break;        			
        		}
        		if(TMath::Abs(delta) < precision) break;        		
        	}
        }
        cout << "Best bin value found: " << start << endl;
        binning[i] = value;
    }

    cout << "*** Finished Binning ***" << endl;
    for(int i = 0; i < x_nbins + 1; i++){
    	cout << var_name << "[" << i << "] = " << binning[i] << " ";
    	if(i < x_nbins) cout << tot_bin[i];
    	cout << endl;
    }
    cout << "************************" << endl;

    return binning;
}

int SystematicsBase::GetEntriesInRange(TTree * tree, const std::string &var_name, const int x_min, const int x_max, const std::string &cuts)
{
	string basecuts = cuts; 
	if(!cuts.empty()) basecuts += " && ";
	sel = Form("%s%f <= %s && %s <= %f", basecuts.c_str(), x_min, var_name.c_str(), var_name.c_str(), x_max);
	return tree->Draw(var_name.c_str(), sel.c_str() , "goff");
}

#endif