#include <DetError.h>

#ifndef __PARTICLEINFO__CPP__
#define __PARTICLEINFO__CPP__

#ifndef kIniValue 
#define kIniValue -999
#endif

#include <TMath.h>
#include <iostream>
using std::cout;
using std::endl;

ParticleInfo::ParticleInfo(const double px, const double py, const double pz) : m_mom3(px, py, pz), m_start(0., 0., 0.),
	m_finish(0., 0., 0.), m_traj(0., 0., 0.), m_path_length(0.),
	m_have_start(false), m_have_finish(false),  m_have_traj(false), m_contained(false)
{
}

ParticleInfo::~ParticleInfo()
{
}

void ParticleInfo::AddX0(const double x0, const double y0, const double z0)
{
	m_start.SetXYZ(x0, y0, z0);
	m_have_start = true;

	if(m_have_start && m_have_finish){
		if(m_contained)  m_path_length = (m_finish - m_start).Mag();
		else GetUncontainedPathLength();
	}
}

void ParticleInfo::AddX1(const double x1, const double y1, const double z1)
{
	m_have_finish = true;
	m_finish.SetXYZ(x1, y1, z1);
	m_contained = isPointContained(x1, y1, z1);

	if(m_have_start && m_have_finish){
		if(m_contained) m_path_length = (m_finish - m_start).Mag();
		else GetUncontainedPathLength();
	}
}

void ParticleInfo::AddPTraj(const double traj_x0, const double traj_y0, const double traj_z0)
{
	m_have_traj = true;
	m_traj.SetXYZ(traj_x0, traj_y0, traj_z0);

	if(m_have_start && m_have_finish){
		if(m_contained)  m_path_length = (m_finish - m_start).Mag();
		else GetUncontainedPathLength();
	}
}

bool ParticleInfo::isPointContained(double x, double y, double z)
{
	if ((5430.0 < z && z < 10000.0) && inside_hexagon(x,y,1100.0)) return true;
	return false;
}

bool ParticleInfo::inside_hexagon(double x, double y, double apothem)
{
	double max_bound =  apothem;
	double min_bound = -apothem;

	double delta_phi = 60.0 * TMath::DegToRad();
	double phi = 0.0;
	for (int i = 0; i < 3; ++i) {
		double xtmp = cos(phi)*x + sin(phi)*y;
		phi += delta_phi;
		if (xtmp < min_bound || xtmp > max_bound) return false;
	}
	return true;
}

void ParticleInfo::GetUncontainedPathLength()
{
	double length = 0.0;
	const double step_size = 50.0; 
	for (;; length += step_size) {
		double new_x = m_traj.X() + length * m_traj.Unit().X();
		double new_y = m_traj.Y() + length * m_traj.Unit().Y();
		double new_z = m_traj.Z() + length * m_traj.Unit().Z();
		if (!isPointContained(new_x,new_y,new_z)) break;
	}
	m_path_length = length;
}

bool ParticleInfo::isNeutronInelastic(const int ind, const int ntraj, const int traj_mother[], const int traj_id[], const int traj_proc[],
	const double traj_x0[], const double traj_y0[], const double traj_z0[], const double traj_xf[], const double traj_yf[], const double traj_zf[])
{
    for (int i = 0; i < ntraj; ++i) {
        int parent = traj_mother[i];
        if (parent == 0) continue;

        if (parent != traj_id[ind]) continue;

        int proc = traj_proc[i];

        TVector3 one(traj_x0[i], traj_y0[i], traj_z0[i]);
        TVector3 two(traj_xf[i], traj_yf[i], traj_zf[i]);
        double d = (two - one).Mag();
        if (d < 1.e-3 && proc == 1) return true;
    }
    return false;
}

int ParticleInfo::CountFSParticles(const int pdg, const double P_min, const int nFSPart, const int FSPartPDG[], const double FSPartPx[],
	const double FSPartPy[], const double FSPartPz[], int &index)
{
	index = kIniValue;
    int count = 0;
    double mom_max = 0.;
    for (int i = 0; i < nFSPart; i++){
        if (FSPartPDG[i] == pdg){ 
        	TVector3 mom(FSPartPx[i], FSPartPy[i], FSPartPz[i]);
            if (mom.Mag() > P_min){ 
            	count++;
            	if(mom.Mag() > mom_max){
            		mom_max = mom.Mag();
            		index = i;
            	}
            }
        }
    }
    return count;
}

#endif

#ifndef __DETERROR__CPP__
#define __DETERROR__CPP__

#include <PlotUtils/MnvNormalization.h>
#include <TVector3.h>
#include <TRandom3.h>

#include <iostream>
using std::cout;
using std::endl;

using namespace PlotUtils;

#ifndef kIniValue 
#define kIniValue -999
#endif

#ifndef kEpsilon 
#define kEpsilon 1e-12
#endif

const int DetError::m_nToys = 500;
const std::vector<double> DetError::m_nominal = DetError::MakeNormalShiftedUniverses();
const std::vector<double> DetError::m_Eshifts = DetError::GenerateEnergyShifts();
const std::vector<double> DetError::m_MuTshifts = DetError::GenerateMuonThetashifts();
const std::vector<double> DetError::m_Emptyshifts = DetError::MakeEmptyShifts();
std::vector<double> DetError::m_relEshifts = DetError::m_Emptyshifts;//DetError::MakeEmptyShifts();
bool DetError::m_rel_warn = true;

// std::vector<double> em_energy_random_shifts;
// std::vector<double> muonP_random_shifts;
// std::vector<double> muon_theta_random_shifts;
// std::vector< std::vector<double> > Birks_random_shifts2D;

DetError::DetError()
{

}

DetError::~DetError()
{

}

std::vector<double> DetError::GetFractionalError(const double var, const std::vector<double> &err_vec)
{
	std::vector<double> fracE;
	for(size_t i = 0; err_vec.size(); i++){
		double tmp = err_vec[i]/var;
		fracE.push_back( tmp );
	}
	return fracE;
}

std::vector<double> DetError::GetRelEnergyShifts(const double mom, const double mass)
{	
	std::vector<double> spread;
	m_relEshifts.clear();
	for(size_t i = 0; m_Eshifts.size(); i++){
		double deltaE = sqrt( (1 + m_Eshifts[i])*(1 + m_Eshifts[i])*mom*mom + mass*mass) - sqrt(mom*mom + mass*mass); 
		spread.push_back( deltaE );

		double fracE = deltaE/sqrt(mom*mom + mass*mass);
		m_relEshifts.push_back( fracE );
	}
	return spread;
}

std::vector<double> DetError::GetRelEShifts(const double var)
{
	if(m_rel_warn){
		m_rel_warn = false;
		cout << "DetError::GetRelEShifts(const double var) : Called : Warning ";
		cout << ": Must be called after GetRelEnergyShifts(const double mom, const double mass)" << endl;
	}

	std::vector<double> spread;
	for(size_t i = 0; m_relEshifts.size(); i++) 
		spread.push_back( var*m_relEshifts[i] );
	return spread;
}

std::vector<double> DetError::GetLinearEnergyShifts(const double var)
{	
	std::vector<double> spread;
	for(size_t i = 0; m_Eshifts.size(); i++) {
		double temp = var*m_Eshifts[i];
		cout << "m_Eshifts[" << i << "] = " << var << " * " << m_Eshifts[i] << " = " << temp << endl;
		if(temp < kEpsilon) temp = 0.;
		spread.push_back( temp );
	}
	return spread;
}

std::vector<double> DetError::GenerateShifts(double sigma)
{
    std::vector<double> random_shifts;
    for (unsigned int i = 0; i < m_nominal.size(); ++i){
        double temp = sigma * m_nominal[i];
		cout << "temp[" << i << "] = " << sigma << "*" << m_nominal[i] << " = " << temp << endl;
		if(temp < kEpsilon) temp = 0.;
        random_shifts.push_back(temp);
    }   
    return random_shifts;
}

std::vector<double> DetError::GetShifts(const double nominal, const std::vector<double> &shifts)
{
	std::vector<double> variations;
	for(size_t i = 0; i < shifts.size(); i++) 
		variations.push_back( shifts[i] - nominal );
	return variations;
}

std::vector<double> DetError::GetProtonTrackingErr(double track_length)
{
    double correction = (track_length < 8*17.0 ? 0.046 : 0.003); //Ref: TN048
    return GetErrorVec(correction);
}

std::vector<double> DetError::GetProtonShifts(const double shifts_up[10], const double shifts_down[10], const int nProtons, bool is_single)
{
	// This deals with: MassModel, MEU, BetheBloch
	// if !is_single: 0 < nProtons are down shifts
	//                nProtons < 2nProtons are up shifts 
    std::vector<double> energy_shifts;
	int n_protons = nProtons;
    if(is_single) n_protons = 1;
    FillProtonEnergyShiftVector(energy_shifts, n_protons, shifts_up);
    FillProtonEnergyShiftVector(energy_shifts, n_protons, shifts_down);
    return energy_shifts;
}

void DetError::FillProtonEnergyShiftVector(std::vector<double> &energy_shifts, const int nProtons, const double shifts[10])
{
    for (int i = 0; i < nProtons; ++i) energy_shifts.push_back(shifts[i]);
}

std::vector<double> DetError::GetMINOSCorrectionErr(const double CCProtonPi0_minos_trk_p, const int run, const int type, const std::string &processing_name)
{
	std::string playlist = GetPlaylist(run, type);
	if (playlist.compare("minerva_2p2h") == 0) playlist = "minerva13C";
	MnvNormalizer normalizer(processing_name, playlist);
	double correctionErr = normalizer.GetCorrectionErr(CCProtonPi0_minos_trk_p);
	return GetErrorVec(correctionErr);
}

std::vector<double> DetError::GetPionResponseErr(const bool charge_exchange)
{
	double correctionErr = 0.0;
    // truth_isBckg_SingleChargedPion_ChargeExchanged
	if (charge_exchange) correctionErr = 0.5;
	return GetErrorVec(correctionErr);
}

// For build the systematic variation:
std::string DetError::GetPlaylist(const int run, const int type)
{
    std::string playlist = "";
    (void)type;
    // if ( IsEvent2p2h(type) ) playlist = "minerva_2p2h";
    if (run >= 10200 && run <= 10249) playlist = "minerva1";
    else if (run >= 10250 && run <= 10254) playlist = "minerva7";
    else if (run >= 10255 && run <= 10259) playlist = "minerva9";
    else if (run >= 12200 && run <= 12209) playlist = "minerva13A";
    else if (run >= 12210 && run <= 12219) playlist = "minerva13B";
    else if (run >= 13200 && run <= 13299) playlist = "minerva13C";
    else if (run >= 14201 && run <= 14209) playlist = "minerva13D";
    else if (run >= 14210 && run <= 14229) playlist = "minerva13E";
    else{
    	cout << "ERROR: No Playlist Found for run " << run << " and type " << type << "." << endl; 
    	exit(0);
    }
    return playlist;
}

//  const int nFSPart, const int * FSPartPDG[], const double * FSPartPx[],
// 	const double * FSPartPy[], const double * FSPartPz[], const double * x0[], const double * y0[], const double * z0[],
// 	const double * xf[],const double * yf[],const double * zf[], const double * traj_px0[], const double * traj_py0[],
// 	const double * traj_pz0[], const int ntraj, const int * traj_mother[], const int * traj_id[], const int * traj_proc[], const double * traj_E0[]
std::vector<double> DetError::GetNeutronResponseErr(const int nFSPart, const int FSPartPDG[], const double FSPartPx[],
	const double FSPartPy[], const double FSPartPz[], const double x0[], const double y0[], const double z0[],
	const double xf[],const double yf[],const double zf[], const double traj_px0[], const double traj_py0[],
	const double traj_pz0[], const int ntraj, const int traj_mother[], const int traj_id[], const int traj_proc[], const double traj_E0[])
{
	int index;
	if(ParticleInfo::CountFSParticles(2112, 150., nFSPart, FSPartPDG, FSPartPx, FSPartPy, FSPartPz, index) < 1) return GetErrorVec(0.);

	ParticleInfo neutron(FSPartPx[index], FSPartPy[index], FSPartPz[index]);
	neutron.AddX0(x0[index], y0[index], z0[index]);
	neutron.AddX1(xf[index], yf[index], zf[index]);
	if(neutron.IsContained()) neutron.AddPTraj(traj_px0[index], traj_py0[index], traj_pz0[index]);
	double path_length = neutron.PathLength();

	bool isInelastic = neutron.isNeutronInelastic(index, ntraj, traj_mother, traj_id, traj_proc, x0, y0, z0, xf, yf, zf);
    
    bool reweightable = (path_length > 0.0) && isInelastic; 

    double coeff = 0.0;
    if (reweightable) {
        // From Kevin, taken from coherent pion analysis
        const double c0 = 0.0010528;
        const double c1 = -0.3269;
        const double c2 = 159.55;
        double l0 = 1/(c0 + c1/neutron.Mom() + c2/(neutron.Mom() * neutron.Mom()));
        double l  = path_length;
        if (neutron.IsContained()) coeff = (-l/l0) / (exp(l/l0) - 1);
        else coeff = l/l0;
    }

    double neutron_KE = traj_E0[index] - ParticleInfo::NeutronMass; 
    double f = Calc_f(neutron_KE);
    double correctionErr = coeff * f;

	return GetErrorVec(correctionErr);
}

double DetError::Calc_f(double neutron_KE)
{
    double var_unc = std::max(0.0,std::min(150.0,-150 + neutron_KE)/1500.)
        + std::min(0.25,0.1 + (3*std::max(0.0,50 - neutron_KE))/500.);

    return var_unc;
}

// double DetError::calcDistance(const TVector3 x0, const TVector3 x1)
// {
// 	TVector3 r = x1 - x0;
// 	return r.Mag();
// }

// should this be inline??
//inline 
std::vector<double> DetError::GetErrorVec(const double variation)
{
	std::vector<double> fix;
	fix.push_back(1. - variation);
	fix.push_back(1. + variation);
	return fix;
}

std::vector<double> DetError::MakeNormalShiftedUniverses()
{
	std::vector<double> vec;
	TRandom3 Random;
	for(int i = 0; i < m_nToys; i++){
		double val = Random.Gaus(0.0, 1.0);
		vec.push_back(val);
	}
	return vec;
}	

std::vector<double> DetError::MakeEmptyShifts()
{
	std::vector<double> empty;
	for(int i = 0; i < m_nToys; i++)
		empty.push_back( 0. );
	return empty;
}

std::vector<double> DetError::GenerateEnergyShifts(const double mc_1sigma, const double data_1sigma)
{
    // Fill EM Energy Scale Shifts
    double em_uncertainty = sqrt(mc_1sigma*mc_1sigma + data_1sigma*data_1sigma);
    cout << "em_uncertainty = sqrt(" << mc_1sigma <<"^2 + " << data_1sigma << "^2) = " << em_uncertainty << endl;
    return GenerateShifts(em_uncertainty); // ~ Gaussian(0.0, em_uncertainty)
}

void DetError::PrintEnergyShifts()
{
	for(size_t i = 0; i < m_Eshifts.size(); i++){
		cout << "m_Eshifts[" << i << "]" << m_Eshifts[i] << endl;
	}
}


std::vector<double> DetError::GenerateMuonThetashifts(const double muonThetaX_Err, const double muonThetaY_Err)
{
    // This is merely: sec^2(theta_X) + sec^2(theta_Y) = sec^2(theta) +1 
    double cos_muonTheta_Err = 1/(sqrt(1.0/pow(cos(muonThetaX_Err), 2) + 1.0/pow(cos(muonThetaY_Err), 2) -1) ); 
    double muonTheta_Err = acos(cos_muonTheta_Err);
    return GenerateShifts(muonTheta_Err);
}

#endif