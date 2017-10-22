#ifndef __PARTICLEINFO__H__
#define __PARTICLEINFO__H__

#include <TVector3.h>

class ParticleInfo{
public:
	ParticleInfo(const double px, const double py, const double pz);
	~ParticleInfo();

	void AddX0(const double x0, const double y0, const double z0);
	void AddX1(const double x1, const double y1, const double z1);
	void AddPTraj(const double traj_x0, const double traj_y0, const double traj_z0);
	bool IsContained() const{ return m_contained; }

	double Mom() const { return m_mom3.Mag(); }
	double PathLength() const { return m_path_length; }//.Mag(); }

	static int CountFSParticles(const int pdg, const double P_min, const int nFSPart, const int FSPartPDG[], const double FSPartPx[],
	const double FSPartPy[], const double FSPartPz[]);

	static int GetIndex(const int pdg, const int detmc_ntrajectory2, const int detmc_traj_mother[],
		const int detmc_traj_pdg[], const double detmc_traj_E0[]);

	bool isNeutronInelastic(const int ind, const int ntraj, const int traj_mother[], const int traj_id[], const int traj_proc[],
	const double traj_x0[], const double traj_y0[], const double traj_z0[], const double traj_xf[], const double traj_yf[], const double traj_zf[]);

	static const double NeutronMass = 939.565;
	static const double PionMass = 134.9766;

private:
	TVector3 m_mom3;
	TVector3 m_start;
	TVector3 m_finish;
	TVector3 m_traj;

	void GetUncontainedPathLength();
	double m_path_length;

	bool m_have_start;
	bool m_have_finish;
	bool m_have_traj;
	bool m_contained;

	bool isPointContained(double x, double y, double z);
	bool inside_hexagon(double x, double y, double apothem);
};

#endif

#ifndef __DETERROR__H__
#define __DETERROR__H__

#include <vector>
#include <string>


#include <PlotUtils/MnvNormalization.h>

using namespace PlotUtils;

class FileIO;

class DetError
{
public:
	DetError();
	DetError(FileIO * fChain);
	~DetError();

	struct Default
	{
		std::vector<double> michel;
		std::vector<double> michel_true;
		std::vector<double> michel_false;
		std::vector<double> mu_trking;
		std::vector<double> neutron_res;
		std::vector<double> pi_res;
		std::vector<double> pr_trking;
		
		void clear(){
			michel.clear();
			michel_true.clear();
			michel_false.clear();
			mu_trking.clear();
			neutron_res.clear();
			pi_res.clear();
			pr_trking.clear();
		}
	};

	Default GetDefaults();

	// Vertical Errors:
	static std::vector<double> GetMINOSCorrectionErr(const double minos_trk_p, const int run, const int type);

	static std::vector<double> GetPionResponseErr(const bool has_charge_exchanged);

	static std::vector<double> GetNeutronResponseErr(const int nFSPart, const int FSPartPDG[], 
		const double FSPartPx[], const double FSPartPy[], const double FSPartPz[], const int ntraj,
		const int traj_pdg[], const int traj_mother[], const int traj_id[], const int traj_proc[],
		const double traj_px0[], const double traj_py0[], const double traj_pz0[], const double traj_E0[],
		const double traj_x0[], const double traj_y0[], const double traj_z0[],
		const double traj_xf[],const double traj_yf[],const double traj_zf[]);

	static double GetMichelErr(const bool truth_isBckg_withMichel);
	static double GetMichelErrTrue(const bool truth_isBckg_withMichel);
	static double GetMichelErrFalse(const bool truth_isBckg_withMichel);

	static std::vector<double> GetFractionalError(const double var, const std::vector<double> &err_vec);

	// Lateral Errors:
	static std::vector<double> GetProtonTrackingErr(double track_length);
	static std::vector<double> GetProtonShifts(const double shifts_up[10], const double shifts_down[10], const int nProtons, bool is_single = true);
	static std::vector<double> GetShifts(const double nominal, const std::vector<double> &shifts);

	static std::vector<double> GenerateShifts(double sigma);
	
	static std::vector<double> GenerateEnergyShifts(const double mc_1sigma = 0.013, const double data_1sigma = 0.019);
	static std::vector<double> GenerateMuonThetashifts(const double muonThetaX_Err = 0.001, const double muonThetaY_Err = 0.0009);

	static std::vector<double> GetEnergyShifts(){ return m_Eshifts; }

	static std::vector<double> GetRelEnergyShifts(const double mom, const double mass = ParticleInfo::PionMass);

	static std::vector<double> GetRelEShifts(const double var);

	static std::vector<double> GetLinearEnergyShifts(const double var);
	
	static std::vector<double> GetMuonThetaShifts(){ return m_MuTshifts; }

	static std::vector<double> GetMuonPShifts(const double muon_E_shift, const double muon_shift);

	static void GetCorrectedMuon3Mom(double &px, double &py, double &pz, const int n_theta_nodes,
		const double theta_nodes[], bool wrtbeam = false);

	static double GetCorrectedMuonTheta(const int n_theta_nodes, const double theta_nodes[], bool wrtbeam = false);

    // FillVertErrorBand_MuonTracking_ByHand(hist, var); CCProtonPi0_minos_trk_p = minos_trk_p
	static void PrintEnergyShifts();
	// Kinematic specific variatiions:
	// dEnu = dEpi * E_pi  (where dEpi = fractional error on pi)
	// dQ2 = (Q2 + m^2)dEnu (where dEnu = fractional error)
	// dW2 = (2m_nEnu + Q2 - m^2)dEnu (where dEnu = fractional error)

	// // Analysis specific:
	// static std::vector<double> GetEnuShifts(const double E_pi, const std::vector<double> &pi_frac_er);
	// static std::vector<double> GetQ2Shifts(const double Q2plusM2mu, const std::vector<double> &Enu_frac_er);
	// static std::vector<double> GetW2Shifts(const double Mn2Enu_Q2_M2mu, const std::vector<double> &Enu_frac_er);


private:
	FileIO * m_chain;
	Default m_def;

	static std::string GetPlaylist(const int run, const int type);

	static const std::string m_processing_name;
	static const std::string m_init_playlist;
	static MnvNormalizer * m_normalizer;

	static std::vector<double> GetErrorVec(const double variation);

	static double Calc_f(double neutron_KE);

	static void FillProtonEnergyShiftVector(std::vector<double> &energy_shifts, const int nProtons, const double shifts[10]);

	// Build Lateral Error Universes:
	static const int m_nToys;
	static const std::vector<double> m_nominal;
	static std::vector<double> MakeNormalShiftedUniverses();

	static const std::vector<double> m_Eshifts;
	static const std::vector<double> m_MuTshifts;
	static const std::vector<double> m_Emptyshifts;
	static std::vector<double> MakeEmptyShifts();

	static std::vector<double> m_relEshifts;
	static bool m_rel_warn;

	// Empty error:
	static const std::vector<double> m_noError;

	// Make the constant shifts once:
	static const std::vector<double> m_michel_true;
	static const std::vector<double> m_michel_false;

	// Proton uncetainty:
	const std::vector<double> m_prEshort;
	const std::vector<double> m_prElong;
};

#endif