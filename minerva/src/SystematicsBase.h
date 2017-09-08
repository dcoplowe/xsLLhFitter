#ifndef __SYSTEMATICSBASE_H__
#define __SYSTEMATICSBASE_H__

#include <string>
#include <vector>

class TFile;
class TTree;
class MnvH1D;

class SystematicsBase
{
public:
	SystematicsBase(std::string in_filename, std::string in_treename, bool verbose);
	~SystematicsBase();
	virtual void Run() = 0;

protected:
	bool m_verbose;

	TFile * m_infile;
	TTree * m_intree;

	TFile * m_outfile;
	void SetupOutFile(std::string outfilename);

	std::vector<MnvH1D*> m_1Dhists;

private:
	std::string	m_date;

};

#endif