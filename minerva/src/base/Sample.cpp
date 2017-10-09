#ifndef __SAMPLE_CPP__
#define __SAMPLE_CPP__

#include <Sample.h>
#include <ErrorType.h>

Sample::Sample(const std::string& name, const int nbins, const double x_low, const double x_high) : 
	MnvH1D(Form("%s_nbins%.3d_lowE%.3d_higE%.3d", name.c_str(), nbins, (int)x_low, (int)x_high), "", nbins, x_low, x_high), 
	m_value(-999.), m_wgt(-999.), m_start(-999), m_anabin(-999)
{
	m_error.clear();

	int n_anabins = GetNbinsX() + 2;
	m_anaHist = new MnvH1D(Form("%s_nbins%.3d_anaHist", GetName(), n_anabins), "", n_anabins, 0., (double)n_anabins);
	// Do we want to inlcude under and overflow bins in this class?
	// This makes a histogram for each interaction/reweightable var. type if we are producing splines
}

Sample::Sample(const std::string& name, const int nbins, const double * x_bins) : 
	MnvH1D(Form("%s_nbins%.3d_lowE%.3d_higE%.3d", name.c_str(), nbins, (int)x_bins[0], (int)x_bins[nbins + 1]), "", nbins, x_bins),
	m_value(-999.), m_wgt(-999.), m_start(-999), m_anabin(-999)
{
	m_error.clear();
	int n_anabins = GetNbinsX() + 2;
	m_anaHist = new MnvH1D(Form("%s_nbins%.3d_anaHist", GetName(), n_anabins), "", n_anabins, 0., (double)n_anabins);
}

Sample::~Sample()
{
	m_error.clear();
	if(m_anaHist) delete m_anaHist;
}

int Sample::Fill(const double var, const double wgt)
{
	m_value = var;
	m_wgt = wgt;
	m_anabin = GetXaxis()->FindBin(m_value) + 1;
	m_anaHist->Fill(m_anabin, m_wgt);
	return MnvH1D::Fill(m_value, m_wgt);
}

bool Sample::FillLatErrorBand(const std::string& name, const double value, const std::vector<double>& shifts,
	const double cvweight, const bool fillcv, const double *weights)
{
	bool h1_fill = MnvH1D::FillLatErrorBand(name, value, shifts, cvweight, fillcv, weights);
	bool h2_fill = true; //m_anaHist->FillLatErrorBand(name, GetXaxis()->FindBin(value) + 1, shifts, cvweight, fillcv, weights);
	return (h1_fill && h2_fill);
}

bool Sample::FillLatErrorBand(const std::string& name, const double value, const double * shifts, const double cvweight,
	const bool fillcv, const double *weights)
{
	bool h1_fill = MnvH1D::FillLatErrorBand(name, value, shifts, cvweight, fillcv, weights);
	bool h2_fill = true; //m_anaHist->FillLatErrorBand(name, GetXaxis()->FindBin(value) + 1, shifts, cvweight, fillcv, weights);
	return (h1_fill && h2_fill);
}

bool Sample::FillLatErrorBand(const std::string& name, const double value, const double shiftDown, const double shiftUp,
	const double cvweight, const bool fillcv)
{
	bool h1_fill = MnvH1D::FillLatErrorBand(name, value, shiftDown, shiftUp, cvweight, fillcv);
	bool h2_fill = true; //m_anaHist->FillLatErrorBand(name, GetXaxis()->FindBin(value) + 1, shiftDown, shiftUp, cvweight, fillcv);
	return (h1_fill && h2_fill);
}

bool Sample::FillVertErrorBand(const std::string& name, const double value, const std::vector<double>& weights,
	const double cvweight, double cvWeightFromMe)
{
	bool h1_fill = MnvH1D::FillVertErrorBand(name, value, weights, cvweight, cvWeightFromMe);
	bool h2_fill = true; //m_anaHist->FillVertErrorBand(name, GetXaxis()->FindBin(value) + 1, weights, cvweight, cvWeightFromMe);
	return (h1_fill && h2_fill);
}

bool Sample::FillVertErrorBand(const std::string& name, const double value, const double * weights,
	const double cvweight, double cvWeightFromMe)
{
	bool h1_fill = MnvH1D::FillVertErrorBand(name, value, weights, cvweight, cvWeightFromMe);
	bool h2_fill = true; //m_anaHist->FillVertErrorBand(name, GetXaxis()->FindBin(value) + 1, weights, cvweight, cvWeightFromMe);
	return (h1_fill && h2_fill);
}

bool Sample::FillVertErrorBand(const std::string& name, const double value, const double weightDown, const double weightUp,
	const double cvweight, double cvWeightFromMe)
{
	bool h1_fill = MnvH1D::FillVertErrorBand(name, value, weightDown, weightUp, cvweight, cvWeightFromMe);
	bool h2_fill = true; //m_anaHist->FillVertErrorBand(name, GetXaxis()->FindBin(value) + 1, weightDown, weightUp, cvweight, cvWeightFromMe);
	return (h1_fill && h2_fill);
}

bool Sample::FillUncorrError(const std::string& name, const double value, const double err, const double cvweight)
{
	bool h1_fill = MnvH1D::FillUncorrError(name, value, err, cvweight);
	bool h2_fill = true; //m_anaHist->FillUncorrError(name, GetXaxis()->FindBin(value) + 1, err, cvweight);
	return (h1_fill && h2_fill);
}	

bool Sample::FillError(ErrorType * type) const
{
	bool pass = false;
	for(size_t i = 0; i < m_error.size(); i++){
		ErrorType * tmp = m_error[i];
		if(tmp == type){
			pass = true;
			break;
		}
	}
	return pass;
}

MnvH1D * Sample::GetAnaHist() const
{
	return static_cast<MnvH1D*>( m_anaHist->Clone( Form("%s_cp", m_anaHist->GetName() ) ) );
}

// bool Sample::FillLatErrorBand(const std::string& name, const std::vector<double>& shifts,
// 	const double cvweight, const bool fillcv, const double *weights)
// {
// 	bool h1_fill = MnvH1D::FillLatErrorBand(name, m_value, shifts, cvweight, fillcv, weights);
// 	bool h2_fill = true; //m_anaHist->FillLatErrorBand(name, m_anabin, shifts, cvweight, fillcv, weights);
// 	return (h1_fill && h2_fill);
// }

// bool Sample::FillLatErrorBand(const std::string& name, const double * shifts, const double cvweight,
// 	const bool fillcv, const double *weights)
// {
// 	bool h1_fill = MnvH1D::FillLatErrorBand(name, m_value, shifts, cvweight, fillcv, weights);
// 	bool h2_fill = true; //m_anaHist->FillLatErrorBand(name, m_anabin, shifts, cvweight, fillcv, weights);
// 	return (h1_fill && h2_fill);
// }

// bool Sample::FillLatErrorBand(const std::string& name, const double shiftDown, const double shiftUp,
// 	const double cvweight, const bool fillcv)
// {
// 	bool h1_fill = MnvH1D::FillLatErrorBand(name, m_value, shiftDown, shiftUp, cvweight, fillcv);
// 	bool h2_fill = true; //m_anaHist->FillLatErrorBand(name, m_anabin, shiftDown, shiftUp, cvweight, fillcv);
// 	return (h1_fill && h2_fill);
// }

// bool Sample::FillVertErrorBand(const std::string& name, const std::vector<double>& weights,
// 	const double cvweight, double cvWeightFromMe)
// {
// 	bool h1_fill = MnvH1D::FillVertErrorBand(name, m_value, weights, cvweight, cvWeightFromMe);
// 	bool h2_fill = true; //m_anaHist->FillVertErrorBand(name, m_anabin, weights, cvweight, cvWeightFromMe);
// 	return (h1_fill && h2_fill);
// }

// bool Sample::FillVertErrorBand(const std::string& name, const double * weights,
// 	const double cvweight, double cvWeightFromMe)
// {
// 	bool h1_fill = MnvH1D::FillVertErrorBand(name, m_value, weights, cvweight, cvWeightFromMe);
// 	bool h2_fill = true; //m_anaHist->FillVertErrorBand(name, m_anabin, weights, cvweight, cvWeightFromMe);
// 	return (h1_fill && h2_fill);
// }

// bool Sample::FillVertErrorBand(const std::string& name, const double weightDown, const double weightUp,
// 	const double cvweight, double cvWeightFromMe)
// {
// 	bool h1_fill = MnvH1D::FillVertErrorBand(name, m_value, weightDown, weightUp, cvweight, cvWeightFromMe);
// 	bool h2_fill = true; //m_anaHist->FillVertErrorBand(name, m_anabin, weightDown, weightUp, cvweight, cvWeightFromMe);
// 	return (h1_fill && h2_fill);
// }

// bool Sample::FillUncorrError(const std::string& name, const double err, const double cvweight)
// {
// 	bool h1_fill = MnvH1D::FillUncorrError(name, m_value, err, cvweight);
// 	bool h2_fill = true; //m_anaHist->FillUncorrError(name, m_anabin, err, cvweight);
// 	return (h1_fill && h2_fill);
// }




#endif