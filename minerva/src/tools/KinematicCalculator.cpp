#ifndef __KINEMATIC_CALCULATOR_CPP__
#define __KINEMATIC_CALCULATOR_CPP__
	
#include <KinematicCalculator.h>
#include <TMath.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <DataInfo.h>

// #include <iostream>
// using std::cout;
// using std::endl;

KinCalc::PionVars KinCalc::GetPionVars(const double E1, const double E2, const double angle, const bool is_deg)
{
	KinCalc::PionVars vars;

	vars.E1 = GetGammaE(E2, angle, is_deg);
	vars.E2 = GetGammaE(E1, angle, is_deg);
	vars.E1E2_x = DataInfo::Mass::Pi0Sq/Get1cosTh(angle, is_deg);
	vars.E1E2_y = 2*E1*E2;
	vars.m2_x = Get1cosTh(angle, is_deg);
	vars.m2_y = DataInfo::Mass::Pi0Sq/(2*E1*E2);

	return vars;
}

double KinCalc::GetGammaE(const double Energy, const double angle, const bool is_deg)
{
	double den = 2*Energy*Get1cosTh(angle, is_deg);
	return (DataInfo::Mass::Pi0Sq/den);
}

double KinCalc::Get1cosTh(const double angle, const bool is_deg)
{
	// if(is_deg) cout << "IS DEG" << endl;
	double tmp_angle = angle;
	if(is_deg) tmp_angle *= TMath::DegToRad();
	return (1. - TMath::Cos( tmp_angle ));
}

double KinCalc::OpeningAngle(const double vec1[], const double vec2[], const bool in_deg)
{
	double tmp_angle = TMath::ACos( OpeningcTheta(vec1, vec2) );
	if(in_deg) tmp_angle *= TMath::RadToDeg();
	return tmp_angle;
}

double KinCalc::OpeningcTheta(const double vec1[], const double vec2[])
{
	TVector3 vecA(vec1[0], vec1[1], vec1[2]);
	TVector3 vecB(vec2[0], vec2[1], vec2[2]);

	vecA *= 1/vecA.Mag();
	vecB *= 1/vecB.Mag();

	return vecA.Dot(vecB);
}

KinCalc::PionVars KinCalc::GetPionKins(const double g1_dir[], const double g2_dir[], const double g1_E, const double g2_E)
{
	TVector3 g1_mom = GetGammaMom(g1_E, g1_dir);
	TVector3 g2_mom = GetGammaMom(g2_E, g2_dir);
	TVector3 mom = g1_mom + g2_mom;

	PionVars var;
	var.mom = mom.Mag();
	var.E = TMath::Sqrt(mom.Mag2() + DataInfo::Mass::Pi0Sq);;
	var.T = var.E - DataInfo::Mass::Pi0;

	var.mom4[0] = mom.X();
	var.mom4[1] = mom.Y();
	var.mom4[2] = mom.Z();
	var.mom4[3] = var.E;

	var.g1mom[0] = g1_mom.X();
	var.g1mom[1] = g1_mom.Y();
	var.g1mom[2] = g1_mom.Z();
	var.g1mom[3] = g1_E;
	
	var.g2mom[0] = g2_mom.X();
	var.g2mom[1] = g2_mom.Y();
	var.g2mom[2] = g2_mom.Z();
	var.g2mom[3] = g2_E;

	return var;
}

TVector3 KinCalc::GetGammaMom(const double Energy, const double dir[])
{
	TVector3 mom(dir[0], dir[1], dir[2]);
	mom *= 1/mom.Mag();
	mom *= Energy;
	return mom;
}

double KinCalc::GetInvMassSq(const double E1, const double E2, const double angle, const bool is_deg)
{
	return ( 2*E1*E2*Get1cosTh(angle, is_deg) );
}

double KinCalc::Pi0InvMass(const double E1, const double E2, const double cosTheta)
{
    return TMath::Sqrt( 2.*E1*E2*(1. - cosTheta) );
}

double KinCalc::GetInvMass(const double E1, const double E2, const double angle, const bool is_deg)
{
	return TMath::Sqrt( GetInvMassSq(E1, E2, angle, is_deg) );	
}

double KinCalc::GetEnu(const double muon_E, const double pi0_E, const double extra_energy)
{
	return (muon_E + pi0_E + extra_energy);
}

double KinCalc::GetQSq(const double Enu, const double muon_E, const double muon_P, const double muon_angle)
{
	return (2*Enu*(muon_E - muon_P*cos(muon_angle))- DataInfo::Mass::MuonSq);
}

    //m_Enu = Calc_Enu_bool improvedCal(); // Test results are not good! -- Use Calc_Enu(bool improved)
double KinCalc::GetWSq(const double Enu, const double QSq, const double muon_E)
{
	return (DataInfo::Mass::NeutronSq + 2*DataInfo::Mass::Neutron*(Enu - muon_E) - QSq); 
}

KinCalc::DeltaVars KinCalc::GetDeltaVars( const double Enu, const double pr_4mom[], const double pi_4mom[], const double mu_4mom[])
{	
    DeltaVars vars;
	// Mass:
    double delta4P[4];
    for(int i = 0; i < 4; i++){
    	delta4P[i] = pr_4mom[i] + pi_4mom[i];
    }
    TLorentzVector delta_4P(delta4P);
    vars.mass = delta_4P.M();

    // --------------------------------------------------------------------
    // Find Boost from LAB Frame to Delta Rest Frame
    // --------------------------------------------------------------------
	double gamma = delta_4P.Gamma();
    double boost_x = -(delta_4P.Px() / (gamma*delta_4P.M()) );
    double boost_y = -(delta_4P.Py() / (gamma*delta_4P.M()) );
    double boost_z = -(delta_4P.Pz() / (gamma*delta_4P.M()) );

    delta_4P.Boost(boost_x,boost_y,boost_z);

    // --------------------------------------------------------------------
    // Boost All 4-Momentums to Delta Rest Frame
    // --------------------------------------------------------------------
    TLorentzVector beam_4P = GetNeutrino4P(Enu);
    TLorentzVector pi0_4P(pi_4mom);    
    TLorentzVector muon_4P(mu_4mom); 

    beam_4P.Boost(boost_x,boost_y,boost_z);
    muon_4P.Boost(boost_x,boost_y,boost_z);
    pi0_4P.Boost(boost_x,boost_y,boost_z);

     // --------------------------------------------------------------------
    // Form Axes
    // --------------------------------------------------------------------
    TVector3 beam_3P = beam_4P.Vect();
    TVector3 muon_3P = muon_4P.Vect();

    // Z Axis -- Momentum Transfer Axis
    TVector3 axis_z(beam_3P.Px() - muon_3P.Px(), beam_3P.Py() - muon_3P.Py(), beam_3P.Pz() - muon_3P.Pz());
    TVector3 unit_axis_z = axis_z.Unit();

    // Y Axis -- Beam x Muon
    TVector3 axis_y = beam_3P.Cross(muon_3P);
    TVector3 unit_axis_y = axis_y.Unit();

    // X Axis -- Right Handed Coordinate System y_axis x z_axis = x_axis
    TVector3 axis_x = axis_y.Cross(axis_z);
    TVector3 unit_axis_x = axis_x.Unit();

    // --------------------------------------------------------------------
    // Calculate Angles
    // --------------------------------------------------------------------
    TVector3 pi0_3P = pi0_4P.Vect();
    TVector3 unit_pi0_3P = pi0_3P.Unit();
    double pion_P = pi0_3P.Mag();

    // Theta:
    double cos_theta_z = unit_pi0_3P.Dot(unit_axis_z);

    double theta_x = acos(unit_pi0_3P.Dot(unit_axis_x)); 
    double theta_y = acos(unit_pi0_3P.Dot(unit_axis_y)); 
    double theta_z = acos(cos_theta_z); 

    double Px = pion_P*cos(theta_x);
    double Py = pion_P*cos(theta_y);

    // Phi:
    double phi_x = acos(Px/(pion_P*sin(theta_z)));
    double phi = phi_x * TMath::RadToDeg();    

    // Convert to 360
    if (Py < 0 ){
        phi = 360 - phi;
    }

    vars.theta = cos_theta_z;
    vars.phi = phi;
	return vars;
}

TLorentzVector KinCalc::GetNeutrino4P(const double Enu)
{
    const double theta = -0.05887;
    double Px = 0.0;
    double Py = Enu*sin(theta);
    double Pz = Enu*cos(theta); 
    TLorentzVector beam_4P(Px,Py,Pz,Enu);
    return beam_4P;
}

double KinCalc::GetParticleE(const double mom, const double mass)
{
    return TMath::Sqrt(mom*mom + mass*mass);
}
  









#endif