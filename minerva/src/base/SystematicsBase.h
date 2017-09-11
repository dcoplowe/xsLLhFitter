#ifndef __SYSTEMATICSBASE_H__
#define __SYSTEMATICSBASE_H__

#include <string>
#include <vector>
#include <map>

class Sample;

class SystematicsBase
{
public:
	SystematicsBase(int n_universes, bool verbose);
	SystematicsBase(bool verbose);
	~SystematicsBase();
	virtual void Run() = 0;

	void SetNUniverses(int var){ m_Nuniverses = var; }
	void AddSample(std::string name, int nbins, double x_low, double x_high);
	void AddSample(std::string name, int nbins, double * x_bins);
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