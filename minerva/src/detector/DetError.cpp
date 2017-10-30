#include <DetError.h>

#ifndef __PARTICLEINFO__CPP__
#define __PARTICLEINFO__CPP__

#ifndef kIniValue 
#define kIniValue -999
#endif

#include <TMath.h>
#include <iostream>
#include <DataInfo.h>
#include <CalculateKinematics.h>
#include <CutsandCorrections.h>

using std::cout;
using std::endl;

ParticleInfo::ParticleInfo(const double px, const double py, const double pz) : m_mom3(px, py, pz), m_start(0., 0., 0.),
	m_finish(0., 0., 0.), m_path_length(0.),
	m_have_start(false), m_have_finish(false), m_contained(false)
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

// void ParticleInfo::AddPTraj(const double traj_x0, const double traj_y0, const double traj_z0)
// {
// 	m_have_traj = true;
// 	m_traj.SetXYZ(traj_x0, traj_y0, traj_z0);

// 	if(m_have_start && m_have_finish){
// 		if(m_contained)  m_path_length = (m_finish - m_start).Mag();
// 		else GetUncontainedPathLength();
// 	}
// }

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
		double new_x = m_mom3.X() + length * m_mom3.Unit().X();
		double new_y = m_mom3.Y() + length * m_mom3.Unit().Y();
		double new_z = m_mom3.Z() + length * m_mom3.Unit().Z();
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
	const double FSPartPy[], const double FSPartPz[])
{
    int count = 0;
    for (int i = 0; i < nFSPart; i++){
        if (FSPartPDG[i] == pdg){ 
        	TVector3 mom(FSPartPx[i], FSPartPy[i], FSPartPz[i]);
            if (mom.Mag() > P_min){ 
            	count++;
            	break;
            }
        }
    }
    return count;
}

int ParticleInfo::GetIndex(const int pdg, const int detmc_ntrajectory2, const int detmc_traj_mother[], const int detmc_traj_pdg[],
	const double detmc_traj_E0[])
{
	int index = kIniValue;
    // int count = 0;
    double E_min = 0.;
    for (int i = 0; i < detmc_ntrajectory2; i++){
    	if(detmc_traj_mother[i] > 0 && pdg == detmc_traj_pdg[i] && E_min < detmc_traj_E0[i]){
    		E_min = detmc_traj_E0[i];
    		index = i;
    	}
    }
    return index;
}

#endif

#ifndef __DETERROR__CPP__
#define __DETERROR__CPP__

#include <TVector3.h>
#include <TRandom3.h>
#include <FileIO.h>
#include <ReadParam.h>
#include <iostream>
using std::cout;
using std::endl;

#ifndef kIniValue 
#define kIniValue -999
#endif

#ifndef kEpsilon 
#define kEpsilon 1e-12
#endif

// For recalculation of cuts:
const bool DetError::m_new_signal_cuts = ReadParam::GetParameterB("new_signal_cuts", "options/analysis_cuts.txt");

const std::string default_DetError = "options/run_opts.txt";

const int DetError::m_nToys = ReadParam::GetParameterI("ntoys", default_DetError);// 500;
const std::vector<double> DetError::m_nominal = DetError::MakeNormalShiftedUniverses();
const std::vector<double> DetError::m_Eshifts = DetError::GenerateEnergyShifts();
const std::vector<double> DetError::m_MuTshifts = DetError::GenerateMuonThetashifts();
const std::vector<double> DetError::m_Emptyshifts = DetError::MakeEmptyShifts();
const std::vector<double> DetError::m_noError = DetError::GetErrorVec(0.); 
std::vector<double> DetError::m_relEshifts = DetError::m_Emptyshifts;//DetError::MakeEmptyShifts();
bool DetError::m_rel_warn = true;

// Eroica processing name:
const std::string DetError::m_processing_name = ReadParam::GetParameterS("processing_name", default_DetError);
const std::string DetError::m_init_playlist = ReadParam::GetParameterS("init_playlist", default_DetError);
MnvNormalizer * DetError::m_normalizer = new MnvNormalizer(DetError::m_processing_name, DetError::m_init_playlist);

// Uncertainty is 1.1% hence weights 1-0.011 AND 1+0.011
// docDB 11443, slide 38 
const std::vector<double> DetError::m_michel_true = DetError::GetErrorVec(0.011);
// Uncertainty is 0.5% hence weights 1-0.005 AND 1+0.005
  // docDB 11443, slide 38 
const std::vector<double> DetError::m_michel_false = DetError::GetErrorVec(0.005);

// Proton uncertainty:
const std::vector<double> DetError::m_prEshort = DetError::GetErrorVec(0.046);
const std::vector<double> DetError::m_prElong = DetError::GetErrorVec(0.003);

// const std::vector<double> DetError::m_muPshifts = Det
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

DetError::DetError(FileIO * fChain)
{
	m_chain = fChain;
	// Whese 1 +/- correction
	m_def.clear();
}


DetError::DetError(FileIO & fChain)
{
	m_chain = &fChain;
	// Whese 1 +/- correction
	m_def.clear();
}

DetError::Default DetError::GetDefaults()
{
	m_def.clear();
	// Michel (miss-)tagging:
	m_def.michel_true = GetMichelErrTrue(m_chain->truth_isBckg_withMichel);
	m_def.michel_false = GetMichelErrFalse(m_chain->truth_isBckg_withMichel);
	// This is the combined mis-tag uncertainty:
	m_def.michel = GetMichelErr(m_chain->truth_isBckg_withMichel);

	// Muon tracking:
	m_def.mu_trking = GetMINOSCorrectionErr(m_chain->CCProtonPi0_minos_trk_p, m_chain->mc_run, m_chain->mc_intType);

	// Neutron response:
	m_def.neutron_res = GetNeutronResponseErr(m_chain->mc_nFSPart, m_chain->mc_FSPartPDG, m_chain->mc_FSPartPx,
		m_chain->mc_FSPartPy, m_chain->mc_FSPartPz, m_chain->detmc_ntrajectory2, m_chain->detmc_traj_pdg, 
		m_chain->detmc_traj_mother, m_chain->detmc_traj_id, m_chain->detmc_traj_proc, m_chain->detmc_traj_px0,
		m_chain->detmc_traj_py0, m_chain->detmc_traj_pz0, m_chain->detmc_traj_E0, m_chain->detmc_traj_x0, 
		m_chain->detmc_traj_y0, m_chain->detmc_traj_z0, m_chain->detmc_traj_xf,m_chain->detmc_traj_yf,m_chain->detmc_traj_zf);

	// Pion response:
	m_def.pi_res = GetPionResponseErr(m_chain->truth_isBckg_SingleChargedPion_ChargeExchanged);

	// Proton tracking:
	if(m_chain->nProtonCandidates > 0) m_def.pr_trking = GetProtonTrackingErr(m_chain->proton_length);
	else m_def.pr_trking = m_noError;

	return m_def;
}

void DetError::PrintDefaults()
{
	for(size_t ll = 0; ll < m_def.michel.size(); ll++){
		if(ll == 0) cout << "Lower  Bound: ";
		else cout << "Upper  Bound: ";
		cout << " Michel = " << m_def.michel[ll];
		cout << " Michel True = " << m_def.michel_true[ll];
		cout << " Michel False = " << m_def.michel_false[ll];
		cout << " Moun Trcking = " << m_def.mu_trking[ll];
		cout << " Neutron res. = " << m_def.neutron_res[ll];
		cout << " Pion res. = " << m_def.pi_res[ll];
		cout << " Proton Trcking. = " << m_def.pr_trking[ll];
		cout << endl;
	}
}

// Uncertainty is 0.5% hence weights 1-0.005 AND 1+0.005
// docDB 11443, slide 38 
// Uncertainty is 1.1% hence weights 1-0.011 AND 1+0.011
// docDB 11443, slide 38 
// Michel Fake Error Applied only to Events without True Michel 
std::vector<double> DetError::GetMichelErr(const bool truth_isBckg_withMichel)
{
    if(!truth_isBckg_withMichel) return m_michel_false;
    return m_michel_true;
}

std::vector<double> DetError::GetMichelErrTrue(const bool truth_isBckg_withMichel)
{
    if (truth_isBckg_withMichel) return m_michel_true;
    return m_noError;
}

std::vector<double> DetError::GetMichelErrFalse(const bool truth_isBckg_withMichel)
{
    if (!truth_isBckg_withMichel) return m_michel_false;
    return m_noError;
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
	for(size_t i = 0; i < m_Eshifts.size(); i++) {
		double temp = var*m_Eshifts[i];
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
    //Ref: TN048
    if(track_length < 8*17.0) return m_prEshort;
    else return m_prElong;
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

std::vector<double> DetError::GetMINOSCorrectionErr(const double CCProtonPi0_minos_trk_p, const int run, const int type)
{
	std::string playlist = GetPlaylist(run, type);
    if (playlist.compare("minerva_2p2h") == 0) playlist = "minerva13C";
    m_normalizer->LoadPlaylist(playlist);
	double correctionErr = m_normalizer->GetCorrectionErr(CCProtonPi0_minos_trk_p);
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

std::vector<double> DetError::GetNeutronResponseErr(
	const int nFSPart, const int FSPartPDG[], const double FSPartPx[], const double FSPartPy[], const double FSPartPz[], 
	const int ntraj, const int traj_pdg[], const int traj_mother[], const int traj_id[], const int traj_proc[],
	const double traj_px0[], const double traj_py0[], const double traj_pz0[], const double traj_E0[],
	const double traj_x0[], const double traj_y0[], const double traj_z0[],
	const double traj_xf[],const double traj_yf[],const double traj_zf[])
{
	int find_pdg = 2112;
	double mom_thresh = 150.;

	if( ParticleInfo::CountFSParticles(find_pdg, mom_thresh, nFSPart, FSPartPDG, FSPartPx, FSPartPy, FSPartPz) < 1 ) return m_noError;

	// detmc_ntrajectory2, detmc_traj_mother, detmc_traj_mother, detmc_traj_pdg, detmc_traj_E0
	int index = ParticleInfo::GetIndex(find_pdg, ntraj, traj_mother, traj_pdg, traj_E0);

	ParticleInfo neutron(traj_px0[index], traj_py0[index], traj_pz0[index]);
	neutron.AddX0(traj_x0[index], traj_y0[index], traj_z0[index]);
	neutron.AddX1(traj_xf[index], traj_yf[index], traj_zf[index]);
	// if(neutron.IsContained()) neutron.AddPTraj(traj_px0[index], traj_py0[index], traj_pz0[index]);
	double path_length = neutron.PathLength();

	bool isInelastic = neutron.isNeutronInelastic(index, ntraj, traj_mother, traj_id, traj_proc, traj_x0, traj_y0, traj_z0, traj_xf, traj_yf, traj_zf);
    
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
    // cout << "em_uncertainty = sqrt(" << mc_1sigma <<"^2 + " << data_1sigma << "^2) = " << em_uncertainty << endl;
    return GenerateShifts(em_uncertainty); // ~ Gaussian(0.0, em_uncertainty)
}

void DetError::PrintEnergyShifts()
{
	for(size_t i = 0; i < m_Eshifts.size(); i++){
		cout << "m_Eshifts[" << i << "] = " << m_Eshifts[i] << endl;
	}
}

std::vector<double> DetError::GenerateMuonThetashifts(const double muonThetaX_Err, const double muonThetaY_Err)
{
    // This is merely: sec^2(theta_X) + sec^2(theta_Y) = sec^2(theta) +1 
    double cos_muonTheta_Err = 1/(sqrt(1.0/pow(cos(muonThetaX_Err), 2) + 1.0/pow(cos(muonThetaY_Err), 2) -1) ); 
    double muonTheta_Err = acos(cos_muonTheta_Err);
    return GenerateShifts(muonTheta_Err);
}

std::vector<double> DetError::GetMuonPShifts(const double muon_E_shift, const double muon_shift)
{
	double muonP_uncertainty = muon_E_shift/muon_shift;
    return GenerateShifts(muonP_uncertainty); // ~ Gaussian(0.0, muonP_uncertainty)
}

void DetError::GetCorrectedMuon3Mom(double &px, double &py, double &pz, const int n_theta_nodes, const double theta_nodes[], bool wrtbeam)
{
	TVector3 mom(px, py, pz);
	double theta = GetCorrectedMuonTheta(n_theta_nodes, theta_nodes, wrtbeam);
	mom.SetTheta(theta);
	px = mom.X();
	py = mom.Y();
	pz = mom.Z();
}

// THis isn't needed as we have already calculated it.
double DetError::GetCorrectedMuonTheta(const int n_theta_nodes, const double theta_nodes[], bool wrtbeam)
{
	// We need sort out this so that we are can be in min. coords too.
	(void)wrtbeam;
	double corrected_theta = kIniValue;
	if (n_theta_nodes >= 28) corrected_theta = theta_nodes[28];
	else if (n_theta_nodes >= 19) corrected_theta = theta_nodes[19];
	else if (n_theta_nodes >= 9) corrected_theta = theta_nodes[9];
	// else corrected_theta = muon_theta_beam;
	return corrected_theta;
}

double * DetError::GetWgts(DetError::LatType type) const
{
	// If we are varying W, Q2 and Enu need to be calculated too

	// Setup variables as their defaults:
	double E_g1 = m_chain->gamma1_E;
	double E_g2 = m_chain->gamma2_E;
	
	double pi0_P = m_chain->pi0_P;
	double pi0_E = m_chain->pi0_E;

	double Emu = m_chain->muon_E;
	double Pmu = m_chain->muon_P;
	double Thetamu = m_chain->muon_theta_beam;

	double Enu = m_chain->Enu;
	double Q2 = m_chain->QSq;
	double W = m_chain->W;

	double extra_energy = m_chain->vertex_blob_energy + m_chain->Extra_Energy_Total;

	int nProtons = m_chain->nProtonCandidates;
	// double extra_energy = 0.0;
	for(int p = 0; p < nProtons; p++) extra_energy += m_chain->all_protons_KE[p];
	
	int ntoys = m_nToys;
	if(type >= kPrMass) ntoys = 2;
	
	double * wgts = new double[ ntoys ];

	// std::vector<double> prShifts;
	// prShifts.clear();

	std::vector< std::vector<double> > variation;
	variation.clear();

	switch(type){
			case kMuMom:
			{
				variation.push_back( DetError::GetMuonPShifts(m_chain->muon_E_shift, m_chain->muon_P) );	
				break;
			}
			case kPrBirks:
			{
				for(int p = 0; p < nProtons; p++){
					double error = m_chain->all_protons_energy_shift_Birks[p]/m_chain->all_protons_E[p];
					variation.push_back( GenerateShifts(error) );
				}
				break;
			}
			case kPrMass:
			{
				variation.push_back( GetProtonShifts(m_chain->all_protons_energy_shift_Mass_Up, 
					m_chain->all_protons_energy_shift_Mass_Down, nProtons) );
				break;
			}
			case kPrBethBloch:
			{
				variation.push_back( GetProtonShifts(m_chain->all_protons_energy_shift_BetheBloch_Up,
					m_chain->all_protons_energy_shift_BetheBloch_Down, nProtons) );
				break;
			}
			case kPrMEU:
			{
				variation.push_back( GetProtonShifts(m_chain->all_protons_energy_shift_MEU_Up,
					m_chain->all_protons_energy_shift_MEU_Down, nProtons) );
				break;
			}				
			default: break;
		}

	for(int i = 0; i < ntoys; i++){
		double tmp_E_g1 = E_g1;
		double tmp_E_g2 = E_g2;

		double tmp_pi0_P = pi0_P;
		double tmp_pi0_E = pi0_E;

		double tmp_Emu = Emu;
		double tmp_Pmu = Pmu;
		double tmp_Thetamu = Thetamu;

		double tmp_extra_energy = extra_energy;// + total_proton_KE;

		switch(type){
			case kEMScale:
			{
				// EM Energy Dependent Variables
				tmp_E_g1 *= (1.0 + m_Eshifts[i]);
				tmp_E_g2 *= (1.0 + m_Eshifts[i]);
				tmp_pi0_P *= (1.0 + m_Eshifts[i]);
				tmp_pi0_E = KinCalc::GetParticleE(tmp_pi0_P, DataInfo::Mass::Pi0);
				break;
			}
			case kMuMom:
			{
				// Muon Variables
				tmp_Pmu *= (1.0 + variation[0][i]);
				tmp_Emu = KinCalc::GetParticleE(tmp_Pmu, DataInfo::Mass::Muon);        
				break;
			}
			case kMuTheta:
			{
				tmp_Thetamu *= (1.0 + m_MuTshifts[i]);
				break;
			}
			case kPrBirks:
			{
				for (int p = nProtons; p < nProtons; ++p ){
					tmp_extra_energy += variation[p][i];// prShifts[p]; 
				}
				break;
			}
			case kPrMass:
			case kPrBethBloch:
			case kPrMEU:
			{
				for (int p = i*nProtons; p < (i+1)*nProtons; ++p ){
					tmp_extra_energy += variation[0][p];// prShifts[p]; 
				}
			}				
			default: break;
		}

		// For new signal def. much of this is not required:
		// Only muon and gammas info is need.
		double tmp_W = W;
		if(!m_new_signal_cuts){
			double tmp_Enu = Enu;
			double tmp_QSq = QSq;
			if(type != kMuTheta) tmp_Enu = KinCalc::GetEnu(tmp_Emu, tmp_pi0_E, tmp_extra_energy);		
			tmp_QSq = KinCalc::GetQSq(tmp_Enu, tmp_Emu, tmp_Pmu, tmp_Thetamu);
			double tmp_Wsq = KinCalc::GetWSq(tmp_Enu, tmp_QSq, tmp_Emu);
			tmp_W = tmp_Wsq > 0 ? sqrt(tmp_Wsq) : kIniValue;
			EnuEmu = tmp_Enu;
		}
		else EnuEmu = Emu;

		if(CutsandCorrections::CheckCaloCuts(tmp_E_g1, tmp_E_g2, m_chain->pi0_cos_openingAngle, EnuEmu, tmp_W)) wgts[i] = 1.;
		else wgts[i] = 0.;
	}
    return wgts;
}
	
#endif