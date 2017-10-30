#ifndef __CUTSANDCORRECTIONS__H__
#define __CUTSANDCORRECTIONS__H__

#include <string>
#include <PlotUtils/MnvNormalization.h>

#ifdef __ENABLE__CUTSTATS__
#include <CutStatistics.h>
#endif

using namespace PlotUtils;

class FileIO;

#ifdef __ENABLE__REWEIGHT__
class ReweightEvent;
#endif

class CutsandCorrections
{
public:
	CutsandCorrections();
	CutsandCorrections(bool applyWgts);
	~CutsandCorrections();

	bool PassedCuts(FileIO * fChain);

	#ifdef __ENABLE__REWEIGHT__
	void MakeCutTables(const std::string &file_name){ stats.MakeCutTables(file_name); }
	#endif

	static bool isDeltaRichSample(FileIO * fChain);
	// double GetMuonThetaCorrection();
	// double GetMINOSCorrection(const std::string &processing_name = "Eroica");

	static double GetMuonThetaCorrection(FileIO * fChain);

	// static double GetMINOSCorrection(FileIO * fChain, const std::string &processing_name = "Eroica");
	static double GetMINOSCorrection(FileIO * fChain);
	// static void SetParamsFromFile(const std::string &file);

	static void AddCutInfo(std::string &filename);

	static void PrintSetupInfo();
	
	static bool IsOpeningAngleSmallAndEnergyLow(const double E_g1, const double E_g2, const double pi0_cos_openingAngle);
	static bool IsEmuInRange(const double Energy);
	static bool IsEnuInRange(const double Energy);
	static bool IsInRange(const double Energy, const double min, const double max);
	static bool IsWInRange(const double W);

private:
	const bool m_applyWgts;
	#ifdef __ENABLE__REWEIGHT__
	ReweightEvent * m_RW;
	#endif
	
	// Counters (for cut statistics):
	#ifdef __ENABLE__CUTSTATS__
	CutStatistics stats;
	#endif

	static std::string GetPlaylist(const int run, const int type);
	static const std::string m_processing_name;
	static const std::string m_init_playlist;
	static PlotUtils::MnvNormalizer * m_normalizer;

	static const double m_Emu_min;
	static const double m_Emu_max;
	static const double m_Enu_min;
	static const double m_Enu_max;

	static const double m_max_muon_theta;

	static const double m_max_gamma_dist;

	static const bool m_applyPhotonDistance;
	static const double m_minPhotonDistance_1;
	static const double m_minPhotonDistance_2;

	static const double m_min_pi0_cos_openingAngle;
	static const double m_min_pi0_energy;

	static const double m_W_max;

	static const bool m_make_pi0M_cut;
	static const double m_min_Pi0_invMass;
	static const double m_max_Pi0_invMass;

	static const bool m_sideBand_LowInvMass;
	static const bool m_sideBand_HighInvMass;

	static const bool m_sideBand_PID;
	static const bool m_sideBand_Michel;

	static const bool m_applyProtonScore;
	static const double m_minProtonScore_LLR;

	static const bool m_new_signal_cuts;
	static const bool m_prP_threshold;
	static const double m_minPrMom;

};

#endif