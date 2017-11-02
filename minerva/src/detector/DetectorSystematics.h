#ifndef __DETECTORSYSTEMATICS_H__
#define __DETECTORSYSTEMATICS_H__

#include <SystematicsBase.h>
#include <TMatrixD.h>
#include <vector>
#include <DetError.h>
#include <TCanvas.h>

class TH1D;
class ErrorType;

#include <PlotUtils/MnvH1D.h> 
#include <PlotUtils/MnvH2D.h>
using namespace PlotUtils;

#ifndef kIniValue
#define kIniValue -999
#endif

class DetectorSystematics : public SystematicsBase
{
public:
	DetectorSystematics(int n_universes, bool verbose = true);
	DetectorSystematics(bool verbose = true);
	~DetectorSystematics();

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

	bool AddDefaults();

	void MakeBinning(const std::string &out_name = "");

	bool FillDefaults(const std::string& sam_name, const double value, const DetError::Default elist,
		const double cvweight  = 1.0, double cvWeightFromMe = 1);

	TCanvas * DrawErrors(bool asFrac=false, bool cov_area_normalize=false);
	std::vector<TCanvas*> DrawErrorsBySample(bool asFrac=false, bool cov_area_normalize=false);

	static void SliceNorm(TMatrixD &cov);
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------

	// Things required for calculating the error shifts:
	static std::string GetPlaylist(const int run, const int type = kIniValue);

private:
	int m_counter;
	std::vector<ErrorType*> m_errors;
	bool IsUniqueError(const std::string &name);
	ErrorType * FindError(const std::string &name);

    static TH2D * NormalHist(TH2D * hraw, double thres = 0, bool kmax=true);
    static TH1D * ToPDF(TH1D * hraw, std::string hn = "");

	void Prepare();
	bool m_anaHist_set;
	// This is shit, should try and move away from mutables:
	TH1D * m_HanaHist;
	MnvH1D * m_anaHist;	

	// static int FillSampleNo;
	static const std::string m_ver_name;
	static const std::string m_lat_name;
	static const std::string m_uncer_name;

	TCanvas * PlotErrorBase(MnvH1D * fHisto, bool asFrac, bool cov_area_normalize);
};

#endif