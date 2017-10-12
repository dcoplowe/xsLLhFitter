#ifndef __FILEIOBASE_H__
#define __FILEIOBASE_H__

#include <string>
#include <TROOT.h>

class TFile;
class TTree;

#ifndef kIniValue
#define kIniValue -999
#endif

class FileIOBase
{
public:
	FileIOBase(const std::string &in_filename, const std::string &in_treename, bool verbose = false);
	FileIOBase(bool verbose = false);
	
	~FileIOBase();

	// In file stuff:
	Int_t   inCurrent;
	TTree * fChain;
	void SetupInFile(const std::string &in_filename, const std::string &in_treename);

	Int_t    GetEntry(Long64_t entry);
	Long64_t GetEntries();
	void SetMaxEntries(Long64_t max_entries);
	void ResetMaxEntries();

	// Outfile stuff: (Can use alone of as part of class, idea of using with class is for
	// the filling of the Likelihood Fit ntuple)
	TFile * outfile;
	void SetupOutFile(const std::string &outfilename);
	static std::string SetDate();
	static TFile * MakeOutFile(const std::string &outfilename);
	void SetupLLNTuple(TFile * infile = 0x0);
	void SetSample(Int_t sample_no){ m_sample = sample_no; }
	// Sample flag needs to be set for LL fitter to read in
	void Fill();
	void Write();

	static std::string GetEnv(const char * name);

protected:
	bool m_verbose;
	virtual	void Init() = 0;

	TTree * m_LLtuple;	
	virtual void InitLLNTuple() = 0;

private:
	TFile * m_infile;
	static const std::string m_date;

	Int_t m_sample;

	Long64_t m_totentries;
	Int_t m_entries;
	Int_t m_per10;
};

#endif