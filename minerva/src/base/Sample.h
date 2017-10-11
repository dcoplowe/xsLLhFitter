#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <string>
#include <vector>
#include <TObject.h>
// #include <TROOT.h>

// class MnvH1D;
// class MnvH2D;
class ErrorType;

#include <PlotUtils/MnvH1D.h> 
// #include <PlotUtils/MnvH2D.h>
using namespace PlotUtils;

class Sample : public MnvH1D, public TObject
{
public:
	// Should simplify this and remove the model bit. 
	// Just make this more of a wrapper for the Minverva histo class.
	// static std::string CurrentSample;

	Sample(const std::string& name, const  int nbins, const double x_low, const double x_high);
	Sample(const std::string& name, const int nbins, const double * x_bins);
	virtual ~Sample();

	int GetSampPos() const { return m_sampos; }

	void AddError(ErrorType * type){ m_error.push_back(type); }
	bool FillError(ErrorType * type) const;

	void SetStartingAnalBin(const int nth_bin){ m_start = nth_bin; }
	int GetAnaBin() const { return (m_anabin + m_start); }
	MnvH1D * GetAnaHist() const;

	int Fill(double var, double wgt);

	bool FillLatErrorBand(const std::string& name, const double value, const std::vector<double>& shifts,
		const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
	
	bool FillLatErrorBand(const std::string& name, const double value, const double * shifts, const double cvweight = 1.0,
		const bool fillcv = true, const double *weights = 0 );
	
	bool FillLatErrorBand(const std::string& name, const double value, const double shiftDown, const double shiftUp,
		const double cvweight = 1.0, const bool fillcv = true );
	
	bool FillVertErrorBand(const std::string& name, const double value, const std::vector<double>& weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	
	bool FillVertErrorBand(const std::string& name, const double value, const double * weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	
	bool FillVertErrorBand(const std::string& name, const double value, const double weightDown, const double weightUp,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	
	bool FillUncorrError(const std::string& name, const double value, const double err, const double cvweight = 1.0 );
	
	// // For internally stored value:
	// bool FillLatErrorBand(const std::string& name, const std::vector<double>& shifts,
	// 	const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
	
	// bool FillLatErrorBand(const std::string& name, const double * shifts, const double cvweight = 1.0,
	// 	const bool fillcv = true, const double *weights = 0 );

	// bool FillLatErrorBand(const std::string& name, const double shiftDown, const double shiftUp,
	// 	const double cvweight = 1.0, const bool fillcv = true );
	
	// bool FillVertErrorBand(const std::string& name, const std::vector<double>& weights,
	// 	const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	
	// bool FillVertErrorBand(const std::string& name, const double * weights,
	// 	const double cvweight  = 1.0, double cvWeightFromMe = 1.);

	// bool FillVertErrorBand(const std::string& name, const double weightDown, const double weightUp,
	// 	const double cvweight  = 1.0, double cvWeightFromMe = 1. );
	// bool FillUncorrError(const std::string& name, const double err, const double cvweight = 1.0 );
	
	// void Fill(const double value, const double wgt);

	ClassDef(Sample, 1) // Sample to hold different topologies

private:
	// std::string m_name;
	// These are for preserving the order with which they were made.
	static int m_Nsamples;
	int m_sampos;

	std::vector<ErrorType*> m_error;
	MnvH1D * m_anaHist;

	double m_value;
	double m_wgt;

	int	m_start;
	int m_anabin;
};

#endif