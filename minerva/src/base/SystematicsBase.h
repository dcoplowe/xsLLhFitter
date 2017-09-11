#ifndef __SYSTEMATICSBASE_H__
#define __SYSTEMATICSBASE_H__

#include <string>
#include <vector>

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

protected:
	int m_Nuniverses;
	bool m_verbose;
	std::vector<Sample*> m_samples;
};

#endif