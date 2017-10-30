#ifndef __CUTSANDCORRECTIONS__CPP__
#define __CUTSANDCORRECTIONS__CPP__

#include <CutsandCorrections.h>
#include <FileIO.h>
#include <iostream>
// #include <PlotUtils/MnvNormalization.h>
#include <ReadParam.h>
#include <TMath.h>
#include <DataInfo.h>

#ifdef __ENABLE__REWEIGHT__
#include <ReweightEvent.h>
#endif

using namespace std;

// const std::string DataInfo::Opts::Run = "options/run_options.txt";

const double CutsandCorrections::m_Emu_min = ReadParam::GetParameterD("Emu_min", DataInfo::Opts::Run);
const double CutsandCorrections::m_Emu_max = ReadParam::GetParameterD("Emu_max", DataInfo::Opts::Run);
const double CutsandCorrections::m_Enu_min = ReadParam::GetParameterD("Enu_min", DataInfo::Opts::Run);
const double CutsandCorrections::m_Enu_max = ReadParam::GetParameterD("Enu_max", DataInfo::Opts::Run);

const double CutsandCorrections::m_max_muon_theta = ReadParam::GetParameterD("max_muon_theta", DataInfo::Opts::Run);
const double CutsandCorrections::m_max_gamma_dist = ReadParam::GetParameterD("max_gamma_dist", DataInfo::Opts::Run);

const bool CutsandCorrections::m_applyPhotonDistance = ReadParam::GetParameterD("applyPhotonDistance", DataInfo::Opts::Run);
const double CutsandCorrections::m_minPhotonDistance_1 = ReadParam::GetParameterD("minPhotonDistance_1", DataInfo::Opts::Run);
const double CutsandCorrections::m_minPhotonDistance_2 = ReadParam::GetParameterD("minPhotonDistance_2", DataInfo::Opts::Run);

const double CutsandCorrections::m_min_pi0_cos_openingAngle  = ReadParam::GetParameterD("min_pi0_cos_openingAngle", DataInfo::Opts::Run);
const double CutsandCorrections::m_min_pi0_energy = ReadParam::GetParameterD("min_pi0_energy", DataInfo::Opts::Run);

const double CutsandCorrections::m_W_max = ReadParam::GetParameterD("W_max", DataInfo::Opts::Run);

const bool CutsandCorrections::m_make_pi0M_cut = ReadParam::GetParameterB("make_pi0M_cut", DataInfo::Opts::Run);
const double CutsandCorrections::m_min_Pi0_invMass = ReadParam::GetParameterD("min_Pi0_invMass", DataInfo::Opts::Run);
const double CutsandCorrections::m_max_Pi0_invMass = ReadParam::GetParameterD("max_Pi0_invMass", DataInfo::Opts::Run);

const bool CutsandCorrections::m_sideBand_LowInvMass = ReadParam::GetParameterB("sideBand_LowInvMass", DataInfo::Opts::Run);
const bool CutsandCorrections::m_sideBand_HighInvMass = ReadParam::GetParameterB("sideBand_HighInvMass", DataInfo::Opts::Run);
// Ozgur's Sidebands:
const bool CutsandCorrections::m_sideBand_PID = ReadParam::GetParameterB("sideBand_PID", DataInfo::Opts::Run);
const bool CutsandCorrections::m_sideBand_Michel = ReadParam::GetParameterB("sideBand_Michel", DataInfo::Opts::Run);

const bool CutsandCorrections::m_applyProtonScore = ReadParam::GetParameterB("applyProtonScore", DataInfo::Opts::Run);
const double CutsandCorrections::m_minProtonScore_LLR = ReadParam::GetParameterD("minProtonScore_LLR", DataInfo::Opts::Run);

const bool CutsandCorrections::m_new_signal_cuts = ReadParam::GetParameterB("new_signal_cuts", DataInfo::Opts::Run);
const bool CutsandCorrections::m_prP_threshold = ReadParam::GetParameterB("apply_proton_Pthreshold", DataInfo::Opts::Run);
const double CutsandCorrections::m_minPrMom = ReadParam::GetParameterD("proton_minMom", DataInfo::Opts::Run);

// Eroica processing name:
const std::string CutsandCorrections::m_processing_name = ReadParam::GetParameterS("processing_name", DataInfo::Opts::Run);
const std::string CutsandCorrections::m_init_playlist = ReadParam::GetParameterS("init_playlist", DataInfo::Opts::Run);
MnvNormalizer * CutsandCorrections::m_normalizer = new MnvNormalizer(CutsandCorrections::m_processing_name, CutsandCorrections::m_init_playlist);

#ifdef __ENABLE__CUTSTATS__
CutsandCorrections::CutsandCorrections() : m_applyWgts(false), stats()
#else
CutsandCorrections::CutsandCorrections() : m_applyWgts(false)
#endif
{
    PrintSetupInfo();
}
#ifdef __ENABLE__CUTSTATS__
CutsandCorrections::CutsandCorrections(const bool applyWgts) : m_applyWgts(applyWgts), stats()
#else
CutsandCorrections::CutsandCorrections(const bool applyWgts) : m_applyWgts(applyWgts)
#endif
{
    PrintSetupInfo();

    #ifdef __ENABLE__REWEIGHT__
    if(m_applyWgts){
        m_RW = new ReweightEvent();
    }
    else m_RW = 0x0;
    #endif
}

CutsandCorrections::~CutsandCorrections()
{
    #ifdef __ENABLE__REWEIGHT__
    if(m_applyWgts){
        if(m_RW != NULL) delete m_RW;
    }
    #endif
}

bool CutsandCorrections::PassedCuts(FileIO * fChain)
{
    // Want to fill cut statistics here.
    // Perhaps another class to contain all the cut histograms.

    // Apply reweights for cut histogram:
    // This isn't used yet.
    double wgt = 1.;
    #ifdef __ENABLE__REWEIGHT__
    if(m_applyWgts) wgt = m_RW->GetWgt(fChain);
    #endif

    #ifdef __ENABLE__CUTSTATS__
    stats.All.Inc(fChain->truth_isSignal, wgt);
    #endif

    if(fChain->Cut_Vertex_None == 1) return false;
    
    #ifdef __ENABLE__CUTSTATS__
    stats.Vertex_None.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Vertex Reconstructable Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_Vertex_Not_Reconstructable == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Vertex_Not_Reconstructable.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Vertex Fiducial Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_Vertex_Not_Fiducial == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Vertex_Not_Fiducial.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Muon Cut -- If Cut_Muon_None == 1 --> No MINOS Matched Muon
    // ------------------------------------------------------------------------
    if(fChain->Cut_Muon_None == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Muon_None.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Anti-Muon Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_Muon_Charge == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Muon_Charge.Inc(fChain->truth_isSignal, wgt);
    #endif

    double muon_theta_beam = fChain->muon_theta_beam;
    if(muon_theta_beam*TMath::RadToDeg() > m_max_muon_theta) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Muon_Angle.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Michel Cuts 
    // -----------------------------------------------------------------------
    if(fChain->Cut_Vertex_Michel_Exist == 1 && !m_sideBand_Michel ) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Vertex_Michel_Exist.Inc(fChain->truth_isSignal, wgt);
    #endif

    if(fChain->Cut_EndPoint_Michel_Exist == 1 && !m_sideBand_Michel) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.EndPoint_Michel_Exist.Inc(fChain->truth_isSignal, wgt);
    #endif

    if(fChain->Cut_secEndPoint_Michel_Exist == 1 && !m_sideBand_Michel) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.secEndPoint_Michel_Exist.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Tracked Particle Reconstruction Fails
    // ------------------------------------------------------------------------
    if(fChain->Cut_Particle_None == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Particle_None.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Cannot Find Proton in Tracked Particles
    // ------------------------------------------------------------------------
    if(fChain->Cut_Proton_None == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Proton_None.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Proton Momentum NaN
    // ------------------------------------------------------------------------
    if(fChain->Cut_Proton_Bad == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Proton_Bad.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_All.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_All.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // Apply Proton Score to All Proton Candidates
    // ------------------------------------------------------------------------
    if (fChain->nProtonCandidates > 0){

        for( int i = 0; i < fChain->nProtonCandidates; i++){            
            if(m_applyProtonScore && fChain->all_protons_LLRScore[i] < m_minProtonScore_LLR ){//Preserved for when cut stats are added.
                if(!m_sideBand_PID) return false;
                else break;
            }
        }

        #ifdef __ENABLE__CUTSTATS__
        stats.TwoTrk_ProtonScore.Inc(fChain->truth_isSignal, wgt);
        #endif
    }

    #ifdef __ENABLE__CUTSTATS__
    stats.ProtonScore.Inc(fChain->truth_isSignal, wgt);
    #endif

    if(fChain->nProtonCandidates > 0 && m_new_signal_cuts && m_prP_threshold){
        for( int i = 0; i < fChain->nProtonCandidates; i++){            
            if(m_prP_threshold && fChain->all_protons_P[i] < m_minPrMom) return false;
        }
        #ifdef __ENABLE__CUTSTATS__
        stats.TwoTrk_ProtonMom.Inc(fChain->truth_isSignal, wgt);
        #endif
    }
    #ifdef __ENABLE__CUTSTATS__
    stats.ProtonMom.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // PreFilter Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_PreFilter_Pi0 == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.PreFilter_Pi0.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_PreFilter_Pi0.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_PreFilter_Pi0.Inc(fChain->truth_isSignal, wgt);
    #endif

    // ------------------------------------------------------------------------
    // ConeBlobs Cut -- If Cut_ConeBlobs == 1 --> Failed Pi0 Reconstruction
    // ------------------------------------------------------------------------
    if(fChain->Cut_ConeBlobs == 1 ) return false;
    
    #ifdef __ENABLE__CUTSTATS__
    stats.ConeBlobs.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_ConeBlobs.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_ConeBlobs.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Blob Direction Bad Cut
    // ------------------------------------------------------------------------
    if (fChain->Cut_BlobDirectionBad == 1 ) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.BlobDirectionBad.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_BlobDirectionBad.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_BlobDirectionBad.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Pi0 Momentum NaN
    // ------------------------------------------------------------------------
    if (fChain->Cut_Pi0_Bad == 1) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Pi0_Bad.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_Pi0_Bad.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_Pi0_Bad.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Check for Michel Electrons at Begin & End Points of the Showers
    // ------------------------------------------------------------------------
    bool isGamma1_Michel = fChain->gamma1_isMichel_begin || fChain->gamma1_isMichel_end;
    bool isGamma2_Michel = fChain->gamma2_isMichel_begin || fChain->gamma2_isMichel_end;
    bool isShower_Michel_Exist = isGamma1_Michel || isGamma2_Michel;
    if (isShower_Michel_Exist && !m_sideBand_Michel) return false;

    // No End point for vertical showers - so use the following
    double g1_long_dist = abs(fChain->gamma1_vertex[2] - fChain->vtx_z);
    double g2_long_dist = abs(fChain->gamma2_vertex[2] - fChain->vtx_z);
    if (fChain->Cut_Vertex_Large_Michel_Exist == 1 && g1_long_dist <= m_max_gamma_dist ) return false;
    if (fChain->Cut_Vertex_Large_Michel_Exist == 1 && g2_long_dist <= m_max_gamma_dist ) return false;

    #ifdef __ENABLE__CUTSTATS__
    stats.Shower_Michel_Exist.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Gamma1 Conv Length Cut Hist
    // ------------------------------------------------------------------------
    if (m_applyPhotonDistance && fChain->gamma1_dist_vtx * 0.1 < m_minPhotonDistance_1) return false;
    
    #ifdef __ENABLE__CUTSTATS__
    stats.Photon1DistanceLow.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_Photon1DistanceLow.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_Photon1DistanceLow.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Gamma2 Conv Length Cut
    // ------------------------------------------------------------------------
    if (m_applyPhotonDistance && fChain->gamma2_dist_vtx * 0.1 < m_minPhotonDistance_2) return false;
    
    #ifdef __ENABLE__CUTSTATS__
    stats.Photon2DistanceLow.Inc(fChain->truth_isSignal, wgt);
    if(fChain->nProtonCandidates == 0) stats.OneTrk_Photon2DistanceLow.Inc(fChain->truth_isSignal, wgt);
    else stats.TwoTrk_Photon2DistanceLow.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Low Gamma Energies AND Small Opening Angle Cut 
    // ------------------------------------------------------------------------
    if (IsOpeningAngleSmallAndEnergyLow(fChain->gamma1_E, fChain->gamma2_E, fChain->pi0_cos_openingAngle)) return false; 
    
    #ifdef __ENABLE__CUTSTATS__
    stats.LowE_SmallAngle.Inc(fChain->truth_isSignal, wgt);
    #endif
    // ------------------------------------------------------------------------
    // Neutrino/Muon Energy Cut
    // ------------------------------------------------------------------------
    // DC 12/10/17: Ammended this to account for new signal def.
    if(!m_new_signal_cuts && !IsEnuInRange(fChain->Enu)) return false;//m_Enu <- Check if this is going to work. passing par into Enu
    else if(m_new_signal_cuts && !IsEmuInRange(fChain->muon_E)) return false;

    #ifdef __ENABLE__CUTSTATS__
    if(m_new_signal_cuts){
        stats.Emu.Inc(fChain->truth_isSignal, wgt);
        if(fChain->nProtonCandidates == 0) stats.OneTrk_Emu.Inc(fChain->truth_isSignal, wgt);
        else stats.TwoTrk_Emu.Inc(fChain->truth_isSignal, wgt);
    }
    else{
        stats.beamEnergy.Inc(fChain->truth_isSignal, wgt);
        if(fChain->nProtonCandidates == 0) stats.OneTrk_beamEnergy.Inc(fChain->truth_isSignal, wgt);
        else stats.TwoTrk_beamEnergy.Inc(fChain->truth_isSignal, wgt);
    }
    #endif

    // ------------------------------------------------------------------------
    // W Cut
    // ------------------------------------------------------------------------
    // DC: 12/10/17: This is removed in the new signal def. so probably want to include this in the 
    // Make analysis cuts.
    // DC 12/10/17: Ammended this to account for new signal def.
    if(!m_new_signal_cuts && !IsWInRange(fChain->W) ) return false;//Same as m_Enu

    #ifdef __ENABLE__CUTSTATS__
    if(!m_new_signal_cuts){
        stats.W.Inc(fChain->truth_isSignal, wgt);
        if(fChain->nProtonCandidates == 0) stats.OneTrk_W.Inc(fChain->truth_isSignal, wgt);
        else stats.TwoTrk_W.Inc(fChain->truth_isSignal, wgt);
    }
    #endif

    // ------------------------------------------------------------------------
    // Pi0 Invariant Mass Cut
    // ------------------------------------------------------------------------

    // DC 02/10/17: Add flag to turn on/off inv mass cut -- we can to consider cuts to 
    // the effect of the new pi0 mom. reconstruction and what its effect is on the inv.
    // mass distribution. 
    if(m_make_pi0M_cut){
        bool isLowInvMassEvent = false;
        bool isHighInvMassEvent = false;

        if (fChain->pi0_invMass < m_min_Pi0_invMass) isLowInvMassEvent = true;
        if (fChain->pi0_invMass > m_max_Pi0_invMass) isHighInvMassEvent = true;
    
        if(isLowInvMassEvent && !m_sideBand_LowInvMass) return false;
        if(isHighInvMassEvent && !m_sideBand_HighInvMass) return false;

        #ifdef __ENABLE__CUTSTATS__
        stats.Pi0_invMass.Inc(fChain->truth_isSignal, wgt);
        if(fChain->nProtonCandidates == 0) stats.OneTrk_Pi0_invMass.Inc(fChain->truth_isSignal, wgt);
        else stats.TwoTrk_Pi0_invMass.Inc(fChain->truth_isSignal, wgt);
        #endif
    }

    if(m_new_signal_cuts){
        if(fChain->nProtonCandidates < 1) return false;
        // if(!IsEmuInRange(fChain->muon_E)) return false;
        #ifdef __ENABLE__CUTSTATS__
        stats.HaveProton.Inc(fChain->truth_isSignal, wgt);
        #endif
    }

    //-------------------------------------------------------------------------
    // Satisfied All Cuts
    //-------------------------------------------------------------------------
    return true;
}

bool CutsandCorrections::isDeltaRichSample(FileIO * fChain)
{
    return (fChain->nProtonCandidates > 0) && (fChain->W > 0 && fChain->W < 1400);
}

bool CutsandCorrections::IsOpeningAngleSmallAndEnergyLow(const double E_g1, const double E_g2, const double pi0_cos_openingAngle)
{
    bool isAngleSmall = pi0_cos_openingAngle > m_min_pi0_cos_openingAngle;// 0.95;
    bool isEnergyLow = (E_g1 + E_g2) < m_min_pi0_energy; // 400.0;
    if (isEnergyLow && isAngleSmall) return true;
    return false;
}

bool CutsandCorrections::IsInRange(const double Energy, const double min, const double max)
{
    return (min < Energy && Energy < max);
}

bool CutsandCorrections::IsEmuInRange(const double Energy)
{
    return IsInRange(Energy, m_Emu_min, m_Emu_max);
}

bool CutsandCorrections::IsEnuInRange(const double Energy)
{
    return IsInRange(Energy, m_Enu_min, m_Enu_max);
}

bool CutsandCorrections::IsWInRange(const double W){
    return IsInRange(W, 0., m_W_max);
}

bool CutsandCorrections::IsInvMassInRange(const double m)
{
    return IsInRange(m, m_min_Pi0_invMass, m_max_Pi0_invMass);
}

double CutsandCorrections::GetMuonThetaCorrection(FileIO * fChain)
{
	double corrected_theta = fChain->muon_theta_beam;
    if (fChain->muon_theta_allNodes_sz >= 28) corrected_theta = fChain->muon_theta_allNodes[28];
    else if (fChain->muon_theta_allNodes_sz >= 19) corrected_theta = fChain->muon_theta_allNodes[19];
    else if (fChain->muon_theta_allNodes_sz >= 9) corrected_theta = fChain->muon_theta_allNodes[9];
    return corrected_theta;
}

// Seems to cause a memory leak so have changed this to have simply call a static pointer to MnvNormalizer (processing name shouldn't
// ever be changed whilst running or data/mc (I think):
// OLD:
// double CutsandCorrections::GetMINOSCorrection(FileIO * fChain, const std::string &processing_name)
// {
//     std::string playlist = GetPlaylist(fChain->mc_run, fChain->mc_intType);
//     if (playlist.compare("minerva_2p2h") == 0) playlist = "minerva13C";
//     MnvNormalizer normalizer = new MnvNormalizer(processing_name, playlist);
//     double correction = normalizer->GetCorrection(fChain->CCProtonPi0_minos_trk_p);
//     delete normalizer;
//     return normalizer.GetCorrection(fChain->CCProtonPi0_minos_trk_p);
// }

double CutsandCorrections::GetMINOSCorrection(FileIO * fChain)
{
    std::string playlist = GetPlaylist(fChain->mc_run, fChain->mc_intType);
    if (playlist.compare("minerva_2p2h") == 0) playlist = "minerva13C";
    m_normalizer->LoadPlaylist(playlist);
    return m_normalizer->GetCorrection(fChain->CCProtonPi0_minos_trk_p);
}

std::string CutsandCorrections::GetPlaylist(const int run, const int type)
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

bool CutsandCorrections::CheckCaloCuts(const double E_g1, const double E_g2, const double pi0_cos_openingAngle,
    const double EnuEmu, const double W)
{
    bool base = !IsOpeningAngleSmallAndEnergyLow(E_g1, E_g2, pi0_cos_openingAngle);

    bool cuts = false;
    if(m_new_signal_cuts){
        cuts = IsEmuInRange(EnuEmu);
    }
    else{
        cuts = IsEnuInRange(EnuEmu) && IsWInRange(W);
    }

    bool pi0inv = true;
    if(m_make_pi0M_cut) pi0inv = IsInvMassInRange(KinCalc::Pi0InvMass(E_g1, E_g2, pi0_cos_openingAngle));

    return (base && cuts && pi0inv);
}


// void CutsandCorrections::SetParamsFromFile(const std::string &file)
// {
//     // Need to add something to keep params to defualt values if they do no exist in params file.
//     m_Emu_min = ReadParam::GetParameterD("Emu_min", file);
//     m_Emu_max = ReadParam::GetParameterD("Emu_max", file);
//     m_Enu_min = ReadParam::GetParameterD("Enu_min", file);
//     m_Enu_max = ReadParam::GetParameterD("Enu_max", file);

//     m_max_muon_theta = ReadParam::GetParameterD("max_muon_theta", file);
//     m_max_gamma_dist = ReadParam::GetParameterD("max_gamma_dist", file);

//     m_applyPhotonDistance = ReadParam::GetParameterD("applyPhotonDistance", file);
//     m_minPhotonDistance_1 = ReadParam::GetParameterD("minPhotonDistance_1", file);
//     m_minPhotonDistance_2 = ReadParam::GetParameterD("minPhotonDistance_2", file);

//     m_min_pi0_cos_openingAngle  = ReadParam::GetParameterD("min_pi0_cos_openingAngle", file);
//     m_min_pi0_energy = ReadParam::GetParameterD("min_pi0_energy", file);

//     m_W_max = ReadParam::GetParameterD("W_max", file);

//     m_make_pi0M_cut = ReadParam::GetParameterB("make_pi0M_cut", file);
//     m_min_Pi0_invMass = ReadParam::GetParameterD("min_Pi0_invMass", file);
//     m_max_Pi0_invMass = ReadParam::GetParameterD("max_Pi0_invMass", file);

//     m_sideBand_LowInvMass = ReadParam::GetParameterB("sideBand_LowInvMass", file);
//     m_sideBand_HighInvMass = ReadParam::GetParameterB("sideBand_HighInvMass", file);

//     // Ozgur's Sidebands:
//     m_sideBand_PID = ReadParam::GetParameterB("sideBand_PID", file);
//     // Ozgur's Sidebands:
//     m_sideBand_PID = ReadParam::GetParameterB("sideBand_PID", file);
//     m_sideBand_Michel = ReadParam::GetParameterB("sideBand_Michel", file);

//     m_applyProtonScore = ReadParam::GetParameterB("applyProtonScore", file);
//     m_minProtonScore_LLR = ReadParam::GetParameterD("minProtonScore_LLR", file);

//     m_new_signal_cuts = ReadParam::GetParameterB("new_signal_cuts", file);
// }

void CutsandCorrections::PrintSetupInfo()
{
    cout << "-----------------------------------------------------------------------------------" << endl;
    cout << "                       Cuts and Corrections Setup Summary    " << endl;
    cout << "                                 " << endl;
    cout << "                      sideBand_Michel: " << (m_sideBand_Michel ? "Yes" : "No") << endl;
    cout << "                         sideBand_PID: " << (m_sideBand_PID ? "Yes" : "No") << endl;
    cout << "                  sideBand_LowInvMass: " << (m_sideBand_LowInvMass ? "Yes" : "No") << endl;
    cout << "                 sideBand_HighInvMass: " << (m_sideBand_HighInvMass ? "Yes" : "No") << endl;
    cout << "                     applyProtonScore: " << (m_applyProtonScore ? "Yes" : "No") << endl;
    cout << "                   minProtonScore_LLR: " << m_minProtonScore_LLR << endl;
    cout << "                  applyPhotonDistance: " << (m_applyPhotonDistance ? "Yes" : "No") << endl;
    cout << "                  minPhotonDistance_1: " << m_minPhotonDistance_1 << endl;
    cout << "                  minPhotonDistance_2: " << m_minPhotonDistance_2 << endl;
    cout << "                      min_Pi0_invMass: " << m_min_Pi0_invMass << endl;
    cout << "                      max_Pi0_invMass: " << m_max_Pi0_invMass << endl;
    cout << "                Apply new signal cuts: " << (m_new_signal_cuts ? "Yes" : "No") << endl;
    cout << "                    Make pi0 mass cut: " << (m_make_pi0M_cut ? "Yes" : "No") << endl;
    cout << "                             " << m_Emu_min << " < Emu (MeV) < " <<  m_Emu_max << endl;
    cout << "                             " << m_Enu_min << " < Enu (MeV) < " <<  m_Enu_max << endl;
    cout << endl;
    cout << "                        Prossing Name: " << m_processing_name << endl;
    cout << "                    Playlist at setup: " << m_init_playlist << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;
}

void CutsandCorrections::AddCutInfo(std::string &filename)
{
    size_t dotroot = filename.find(".root");
    if(dotroot != string::npos) filename = filename.substr(0, dotroot);

    filename += "_";
    if(m_new_signal_cuts) filename += "new";
    else filename += "old";
    filename += "SIG";

    filename += "_FD";
    if(m_applyPhotonDistance) filename += "y";
    else filename += "n";

    filename += "_prScore";
    if(m_applyProtonScore) filename += "y";
    else filename += "n";

    // 
    if(m_new_signal_cuts){
        filename += "_prMom";
        if(m_prP_threshold) filename += "y";
        else filename += "n";
    }

    filename += "_pi0M";
    if(m_make_pi0M_cut) filename += "cuty";
    else if(m_sideBand_LowInvMass || m_sideBand_HighInvMass){
        if(m_sideBand_LowInvMass && m_sideBand_HighInvMass) filename += "lowhigh";
        else if(m_sideBand_LowInvMass) filename += "low";
        else  filename += "high";
        filename += "SB";
    }
    else filename += "cutn";

    filename += "_prPIDSB";
    if(m_sideBand_PID) filename += "y";
    else filename += "n";

    filename += "_meSB";
    if(m_sideBand_Michel) filename += "y";
    else filename += "n";

    filename += ".root";
}

#endif