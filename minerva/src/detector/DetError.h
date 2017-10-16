#ifndef __DETERROR__H__
#define __DETERROR__H__

#include <vector>

class DetError
{
public:
	DetError();
	~DetError();
	
	static std::vector<double> GetProtonTrackingErr(double track_length);
	static std::vector<double> GetProtonShifts(const double shifts_up[10], const double shifts_down[10], const int nProtons, bool is_single = true);
	static std::vector<double> GetShifts(const double nominal, const std::vector<double> &shifts);

	static std::vector<double> GenerateShifts(double sigma);
	
	static std::vector<double> GenerateEnergyShifts(const double mc_1sigma = 0.013, const double data_1sigma = 0.019);
	static std::vector<double> GenerateMuonThetashifts(const double muonThetaX_Err = 0.001, const double muonThetaY_Err = 0.0009);

	static std::vector<double> GetEnergyShifts(){ return m_Eshifts; }
	static std::vector<double> GetMuonThetaShifts(){ return m_MuTshifts; }

    // FillVertErrorBand_MuonTracking_ByHand(hist, var); CCProtonPi0_minos_trk_p = minos_trk_p
	static std::vector<double> GetMINOSCorrectionErr(const double minos_trk_p, const int run, const int type,
		const std::string &processing_name = "Eroica");

	static std::vector<double> GetPionResponseErr(const bool has_charge_exchanged);

	static std::vector<double> GetNeutronResponseErr(const int nFSPart, const int FSPartPDG[], const double FSPartPx[],
	const double FSPartPy[], const double FSPartPz[], const double x0[], const double y0[], const double z0[],
	const double xf[],const double yf[],const double zf[], const double traj_px0[], const double traj_py0[],
	const double traj_pz0[], const int ntraj, const int traj_mother[], const int traj_id[], const int traj_proc[], const double traj_E0[]);

private:
	static std::string GetPlaylist(const int run, const int type);
	static std::vector<double> GetErrorVec(const double variation);

	static double Calc_f(double neutron_KE);

	static void FillProtonEnergyShiftVector(std::vector<double> &energy_shifts, const int nProtons, const double shifts[10]);

	// Build Lateral Error Universes:
	static const int m_nToys;
	static const std::vector<double> m_nominal;
	static std::vector<double> MakeNormalShiftedUniverses();

	static const std::vector<double> m_Eshifts;
	static const std::vector<double> m_MuTshifts;

    // FillVertErrorBand_PionResponse_ByHand(hist, var);
    // FillVertErrorBand_NeutronResponse_ByHand(hist, var);
};

#endif

#ifndef __PARTICLEINFO__H__
#define __PARTICLEINFO__H__

#include <TVector3.h>

class ParticleInfo{
public:
	ParticleInfo(const double px, const double py, const double pz);
	~ParticleInfo(){;}

	void AddX0(const double x0, const double y0, const double z0);
	void AddX1(const double x1, const double y1, const double z1);
	void AddPTraj(const double traj_x0, const double traj_y0, const double traj_z0);
	bool IsContained() const{ return m_isContained; }

	double Mom() const { return m_mom3.Mag(); }
	double PathLength() const { return m_path_length; }//.Mag(); }

	static int CountFSParticles(const int pdg, const double P_min, const int nFSPart, const int FSPartPDG[], const double FSPartPx[],
	const double FSPartPy[], const double FSPartPz[], int &index);

private:
	TVector3 m_mom3;
	TVector3 m_start;
	TVector3 m_finish;
	TVector3 m_traj;
	double m_path_length;

	bool m_have_start;
	bool m_have_finish;
	bool m_have_traj;
	bool m_isContained;

	bool isPointContained(double x, double y, double z);
	bool inside_hexagon(double x, double y, double apothem);
};

#endif
