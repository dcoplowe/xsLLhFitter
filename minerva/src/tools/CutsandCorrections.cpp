#ifndef __CUTSANDCORRECTIONS__CPP__
#define __CUTSANDCORRECTIONS__CPP__

#include <CutsandCorrections.h>
#include <FileIO.h>
#include <iostream>
// #include <PlotUtils/MnvNormalization.h>
#include <ReadParam.h>
#include <TMath.h>
#include <ReweightEvent.h>

using namespace std;

const std::string default_CutsandCorrections = "options/analysis_cuts.txt";

double CutsandCorrections::m_Emu_min = ReadParam::GetParameterD("Emu_min", default_CutsandCorrections);
double CutsandCorrections::m_Emu_max = ReadParam::GetParameterD("Emu_max", default_CutsandCorrections);
double CutsandCorrections::m_Enu_min = ReadParam::GetParameterD("Enu_min", default_CutsandCorrections);
double CutsandCorrections::m_Enu_max = ReadParam::GetParameterD("Enu_max", default_CutsandCorrections);

double CutsandCorrections::m_max_muon_theta = ReadParam::GetParameterD("max_muon_theta", default_CutsandCorrections);
double CutsandCorrections::m_max_gamma_dist = ReadParam::GetParameterD("max_gamma_dist", default_CutsandCorrections);

bool CutsandCorrections::m_applyPhotonDistance = ReadParam::GetParameterD("applyPhotonDistance", default_CutsandCorrections);
double CutsandCorrections::m_minPhotonDistance_1 = ReadParam::GetParameterD("minPhotonDistance_1", default_CutsandCorrections);
double CutsandCorrections::m_minPhotonDistance_2 = ReadParam::GetParameterD("minPhotonDistance_2", default_CutsandCorrections);

double CutsandCorrections::m_min_pi0_cos_openingAngle  = ReadParam::GetParameterD("min_pi0_cos_openingAngle", default_CutsandCorrections);
double CutsandCorrections::m_min_pi0_energy = ReadParam::GetParameterD("min_pi0_energy", default_CutsandCorrections);

double CutsandCorrections::m_W_max = ReadParam::GetParameterD("W_max", default_CutsandCorrections);

bool CutsandCorrections::m_make_pi0M_cut = ReadParam::GetParameterB("make_pi0M_cut", default_CutsandCorrections);
double CutsandCorrections::m_min_Pi0_invMass = ReadParam::GetParameterD("min_Pi0_invMass", default_CutsandCorrections);
double CutsandCorrections::m_max_Pi0_invMass = ReadParam::GetParameterD("max_Pi0_invMass", default_CutsandCorrections);

bool CutsandCorrections::m_sideBand_LowInvMass = ReadParam::GetParameterB("sideBand_LowInvMass", default_CutsandCorrections);
bool CutsandCorrections::m_sideBand_HighInvMass = ReadParam::GetParameterB("sideBand_HighInvMass", default_CutsandCorrections);
// Ozgur's Sidebands:
bool CutsandCorrections::m_sideBand_PID = ReadParam::GetParameterB("sideBand_PID", default_CutsandCorrections);
bool CutsandCorrections::m_sideBand_Michel = ReadParam::GetParameterB("sideBand_Michel", default_CutsandCorrections);

bool CutsandCorrections::m_applyProtonScore = ReadParam::GetParameterB("applyProtonScore", default_CutsandCorrections);
double CutsandCorrections::m_minProtonScore_LLR = ReadParam::GetParameterD("minProtonScore_LLR", default_CutsandCorrections);

bool CutsandCorrections::m_new_signal_cuts = ReadParam::GetParameterB("new_signal_cuts", default_CutsandCorrections);

// Eroica processing name:
const std::string CutsandCorrections::m_processing_name = ReadParam::GetParameterS("processing_name", default_CutsandCorrections);
const std::string CutsandCorrections::m_init_playlist = ReadParam::GetParameterS("init_playlist", default_CutsandCorrections);
MnvNormalizer * CutsandCorrections::m_normalizer = new MnvNormalizer(CutsandCorrections::m_processing_name, CutsandCorrections::m_init_playlist);

CutsandCorrections::CutsandCorrections() : m_applyWgts(false)
{
    PrintSetupInfo();
}

CutsandCorrections::CutsandCorrections(const bool applyWgts) : m_applyWgts(applyWgts)
{
    PrintSetupInfo();

    if(m_applyWgts){
        m_RW = new ReweightEvent();
    }
    else m_RW = 0x0;
}

CutsandCorrections::~CutsandCorrections()
{
    if(m_applyWgts){
        if(m_RW != NULL) delete m_RW;
    }
}

bool CutsandCorrections::PassedCuts(FileIO * fChain)
{
    // Want to fill cut statistics here.
    // Perhaps another class to contain all the cut histograms.

    // Apply reweights for cut histogram:
    // This isn't used yet.
    double wgt = 1.;
    if(m_applyWgts) wgt = m_RW->GetWgt(fChain);

    if(fChain->Cut_Vertex_None == 1) return false;
    // ------------------------------------------------------------------------
    // Vertex Reconstructable Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_Vertex_Not_Reconstructable == 1) return false;
    // ------------------------------------------------------------------------
    // Vertex Fiducial Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_Vertex_Not_Fiducial == 1) return false;

    // ------------------------------------------------------------------------
    // Muon Cut -- If Cut_Muon_None == 1 --> No MINOS Matched Muon
    // ------------------------------------------------------------------------
    if(fChain->Cut_Muon_None == 1) return false;

    // ------------------------------------------------------------------------
    // Anti-Muon Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_Muon_Charge == 1) return false;

    double muon_theta_beam = fChain->muon_theta_beam;
    if(muon_theta_beam*TMath::RadToDeg() > m_max_muon_theta) return false;

    // ------------------------------------------------------------------------
    // Michel Cuts 
    // -----------------------------------------------------------------------
    if(fChain->Cut_Vertex_Michel_Exist == 1 && !m_sideBand_Michel ) return false;
    if(fChain->Cut_EndPoint_Michel_Exist == 1 && !m_sideBand_Michel) return false;
    if(fChain->Cut_secEndPoint_Michel_Exist == 1 && !m_sideBand_Michel) return false;

    // ------------------------------------------------------------------------
    // Tracked Particle Reconstruction Fails
    // ------------------------------------------------------------------------
    if(fChain->Cut_Particle_None == 1) return false;

    // ------------------------------------------------------------------------
    // Cannot Find Proton in Tracked Particles
    // ------------------------------------------------------------------------
    if(fChain->Cut_Proton_None == 1) return false;

    // ------------------------------------------------------------------------
    // Proton Momentum NaN
    // ------------------------------------------------------------------------
    if(fChain->Cut_Proton_Bad == 1) return false;

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
    }

    // ------------------------------------------------------------------------
    // PreFilter Cut
    // ------------------------------------------------------------------------
    if(fChain->Cut_PreFilter_Pi0 == 1) return false;

    // ------------------------------------------------------------------------
    // ConeBlobs Cut -- If Cut_ConeBlobs == 1 --> Failed Pi0 Reconstruction
    // ------------------------------------------------------------------------
    if(fChain->Cut_ConeBlobs == 1 ) return false;

    // ------------------------------------------------------------------------
    // Blob Direction Bad Cut
    // ------------------------------------------------------------------------
    if (fChain->Cut_BlobDirectionBad == 1 ) return false;
    
    // ------------------------------------------------------------------------
    // Pi0 Momentum NaN
    // ------------------------------------------------------------------------
    if (fChain->Cut_Pi0_Bad == 1) return false;
    
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

    // ------------------------------------------------------------------------
    // Gamma1 Conv Length Cut Hist
    // ------------------------------------------------------------------------
    if (m_applyPhotonDistance && fChain->gamma1_dist_vtx * 0.1 < m_minPhotonDistance_1) return false;

    // ------------------------------------------------------------------------
    // Gamma2 Conv Length Cut
    // ------------------------------------------------------------------------
    if (m_applyPhotonDistance && fChain->gamma2_dist_vtx * 0.1 < m_minPhotonDistance_2) return false;

    // ------------------------------------------------------------------------
    // Low Gamma Energies AND Small Opening Angle Cut 
    // ------------------------------------------------------------------------
    if (IsOpeningAngleSmallAndEnergyLow(fChain->gamma1_E, fChain->gamma2_E, fChain->pi0_cos_openingAngle)) return false; 

    // ------------------------------------------------------------------------
    // Neutrino/Muon Energy Cut
    // ------------------------------------------------------------------------
    // DC 12/10/17: Ammended this to account for new signal def.
    if(!m_new_signal_cuts && !IsEnuInRange(fChain->Enu)) return false;//m_Enu <- Check if this is going to work. passing par into Enu
    else if(m_new_signal_cuts && !IsEmuInRange(fChain->muon_E)) return false;

    // ------------------------------------------------------------------------
    // W Cut
    // ------------------------------------------------------------------------
    // DC: 12/10/17: This is removed in the new signal def. so probably want to include this in the 
    // Make analysis cuts.
    // DC 12/10/17: Ammended this to account for new signal def.
    if(!m_new_signal_cuts && !IsWInRange(fChain->W) ) return false;//Same as m_Enu

    // ------------------------------------------------------------------------
    // Pi0 Invariant Mass Cut
    // ------------------------------------------------------------------------

    // DC 02/10/17: Add flag to turn on/off inv mass cut -- we can to consider cuts to 
    // the effect of the new pi0 mom. reconstruction and what its effect is on the inv.
    // mass distribution. 
    if(m_make_pi0M_cut){
        double isLowInvMassEvent = false;
        double isHighInvMassEvent = false;

        if (fChain->pi0_invMass < m_min_Pi0_invMass) isLowInvMassEvent = true;
        if (fChain->pi0_invMass > m_max_Pi0_invMass) isHighInvMassEvent = true;
    
        if(isLowInvMassEvent && !m_sideBand_LowInvMass) return false;
        if(isHighInvMassEvent && !m_sideBand_HighInvMass) return false;
    }

    if(m_new_signal_cuts){
        if(fChain->nProtonCandidates < 1) return false;
        // if(!IsEmuInRange(fChain->muon_E)) return false;
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

bool CutsandCorrections::CheckCaloCuts(const double E_g1, const double E_g2, const double pi0_cos_openingAngle, const double EnuEmu, const double W)
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

void CutsandCorrections::SetParamsFromFile(const std::string &file)
{
    // Need to add something to keep params to defualt values if they do no exist in params file.
    m_Emu_min = ReadParam::GetParameterD("Emu_min", file);
    m_Emu_max = ReadParam::GetParameterD("Emu_max", file);
    m_Enu_min = ReadParam::GetParameterD("Enu_min", file);
    m_Enu_max = ReadParam::GetParameterD("Enu_max", file);

    m_max_muon_theta = ReadParam::GetParameterD("max_muon_theta", file);
    m_max_gamma_dist = ReadParam::GetParameterD("max_gamma_dist", file);

    m_applyPhotonDistance = ReadParam::GetParameterD("applyPhotonDistance", file);
    m_minPhotonDistance_1 = ReadParam::GetParameterD("minPhotonDistance_1", file);
    m_minPhotonDistance_2 = ReadParam::GetParameterD("minPhotonDistance_2", file);

    m_min_pi0_cos_openingAngle  = ReadParam::GetParameterD("min_pi0_cos_openingAngle", file);
    m_min_pi0_energy = ReadParam::GetParameterD("min_pi0_energy", file);

    m_W_max = ReadParam::GetParameterD("W_max", file);

    m_make_pi0M_cut = ReadParam::GetParameterB("make_pi0M_cut", file);
    m_min_Pi0_invMass = ReadParam::GetParameterD("min_Pi0_invMass", file);
    m_max_Pi0_invMass = ReadParam::GetParameterD("max_Pi0_invMass", file);

    m_sideBand_LowInvMass = ReadParam::GetParameterB("sideBand_LowInvMass", file);
    m_sideBand_HighInvMass = ReadParam::GetParameterB("sideBand_HighInvMass", file);

    // Ozgur's Sidebands:
    m_sideBand_PID = ReadParam::GetParameterB("sideBand_PID", file);
    // Ozgur's Sidebands:
    m_sideBand_PID = ReadParam::GetParameterB("sideBand_PID", file);
    m_sideBand_Michel = ReadParam::GetParameterB("sideBand_Michel", file);

    m_applyProtonScore = ReadParam::GetParameterB("applyProtonScore", file);
    m_minProtonScore_LLR = ReadParam::GetParameterD("minProtonScore_LLR", file);

    m_new_signal_cuts = ReadParam::GetParameterB("new_signal_cuts", file);
}

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

    filename += "_pi0M";
    if(m_make_pi0M_cut) filename += "cuty";
    else if(m_sideBand_LowInvMass || m_sideBand_HighInvMass){
        if(m_sideBand_LowInvMass && m_sideBand_HighInvMass) filename += "lowhighSB";
        else if(m_sideBand_LowInvMass) filename += "lowSB";
        else  filename += "highSB";
    }
    else filename += "cutn";

    filename += "_FD";
    if(m_applyPhotonDistance) filename += "y";
    else filename += "n";

    filename += "_prPIDSB";
    if(m_sideBand_PID) filename += "y";
    else filename += "n";

    filename += "_prScore";
    if(m_applyProtonScore) filename += "y";
    else filename += "n";

    filename += "_meSB";
    if(m_sideBand_Michel) filename += "y";
    else filename += "n";

    filename += ".root";
}

#endif