#ifndef __SAMPLE_CPP__
#define __SAMPLE_CPP__

#include <Sample.h>

#include <TH1D.h>
#include <PlotUtils/MnvH1D.h> 
#include <PlotUtils/MnvH2D.h> 
using namespace PlotUtils;

Sample::Sample(std::string name, int nbins, double x_low, double x_high), m_Nhists(1), m_det(true), m_name( (name + "_Det") )
{
	// Do we want to inlcude under and overflow bins in this class?
	// This makes a histogram for each interaction/reweightable var. type if we are producing splines
	for(int i = 0; i < m_hists; i++) m_1Dhists.push_back( new MnvH1D(Form("%s_Det", m_name.c_str()), "", nbins, x_low, x_high) );
}

Sample::Sample(std::string name, int nbins, double * x_bins), m_Nhists(1), m_det(true), m_name( (name + "_Det") )
{
	// Do we want to inlcude under and overflow bins in this class?
	// This makes a histogram for each interaction/reweightable var. type if we are producing splines
	for(int i = 0; i < m_Nhists; i++) m_1Dhists.push_back( new MnvH1D(Form("%s_Det", m_name.c_str()), "", nbins, x_bins) );
}

Sample::~Sample()
{
	m_1Dhists.clear();
}

void Sample::Fill(double value, double wgt)
{
	if(m_type == Det) m_1Dhists[0]->Fill(value, wgt);
	else if(IsModel(model)) m_1Dhists[model]->Fill(value, wgt);
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

bool Sample::FillLatErrorBand( const std::string& name, const double val, const std::vector<double>& shifts,
	const double cvweight, const bool fillcv, const double *weights)
{
	if(m_type == Det) m_1Dhists[0]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights)
	else if(IsModel(model)) m_1Dhists[model]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights);
}

bool Sample::FillLatErrorBand( const std::string& name, cons, const double val, const double * shifts, const double cvweight,
	const bool fillcv, const double *weights)
{
	if(m_type == Det) m_1Dhists[0]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights);
	else if(IsModel(model)) m_1Dhists[model]->FillLatErrorBand(name, val, shifts, cvweight, fillcv, weights);
}

bool Sample::FillLatErrorBand( const std::string& name, const double val, const double shiftDown, const double shiftUp,
	const double cvweight, const bool fillcv)
{
	if(m_type == Det) m_1Dhists[0]->FillLatErrorBand(name, val, shiftDown, shiftUp, cvweight, fillcv);
	else if(IsModel(model)) m_1Dhists[model]->FillLatErrorBand(name, val, shiftDown, shiftUp, cvweight, fillcv);
}

bool Sample::FillVertErrorBand( const std::string& name, const double val, const std::vector<double>& weights,
	const double cvweight, double cvWeightFromMe)
{
	if(m_type == Det) m_1Dhists[0]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
	else if(IsModel(model)) m_1Dhists[model]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
}

bool Sample::FillVertErrorBand( const std::string& name, const double val, const double * weights,
	const double cvweight, double cvWeightFromMe)
{
	if(m_type == Det) m_1Dhists[0]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
	else if(IsModel(model)) m_1Dhists[model]->FillVertErrorBand(name, val, weights, cvweight, cvWeightFromMe);
}

bool Sample::FillVertErrorBand( const std::string& name, const double val, const double weightDown, const double weightUp,
	const double cvweight, double cvWeightFromMe)
{
	if(m_type == Det) m_1Dhists[0]->FillVertErrorBand(name, val, weightDown, weightUp, cvweight, cvWeightFromMe);
	else if(IsModel(model)) m_1Dhists[model]->FillVertErrorBand(name, val, weightDown, weightUp, cvweight, cvWeightFromMe);
}

bool Sample::FillUncorrError( const std::string& name, const double val, const double err, const double cvweight)
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