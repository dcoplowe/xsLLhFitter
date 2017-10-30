#ifndef __KINEMATIC_CALCULATOR_H__
#define __KINEMATIC_CALCULATOR_H__

class TVector3;
class TLorentzVector;

#include <TROOT.h>

class KinCalc
{
public:
	KinCalc(){;}
	~KinCalc(){;}
	
	struct PionVars
	{
		double E1;
		double E2;
		double E1E2_x;
		double E1E2_y;
		double m2_x;
		double m2_y;

		double mom;
		double mom4[4];
		double E;
		double T;
		double g1mom[4];
		double g2mom[4];	
	};

	struct DeltaVars
	{
		double mass;
		double theta;
		double phi;
	};

	static PionVars GetPionVars(const double E1, const double E2, const double angle, const bool is_deg = true);
	static double Get1cosTh(const double angle, const bool is_deg = true);

	static double GetInvMassSq(const double E1, const double E2, const double angle, const bool is_deg = true);
	static double GetInvMass(const double E1, const double E2, const double angle, const bool is_deg = true);
	static double Pi0InvMass(const double E1, const double E2, const double cosTheta);

	static TVector3 GetGammaMom(const double Energy, const double dir[]);

	static PionVars GetPionKins(const double g1_dir[], const double g2_dir[], const double g1_E, const double g2_E);
			
	static double OpeningAngle(const double vec1[], const double vec2[], const bool in_deg = true);
    static double OpeningcTheta(const double vec1[], const double vec2[]);

    static double GetEnu(const double muon_E, const double pi0_E, const double extra_energy);
    static double GetQSq(const double Enu, const double muon_E, const double muon_P, const double muon_angle);
    //m_Enu = Calc_Enu_bool improvedCal(); // Test results are not good! -- Use Calc_Enu(bool improved)
    static double GetWSq(const double Enu, const double QSq, const double muon_E);

    static double GetParticleE(const double mom, const double mass);

    static DeltaVars GetDeltaVars(const double Enu, const double pr_4mom[], const double pi_4mom[], const double mu_4mom[]);
    static TLorentzVector GetNeutrino4P(const double Enu);

private:
	static double GetGammaE(const double Energy, const double angle, const bool is_deg);
};

#endif