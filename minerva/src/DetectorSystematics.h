#ifndef __DETECTORSYSTEMATICS_H__
#define __DETECTORSYSTEMATICS_H__

#include <SystematicsBase.h>

class DetectorSystematics : public SystematicsBase
{
public:
	DetectorSystematics(std::string in_filename, std::string in_treename, std::string out_filename, bool verbose);
	DetectorSystematics();
	DetectorSystematics(int n_universes, bool verbose);
	DetectorSystematics(bool verbose);
	~DetectorSystematics();

	void Run();
	void AddSample(std::string name, Int_t nbins, Double_t x_low, Double_t x_high);
	void AddSample(std::string name, Int_t nbins, Double_t * x_bins);

	// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
	bool AddLatErrorBand( const std::string& name);
	bool AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base);
	bool AddLatErrorBandAndFillWithCV( const std::string& name);
	bool AddVertErrorBand( const std::string& name);
	bool AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base);
	bool AddVertErrorBandAndFillWithCV( const std::string& name);

	bool AddUncorrError( const std::string& name );
	bool AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent = false );
	bool AddUncorrErrorAndFillWithCV( const std::string& name );

	bool AddMissingErrorBandsAndFillWithCV( const MnvH1D& ref );
	bool AddMissingErrorBandsAndFillWithCV( const MnvH2D& ref );
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------


};

#endif