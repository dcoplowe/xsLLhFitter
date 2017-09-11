#ifndef __SYSTEMATICSBASE_H__
#define __SYSTEMATICSBASE_H__

#include <string>
#include <vector>
#include <map>

class Sample;

class SystematicsBase
{
public:
	SystematicsBase(const int n_universes, const bool verbose);
	SystematicsBase(const bool verbose);
	~SystematicsBase();
	virtual void Run() = 0;

	void SetNUniverses(const int var){ m_Nuniverses = var; }
	void AddSample(const std::string &name, const int nbins, const double x_low, const double x_high);
	void AddSample(const std::string &name, const int nbins, const double * x_bins);
	int GetNSamples(){ return (int)m_samples.size(); }

	void FillSample(const std::string &name, const double val, const double weight = 1.0);

protected:
	int m_Nuniverses;
	bool m_verbose;
	std::map<std::string, Sample*> m_samples;
	// std::vector<Sample*> m_samples;
	// std::map<std::string, int> m_sample_map;
	bool IsUniqueName(const std::string &name);

};

#endif