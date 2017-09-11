#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <string>
#include <vector>
// #include <TROOT.h>

// class MnvH1D;
// class MnvH2D;
// class TH1D;

#include <PlotUtils/MnvH1D.h> 
#include <PlotUtils/MnvH2D.h>
using namespace PlotUtils;

class Sample
{
public:
	// Should simplify this and remove the model bit. 
	// Just make this more of a wrapper for the Minverva histo class.

	Sample(const std::string& name, const  int nbins, const double x_low, const double x_high);
	Sample(const std::string& name, const int nbins, const double * x_bins);
	~Sample();

	std::vector<MnvH1D*> GetMnvH1D(){ return m_1Dhists; }
	MnvH1D * GetMnvH1D(int n){ return m_1Dhists[n]; }
	int GetNHists(){ return (int)m_1Dhists.size(); }
	void Fill(const int fill_nhist, const double value, const double wgt);
	void Fill(const double value, const double wgt);

	// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
	bool AddLatErrorBand( const std::string& name, const int nhists = -1 );
	bool AddLatErrorBand(const int set_nhist, const std::string& name, const int nhists = -1 );

	bool AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base );
	bool AddLatErrorBand(const int set_nhist, const std::string& name, const std::vector<TH1D*>& base );

	bool AddLatErrorBandAndFillWithCV( const std::string& name, const int nhists );
	bool AddLatErrorBandAndFillWithCV(const int set_nhist, const std::string& name, const int nhists );
	
	bool AddVertErrorBand( const std::string& name, const int nhists = -1 );
	bool AddVertErrorBand(const int set_nhist, const std::string& name, const int nhists = -1 );

	bool AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base );
	bool AddVertErrorBand(const int set_nhist, const std::string& name, const std::vector<TH1D*>& base );

	bool AddVertErrorBandAndFillWithCV( const std::string& name, const int nhists );
	bool AddVertErrorBandAndFillWithCV(const int set_nhist, const std::string& name, const int nhists );

	bool AddUncorrError( const std::string& name );
	bool AddUncorrError(const int set_nhist, const std::string& name );

	bool AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent = false );
	bool AddUncorrError(const int set_nhist, const std::string& name, const TH1D* hist, bool errInContent = false );

	bool AddUncorrErrorAndFillWithCV( const std::string& name );
	bool AddUncorrErrorAndFillWithCV(const int set_nhist, const std::string& name );

	bool AddMissingErrorBandsAndFillWithCV( const MnvH1D& ref );
	bool AddMissingErrorBandsAndFillWithCV(const int set_nhist, const MnvH1D& ref );

	bool AddMissingErrorBandsAndFillWithCV( const MnvH2D& ref );
	bool AddMissingErrorBandsAndFillWithCV(const int set_nhist, const MnvH2D& ref );

	bool FillLatErrorBand(const int fill_nhist, const std::string& name, const double val, const std::vector<double>& shifts,
		const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand(const int fill_nhist, const std::string& name, const double val, const double * shifts, const double cvweight = 1.0,
		const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand(const int fill_nhist, const std::string& name, const double val, const double shiftDown, const double shiftUp,
		const double cvweight = 1.0, const bool fillcv = true );
	bool FillVertErrorBand(const int fill_nhist, const std::string& name, const double val, const std::vector<double>& weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand(const int fill_nhist, const std::string& name, const double val, const double * weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand(const int fill_nhist, const std::string& name, const double val, const double weightDown, const double weightUp,
		const double cvweight  = 1.0, double cvWeightFromMe = 1. );
	bool FillUncorrError(const int fill_nhist, const std::string& name, const double val, const double err, const double cvweight = 1.0 );
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------

private:
	int	m_Nhists;
	bool m_det;
	std::string m_name;
	std::vector<MnvH1D*> m_1Dhists;
};

#endif