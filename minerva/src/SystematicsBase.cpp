#ifndef __SYSTEMATICSBASE_CPP__
#define __SYSTEMATICSBASE_CPP__

#include <TFile.h>
#include <TTree.h>
#include <cassert>
#include <stdlib>
#include <PlotUtils/MnvH1D.h>
#include <TDatime.h>

using std::string;
using std::cout;
using std::endl;
using namespace PlotUtils;

SystematicsBase::SystematicsBase(std::string in_filename, in_treename, bool verbose) : m_verbose(verbose)
{
	cout << "SystematicsBase::SystematicsBase()" << endl;
	// Check the in_file
	// 1) if starts with "/" assume it's the complete path to a file
	// 2) if contains / assum its in a path in the $(DATA_DIR)
	// 3) else assume it's in $(DATA_DIR)

	string tmp_infile = "";
	if(in_filename[0] == '/') tmp_infile = in_filename;
	else tmp_infile = std::getenv("data") + "/NTupleAnalysis/MC/Reduced/" + in_filename;

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
	m_intree = static_cast<>( m_infile->Get(in_treename.c_str() ) ); 
	assert(m_intree);

	m_1Dhists.clear();

	TDatime time;
	m_date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );

	m_date;
	m_outfile = 0x0;

}

SystematicsBase::~SystematicsBase()
{
	if(m_infile->IsOpen()){
		delete m_intree; 
		m_infile->Close();
		delete m_infile;
	}

	if(m_outfile){
		if(m_outfile->IsOpen()){
			m_outfile->Close();
			delete	m_outfile;
		}
	}
}

SystematicsBase::SetupOutFile(std::string outfilename)
{
	if(outfilename.empty()){
		cout << __FILE__ << ":" << __LINE__ << " : ERROR : Tree name is empty" << endl;
		exit(0);
	}

	string tmp_file = "";
	if(outfilename[0] == '/') tmp_file = outfilename;
	else tmp_file = std::getenv("DATA_DIR") + "/" + outfilename;

	// Add date:
	string endname = ".root";
	size_t is_dotroot = tmp_file.find(endname);
	if(is_dotroot != string::npos) tmp_file = tmp_file.substr(0, is_dotroot);
	tmp_file += "_";
	tmp_file += m_date;
	tmp_file += endname;
	m_outfile = new TFile( tmp_file.c_str(), "RECREATE");
	assert(m_outfile);
}

#endif
