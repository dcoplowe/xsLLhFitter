#ifndef __CUTSANDCORRECTIONS__H__
#define __CUTSANDCORRECTIONS__H__

#include <string>
#include <PlotUtils/MnvNormalization.h>

using namespace PlotUtils;

class FileIO;

#ifdef __REWEIGHT__
class ReweightEvent;
#endif

class CutsandCorrections
{
public:
	CutsandCorrections();
	CutsandCorrections(bool applyWgts);
	~CutsandCorrections();

	bool PassedCuts(FileIO * fChain);
	static bool isDeltaRichSample(FileIO * fChain);
	// double GetMuonThetaCorrection();
	// double GetMINOSCorrection(const std::string &processing_name = "Eroica");

	static double GetMuonThetaCorrection(FileIO * fChain);

	// static double GetMINOSCorrection(FileIO * fChain, const std::string &processing_name = "Eroica");
	static double GetMINOSCorrection(FileIO * fChain);

	static void SetParamsFromFile(const std::string &file);

	static void AddCutInfo(std::string &filename);

	static void PrintSetupInfo();

	static bool IsOpeningAngleSmallAndEnergyLow(const double E_g1, const double E_g2, const double pi0_cos_openingAngle);
	static bool IsEmuInRange(const double Energy);
	static bool IsEnuInRange(const double Energy);
	static bool IsInRange(const double Energy, const double min, const double max);
	static bool IsWInRange(const double W);

	static bool CheckCaloCuts(const double E_g1, const double E_g2, const double pi0_cos_openingAngle, const double EnuEmu, const double W);


private:
	const bool m_applyWgts;
	#ifdef __REWEIGHT__
	ReweightEvent * m_RW;
	#endif

	static std::string GetPlaylist(const int run, const int type);
	static const std::string m_processing_name;
	static const std::string m_init_playlist;
	static PlotUtils::MnvNormalizer * m_normalizer;

	static double m_Emu_min;
	static double m_Emu_max;
	static double m_Enu_min;
	static double m_Enu_max;

	static double m_max_muon_theta;

	static double m_max_gamma_dist;

	static bool m_applyPhotonDistance;
	static double m_minPhotonDistance_1;
	static double m_minPhotonDistance_2;

	static double m_min_pi0_cos_openingAngle;
	static double m_min_pi0_energy;

	static double m_W_max;

	static bool m_make_pi0M_cut;
	static double m_min_Pi0_invMass;
	static double m_max_Pi0_invMass;

	static bool m_sideBand_LowInvMass;
	static bool m_sideBand_HighInvMass;

	static bool m_sideBand_PID;
	static bool m_sideBand_Michel;

	static bool m_applyProtonScore;
	static double m_minProtonScore_LLR;

	static bool m_new_signal_cuts;
};

#endif