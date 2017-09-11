#ifndef __DETECTORSYSTEMATICS_H__
#define __DETECTORSYSTEMATICS_H__

#include <SystematicsBase.h>

class TH1D;

#include <PlotUtils/MnvH1D.h> 
#include <PlotUtils/MnvH2D.h>
using namespace PlotUtils;

class DetectorSystematics : public SystematicsBase
{
public:
	DetectorSystematics(int n_universes, bool verbose = true);
	DetectorSystematics(bool verbose = true);
	~DetectorSystematics();

	void Run();

	// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
	bool AddLatErrorBand(const std::string& name);
	bool AddLatErrorBand(const std::string& name, const std::vector<TH1D*>& base);
	bool AddLatErrorBandAndFillWithCV(const std::string& name);
	bool AddVertErrorBand(const std::string& name);
	bool AddVertErrorBand(const std::string& name, const std::vector<TH1D*>& base);
	bool AddVertErrorBandAndFillWithCV(const std::string& name);

	bool AddUncorrError(const std::string& name );
	bool AddUncorrError(const std::string& name, const TH1D* hist, bool errInContent = false );
	bool AddUncorrErrorAndFillWithCV(const std::string& name );

	bool AddMissingErrorBandsAndFillWithCV(const MnvH1D& ref );
	bool AddMissingErrorBandsAndFillWithCV(const MnvH2D& ref );

	bool FillLatErrorBand(const std::string& name, const double val, const std::vector<double>& shifts,
		const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand(const std::string& name, const double val, const double * shifts, const double cvweight = 1.0,
		const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand(const std::string& name, const double val, const double shiftDown, const double shiftUp,
		const double cvweight = 1.0, const bool fillcv = true );
	bool FillVertErrorBand(const std::string& name, const double val, const std::vector<double>& weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand(const std::string& name, const double val, const double * weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand(const std::string& name, const double val, const double weightDown, const double weightUp,
		const double cvweight  = 1.0, double cvWeightFromMe = 1. );
	bool FillUncorrError(const std::string& name, const double val, const double err, const double cvweight = 1.0 );
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
	
};

#endif