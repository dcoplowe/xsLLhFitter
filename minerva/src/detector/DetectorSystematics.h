#ifndef __DETECTORSYSTEMATICS_H__
#define __DETECTORSYSTEMATICS_H__

#include <SystematicsBase.h>
#include <TMatrixD.h>
#include <vector>

class TH1D;
class ErrorType;

#include <PlotUtils/MnvH1D.h> 
#include <PlotUtils/MnvH2D.h>
using namespace PlotUtils;

class DetectorSystematics : public SystematicsBase
{
public:
	DetectorSystematics(int n_universes, bool verbose = true);
	DetectorSystematics(bool verbose = true);
	~DetectorSystematics();

	// void Run();
	// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
	bool AddLatErrorBand(const std::string& name, const int n_universes, const std::string &fill_samples = "");
	bool AddLatErrorBandAndFillWithCV(const std::string& name, const int n_universes, const std::string &fill_samples = "");
	bool AddVertErrorBand(const std::string& name, const int n_universes, const std::string &fill_samples = "");
	bool AddVertErrorBandAndFillWithCV(const std::string& name, const int n_universes, const std::string &fill_samples = "");
	bool AddUncorrError(const std::string& name, const std::string &fill_samples = "");
	bool AddUncorrErrorAndFillWithCV(const std::string& name, const std::string &fill_samples = "");

	void BuildAnaHist(const bool includeStat = true);
	MnvH1D * GetAnaHist() const { return m_anaHist; }
	TMatrixD GetCovMatrix(const bool includeStat = false, const bool asFrac = true, const bool cov_area_normalize = false);

	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------

private:
	int m_counter;
	std::vector<ErrorType*> m_errors;
	bool IsUniqueError(const std::string &name);
	ErrorType * FindError(const std::string &name);

	void Prepare();
	bool m_anaHist_set;
	// This is shit, should try and move away from mutables:
	TH1D * m_HanaHist;
	MnvH1D * m_anaHist;	

	// static int FillSampleNo;
	static const std::string m_ver_name;
	static const std::string m_lat_name;
	static const std::string m_uncer_name;
};

#endif