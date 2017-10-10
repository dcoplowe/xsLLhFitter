#ifndef __FILEIOBASE_CPP__
#define __FILEIOBASE_CPP__

#include <FileIOBase.h>

#include <TFile.h>
#include <TTree.h>
#include <cassert>
#include <cstdlib>
#include <TDatime.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

const std::string FileIOBase::m_date = FileIOBase::SetDate();

FileIOBase::FileIOBase(const std::string &in_filename, const std::string &in_treename, bool verbose) : inCurrent(-1), m_verbose(verbose), m_sample(kIniValue)
{
	cout << "FileIOBase::FileIOBase(std::string in_filename, std::string in_treename, bool verbose)" << endl;
	
	string tmp_infile = "";
	if(in_filename[0] == '/') tmp_infile = in_filename;
	else tmp_infile = std::string(std::getenv("data")) + "/NTupleAnalysis/MC/Reduced/" + in_filename;

	if(tmp_infile.empty()){
		cout << __FILE__ << ":" << __LINE__ << " : ERROR : File name is empty" << endl;
		exit(0);
	}

	if(in_treename.empty()){
		cout << __FILE__ << ":" << __LINE__ << " : ERROR : Tree name is empty" << endl;
		exit(0);
	}

	m_infile = new TFile(tmp_infile.c_str());
	assert(m_infile);
	inchain = static_cast<TTree*>( m_infile->Get(in_treename.c_str() ) ); 
	assert(inchain);
	// Init();
	m_LLtuple = 0x0;

	m_totentries = inchain->GetEntries();
	m_entries = m_totentries;
	m_per10 = (Int_t)m_entries/10;

	outfile = 0x0;
}

FileIOBase::FileIOBase(bool verbose) : inCurrent(-1), m_verbose(verbose), m_sample(kIniValue)
{
	cout << "FileIOBase::FileIOBase(bool verbose)" << endl;
	m_infile = 0x0;
	inchain = 0x0;

	TDatime time;
	// m_date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );
	outfile = 0x0;
}

FileIOBase::~FileIOBase()
{
	// if(m_infile->IsOpen()){
	// 	delete inchain; 
	// 	// cout << "FileIOBase::~FileIOBase() : Closing file" << endl;
	// 	m_infile->Close();
	// 	// cout << "FileIOBase::~FileIOBase() : Success" << endl;
	// 	delete m_infile;
	// 	// cout << "FileIOBase::~FileIOBase() : deleted m_infile" << endl;
	// }

	// if(outfile){
	// 	if(outfile->IsOpen()){
	// 		outfile->Close();
	// 		delete	outfile;
	// 	}
	// }

	// if(m_LLtuple) delete m_LLtuple;
}

void FileIOBase::SetupOutFile(const std::string &outfilename)
{
	outfile = FileIOBase::MakeOutFile(outfilename);
}

std::string FileIOBase::SetDate()
{
	TDatime time;
	std::string date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );
	return date;
}

TFile * FileIOBase::MakeOutFile(const std::string &outfilename)
{
	if(outfilename.empty()){
		cout << __FILE__ << ":" << __LINE__ << " : ERROR : Tree name is empty" << endl;
		exit(0);
	}

	string tmp_file = "";
	if(outfilename[0] == '/') tmp_file = outfilename;
	else tmp_file = std::string(std::getenv("DATA_DIR")) + "/" + outfilename;

	// Add date:
	string endname = ".root";
	size_t is_dotroot = tmp_file.find(endname);
	if(is_dotroot != string::npos) tmp_file = tmp_file.substr(0, is_dotroot);
	tmp_file += "_";
	tmp_file += m_date;
	tmp_file += endname;

	cout << "Creating outfile with name: " << tmp_file << endl;
	TFile * file = new TFile( tmp_file.c_str(), "RECREATE");
	assert(file);
	return file;
}

Int_t FileIOBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
	// Always result the sample no before filling the vars (might want to do this for other vars
	// but then again we should only be passing variables from one tree to another):
	m_sample = kIniValue;
	if(entry % m_per10 == 0) cout << "Analysed " << Form("%.2f%%", (double)(entry*100./m_entries) ) << " : " << entry << "/" << m_entries << endl;
	if (!inchain) return 0;
	return inchain->GetEntry(entry);
}

Long64_t FileIOBase::GetEntries()
{
	if (!inchain) return 0;
	return inchain->GetEntries();	
}

void FileIOBase::SetupInFile(const std::string &in_filename, const std::string &in_treename)
{
	string tmp_infile = "";
	if(in_filename[0] == '/') tmp_infile = in_filename;
	else tmp_infile = std::string(std::getenv("data")) + "/NTupleAnalysis/MC/Reduced/" + in_filename;

	if(tmp_infile.empty()){
		cout << __FILE__ << ":" << __LINE__ << " : ERROR : File name is empty" << endl;
		exit(0);
	}

	if(in_treename.empty()){
		cout << __FILE__ << ":" << __LINE__ << " : ERROR : Tree name is empty" << endl;
		exit(0);
	}

	m_infile = new TFile(tmp_infile.c_str());
	assert(m_infile);
	inchain = static_cast<TTree*>( m_infile->Get(in_treename.c_str() ) ); 
	assert(inchain);
	Init();
}

void FileIOBase::SetupLLNTuple(TFile * infile)
{
	if(m_LLtuple){ 
		delete m_LLtuple;
		m_LLtuple = 0x0;
	}

	if(infile) infile->cd();
	else if(outfile) outfile->cd();
	else cout << "May have problem with Directory Shizzle" << endl;

	m_LLtuple = new TTree("selectedEvents", "selectedEvents");	
	m_LLtuple->Branch("cutBranch", &m_sample, "cutBranch/I");
	InitLLNTuple();
}
	// Sample flag needs to be set for LL fitter to read in

void FileIOBase::Fill()
{
	m_LLtuple->Fill();
}

void FileIOBase::Write()
{
	m_LLtuple->Write();
}

void FileIOBase::SetMaxEntries(Long64_t max_entries){ 
	m_entries = (Int_t)max_entries;
	m_per10 = (Int_t)m_entries/10; 
}

void FileIOBase::ResetMaxEntries(){ 
	m_entries = (Int_t)m_totentries;
	m_per10 = (Int_t)m_entries/10; 
}

#endif