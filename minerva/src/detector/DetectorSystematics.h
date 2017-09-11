#ifndef __DETECTORSYSTEMATICS_H__
#define __DETECTORSYSTEMATICS_H__

#include <SystematicsBase.h>

class TH1D;
class MnvH1D;
class MnvH2D;

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
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
	
};

#endif