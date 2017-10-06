#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <FileIOBase.h>

// #include <string>
// #include <TROOT.h>

class TBranch;

class FileIO : public FileIOBase
{
public:
	FileIO(std::string in_filename, std::string in_treename, bool verbose = false) :
	FileIOBase(in_filename, in_treename, verbose){ Init();}
	FileIO(bool verbose = false) : FileIOBase(verbose){;}

private:
	void Init();
	void InitLLNTuple();

public:
	// Vars from ntuple (user must add them from tree.makeclass()):
	// Example:
	// Here goes:
	// Double_t varname;
	// TBranch *b_varname;
	// Then in FileIO::Init() (FileIO.cpp):
	// inchain->SetBranchAddress("varname", &varname, &b_varname);

	Double_t        eventID;
	Int_t           CCProtonPi0_nuFlavor;
	Int_t           CCProtonPi0_nuHelicity;
	Int_t           CCProtonPi0_intCurrent;
	Int_t           CCProtonPi0_intType;
	Double_t        CCProtonPi0_E;
	Double_t        CCProtonPi0_Q2;
	Double_t        CCProtonPi0_x;
	Double_t        CCProtonPi0_y;
	Double_t        CCProtonPi0_W;
	Double_t        CCProtonPi0_score;
	Double_t        CCProtonPi0_leptonE[4];
	Double_t        CCProtonPi0_vtx[4];
	Bool_t          CCProtonPi0_minos_trk_is_contained;
	Bool_t          CCProtonPi0_minos_trk_is_ok;
	Bool_t          CCProtonPi0_minos_used_range;
	Bool_t          CCProtonPi0_minos_used_curvature;
	Int_t           CCProtonPi0_isMuonInsideOD;
	Int_t           CCProtonPi0_minos_trk_end_plane;
	Int_t           CCProtonPi0_minos_trk_quality;
	Int_t           CCProtonPi0_ntrajMuonProng;
	Int_t           CCProtonPi0_r_minos_trk_vtx_plane;
	Int_t           CCProtonPi0_t_minos_trk_numFSMuons;
	Int_t           CCProtonPi0_t_minos_trk_primFSLeptonPDG;
	Int_t           CCProtonPi0_trajMuonProngPDG;
	Int_t           CCProtonPi0_trajMuonProngPrimary;
	Double_t        CCProtonPi0_endMuonTrajMomentum;
	Double_t        CCProtonPi0_endMuonTrajXPosition;
	Double_t        CCProtonPi0_endMuonTrajYPosition;
	Double_t        CCProtonPi0_endMuonTrajZPosition;
	Double_t        CCProtonPi0_hadron_recoil;
	Double_t        CCProtonPi0_hadron_recoil_CCInc;
	Double_t        CCProtonPi0_minos_trk_bave;
	Double_t        CCProtonPi0_minos_trk_chi2;
	Double_t        CCProtonPi0_minos_trk_end_u;
	Double_t        CCProtonPi0_minos_trk_end_v;
	Double_t        CCProtonPi0_minos_trk_end_x;
	Double_t        CCProtonPi0_minos_trk_end_y;
	Double_t        CCProtonPi0_minos_trk_end_z;
	Double_t        CCProtonPi0_minos_trk_eqp;
	Double_t        CCProtonPi0_minos_trk_eqp_qp;
	Double_t        CCProtonPi0_minos_trk_fit_pass;
	Double_t        CCProtonPi0_minos_trk_ndf;
	Double_t        CCProtonPi0_minos_trk_p;
	Double_t        CCProtonPi0_minos_trk_p_curvature;
	Double_t        CCProtonPi0_minos_trk_p_range;
	Double_t        CCProtonPi0_minos_trk_qp;
	Double_t        CCProtonPi0_minos_trk_vtx_x;
	Double_t        CCProtonPi0_minos_trk_vtx_y;
	Double_t        CCProtonPi0_minos_trk_vtx_z;
	Double_t        CCProtonPi0_r_minos_trk_bdL;
	Double_t        CCProtonPi0_r_minos_trk_end_dcosx;
	Double_t        CCProtonPi0_r_minos_trk_end_dcosy;
	Double_t        CCProtonPi0_r_minos_trk_end_dcosz;
	Double_t        CCProtonPi0_r_minos_trk_vtx_dcosx;
	Double_t        CCProtonPi0_r_minos_trk_vtx_dcosy;
	Double_t        CCProtonPi0_r_minos_trk_vtx_dcosz;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMinosInitProjPx;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMinosInitProjPy;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMinosInitProjPz;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMinosInitProjX;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMinosInitProjY;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMinosInitProjZ;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvFinalPx;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvFinalPy;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvFinalPz;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvFinalX;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvFinalY;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvFinalZ;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvInitPx;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvInitPy;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvInitPz;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvInitX;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvInitY;
	Double_t        CCProtonPi0_t_minos_trk_primFSLepMnvInitZ;
	Double_t        CCProtonPi0_trajMuonPhi;
	Double_t        CCProtonPi0_trajMuonProngEnergy;
	Double_t        CCProtonPi0_trajMuonProngMomentum;
	Double_t        CCProtonPi0_trajMuonProngPSelf;
	Double_t        CCProtonPi0_trajMuonProngPx;
	Double_t        CCProtonPi0_trajMuonProngPy;
	Double_t        CCProtonPi0_trajMuonProngPz;
	Double_t        CCProtonPi0_trajMuonTheta;
	Int_t           CCProtonPi0_isProtonInsideOD[10];
	Int_t           CCProtonPi0_ntrajProtonProng[10];
	Int_t           CCProtonPi0_trajProtonProngPDG[10];
	Int_t           CCProtonPi0_trajProtonProngPrimary[10];
	Double_t        CCProtonPi0_endProtonTrajMomentum[10];
	Double_t        CCProtonPi0_endProtonTrajXPosition[10];
	Double_t        CCProtonPi0_endProtonTrajYPosition[10];
	Double_t        CCProtonPi0_endProtonTrajZPosition[10];
	Double_t        CCProtonPi0_trajProtonPhi[10];
	Double_t        CCProtonPi0_trajProtonProngEnergy[10];
	Double_t        CCProtonPi0_trajProtonProngMomentum[10];
	Double_t        CCProtonPi0_trajProtonProngPSelf[10];
	Double_t        CCProtonPi0_trajProtonProngPx[10];
	Double_t        CCProtonPi0_trajProtonProngPy[10];
	Double_t        CCProtonPi0_trajProtonProngPz[10];
	Double_t        CCProtonPi0_trajProtonTheta[10];
	Bool_t          truth_isGamma1_conv_inside;
	Bool_t          truth_isGamma2_conv_inside;
	Bool_t          truth_isSignal;
	Bool_t          truth_isSignal_BeforeFSI;
	Bool_t          truth_isSignalOut_Acceptance;
	Bool_t          truth_isSignalOut_Kinematics;
	Bool_t          truth_isSignal_EventRecord;
	Bool_t          truth_isFidVol;
	Bool_t          truth_isNC;
	Bool_t          truth_ReconstructEvent;
	Bool_t          truth_isBckg_NoPi0;
	Bool_t          truth_isBckg_SinglePi0;
	Bool_t          truth_isBckg_MultiPi0;
	Bool_t          truth_isBckg_Compact_WithPi0;
	Bool_t          truth_isBckg_Compact_QELike;
	Bool_t          truth_isBckg_Compact_SinglePiPlus;
	Bool_t          truth_isBckg_Compact_Other;
	Bool_t          truth_isBckg_NC;
	Bool_t          truth_isBckg_AntiNeutrino;
	Bool_t          truth_isBckg_QELike;
	Bool_t          truth_isBckg_SingleChargedPion;
	Bool_t          truth_isBckg_SingleChargedPion_ChargeExchanged;
	Bool_t          truth_isBckg_DoublePionWithPi0;
	Bool_t          truth_isBckg_DoublePionWithoutPi0;
	Bool_t          truth_isBckg_MultiPionWithPi0;
	Bool_t          truth_isBckg_MultiPionWithoutPi0;
	Bool_t          truth_isBckg_Other;
	Bool_t          truth_isBckg_withMichel;
	Int_t           truth_Bckg_nOther;
	Int_t           truth_Bckg_nPi0_Primary;
	Int_t           truth_Bckg_nPi0_Secondary;
	Int_t           truth_Bckg_nPi0_Total;
	Int_t           truth_Bckg_nPiCharged;
	Int_t           truth_Bckg_nPiCharged_ChargeExchanged;
	Int_t           truth_InNucleus_N_pi0_final;
	Int_t           truth_InNucleus_N_pi0_initial;
	Int_t           truth_InNucleus_N_piminus_final;
	Int_t           truth_InNucleus_N_piminus_initial;
	Int_t           truth_InNucleus_N_piplus_final;
	Int_t           truth_InNucleus_N_piplus_initial;
	Int_t           truth_N_FSParticles;
	Int_t           truth_N_other;
	Int_t           truth_N_pi0;
	Int_t           truth_N_proton;
	Int_t           truth_N_trueMichelElectrons;
	Int_t           truth_blob1_evis_most_pdg;
	Int_t           truth_blob2_evis_most_pdg;
	Int_t           truth_pi0_GrandMother;
	Int_t           truth_pi0_GrandMotherStatus;
	Int_t           truth_pi0_Mother;
	Int_t           truth_pi0_MotherStatus;
	Int_t           truth_pi0_status;
	Int_t           truth_target_material;
	Int_t           truth_track_michel_evis_most_pdg;
	Int_t           truth_vertex_module;
	Int_t           truth_vertex_plane;
	Int_t           truth_vtx_michel_evis_most_pdg;
	Int_t           truth_vtx_michel_large_evis_most_pdg;
	Double_t        truth_Enu_BeforeFSI;
	Double_t        truth_QSq_exp;
	Double_t        truth_QSq_exp_BeforeFSI;
	Double_t        truth_WSq_exp;
	Double_t        truth_W_exp;
	Double_t        truth_W_exp_BeforeFSI;
	Double_t        truth_allClusters_evis_pizero;
	Double_t        truth_blob1_evis_muon;
	Double_t        truth_blob1_evis_neutron;
	Double_t        truth_blob1_evis_piminus;
	Double_t        truth_blob1_evis_piplus;
	Double_t        truth_blob1_evis_pizero;
	Double_t        truth_blob1_evis_proton;
	Double_t        truth_blob1_evis_total_norm;
	Double_t        truth_blob1_evis_total_truth;
	Double_t        truth_blob2_evis_muon;
	Double_t        truth_blob2_evis_neutron;
	Double_t        truth_blob2_evis_piminus;
	Double_t        truth_blob2_evis_piplus;
	Double_t        truth_blob2_evis_pizero;
	Double_t        truth_blob2_evis_proton;
	Double_t        truth_blob2_evis_total_norm;
	Double_t        truth_blob2_evis_total_truth;
	Double_t        truth_eventID;
	Double_t        truth_michelElectron_E;
	Double_t        truth_michelElectron_P;
	Double_t        truth_michelMuon_P;
	Double_t        truth_michelMuon_end_dist_vtx;
	Double_t        truth_michelMuon_length;
	Double_t        truth_michelPion_P;
	Double_t        truth_michelPion_begin_dist_vtx;
	Double_t        truth_michelPion_length;
	Double_t        truth_muon_P;
	Double_t        truth_muon_P_BeforeFSI;
	Double_t        truth_muon_theta;
	Double_t        truth_muon_thetaX_beam;
	Double_t        truth_muon_thetaY_beam;
	Double_t        truth_muon_theta_beam;
	Double_t        truth_muon_theta_beam_BeforeFSI;
	Double_t        truth_pi0_KE;
	Double_t        truth_pi0_KE_BeforeFSI;
	Double_t        truth_pi0_P;
	Double_t        truth_pi0_P_BeforeFSI;
	Double_t        truth_pi0_theta;
	Double_t        truth_pi0_theta_beam;
	Double_t        truth_pi0_theta_beam_BeforeFSI;
	Double_t        truth_proton_P;
	Double_t        truth_proton_theta;
	Double_t        truth_proton_theta_beam;
	Double_t        truth_total_captured_evis_pizero;
	Double_t        truth_total_captured_evis_total_norm;
	Double_t        truth_total_captured_evis_total_truth;
	Double_t        truth_track_michel_evis_total_truth;
	Double_t        truth_vtx_michel_evis_total_truth;
	Double_t        truth_vtx_michel_large_evis_total_truth;
	Double_t        truth_gamma1_4P[4];
	Double_t        truth_gamma1_final_pos[3];
	Double_t        truth_gamma1_final_pos_estimated[3];
	Double_t        truth_gamma1_init_pos[3];
	Double_t        truth_gamma2_4P[4];
	Double_t        truth_gamma2_final_pos[3];
	Double_t        truth_gamma2_final_pos_estimated[3];
	Double_t        truth_gamma2_init_pos[3];
	Int_t           genie_wgt_n_shifts;
        Double_t        truth_genie_wgt_AGKYxF1pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_AhtBY[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_BhtBY[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_CCQEPauliSupViaKF[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_CV1uBY[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_CV2uBY[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_EtaNCEL[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrAbs_N[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrAbs_pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrCEx_N[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrCEx_pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrElas_N[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrElas_pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrInel_N[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrInel_pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrPiProd_N[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_FrPiProd_pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MFP_N[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MFP_pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MaCCQE[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MaCCQEshape[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MaNCEL[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MaRES[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_MvRES[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_NormCCQE[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_NormCCRES[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_NormDISCC[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_NormNCRES[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_RDecBR1gamma[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_Rvn1pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_Rvn2pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_Rvp1pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_Rvp2pi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_Theta_Delta2Npi[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_VecFFCCQEshape[7];   //[genie_wgt_n_shifts]
        Double_t        truth_genie_wgt_shifts[7];   //[genie_wgt_n_shifts]
        Double_t        truth_michelMuon_endPoint[3];
        Double_t        truth_muon_4P[4];
        Double_t        truth_pi0_4P[4];
        Double_t        truth_proton_4P[4];
        Int_t           physEvtNum;
        Int_t           n_hyps;
        Int_t           processType;
        Int_t           primaryPart;
        Int_t           n_slices;
        Int_t           slice_numbers[1];   //[n_slices]
        Int_t           shared_slice;
        Double_t        vtx[4];
        Double_t        vtxErr[4];
        Double_t        E[4];
        Bool_t          found_truth;
        Bool_t          ImprovedMichel_EventHasMichel;
        Bool_t          ImprovedMichel_EventHasMatchedMichel;
        Bool_t          ImprovedMichel_VertexHasMichel;
        Bool_t          ImprovedMichel_EndPointHasMichel;
        Bool_t          ImprovedMichel_secEndPointHasMichel;
        Bool_t          ImprovedMichel_Gamma1HasMichel;
        Bool_t          ImprovedMichel_Gamma2HasMichel;
        Bool_t          is_blobs_recovered;
        Bool_t          is_blobs_recovered_direction;
        Bool_t          is_blobs_recovered_invMass;
        Bool_t          is_blobs_recovered_small_angle;
        Bool_t          is_blobs_recovered_search_view_U;
        Bool_t          is_blobs_recovered_search_view_V;
        Bool_t          gamma1_isMichel_begin;
        Bool_t          gamma1_isMichel_end;
        Bool_t          gamma2_isMichel_begin;
        Bool_t          gamma2_isMichel_end;
        Int_t           Cut_BlobDirectionBad;
        Int_t           Cut_ConeBlobs;
        Int_t           Cut_EndPoint_Michel_Exist;
        Int_t           Cut_Muon_Charge;
        Int_t           Cut_Muon_None;
        Int_t           Cut_Particle_None;
        Int_t           Cut_Pi0_Bad;
        Int_t           Cut_PreFilter_Pi0;
        Int_t           Cut_Proton_Bad;
        Int_t           Cut_Proton_None;
        Int_t           Cut_Vertex_Large_Michel_Exist;
        Int_t           Cut_Vertex_Michel_Exist;
        Int_t           Cut_Vertex_None;
        Int_t           Cut_Vertex_Not_Fiducial;
        Int_t           Cut_Vertex_Not_Reconstructable;
        Int_t           Cut_secEndPoint_Michel_Exist;
        Int_t           anglescan_ncand;
        Int_t           anglescan_ncandx;
        Int_t           anglescan_nfoundBlobs;
        Int_t           detmc_ntrajectory;
        Int_t           detmc_ntrajectory2;
        Int_t           g1blob_1ParFit_ndof;
        Int_t           g1dedx_doublet;
        Int_t           g1dedx_empty_plane;
        Int_t           g1dedx_nplane;
        Int_t           g2blob_1ParFit_ndof;
        Int_t           g2dedx_doublet;
        Int_t           g2dedx_empty_plane;
        Int_t           g2dedx_nplane;
        Int_t           gamma1_blob_nclusters;
        Int_t           gamma1_blob_ndigits;
        Int_t           gamma2_blob_nclusters;
        Int_t           gamma2_blob_ndigits;
        Int_t           muon_N_minosTracks;
        Int_t           muon_charge;
        Int_t           muon_hasMinosMatchStub;
        Int_t           muon_hasMinosMatchTrack;
        Int_t           muon_minervaTrack_types;
        Int_t           muon_minosTrackQuality;
        Int_t           muon_roadUpstreamPlanes;
        Int_t           nProtonCandidates;
        Int_t           nTracks;
        Int_t           nTracks_Close;
        Int_t           nTracks_Discarded;
        Int_t           nTracks_Far;
        Int_t           od_energeticTower;
        Int_t           preFilter_Result;
        Int_t           proton_kinked;
        Int_t           proton_leadingIndice;
        Int_t           vtx_fit_converged;
        Int_t           vtx_module;
        Int_t           vtx_plane;
        Int_t           vtx_primary_index;
        Int_t           vtx_primary_multiplicity;
        Int_t           vtx_secondary_count;
        Int_t           vtx_total_count;
        Double_t        ConeBlobs_usable_evis_Tracker;
        Double_t        Coneblobs_usable_evis_ECAL;
        Double_t        Coneblobs_usable_evis_HCAL;
        Double_t        Extra_Energy_Leftover;
        Double_t        Extra_Energy_Muon;
        Double_t        Extra_Energy_Rejected;
        Double_t        Extra_Energy_Total;
        Double_t        g1blob_1ParFit_fval;
        Double_t        g1blob_2ParFit_vtx_distance;
        Double_t        g1dedx;
        Double_t        g1dedx1;
        Double_t        g1dedx_total;
        Double_t        g1dedx_total1;
        Double_t        g2blob_1ParFit_fval;
        Double_t        g2blob_2ParFit_vtx_distance;
        Double_t        g2dedx;
        Double_t        g2dedx1;
        Double_t        g2dedx_total;
        Double_t        g2dedx_total1;
        Double_t        gamma1_E;
        Double_t        gamma1_E_Old;
        Double_t        gamma1_P;
        Double_t        gamma1_blob_energy;
        Double_t        gamma1_blob_minsep;
        Double_t        gamma1_dEdx;
        Double_t        gamma1_dist_vtx;
        Double_t        gamma1_energy_ecal;
        Double_t        gamma1_energy_hcal;
        Double_t        gamma1_energy_scal_UV;
        Double_t        gamma1_energy_scal_X;
        Double_t        gamma1_energy_trkr;
        Double_t        gamma1_evis_ecal;
        Double_t        gamma1_evis_hcal;
        Double_t        gamma1_evis_scal_UV;
        Double_t        gamma1_evis_scal_X;
        Double_t        gamma1_evis_trkr;
        Double_t        gamma1_maxZ;
        Double_t        gamma1_phi;
        Double_t        gamma1_phi_beam;
        Double_t        gamma1_px;
        Double_t        gamma1_py;
        Double_t        gamma1_pz;
        Double_t        gamma1_theta;
        Double_t        gamma1_theta_beam;
        Double_t        gamma1_time;
        Double_t        gamma2_E;
        Double_t        gamma2_E_Old;
        Double_t        gamma2_P;
        Double_t        gamma2_blob_energy;
        Double_t        gamma2_blob_minsep;
        Double_t        gamma2_dEdx;
        Double_t        gamma2_dist_vtx;
        Double_t        gamma2_energy_ecal;
        Double_t        gamma2_energy_hcal;
        Double_t        gamma2_energy_scal_UV;
        Double_t        gamma2_energy_scal_X;
        Double_t        gamma2_energy_trkr;
        Double_t        gamma2_evis_ecal;
        Double_t        gamma2_evis_hcal;
        Double_t        gamma2_evis_scal_UV;
        Double_t        gamma2_evis_scal_X;
        Double_t        gamma2_evis_trkr;
        Double_t        gamma2_maxZ;
        Double_t        gamma2_phi;
        Double_t        gamma2_phi_beam;
        Double_t        gamma2_px;
        Double_t        gamma2_py;
        Double_t        gamma2_pz;
        Double_t        gamma2_theta;
        Double_t        gamma2_theta_beam;
        Double_t        gamma2_time;
        Double_t        muon_E;
        Double_t        muon_E_shift;
        Double_t        muon_KE;
        Double_t        muon_P;
        Double_t        muon_muScore;
        Double_t        muon_phi;
        Double_t        muon_phi_beam;
        Double_t        muon_px;
        Double_t        muon_py;
        Double_t        muon_pz;
        Double_t        muon_qp;
        Double_t        muon_qpqpe;
        Double_t        muon_roadUpstreamEnergy;
        Double_t        muon_theta;
        Double_t        muon_thetaX_beam;
        Double_t        muon_thetaY_beam;
        Double_t        muon_theta_beam;
        Double_t        muon_theta_beam_biasDown;
        Double_t        muon_theta_beam_biasUp;
        Double_t        od_downstreamFrame;
        Double_t        od_downstreamFrame_z;
        Double_t        od_highStory;
        Double_t        od_highStory_t;
        Double_t        od_lowStory;
        Double_t        od_lowStory_t;
        Double_t        od_maxEnergy;
        Double_t        od_upstreamFrame;
        Double_t        od_upstreamFrame_z;
        Double_t        pi0_E;
        Double_t        pi0_E_Cal;
        Double_t        pi0_KE;
        Double_t        pi0_P;
        Double_t        pi0_cos_openingAngle;
        Double_t        pi0_invMass;
        Double_t        thruth_pi0_invMass;
        Double_t        pi0_invMass_Old;
        Double_t        pi0_openingAngle;
        Double_t        pi0_phi;
        Double_t        pi0_phi_beam;
        Double_t        pi0_px;
        Double_t        pi0_py;
        Double_t        pi0_pz;
        Double_t        pi0_theta;
        Double_t        pi0_theta_beam;
        Double_t        pi0_theta_beam_biasDown;
        Double_t        pi0_theta_beam_biasUp;
        Double_t        preFilter_evis_ECAL;
        Double_t        preFilter_evis_HCAL;
        Double_t        preFilter_evis_NuclearTarget;
        Double_t        preFilter_evis_TotalExceptNuclearTarget;
        Double_t        preFilter_evis_Tracker;
        Double_t        preFilter_evis_nearvtx;
        Double_t        preFilter_evis_total;
        Double_t        preFilter_rejectedEnergy;
        Double_t        proton_E;
        Double_t        proton_KE;
        Double_t        proton_LLRScore;
        Double_t        proton_P;
        Double_t        proton_energy_shift_BetheBloch_Down;
        Double_t        proton_energy_shift_BetheBloch_Up;
        Double_t        proton_energy_shift_Birks;
        Double_t        proton_energy_shift_MEU_Down;
        Double_t        proton_energy_shift_MEU_Up;
        Double_t        proton_energy_shift_Mass_Down;
        Double_t        proton_energy_shift_Mass_Up;
        Double_t        proton_energy_shift_Nominal;
        Double_t        proton_length;
        Double_t        proton_phi;
        Double_t        proton_phi_beam;
        Double_t        proton_pionScore;
        Double_t        proton_protonScore;
        Double_t        proton_px;
        Double_t        proton_py;
        Double_t        proton_pz;
        Double_t        proton_score1_shift_BetheBloch_Down;
        Double_t        proton_score1_shift_BetheBloch_Up;
        Double_t        proton_score1_shift_Birks;
        Double_t        proton_score1_shift_MEU_Down;
        Double_t        proton_score1_shift_MEU_Up;
        Double_t        proton_score1_shift_Mass_Down;
        Double_t        proton_score1_shift_Mass_Up;
        Double_t        proton_score1_shift_Nominal;
        Double_t        proton_theta;
        Double_t        proton_theta_beam;
        Double_t        proton_ln_Q0;
        Double_t        proton_ln_Q1;
        Double_t        proton_ln_Q2;
        Double_t        proton_ln_Q3;
        Double_t        proton_ln_Q4;
        Double_t        proton_ln_Q5;
        Double_t        reco_eventID;
        Double_t        time;
        Double_t        track_michelProng_begin_Z;
        Double_t        track_michelProng_distance;
        Double_t        track_michelProng_end_Z;
        Double_t        track_michelProng_energy;
        Double_t        track_michelProng_time_diff;
        Double_t        vertex_blob_energy;
        Double_t        vertex_blob_evis;
        Double_t        vtx_fit_chi2;
        Double_t        vtx_michelProng_Large_begin_Z;
        Double_t        vtx_michelProng_Large_distance;
        Double_t        vtx_michelProng_Large_end_Z;
        Double_t        vtx_michelProng_Large_energy;
        Double_t        vtx_michelProng_Large_time_diff;
        Double_t        vtx_michelProng_begin_Z;
        Double_t        vtx_michelProng_distance;
        Double_t        vtx_michelProng_end_Z;
        Double_t        vtx_michelProng_energy;
        Double_t        vtx_michelProng_time_diff;
        Double_t        vtx_x;
        Double_t        vtx_y;
        Double_t        vtx_z;
        Int_t           all_protons_kinked[10];
        Int_t           all_protons_odMatch[10];
        Int_t           detmc_traj_id_sz;
        Int_t           detmc_traj_id[365];   //[detmc_traj_id_sz]
        Int_t           detmc_traj_mother_sz;
        Int_t           detmc_traj_mother[365];   //[detmc_traj_mother_sz]
        Int_t           detmc_traj_pdg_sz;
        Int_t           detmc_traj_pdg[365];   //[detmc_traj_pdg_sz]
        Int_t           detmc_traj_proc_sz;
        Int_t           detmc_traj_proc[365];   //[detmc_traj_proc_sz]
        Int_t           detmc_traj_status_sz;
        Int_t           detmc_traj_status[365];   //[detmc_traj_status_sz]
        Int_t           g1dedx_cluster_occupancy_sz;
        Int_t           g1dedx_cluster_occupancy[6];   //[g1dedx_cluster_occupancy_sz]
        Int_t           g2dedx_cluster_occupancy_sz;
        Int_t           g2dedx_cluster_occupancy[6];   //[g2dedx_cluster_occupancy_sz]
        Int_t           nTracks_Secondary_Vtx_sz;
        Int_t           nTracks_Secondary_Vtx[5];   //[nTracks_Secondary_Vtx_sz]
        Double_t        all_protons_E[10];
        Double_t        all_protons_KE[10];
        Double_t        all_protons_LLRScore[10];
        Double_t        all_protons_P[10];
        Double_t        all_protons_chi2_ndf[10];
        Double_t        all_protons_endPointX[10];
        Double_t        all_protons_endPointY[10];
        Double_t        all_protons_endPointZ[10];
        Double_t        all_protons_energy_shift_BetheBloch_Down[10];
        Double_t        all_protons_energy_shift_BetheBloch_Up[10];
        Double_t        all_protons_energy_shift_Birks[10];
        Double_t        all_protons_energy_shift_MEU_Down[10];
        Double_t        all_protons_energy_shift_MEU_Up[10];
        Double_t        all_protons_energy_shift_Mass_Down[10];
        Double_t        all_protons_energy_shift_Mass_Up[10];
        Double_t        all_protons_energy_shift_Nominal[10];
        Double_t        all_protons_length[10];
        Double_t        all_protons_p_calCorrection[10];
        Double_t        all_protons_p_dEdXTool[10];
        Double_t        all_protons_p_visEnergy[10];
        Double_t        all_protons_phi[10];
        Double_t        all_protons_phi_beam[10];
        Double_t        all_protons_pionScore[10];
        Double_t        all_protons_protonScore[10];
        Double_t        all_protons_px[10];
        Double_t        all_protons_py[10];
        Double_t        all_protons_pz[10];
        Double_t        all_protons_score1_shift_BetheBloch_Down[10];
        Double_t        all_protons_score1_shift_BetheBloch_Up[10];
        Double_t        all_protons_score1_shift_Birks[10];
        Double_t        all_protons_score1_shift_MEU_Down[10];
        Double_t        all_protons_score1_shift_MEU_Up[10];
        Double_t        all_protons_score1_shift_Mass_Down[10];
        Double_t        all_protons_score1_shift_Mass_Up[10];
        Double_t        all_protons_score1_shift_Nominal[10];
        Double_t        all_protons_startPointX[10];
        Double_t        all_protons_startPointY[10];
        Double_t        all_protons_startPointZ[10];
        Double_t        all_protons_theta[10];
        Double_t        all_protons_theta_beam[10];
        Int_t           detmc_traj_E0_sz;
        Double_t        detmc_traj_E0[365];   //[detmc_traj_E0_sz]
        Int_t           detmc_traj_Ef_sz;
        Double_t        detmc_traj_Ef[365];   //[detmc_traj_Ef_sz]
        Int_t           detmc_traj_preEf_sz;
        Double_t        detmc_traj_preEf[365];   //[detmc_traj_preEf_sz]
        Int_t           detmc_traj_prepxf_sz;
        Double_t        detmc_traj_prepxf[365];   //[detmc_traj_prepxf_sz]
        Int_t           detmc_traj_prepyf_sz;
        Double_t        detmc_traj_prepyf[365];   //[detmc_traj_prepyf_sz]
        Int_t           detmc_traj_prepzf_sz;
        Double_t        detmc_traj_prepzf[365];   //[detmc_traj_prepzf_sz]
        Int_t           detmc_traj_px0_sz;
        Double_t        detmc_traj_px0[365];   //[detmc_traj_px0_sz]
        Int_t           detmc_traj_pxf_sz;
        Double_t        detmc_traj_pxf[365];   //[detmc_traj_pxf_sz]
        Int_t           detmc_traj_py0_sz;
        Double_t        detmc_traj_py0[365];   //[detmc_traj_py0_sz]
        Int_t           detmc_traj_pyf_sz;
        Double_t        detmc_traj_pyf[365];   //[detmc_traj_pyf_sz]
        Int_t           detmc_traj_pz0_sz;
        Double_t        detmc_traj_pz0[365];   //[detmc_traj_pz0_sz]
        Int_t           detmc_traj_pzf_sz;
        Double_t        detmc_traj_pzf[365];   //[detmc_traj_pzf_sz]
        Int_t           detmc_traj_t0_sz;
        Double_t        detmc_traj_t0[365];   //[detmc_traj_t0_sz]
        Int_t           detmc_traj_tf_sz;
        Double_t        detmc_traj_tf[365];   //[detmc_traj_tf_sz]
        Int_t           detmc_traj_x0_sz;
        Double_t        detmc_traj_x0[365];   //[detmc_traj_x0_sz]
        Int_t           detmc_traj_xf_sz;
        Double_t        detmc_traj_xf[365];   //[detmc_traj_xf_sz]
        Int_t           detmc_traj_y0_sz;
        Double_t        detmc_traj_y0[365];   //[detmc_traj_y0_sz]
        Int_t           detmc_traj_yf_sz;
        Double_t        detmc_traj_yf[365];   //[detmc_traj_yf_sz]
        Int_t           detmc_traj_z0_sz;
        Double_t        detmc_traj_z0[365];   //[detmc_traj_z0_sz]
        Int_t           detmc_traj_zf_sz;
        Double_t        detmc_traj_zf[365];   //[detmc_traj_zf_sz]
        Double_t        fit_vtx[3];
        Int_t           g1dedx_cluster_energy_sz;
        Double_t        g1dedx_cluster_energy[6];   //[g1dedx_cluster_energy_sz]
        Int_t           g1dedx_rev_cluster_energy_sz;
        Double_t        g1dedx_rev_cluster_energy[107];   //[g1dedx_rev_cluster_energy_sz]
        Int_t           g2dedx_cluster_energy_sz;
        Double_t        g2dedx_cluster_energy[6];   //[g2dedx_cluster_energy_sz]
        Int_t           g2dedx_rev_cluster_energy_sz;
        Double_t        g2dedx_rev_cluster_energy[38];   //[g2dedx_rev_cluster_energy_sz]
        Double_t        gamma1_direction[3];
        Double_t        gamma1_end_vertex[3];
        Double_t        gamma1_vertex[3];
        Double_t        gamma2_direction[3];
        Double_t        gamma2_end_vertex[3];
        Double_t        gamma2_vertex[3];
        Int_t           muon_thetaX_allNodes_sz;
        Double_t        muon_thetaX_allNodes[158];   //[muon_thetaX_allNodes_sz]
        Int_t           muon_thetaY_allNodes_sz;
        Double_t        muon_thetaY_allNodes[158];   //[muon_thetaY_allNodes_sz]
        Int_t           muon_theta_allNodes_sz;
        Double_t        muon_theta_allNodes[158];   //[muon_theta_allNodes_sz]
        Int_t           od_distanceBlobTower_sz;
        Double_t        od_distanceBlobTower[2];   //[od_distanceBlobTower_sz]
        Int_t           od_idBlobTime_sz;
        Double_t        od_idBlobTime[2];   //[od_idBlobTime_sz]
        Int_t           od_towerEnergy_sz;
        Double_t        od_towerEnergy[6];   //[od_towerEnergy_sz]
        Int_t           od_towerNClusters_sz;
        Double_t        od_towerNClusters[6];   //[od_towerNClusters_sz]
        Int_t           od_towerTime_sz;
        Double_t        od_towerTime[6];   //[od_towerTime_sz]
        Int_t           od_towerTimeBlobMuon_sz;
        Double_t        od_towerTimeBlobMuon[2];   //[od_towerTimeBlobMuon_sz]
        Int_t           od_towerTimeBlobOD_sz;
        Double_t        od_towerTimeBlobOD[2];   //[od_towerTimeBlobOD_sz]
        Int_t           ev_run;
        Int_t           ev_subrun;
        Int_t           ev_detector;
        Int_t           ev_triggerType;
        Int_t           ev_gate;
        Int_t           ev_global_gate;
        Int_t           ev_gps_time_sec;
        Int_t           ev_gps_time_usec;
        Int_t           mc_run;
        Int_t           mc_subrun;
        Int_t           mc_nInteractions;
        Int_t           mc_MIState;
        Double_t        mc_pot;
        Int_t           mc_beamConfig;
        Int_t           mc_processType;
        Int_t           mc_nthEvtInSpill;
        Int_t           mc_nthEvtInFile;
        Int_t           mc_intType;
        Int_t           mc_current;
        Int_t           mc_charm;
        Double_t        mc_weight;
        Double_t        mc_XSec;
        Double_t        mc_diffXSec;
        Int_t           mc_incoming;
        Double_t        mc_fluxDriverProb;
        Int_t           mc_targetNucleus;
        Int_t           mc_targetZ;
        Int_t           mc_targetA;
        Int_t           mc_targetNucleon;
        Int_t           mc_struckQuark;
        Int_t           mc_seaQuark;
        Int_t           mc_resID;
        Int_t           mc_primaryLepton;
        Double_t        mc_incomingE;
        Double_t        mc_Bjorkenx;
        Double_t        mc_Bjorkeny;
        Double_t        mc_Q2;
        Double_t        mc_nuT;
        Double_t        mc_w;
        Double_t        mc_vtx[4];
        Double_t        mc_incomingPartVec[4];
        Double_t        mc_initNucVec[4];
        Double_t        mc_primFSLepton[4];
        Int_t           mc_nFSPart;
        Double_t        mc_FSPartPx[95];   //[mc_nFSPart]
        Double_t        mc_FSPartPy[95];   //[mc_nFSPart]
        Double_t        mc_FSPartPz[95];   //[mc_nFSPart]
        Double_t        mc_FSPartE[95];   //[mc_nFSPart]
        Int_t           mc_FSPartPDG[95];   //[mc_nFSPart]
        Int_t           mc_er_nPart;
        Int_t           mc_er_ID[121];   //[mc_er_nPart]
        Int_t           mc_er_status[121];   //[mc_er_nPart]
        Double_t        mc_er_posInNucX[121];   //[mc_er_nPart]
        Double_t        mc_er_posInNucY[121];   //[mc_er_nPart]
        Double_t        mc_er_posInNucZ[121];   //[mc_er_nPart]
        Double_t        mc_er_Px[121];   //[mc_er_nPart]
        Double_t        mc_er_Py[121];   //[mc_er_nPart]
        Double_t        mc_er_Pz[121];   //[mc_er_nPart]
        Double_t        mc_er_E[121];   //[mc_er_nPart]
        Int_t           mc_er_FD[121];   //[mc_er_nPart]
        Int_t           mc_er_LD[121];   //[mc_er_nPart]
        Int_t           mc_er_mother[121];   //[mc_er_nPart]
        Int_t           mc_fr_nNuAncestorIDs;
        Int_t           mc_fr_nuAncestorIDs[6];   //[mc_fr_nNuAncestorIDs]
        Int_t           mc_fr_nuParentID;
        Int_t           mc_fr_decMode;
        Double_t        mc_fr_primProtonVtx[3];
        Double_t        mc_fr_primProtonP[4];
        Double_t        mc_fr_nuParentDecVtx[3];
        Double_t        mc_fr_nuParentProdVtx[3];
        Double_t        mc_fr_nuParentProdP[4];
        Double_t        mc_cvweight_total;
        Double_t        wgt;
        Double_t        mc_cvweight_totalFlux;
        Double_t        mc_cvweight_totalXsec;
        Double_t        mc_ppfx1_cvweight;
        Double_t        mc_hornCurrent_cvweight;
        Double_t        mc_gen1_cvweight_total;
        Double_t        gen1_wgt;
        Double_t        mc_gen1_cvweight_totalFlux;
        Double_t        mc_gen1_cvweight_NA49;
        Int_t           mc_wgt_Flux_BeamFocus_sz;
        Double_t        mc_wgt_Flux_BeamFocus[100];   //[mc_wgt_Flux_BeamFocus_sz]
        Int_t           mc_wgt_gen1_Flux_Tertiary_sz;
        Double_t        mc_wgt_gen1_Flux_Tertiary[100];   //[mc_wgt_gen1_Flux_Tertiary_sz]
        Int_t           mc_wgt_gen1_Flux_NA49_sz;
        Double_t        mc_wgt_gen1_Flux_NA49[100];   //[mc_wgt_gen1_Flux_NA49_sz]
        Int_t           mc_wgt_Norm_sz;
        Double_t        mc_wgt_Norm[1];   //[mc_wgt_Norm_sz]
        Int_t           mc_wgt_ppfx1_Total_sz;
        Double_t        mc_wgt_ppfx1_Total[100];   //[mc_wgt_ppfx1_Total_sz]
        Int_t           n_prongs;
        Int_t           prong_nParticles[5];   //[n_prongs]
        Double_t        prong_part_score[5];   //[n_prongs]
        Double_t        prong_part_mass[5];   //[n_prongs]
        Int_t           prong_part_charge[5];   //[n_prongs]
        Int_t           prong_part_pid[5];   //[n_prongs]

        // List of branches
        TBranch        *b_eventID;   //!
        TBranch        *b_CCProtonPi0_nuFlavor;   //!
        TBranch        *b_CCProtonPi0_nuHelicity;   //!
        TBranch        *b_CCProtonPi0_intCurrent;   //!
        TBranch        *b_CCProtonPi0_intType;   //!
        TBranch        *b_CCProtonPi0_E;   //!
        TBranch        *b_CCProtonPi0_Q2;   //!
        TBranch        *b_CCProtonPi0_x;   //!
        TBranch        *b_CCProtonPi0_y;   //!
        TBranch        *b_CCProtonPi0_W;   //!
        TBranch        *b_CCProtonPi0_score;   //!
        TBranch        *b_CCProtonPi0_leptonE;   //!
        TBranch        *b_CCProtonPi0_vtx;   //!
        TBranch        *b_CCProtonPi0_minos_trk_is_contained;   //!
        TBranch        *b_CCProtonPi0_minos_trk_is_ok;   //!
        TBranch        *b_CCProtonPi0_minos_used_range;   //!
        TBranch        *b_CCProtonPi0_minos_used_curvature;   //!
        TBranch        *b_CCProtonPi0_isMuonInsideOD;   //!
        TBranch        *b_CCProtonPi0_minos_trk_end_plane;   //!
        TBranch        *b_CCProtonPi0_minos_trk_quality;   //!
        TBranch        *b_CCProtonPi0_ntrajMuonProng;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_vtx_plane;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_numFSMuons;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLeptonPDG;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngPDG;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngPrimary;   //!
        TBranch        *b_CCProtonPi0_endMuonTrajMomentum;   //!
        TBranch        *b_CCProtonPi0_endMuonTrajXPosition;   //!
        TBranch        *b_CCProtonPi0_endMuonTrajYPosition;   //!
        TBranch        *b_CCProtonPi0_endMuonTrajZPosition;   //!
        TBranch        *b_CCProtonPi0_hadron_recoil;   //!
        TBranch        *b_CCProtonPi0_hadron_recoil_CCInc;   //!
        TBranch        *b_CCProtonPi0_minos_trk_bave;   //!
        TBranch        *b_CCProtonPi0_minos_trk_chi2;   //!
        TBranch        *b_CCProtonPi0_minos_trk_end_u;   //!
        TBranch        *b_CCProtonPi0_minos_trk_end_v;   //!
        TBranch        *b_CCProtonPi0_minos_trk_end_x;   //!
        TBranch        *b_CCProtonPi0_minos_trk_end_y;   //!
        TBranch        *b_CCProtonPi0_minos_trk_end_z;   //!
        TBranch        *b_CCProtonPi0_minos_trk_eqp;   //!
        TBranch        *b_CCProtonPi0_minos_trk_eqp_qp;   //!
        TBranch        *b_CCProtonPi0_minos_trk_fit_pass;   //!
        TBranch        *b_CCProtonPi0_minos_trk_ndf;   //!
        TBranch        *b_CCProtonPi0_minos_trk_p;   //!
        TBranch        *b_CCProtonPi0_minos_trk_p_curvature;   //!
        TBranch        *b_CCProtonPi0_minos_trk_p_range;   //!
        TBranch        *b_CCProtonPi0_minos_trk_qp;   //!
        TBranch        *b_CCProtonPi0_minos_trk_vtx_x;   //!
        TBranch        *b_CCProtonPi0_minos_trk_vtx_y;   //!
        TBranch        *b_CCProtonPi0_minos_trk_vtx_z;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_bdL;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_end_dcosx;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_end_dcosy;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_end_dcosz;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_vtx_dcosx;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_vtx_dcosy;   //!
        TBranch        *b_CCProtonPi0_r_minos_trk_vtx_dcosz;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMinosInitProjPx;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMinosInitProjPy;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMinosInitProjPz;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMinosInitProjX;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMinosInitProjY;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMinosInitProjZ;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvFinalPx;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvFinalPy;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvFinalPz;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvFinalX;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvFinalY;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvFinalZ;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvInitPx;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvInitPy;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvInitPz;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvInitX;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvInitY;   //!
        TBranch        *b_CCProtonPi0_t_minos_trk_primFSLepMnvInitZ;   //!
        TBranch        *b_CCProtonPi0_trajMuonPhi;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngEnergy;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngMomentum;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngPSelf;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngPx;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngPy;   //!
        TBranch        *b_CCProtonPi0_trajMuonProngPz;   //!
        TBranch        *b_CCProtonPi0_trajMuonTheta;   //!
        TBranch        *b_CCProtonPi0_isProtonInsideOD;   //!
        TBranch        *b_CCProtonPi0_ntrajProtonProng;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngPDG;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngPrimary;   //!
        TBranch        *b_CCProtonPi0_endProtonTrajMomentum;   //!
        TBranch        *b_CCProtonPi0_endProtonTrajXPosition;   //!
        TBranch        *b_CCProtonPi0_endProtonTrajYPosition;   //!
        TBranch        *b_CCProtonPi0_endProtonTrajZPosition;   //!
        TBranch        *b_CCProtonPi0_trajProtonPhi;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngEnergy;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngMomentum;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngPSelf;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngPx;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngPy;   //!
        TBranch        *b_CCProtonPi0_trajProtonProngPz;   //!
        TBranch        *b_CCProtonPi0_trajProtonTheta;   //!
        TBranch        *b_truth_isGamma1_conv_inside;   //!
        TBranch        *b_truth_isGamma2_conv_inside;   //!
        TBranch        *b_truth_isSignal;   //!
        TBranch        *b_truth_isSignal_BeforeFSI;   //!
        TBranch        *b_truth_isSignalOut_Acceptance;   //!
        TBranch        *b_truth_isSignalOut_Kinematics;   //!
        TBranch        *b_truth_isSignal_EventRecord;   //!
        TBranch        *b_truth_isFidVol;   //!
        TBranch        *b_truth_isNC;   //!
        TBranch        *b_truth_ReconstructEvent;   //!
        TBranch        *b_truth_isBckg_NoPi0;   //!
        TBranch        *b_truth_isBckg_SinglePi0;   //!
        TBranch        *b_truth_isBckg_MultiPi0;   //!
        TBranch        *b_truth_isBckg_Compact_WithPi0;   //!
        TBranch        *b_truth_isBckg_Compact_QELike;   //!
        TBranch        *b_truth_isBckg_Compact_SinglePiPlus;   //!
        TBranch        *b_truth_isBckg_Compact_Other;   //!
        TBranch        *b_truth_isBckg_NC;   //!
        TBranch        *b_truth_isBckg_AntiNeutrino;   //!
        TBranch        *b_truth_isBckg_QELike;   //!
        TBranch        *b_truth_isBckg_SingleChargedPion;   //!
        TBranch        *b_truth_isBckg_SingleChargedPion_ChargeExchanged;   //!
        TBranch        *b_truth_isBckg_DoublePionWithPi0;   //!
        TBranch        *b_truth_isBckg_DoublePionWithoutPi0;   //!
        TBranch        *b_truth_isBckg_MultiPionWithPi0;   //!
        TBranch        *b_truth_isBckg_MultiPionWithoutPi0;   //!
        TBranch        *b_truth_isBckg_Other;   //!
        TBranch        *b_truth_isBckg_withMichel;   //!
        TBranch        *b_truth_Bckg_nOther;   //!
        TBranch        *b_truth_Bckg_nPi0_Primary;   //!
        TBranch        *b_truth_Bckg_nPi0_Secondary;   //!
        TBranch        *b_truth_Bckg_nPi0_Total;   //!
        TBranch        *b_truth_Bckg_nPiCharged;   //!
        TBranch        *b_truth_Bckg_nPiCharged_ChargeExchanged;   //!
        TBranch        *b_truth_InNucleus_N_pi0_final;   //!
        TBranch        *b_truth_InNucleus_N_pi0_initial;   //!
        TBranch        *b_truth_InNucleus_N_piminus_final;   //!
        TBranch        *b_truth_InNucleus_N_piminus_initial;   //!
        TBranch        *b_truth_InNucleus_N_piplus_final;   //!
        TBranch        *b_truth_InNucleus_N_piplus_initial;   //!
        TBranch        *b_truth_N_FSParticles;   //!
        TBranch        *b_truth_N_other;   //!
        TBranch        *b_truth_N_pi0;   //!
        TBranch        *b_truth_N_proton;   //!
        TBranch        *b_truth_N_trueMichelElectrons;   //!
        TBranch        *b_truth_blob1_evis_most_pdg;   //!
        TBranch        *b_truth_blob2_evis_most_pdg;   //!
        TBranch        *b_truth_pi0_GrandMother;   //!
        TBranch        *b_truth_pi0_GrandMotherStatus;   //!
        TBranch        *b_truth_pi0_Mother;   //!
        TBranch        *b_truth_pi0_MotherStatus;   //!
        TBranch        *b_truth_pi0_status;   //!
        TBranch        *b_truth_target_material;   //!
        TBranch        *b_truth_track_michel_evis_most_pdg;   //!
        TBranch        *b_truth_vertex_module;   //!
        TBranch        *b_truth_vertex_plane;   //!
        TBranch        *b_truth_vtx_michel_evis_most_pdg;   //!
        TBranch        *b_truth_vtx_michel_large_evis_most_pdg;   //!
        TBranch        *b_truth_Enu_BeforeFSI;   //!
        TBranch        *b_truth_QSq_exp;   //!
        TBranch        *b_truth_QSq_exp_BeforeFSI;   //!
        TBranch        *b_truth_WSq_exp;   //!
        TBranch        *b_truth_W_exp;   //!
        TBranch        *b_truth_W_exp_BeforeFSI;   //!
        TBranch        *b_truth_allClusters_evis_pizero;   //!
        TBranch        *b_truth_blob1_evis_muon;   //!
        TBranch        *b_truth_blob1_evis_neutron;   //!
        TBranch        *b_truth_blob1_evis_piminus;   //!
        TBranch        *b_truth_blob1_evis_piplus;   //!
        TBranch        *b_truth_blob1_evis_pizero;   //!
        TBranch        *b_truth_blob1_evis_proton;   //!
        TBranch        *b_truth_blob1_evis_total_norm;   //!
        TBranch        *b_truth_blob1_evis_total_truth;   //!
        TBranch        *b_truth_blob2_evis_muon;   //!
        TBranch        *b_truth_blob2_evis_neutron;   //!
        TBranch        *b_truth_blob2_evis_piminus;   //!
        TBranch        *b_truth_blob2_evis_piplus;   //!
        TBranch        *b_truth_blob2_evis_pizero;   //!
        TBranch        *b_truth_blob2_evis_proton;   //!
        TBranch        *b_truth_blob2_evis_total_norm;   //!
        TBranch        *b_truth_blob2_evis_total_truth;   //!
        TBranch        *b_truth_eventID;   //!
        TBranch        *b_truth_michelElectron_E;   //!
        TBranch        *b_truth_michelElectron_P;   //!
        TBranch        *b_truth_michelMuon_P;   //!
        TBranch        *b_truth_michelMuon_end_dist_vtx;   //!
        TBranch        *b_truth_michelMuon_length;   //!
        TBranch        *b_truth_michelPion_P;   //!
        TBranch        *b_truth_michelPion_begin_dist_vtx;   //!
        TBranch        *b_truth_michelPion_length;   //!
        TBranch        *b_truth_muon_P;   //!
        TBranch        *b_truth_muon_P_BeforeFSI;   //!
        TBranch        *b_truth_muon_theta;   //!
        TBranch        *b_truth_muon_thetaX_beam;   //!
        TBranch        *b_truth_muon_thetaY_beam;   //!
        TBranch        *b_truth_muon_theta_beam;   //!
        TBranch        *b_truth_muon_theta_beam_BeforeFSI;   //!
        TBranch        *b_truth_pi0_KE;   //!
        TBranch        *b_truth_pi0_KE_BeforeFSI;   //!
        TBranch        *b_truth_pi0_P;   //!
        TBranch        *b_truth_pi0_P_BeforeFSI;   //!
        TBranch        *b_truth_pi0_theta;   //!
        TBranch        *b_truth_pi0_theta_beam;   //!
        TBranch        *b_truth_pi0_theta_beam_BeforeFSI;   //!
        TBranch        *b_truth_proton_P;   //!
        TBranch        *b_truth_proton_theta;   //!
        TBranch        *b_truth_proton_theta_beam;   //!
        TBranch        *b_truth_total_captured_evis_pizero;   //!
        TBranch        *b_truth_total_captured_evis_total_norm;   //!
        TBranch        *b_truth_total_captured_evis_total_truth;   //!
        TBranch        *b_truth_track_michel_evis_total_truth;   //!
        TBranch        *b_truth_vtx_michel_evis_total_truth;   //!
        TBranch        *b_truth_vtx_michel_large_evis_total_truth;   //!
        TBranch        *b_truth_gamma1_4P;   //!
        TBranch        *b_truth_gamma1_final_pos;   //!
        TBranch        *b_truth_gamma1_final_pos_estimated;   //!
        TBranch        *b_truth_gamma1_init_pos;   //!
        TBranch        *b_truth_gamma2_4P;   //!
        TBranch        *b_truth_gamma2_final_pos;   //!
        TBranch        *b_truth_gamma2_final_pos_estimated;   //!
        TBranch        *b_truth_gamma2_init_pos;   //!
        TBranch        *b_genie_wgt_n_shifts;   //!
        TBranch        *b_truth_genie_wgt_AGKYxF1pi;   //!
        TBranch        *b_truth_genie_wgt_AhtBY;   //!
        TBranch        *b_truth_genie_wgt_BhtBY;   //!
        TBranch        *b_truth_genie_wgt_CCQEPauliSupViaKF;   //!
        TBranch        *b_truth_genie_wgt_CV1uBY;   //!
        TBranch        *b_truth_genie_wgt_CV2uBY;   //!
        TBranch        *b_truth_genie_wgt_EtaNCEL;   //!
        TBranch        *b_truth_genie_wgt_FrAbs_N;   //!
        TBranch        *b_truth_genie_wgt_FrAbs_pi;   //!
        TBranch        *b_truth_genie_wgt_FrCEx_N;   //!
        TBranch        *b_truth_genie_wgt_FrCEx_pi;   //!
        TBranch        *b_truth_genie_wgt_FrElas_N;   //!
        TBranch        *b_truth_genie_wgt_FrElas_pi;   //!
        TBranch        *b_truth_genie_wgt_FrInel_N;   //!
        TBranch        *b_truth_genie_wgt_FrInel_pi;   //!
        TBranch        *b_truth_genie_wgt_FrPiProd_N;   //!
        TBranch        *b_truth_genie_wgt_FrPiProd_pi;   //!
        TBranch        *b_truth_genie_wgt_MFP_N;   //!
        TBranch        *b_truth_genie_wgt_MFP_pi;   //!
        TBranch        *b_truth_genie_wgt_MaCCQE;   //!
        TBranch        *b_truth_genie_wgt_MaCCQEshape;   //!
        TBranch        *b_truth_genie_wgt_MaNCEL;   //!
        TBranch        *b_truth_genie_wgt_MaRES;   //!
        TBranch        *b_truth_genie_wgt_MvRES;   //!
        TBranch        *b_truth_genie_wgt_NormCCQE;   //!
        TBranch        *b_truth_genie_wgt_NormCCRES;   //!
        TBranch        *b_truth_genie_wgt_NormDISCC;   //!
        TBranch        *b_truth_genie_wgt_NormNCRES;   //!
        TBranch        *b_truth_genie_wgt_RDecBR1gamma;   //!
        TBranch        *b_truth_genie_wgt_Rvn1pi;   //!
        TBranch        *b_truth_genie_wgt_Rvn2pi;   //!
        TBranch        *b_truth_genie_wgt_Rvp1pi;   //!
        TBranch        *b_truth_genie_wgt_Rvp2pi;   //!
        TBranch        *b_truth_genie_wgt_Theta_Delta2Npi;   //!
        TBranch        *b_truth_genie_wgt_VecFFCCQEshape;   //!
        TBranch        *b_truth_genie_wgt_shifts;   //!
        TBranch        *b_truth_michelMuon_endPoint;   //!
        TBranch        *b_truth_muon_4P;   //!
        TBranch        *b_truth_pi0_4P;   //!
        TBranch        *b_truth_proton_4P;   //!
        TBranch        *b_physEvtNum;   //!
        TBranch        *b_n_hyps;   //!
        TBranch        *b_processType;   //!
        TBranch        *b_primaryPart;   //!
        TBranch        *b_n_slices;   //!
        TBranch        *b_slice_numbers;   //!
        TBranch        *b_shared_slice;   //!
        TBranch        *b_vtx;   //!
        TBranch        *b_vtxErr;   //!
        TBranch        *b_E;   //!
        TBranch        *b_found_truth;   //!
        TBranch        *b_ImprovedMichel_EventHasMichel;   //!
        TBranch        *b_ImprovedMichel_EventHasMatchedMichel;   //!
        TBranch        *b_ImprovedMichel_VertexHasMichel;   //!
        TBranch        *b_ImprovedMichel_EndPointHasMichel;   //!
        TBranch        *b_ImprovedMichel_secEndPointHasMichel;   //!
        TBranch        *b_ImprovedMichel_Gamma1HasMichel;   //!
        TBranch        *b_ImprovedMichel_Gamma2HasMichel;   //!
        TBranch        *b_is_blobs_recovered;   //!
        TBranch        *b_is_blobs_recovered_direction;   //!
        TBranch        *b_is_blobs_recovered_invMass;   //!
        TBranch        *b_is_blobs_recovered_small_angle;   //!
        TBranch        *b_is_blobs_recovered_search_view_U;   //!
        TBranch        *b_is_blobs_recovered_search_view_V;   //!
        TBranch        *b_gamma1_isMichel_begin;   //!
        TBranch        *b_gamma1_isMichel_end;   //!
        TBranch        *b_gamma2_isMichel_begin;   //!
        TBranch        *b_gamma2_isMichel_end;   //!
        TBranch        *b_Cut_BlobDirectionBad;   //!
        TBranch        *b_Cut_ConeBlobs;   //!
        TBranch        *b_Cut_EndPoint_Michel_Exist;   //!
        TBranch        *b_Cut_Muon_Charge;   //!
        TBranch        *b_Cut_Muon_None;   //!
        TBranch        *b_Cut_Particle_None;   //!
        TBranch        *b_Cut_Pi0_Bad;   //!
        TBranch        *b_Cut_PreFilter_Pi0;   //!
        TBranch        *b_Cut_Proton_Bad;   //!
        TBranch        *b_Cut_Proton_None;   //!
        TBranch        *b_Cut_Vertex_Large_Michel_Exist;   //!
        TBranch        *b_Cut_Vertex_Michel_Exist;   //!
        TBranch        *b_Cut_Vertex_None;   //!
        TBranch        *b_Cut_Vertex_Not_Fiducial;   //!
        TBranch        *b_Cut_Vertex_Not_Reconstructable;   //!
        TBranch        *b_Cut_secEndPoint_Michel_Exist;   //!
        TBranch        *b_anglescan_ncand;   //!
        TBranch        *b_anglescan_ncandx;   //!
        TBranch        *b_anglescan_nfoundBlobs;   //!
        TBranch        *b_detmc_ntrajectory;   //!
        TBranch        *b_detmc_ntrajectory2;   //!
        TBranch        *b_g1blob_1ParFit_ndof;   //!
        TBranch        *b_g1dedx_doublet;   //!
        TBranch        *b_g1dedx_empty_plane;   //!
        TBranch        *b_g1dedx_nplane;   //!
        TBranch        *b_g2blob_1ParFit_ndof;   //!
        TBranch        *b_g2dedx_doublet;   //!
        TBranch        *b_g2dedx_empty_plane;   //!
        TBranch        *b_g2dedx_nplane;   //!
        TBranch        *b_gamma1_blob_nclusters;   //!
        TBranch        *b_gamma1_blob_ndigits;   //!
        TBranch        *b_gamma2_blob_nclusters;   //!
        TBranch        *b_gamma2_blob_ndigits;   //!
        TBranch        *b_muon_N_minosTracks;   //!
        TBranch        *b_muon_charge;   //!
        TBranch        *b_muon_hasMinosMatchStub;   //!
        TBranch        *b_muon_hasMinosMatchTrack;   //!
        TBranch        *b_muon_minervaTrack_types;   //!
        TBranch        *b_muon_minosTrackQuality;   //!
        TBranch        *b_muon_roadUpstreamPlanes;   //!
        TBranch        *b_nProtonCandidates;   //!
        TBranch        *b_nTracks;   //!
        TBranch        *b_nTracks_Close;   //!
        TBranch        *b_nTracks_Discarded;   //!
        TBranch        *b_nTracks_Far;   //!
        TBranch        *b_od_energeticTower;   //!
        TBranch        *b_preFilter_Result;   //!
        TBranch        *b_proton_kinked;   //!
        TBranch        *b_proton_leadingIndice;   //!
        TBranch        *b_vtx_fit_converged;   //!
        TBranch        *b_vtx_module;   //!
        TBranch        *b_vtx_plane;   //!
        TBranch        *b_vtx_primary_index;   //!
        TBranch        *b_vtx_primary_multiplicity;   //!
        TBranch        *b_vtx_secondary_count;   //!
        TBranch        *b_vtx_total_count;   //!
        TBranch        *b_ConeBlobs_usable_evis_Tracker;   //!
        TBranch        *b_Coneblobs_usable_evis_ECAL;   //!
        TBranch        *b_Coneblobs_usable_evis_HCAL;   //!
        TBranch        *b_Extra_Energy_Leftover;   //!
        TBranch        *b_Extra_Energy_Muon;   //!
        TBranch        *b_Extra_Energy_Rejected;   //!
        TBranch        *b_Extra_Energy_Total;   //!
        TBranch        *b_g1blob_1ParFit_fval;   //!
        TBranch        *b_g1blob_2ParFit_vtx_distance;   //!
        TBranch        *b_g1dedx;   //!
        TBranch        *b_g1dedx1;   //!
        TBranch        *b_g1dedx_total;   //!
        TBranch        *b_g1dedx_total1;   //!
        TBranch        *b_g2blob_1ParFit_fval;   //!
        TBranch        *b_g2blob_2ParFit_vtx_distance;   //!
        TBranch        *b_g2dedx;   //!
        TBranch        *b_g2dedx1;   //!
        TBranch        *b_g2dedx_total;   //!
        TBranch        *b_g2dedx_total1;   //!
        TBranch        *b_gamma1_E;   //!
        TBranch        *b_gamma1_E_Old;   //!
        TBranch        *b_gamma1_P;   //!
        TBranch        *b_gamma1_blob_energy;   //!
        TBranch        *b_gamma1_blob_minsep;   //!
        TBranch        *b_gamma1_dEdx;   //!
        TBranch        *b_gamma1_dist_vtx;   //!
        TBranch        *b_gamma1_energy_ecal;   //!
        TBranch        *b_gamma1_energy_hcal;   //!
        TBranch        *b_gamma1_energy_scal_UV;   //!
        TBranch        *b_gamma1_energy_scal_X;   //!
        TBranch        *b_gamma1_energy_trkr;   //!
        TBranch        *b_gamma1_evis_ecal;   //!
        TBranch        *b_gamma1_evis_hcal;   //!
        TBranch        *b_gamma1_evis_scal_UV;   //!
        TBranch        *b_gamma1_evis_scal_X;   //!
        TBranch        *b_gamma1_evis_trkr;   //!
        TBranch        *b_gamma1_maxZ;   //!
        TBranch        *b_gamma1_phi;   //!
        TBranch        *b_gamma1_phi_beam;   //!
        TBranch        *b_gamma1_px;   //!
        TBranch        *b_gamma1_py;   //!
        TBranch        *b_gamma1_pz;   //!
        TBranch        *b_gamma1_theta;   //!
        TBranch        *b_gamma1_theta_beam;   //!
        TBranch        *b_gamma1_time;   //!
        TBranch        *b_gamma2_E;   //!
        TBranch        *b_gamma2_E_Old;   //!
        TBranch        *b_gamma2_P;   //!
        TBranch        *b_gamma2_blob_energy;   //!
        TBranch        *b_gamma2_blob_minsep;   //!
        TBranch        *b_gamma2_dEdx;   //!
        TBranch        *b_gamma2_dist_vtx;   //!
        TBranch        *b_gamma2_energy_ecal;   //!
        TBranch        *b_gamma2_energy_hcal;   //!
        TBranch        *b_gamma2_energy_scal_UV;   //!
        TBranch        *b_gamma2_energy_scal_X;   //!
        TBranch        *b_gamma2_energy_trkr;   //!
        TBranch        *b_gamma2_evis_ecal;   //!
        TBranch        *b_gamma2_evis_hcal;   //!
        TBranch        *b_gamma2_evis_scal_UV;   //!
        TBranch        *b_gamma2_evis_scal_X;   //!
        TBranch        *b_gamma2_evis_trkr;   //!
        TBranch        *b_gamma2_maxZ;   //!
        TBranch        *b_gamma2_phi;   //!
        TBranch        *b_gamma2_phi_beam;   //!
        TBranch        *b_gamma2_px;   //!
        TBranch        *b_gamma2_py;   //!
        TBranch        *b_gamma2_pz;   //!
        TBranch        *b_gamma2_theta;   //!
        TBranch        *b_gamma2_theta_beam;   //!
        TBranch        *b_gamma2_time;   //!
        TBranch        *b_muon_E;   //!
        TBranch        *b_muon_E_shift;   //!
        TBranch        *b_muon_KE;   //!
        TBranch        *b_muon_P;   //!
        TBranch        *b_muon_muScore;   //!
        TBranch        *b_muon_phi;   //!
        TBranch        *b_muon_phi_beam;   //!
        TBranch        *b_muon_px;   //!
        TBranch        *b_muon_py;   //!
        TBranch        *b_muon_pz;   //!
        TBranch        *b_muon_qp;   //!
        TBranch        *b_muon_qpqpe;   //!
        TBranch        *b_muon_roadUpstreamEnergy;   //!
        TBranch        *b_muon_theta;   //!
        TBranch        *b_muon_thetaX_beam;   //!
        TBranch        *b_muon_thetaY_beam;   //!
        TBranch        *b_muon_theta_beam;   //!
        TBranch        *b_muon_theta_beam_biasDown;   //!
        TBranch        *b_muon_theta_beam_biasUp;   //!
        TBranch        *b_od_downstreamFrame;   //!
        TBranch        *b_od_downstreamFrame_z;   //!
        TBranch        *b_od_highStory;   //!
        TBranch        *b_od_highStory_t;   //!
        TBranch        *b_od_lowStory;   //!
        TBranch        *b_od_lowStory_t;   //!
        TBranch        *b_od_maxEnergy;   //!
        TBranch        *b_od_upstreamFrame;   //!
        TBranch        *b_od_upstreamFrame_z;   //!
        TBranch        *b_pi0_E;   //!
        TBranch        *b_pi0_E_Cal;   //!
        TBranch        *b_pi0_KE;   //!
        TBranch        *b_pi0_P;   //!
        TBranch        *b_pi0_cos_openingAngle;   //!
        TBranch        *b_pi0_invMass;   //!
        TBranch        *b_truth_pi0_invMass;   //!
        TBranch        *b_pi0_invMass_Old;   //!
        TBranch        *b_pi0_openingAngle;   //!
        TBranch        *b_pi0_phi;   //!
        TBranch        *b_pi0_phi_beam;   //!
        TBranch        *b_pi0_px;   //!
        TBranch        *b_pi0_py;   //!
        TBranch        *b_pi0_pz;   //!
        TBranch        *b_pi0_theta;   //!
        TBranch        *b_pi0_theta_beam;   //!
        TBranch        *b_pi0_theta_beam_biasDown;   //!
        TBranch        *b_pi0_theta_beam_biasUp;   //!
        TBranch        *b_preFilter_evis_ECAL;   //!
        TBranch        *b_preFilter_evis_HCAL;   //!
        TBranch        *b_preFilter_evis_NuclearTarget;   //!
        TBranch        *b_preFilter_evis_TotalExceptNuclearTarget;   //!
        TBranch        *b_preFilter_evis_Tracker;   //!
        TBranch        *b_preFilter_evis_nearvtx;   //!
        TBranch        *b_preFilter_evis_total;   //!
        TBranch        *b_preFilter_rejectedEnergy;   //!
        TBranch        *b_proton_E;   //!
        TBranch        *b_proton_KE;   //!
        TBranch        *b_proton_LLRScore;   //!
        TBranch        *b_proton_P;   //!
        TBranch        *b_proton_energy_shift_BetheBloch_Down;   //!
        TBranch        *b_proton_energy_shift_BetheBloch_Up;   //!
        TBranch        *b_proton_energy_shift_Birks;   //!
        TBranch        *b_proton_energy_shift_MEU_Down;   //!
        TBranch        *b_proton_energy_shift_MEU_Up;   //!
        TBranch        *b_proton_energy_shift_Mass_Down;   //!
        TBranch        *b_proton_energy_shift_Mass_Up;   //!
        TBranch        *b_proton_energy_shift_Nominal;   //!
        TBranch        *b_proton_length;   //!
        TBranch        *b_proton_phi;   //!
        TBranch        *b_proton_phi_beam;   //!
        TBranch        *b_proton_pionScore;   //!
        TBranch        *b_proton_protonScore;   //!
        TBranch        *b_proton_px;   //!
        TBranch        *b_proton_py;   //!
        TBranch        *b_proton_pz;   //!
        TBranch        *b_proton_score1_shift_BetheBloch_Down;   //!
        TBranch        *b_proton_score1_shift_BetheBloch_Up;   //!
        TBranch        *b_proton_score1_shift_Birks;   //!
        TBranch        *b_proton_score1_shift_MEU_Down;   //!
        TBranch        *b_proton_score1_shift_MEU_Up;   //!
        TBranch        *b_proton_score1_shift_Mass_Down;   //!
        TBranch        *b_proton_score1_shift_Mass_Up;   //!
        TBranch        *b_proton_score1_shift_Nominal;   //!
        TBranch        *b_proton_theta;   //!
        TBranch        *b_proton_theta_beam;   //!
        TBranch        *b_proton_ln_Q0;
        TBranch        *b_proton_ln_Q1;
        TBranch        *b_proton_ln_Q2;
        TBranch        *b_proton_ln_Q3;
        TBranch        *b_proton_ln_Q4;
        TBranch        *b_proton_ln_Q5;
        TBranch        *b_reco_eventID;   //!
        TBranch        *b_time;   //!
        TBranch        *b_track_michelProng_begin_Z;   //!
        TBranch        *b_track_michelProng_distance;   //!
        TBranch        *b_track_michelProng_end_Z;   //!
        TBranch        *b_track_michelProng_energy;   //!
        TBranch        *b_track_michelProng_time_diff;   //!
        TBranch        *b_vertex_blob_energy;   //!
        TBranch        *b_vertex_blob_evis;   //!
        TBranch        *b_vtx_fit_chi2;   //!
        TBranch        *b_vtx_michelProng_Large_begin_Z;   //!
        TBranch        *b_vtx_michelProng_Large_distance;   //!
        TBranch        *b_vtx_michelProng_Large_end_Z;   //!
        TBranch        *b_vtx_michelProng_Large_energy;   //!
        TBranch        *b_vtx_michelProng_Large_time_diff;   //!
        TBranch        *b_vtx_michelProng_begin_Z;   //!
        TBranch        *b_vtx_michelProng_distance;   //!
        TBranch        *b_vtx_michelProng_end_Z;   //!
        TBranch        *b_vtx_michelProng_energy;   //!
        TBranch        *b_vtx_michelProng_time_diff;   //!
        TBranch        *b_vtx_x;   //!
        TBranch        *b_vtx_y;   //!
        TBranch        *b_vtx_z;   //!
        TBranch        *b_all_protons_kinked;   //!
        TBranch        *b_all_protons_odMatch;   //!
        TBranch        *b_detmc_traj_id_sz;   //!
        TBranch        *b_detmc_traj_id;   //!
        TBranch        *b_detmc_traj_mother_sz;   //!
        TBranch        *b_detmc_traj_mother;   //!
        TBranch        *b_detmc_traj_pdg_sz;   //!
        TBranch        *b_detmc_traj_pdg;   //!
        TBranch        *b_detmc_traj_proc_sz;   //!
        TBranch        *b_detmc_traj_proc;   //!
        TBranch        *b_detmc_traj_status_sz;   //!
        TBranch        *b_detmc_traj_status;   //!
        TBranch        *b_g1dedx_cluster_occupancy_sz;   //!
        TBranch        *b_g1dedx_cluster_occupancy;   //!
        TBranch        *b_g2dedx_cluster_occupancy_sz;   //!
        TBranch        *b_g2dedx_cluster_occupancy;   //!
        TBranch        *b_nTracks_Secondary_Vtx_sz;   //!
        TBranch        *b_nTracks_Secondary_Vtx;   //!
        TBranch        *b_all_protons_E;   //!
        TBranch        *b_all_protons_KE;   //!
        TBranch        *b_all_protons_LLRScore;   //!
        TBranch        *b_all_protons_P;   //!
        TBranch        *b_all_protons_chi2_ndf;   //!
        TBranch        *b_all_protons_endPointX;   //!
        TBranch        *b_all_protons_endPointY;   //!
        TBranch        *b_all_protons_endPointZ;   //!
        TBranch        *b_all_protons_energy_shift_BetheBloch_Down;   //!
        TBranch        *b_all_protons_energy_shift_BetheBloch_Up;   //!
        TBranch        *b_all_protons_energy_shift_Birks;   //!
        TBranch        *b_all_protons_energy_shift_MEU_Down;   //!
        TBranch        *b_all_protons_energy_shift_MEU_Up;   //!
        TBranch        *b_all_protons_energy_shift_Mass_Down;   //!
        TBranch        *b_all_protons_energy_shift_Mass_Up;   //!
        TBranch        *b_all_protons_energy_shift_Nominal;   //!
        TBranch        *b_all_protons_length;   //!
        TBranch        *b_all_protons_p_calCorrection;   //!
        TBranch        *b_all_protons_p_dEdXTool;   //!
        TBranch        *b_all_protons_p_visEnergy;   //!
        TBranch        *b_all_protons_phi;   //!
        TBranch        *b_all_protons_phi_beam;   //!
        TBranch        *b_all_protons_pionScore;   //!
        TBranch        *b_all_protons_protonScore;   //!
        TBranch        *b_all_protons_px;   //!
        TBranch        *b_all_protons_py;   //!
        TBranch        *b_all_protons_pz;   //!
        TBranch        *b_all_protons_score1_shift_BetheBloch_Down;   //!
        TBranch        *b_all_protons_score1_shift_BetheBloch_Up;   //!
        TBranch        *b_all_protons_score1_shift_Birks;   //!
        TBranch        *b_all_protons_score1_shift_MEU_Down;   //!
        TBranch        *b_all_protons_score1_shift_MEU_Up;   //!
        TBranch        *b_all_protons_score1_shift_Mass_Down;   //!
        TBranch        *b_all_protons_score1_shift_Mass_Up;   //!
        TBranch        *b_all_protons_score1_shift_Nominal;   //!
        TBranch        *b_all_protons_startPointX;   //!
        TBranch        *b_all_protons_startPointY;   //!
        TBranch        *b_all_protons_startPointZ;   //!
        TBranch        *b_all_protons_theta;   //!
        TBranch        *b_all_protons_theta_beam;   //!
        TBranch        *b_detmc_traj_E0_sz;   //!
        TBranch        *b_detmc_traj_E0;   //!
        TBranch        *b_detmc_traj_Ef_sz;   //!
        TBranch        *b_detmc_traj_Ef;   //!
        TBranch        *b_detmc_traj_preEf_sz;   //!
        TBranch        *b_detmc_traj_preEf;   //!
        TBranch        *b_detmc_traj_prepxf_sz;   //!
        TBranch        *b_detmc_traj_prepxf;   //!
        TBranch        *b_detmc_traj_prepyf_sz;   //!
        TBranch        *b_detmc_traj_prepyf;   //!
        TBranch        *b_detmc_traj_prepzf_sz;   //!
        TBranch        *b_detmc_traj_prepzf;   //!
        TBranch        *b_detmc_traj_px0_sz;   //!
        TBranch        *b_detmc_traj_px0;   //!
        TBranch        *b_detmc_traj_pxf_sz;   //!
        TBranch        *b_detmc_traj_pxf;   //!
        TBranch        *b_detmc_traj_py0_sz;   //!
        TBranch        *b_detmc_traj_py0;   //!
        TBranch        *b_detmc_traj_pyf_sz;   //!
        TBranch        *b_detmc_traj_pyf;   //!
        TBranch        *b_detmc_traj_pz0_sz;   //!
        TBranch        *b_detmc_traj_pz0;   //!
        TBranch        *b_detmc_traj_pzf_sz;   //!
        TBranch        *b_detmc_traj_pzf;   //!
        TBranch        *b_detmc_traj_t0_sz;   //!
        TBranch        *b_detmc_traj_t0;   //!
        TBranch        *b_detmc_traj_tf_sz;   //!
        TBranch        *b_detmc_traj_tf;   //!
        TBranch        *b_detmc_traj_x0_sz;   //!
        TBranch        *b_detmc_traj_x0;   //!
        TBranch        *b_detmc_traj_xf_sz;   //!
        TBranch        *b_detmc_traj_xf;   //!
        TBranch        *b_detmc_traj_y0_sz;   //!
        TBranch        *b_detmc_traj_y0;   //!
        TBranch        *b_detmc_traj_yf_sz;   //!
        TBranch        *b_detmc_traj_yf;   //!
        TBranch        *b_detmc_traj_z0_sz;   //!
        TBranch        *b_detmc_traj_z0;   //!
        TBranch        *b_detmc_traj_zf_sz;   //!
        TBranch        *b_detmc_traj_zf;   //!
        TBranch        *b_fit_vtx;   //!
        TBranch        *b_g1dedx_cluster_energy_sz;   //!
        TBranch        *b_g1dedx_cluster_energy;   //!
        TBranch        *b_g1dedx_rev_cluster_energy_sz;   //!
        TBranch        *b_g1dedx_rev_cluster_energy;   //!
        TBranch        *b_g2dedx_cluster_energy_sz;   //!
        TBranch        *b_g2dedx_cluster_energy;   //!
        TBranch        *b_g2dedx_rev_cluster_energy_sz;   //!
        TBranch        *b_g2dedx_rev_cluster_energy;   //!
        TBranch        *b_gamma1_direction;   //!
        TBranch        *b_gamma1_end_vertex;   //!
        TBranch        *b_gamma1_vertex;   //!
        TBranch        *b_gamma2_direction;   //!
        TBranch        *b_gamma2_end_vertex;   //!
        TBranch        *b_gamma2_vertex;   //!
        TBranch        *b_muon_thetaX_allNodes_sz;   //!
        TBranch        *b_muon_thetaX_allNodes;   //!
        TBranch        *b_muon_thetaY_allNodes_sz;   //!
        TBranch        *b_muon_thetaY_allNodes;   //!
        TBranch        *b_muon_theta_allNodes_sz;   //!
        TBranch        *b_muon_theta_allNodes;   //!
        TBranch        *b_od_distanceBlobTower_sz;   //!
        TBranch        *b_od_distanceBlobTower;   //!
        TBranch        *b_od_idBlobTime_sz;   //!
        TBranch        *b_od_idBlobTime;   //!
        TBranch        *b_od_towerEnergy_sz;   //!
        TBranch        *b_od_towerEnergy;   //!
        TBranch        *b_od_towerNClusters_sz;   //!
        TBranch        *b_od_towerNClusters;   //!
        TBranch        *b_od_towerTime_sz;   //!
        TBranch        *b_od_towerTime;   //!
        TBranch        *b_od_towerTimeBlobMuon_sz;   //!
        TBranch        *b_od_towerTimeBlobMuon;   //!
        TBranch        *b_od_towerTimeBlobOD_sz;   //!
        TBranch        *b_od_towerTimeBlobOD;   //!
        TBranch        *b_ev_run;   //!
        TBranch        *b_ev_subrun;   //!
        TBranch        *b_ev_detector;   //!
        TBranch        *b_ev_triggerType;   //!
        TBranch        *b_ev_gate;   //!
        TBranch        *b_ev_global_gate;   //!
        TBranch        *b_ev_gps_time_sec;   //!
        TBranch        *b_ev_gps_time_usec;   //!
        TBranch        *b_mc_run;   //!
        TBranch        *b_mc_subrun;   //!
        TBranch        *b_mc_nInteractions;   //!
        TBranch        *b_mc_MIState;   //!
        TBranch        *b_mc_pot;   //!
        TBranch        *b_mc_beamConfig;   //!
        TBranch        *b_mc_processType;   //!
        TBranch        *b_mc_nthEvtInSpill;   //!
        TBranch        *b_mc_nthEvtInFile;   //!
        TBranch        *b_mc_intType;   //!
        TBranch        *b_mc_current;   //!
        TBranch        *b_mc_charm;   //!
        TBranch        *b_mc_weight;   //!
        TBranch        *b_mc_XSec;   //!
        TBranch        *b_mc_diffXSec;   //!
        TBranch        *b_mc_incoming;   //!
        TBranch        *b_mc_fluxDriverProb;   //!
        TBranch        *b_mc_targetNucleus;   //!
        TBranch        *b_mc_targetZ;   //!
        TBranch        *b_mc_targetA;   //!
        TBranch        *b_mc_targetNucleon;   //!
        TBranch        *b_mc_struckQuark;   //!
        TBranch        *b_mc_seaQuark;   //!
        TBranch        *b_mc_resID;   //!
        TBranch        *b_mc_primaryLepton;   //!
        TBranch        *b_mc_incomingE;   //!
        TBranch        *b_mc_Bjorkenx;   //!
        TBranch        *b_mc_Bjorkeny;   //!
        TBranch        *b_mc_Q2;   //!
        TBranch        *b_mc_nuT;   //!
        TBranch        *b_mc_w;   //!
        TBranch        *b_mc_vtx;   //!
        TBranch        *b_mc_incomingPartVec;   //!
        TBranch        *b_mc_initNucVec;   //!
        TBranch        *b_mc_primFSLepton;   //!
        TBranch        *b_mc_nFSPart;   //!
        TBranch        *b_mc_FSPartPx;   //!
        TBranch        *b_mc_FSPartPy;   //!
        TBranch        *b_mc_FSPartPz;   //!
        TBranch        *b_mc_FSPartE;   //!
        TBranch        *b_mc_FSPartPDG;   //!
        TBranch        *b_mc_er_nPart;   //!
        TBranch        *b_mc_er_ID;   //!
        TBranch        *b_mc_er_status;   //!
        TBranch        *b_mc_er_posInNucX;   //!
        TBranch        *b_mc_er_posInNucY;   //!
        TBranch        *b_mc_er_posInNucZ;   //!
        TBranch        *b_mc_er_Px;   //!
        TBranch        *b_mc_er_Py;   //!
        TBranch        *b_mc_er_Pz;   //!
        TBranch        *b_mc_er_E;   //!
        TBranch        *b_mc_er_FD;   //!
        TBranch        *b_mc_er_LD;   //!
        TBranch        *b_mc_er_mother;   //!
        TBranch        *b_mc_fr_nNuAncestorIDs;   //!
        TBranch        *b_mc_fr_nuAncestorIDs;   //!
        TBranch        *b_mc_fr_nuParentID;   //!
        TBranch        *b_mc_fr_decMode;   //!
        TBranch        *b_mc_fr_primProtonVtx;   //!
        TBranch        *b_mc_fr_primProtonP;   //!
        TBranch        *b_mc_fr_nuParentDecVtx;   //!
        TBranch        *b_mc_fr_nuParentProdVtx;   //!
        TBranch        *b_mc_fr_nuParentProdP;   //!
        TBranch        *b_mc_cvweight_total;   //!
        TBranch        *b_wgt;   //!
        TBranch        *b_mc_cvweight_totalFlux;   //!
        TBranch        *b_mc_cvweight_totalXsec;   //!
        TBranch        *b_mc_ppfx1_cvweight;   //!
        TBranch        *b_mc_hornCurrent_cvweight;   //!
        TBranch        *b_mc_gen1_cvweight_total;   //!
        TBranch        *b_gen1_wgt;   //!
        TBranch        *b_mc_gen1_cvweight_totalFlux;   //!
        TBranch        *b_mc_gen1_cvweight_NA49;   //!
        TBranch        *b_mc_wgt_Flux_BeamFocus_sz;   //!
        TBranch        *b_mc_wgt_Flux_BeamFocus;   //!
        TBranch        *b_mc_wgt_gen1_Flux_Tertiary_sz;   //!
        TBranch        *b_mc_wgt_gen1_Flux_Tertiary;   //!
        TBranch        *b_mc_wgt_gen1_Flux_NA49_sz;   //!
        TBranch        *b_mc_wgt_gen1_Flux_NA49;   //!
        TBranch        *b_mc_wgt_Norm_sz;   //!
        TBranch        *b_mc_wgt_Norm;   //!
        TBranch        *b_mc_wgt_ppfx1_Total_sz;   //!
        TBranch        *b_mc_wgt_ppfx1_Total;   //!
        TBranch        *b_n_prongs;   //!
        TBranch        *b_prong_nParticles;   //!
        TBranch        *b_prong_part_score;   //!
        TBranch        *b_prong_part_mass;   //!
        TBranch        *b_prong_part_charge;   //!
        TBranch        *b_prong_part_pid;   //!

};

#endif