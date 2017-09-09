#include <SystematicsBase.h>

#ifndef __SAMPLE_CPP__
#define __SAMPLE_CPP__

#include <TH1D.h>
#include <PlotUtils/MnvH1D.h> 
#include <PlotUtils/MnvH2D.h> 
using namespace PlotUtils;

Sample::Sample(std::string name, int nbins, double x_low, double x_high, Type type) m_type(type)
{
	// Do we want to inlcude under and overflow bins in this class?
	// This makes a histogram for each interaction type if we are producing splines
	int n_hists = (int)Det;
	if(m_type == Det) n_hists = 1;
	for(int i = 0; i < n_hists; i++) m_1Dhists.push_back( new MnvH1D(Form("%s_%s", name.c_str(), GetType()), "", nbins, x_low, x_high) );
}

Sample::Sample(std::string name, int nbins, double * x_bins, Type type) m_type(type)
{
	// This makes a histogram for each interaction type if we are producing splines
	int n_hists = (int)Det;
	if(m_type == Det) n_hists = 1;
	for(int i = 0; i < n_hists; i++) m_1Dhists.push_back( new MnvH1D(Form("%s_%s", name.c_str(), GetType()), "", nbins, x_bins) );
}

Sample::~Sample()
{
	m_1Dhists.clear();
}

void Sample::Fill(double value, double wgt, int model)
{
	if(m_type == Det) m_1Dhists[0]->Fill(value, wgt);
	else if(IsModel(model)) m_1Dhists[model]->Fill(value, wgt);
}

bool Sample::IsModel(int model)
{
	bool pass = false;
	Type mod_type = static_cast<Type>(model);
	switch(mod_type){
		case ModQE: pass = true; break;
		case ModQE: pass = true; break;
		case ModQE: pass = true; break;
		default: pass = false; break;
	}
	return pass;
}

const char * Sample::GetType()
{
	string out;
	switch(m_type){
		case Det: out = "Det"; break;
		case ModQE: out = "ModQE"; break;
		default: out = "Unknown"; break;
	}
	return out.c_str();
}

// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
bool Sample::AddLatErrorBand( const std::string& name, const int nhists)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddLatErrorBand(name, nhists)) counter++;
	}
	return(counter == m_1Dhists.size());
}

bool Sample::AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddLatErrorBand(name, nhists)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddLatErrorBandAndFillWithCV( const std::string& name, const int nhists)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddLatErrorBandAndFillWithCV(name, nhists)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddVertErrorBand( const std::string& name, const int nhists)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddVertErrorBand(name, nhists)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddVertErrorBand(name, nhists)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddVertErrorBandAndFillWithCV( const std::string& name, const int nhists)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddVertErrorBandAndFillWithCV(name, nhists)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddUncorrError( const std::string& name)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddUncorrError(name)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddUncorrError(name, hist, errInContent)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddUncorrErrorAndFillWithCV( const std::string& name)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddUncorrErrorAndFillWithCV(name)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddMissingErrorBandsAndFillWithCV( const MnvH1D& ref)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddMissingErrorBandsAndFillWithCV(ref)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::AddMissingErrorBandsAndFillWithCV( const MnvH2D& ref)
{
	size_t counter = 0;
	for(size_t i = 0; i < m_1Dhists.size(); i++){
		if(m_1Dhists[i]->AddMissingErrorBandsAndFillWithCV(ref)) counter++;
	}
	return (counter == m_1Dhists.size());
}

bool Sample::FillLatErrorBand( const std::string& name, int model, const double val, const std::vector<double>& shifts,
	const double cvweight, const bool fillcv, const double *weights)
{
	if(m_type == Det) m_1Dhists[0]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights)
	else if(IsModel(model)) m_1Dhists[model]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights);
}

bool Sample::FillLatErrorBand( const std::string& name, const int model, const double val, const double * shifts, const double cvweight,
	const bool fillcv, const double *weights)
{
	if(m_type == Det) m_1Dhists[0]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights);
	else if(IsModel(model)) m_1Dhists[model]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights);
}

bool Sample::FillLatErrorBand( const std::string& name, int model, const double val, const double shiftDown, const double shiftUp,
	const double cvweight, const bool fillcv)
{
	if(m_type == Det) m_1Dhists[0]->FillLatErrorBand(name, val, shiftDown, shiftUp, cvweight, fillcv);
	else if(IsModel(model)) m_1Dhists[model]->FillLatErrorBand(name, val, shiftDown, shiftUp, cvweight, fillcv);
}

bool Sample::FillVertErrorBand( const std::string& name, int model, const double val, const std::vector<double>& weights,
	const double cvweight, double cvWeightFromMe)
{
	if(m_type == Det) m_1Dhists[0]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
	else if(IsModel(model)) m_1Dhists[model]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
}

bool Sample::FillVertErrorBand( const std::string& name, int model, const double val, const double * weights,
	const double cvweight, double cvWeightFromMe)
{
	if(m_type == Det) m_1Dhists[0]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
	else if(IsModel(model)) m_1Dhists[model]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
}

bool Sample::FillVertErrorBand( const std::string& name, int model, const double val, const double weightDown, const double weightUp,
	const double cvweight, double cvWeightFromMe)
{
	if(m_type == Det) m_1Dhists[0]->FillVertErrorBand(name, val, weightDown, weightUp, cvweight, cvWeightFromMe);
	else if(IsModel(model)) m_1Dhists[model]->FillVertErrorBand(name, val, weightDown, weightUp, cvweight, cvWeightFromMe);
}

bool Sample::FillUncorrError( const std::string& name, int model, const double val, const double err, const double cvweight)
{
	if(m_type == Det) m_1Dhists[0]->FillUncorrError(name, val, err, cvweight);
	else if(IsModel(model)) m_1Dhists[model]->FillUncorrError(name, val, err, cvweight);
}

// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
// -------------------------------------------------------- END MnvH1D --------------------------------------------------------


#endif

#ifndef __SYSTEMATICSBASE_CPP__
#define __SYSTEMATICSBASE_CPP__

#include <TFile.h>
#include <TTree.h>
#include <cassert>
#include <cstdlib>
#include <PlotUtils/MnvH1D.h> 
#include <TDatime.h>

using std::string;
using std::cout;
using std::endl;
using namespace PlotUtils;

SystematicsBase::SystematicsBase() : m_verbose(false), m_Nuniverses(1)
{
	cout << "SystematicsBase::SystematicsBase()" << endl;
	m_infile = 0x0;	
	m_intree = 0x0;
	m_1Dhists.clear();

	TDatime time;
	m_date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );
	m_outfile = 0x0;
}

SystematicsBase::SystematicsBase(int n_universes, bool verbose) : m_verbose(verbose), m_Nuniverses(n_universes)
{
	cout << "SystematicsBase::SystematicsBase(int n_universes, bool verbose)" << endl;
	m_infile = 0x0;	
	m_intree = 0x0;
	m_1Dhists.clear();

	TDatime time;
	m_date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );
	m_outfile = 0x0;
}

SystematicsBase::SystematicsBase(bool verbose) : m_verbose(verbose), m_Nuniverses(1)
{
	cout << "SystematicsBase::SystematicsBase(bool verbose)" << endl;
	m_infile = 0x0;	
	m_intree = 0x0;
	m_1Dhists.clear();

	TDatime time;
	m_date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );
	m_outfile = 0x0;
}

SystematicsBase::SystematicsBase(std::string in_filename, std::string in_treename, bool verbose) : m_verbose(verbose), m_Nuniverses(1)
{
	cout << "SystematicsBase::SystematicsBase(std::string in_filename, std::string in_treename, bool verbose)" << endl;
	// Check the in_file
	// 1) if starts with "/" assume it's the complete path to a file
	// 2) if contains / assum its in a path in the $(DATA_DIR)
	// 3) else assume it's in $(DATA_DIR)

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
	m_intree = static_cast<TTree*>( m_infile->Get(in_treename.c_str() ) ); 
	assert(m_intree);

	m_1Dhists.clear();

	TDatime time;
	m_date = Form("%.2d%.2d%.2d", (int)time.GetMonth(), (int)time.GetDay(), (int)(time.GetYear() - 2000) );
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

void SystematicsBase::SetupOutFile(std::string outfilename)
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
	m_outfile = new TFile( tmp_file.c_str(), "RECREATE");
	assert(m_outfile);
}
#endif
