#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <FileIOBase.h>

// #include <string>
// #include <TROOT.h>

class TBranch;

class FileIO : public FileIOBase
{
public:
	FileIO(std::string in_filename, std::string in_treename, bool verbose = false) : 
	FileIOBase(in_filename, in_treename, verbose){ Init();}
	FileIO(bool verbose = false) : FileIOBase(verbose){;}

private:
	void Init();
	void InitLLNTuple();

public:
	// Vars from ntuple (user must add them from tree.makeclass()):
	// Example:
	// Here goes:
	// Double_t varname;
	// TBranch *b_varname;
	// Then in FileIO::Init() (FileIO.cpp):
	// fChain->SetBranchAddress("varname", &varname, &b_varname);

	__ADD_PUBLIC_VARS_AND_BRANCHES__

};

#endif