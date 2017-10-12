#ifndef __SYSTEMATICSBASE_H__
#define __SYSTEMATICSBASE_H__

#include <string>
#include <vector>
#include <map>

class Sample;
class TTree;

class SystematicsBase
{
public:
	SystematicsBase(const int n_universes, const bool verbose);
	SystematicsBase(const bool verbose);
	~SystematicsBase();
	// virtual void Run() = 0;

	void SetNUniverses(const int var){ m_Nuniverses = var; }
	void AddSample(const std::string &name, const int nbins, const double x_low, const double x_high);
	void AddSample(const std::string &name, const int nbins, const double * x_bins);
	int GetNSamples(){ return (int)m_samples.size(); }

	void FillSample(const std::string &name, const double val, const double weight = 1.0);

	// For simplicity fill samples with error bands:
	bool FillLatErrorBand(const std::string& sam_name, const std::string& name, const double value, const std::vector<double>& shifts,
		const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand(const std::string& sam_name, const std::string& name, const double value, const double * shifts, const double cvweight = 1.0,
		const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand(const std::string& sam_name, const std::string& name, const double value, const double shiftDown, const double shiftUp,
		const double cvweight = 1.0, const bool fillcv = true );
	bool FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const std::vector<double>& weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const double * weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand(const std::string& sam_name, const std::string& name, const double value, const double weightDown, const double weightUp,
		const double cvweight  = 1.0, double cvWeightFromMe = 1. );
	bool FillUncorrError(const std::string& sam_name, const std::string& name, const double value, const double err, const double cvweight = 1.0 );

	// If -999 is the presision use error as stat error.
	static double * GetOptBinning(TTree * intree, const std::string &var_name, const int x_nbins, const double x_min, const double x_max,
    const std::string &cuts = "", const double precision = -999);

protected:
	int m_Nuniverses;
	bool m_verbose;
	int m_Nsamples;
	std::map<std::string, Sample*> m_samples;
	bool IsUniqueSample(const std::string &name);
	// virtual	void Prepare() = 0;

	double m_value;
    double m_wgt;
	std::string m_CurrentSample;

	static int GetEntriesInRange(TTree * tree, const std::string &var_name, const int x_min, const int x_max,  const std::string &cuts);


	// static FillSample;
};

#endif