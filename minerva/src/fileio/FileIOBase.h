#ifndef __FILEIOBASE_H__
#define __FILEIOBASE_H__

#include <string>
#include <TROOT.h>

class TFile;
class TTree;

class FileIOBase
{
public:
	FileIOBase(std::string in_filename, std::string in_treename, bool verbose = false);
	FileIOBase(bool verbose = false);
	
	~FileIOBase();

	Int_t   inCurrent;
	TTree * inchain;

	void SetupInFile(std::string in_filename, std::string in_treename);

	Int_t    GetEntry(Long64_t entry);
	Long64_t GetEntries();

	TFile * outfile;
	void SetupOutFile(std::string outfilename);

protected:
	bool m_verbose;
	virtual	void Init() = 0;

private:
	TFile * m_infile;
	std::string	m_date;
};

#endif