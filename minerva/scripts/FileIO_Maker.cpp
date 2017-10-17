#ifndef __FILEIO_CPP__
#define __FILEIO_CPP__

#include <FileIO.h>

#include <iostream>
#include <TTree.h>
#include <TBranch.h>
#include <cassert>

using std::cout;
using std::endl;

void FileIO::InitLLNTuple()
{
    assert(m_LLtuple);

    // The following are required my the LL fitter. (DC 290917: Some may be redundent but this needs inverstigating)
    
    // This are the analysis bins:
    // If doing the 
    // Want to set these vars in main scope: Need to think about how this is set, probably need to be global vars in base, could then
    // set the vars in get entry
    Double_t D1true = kIniValue;
    Double_t D1Reco = kIniValue;
    Double_t D2true = kIniValue;
    Double_t D2Reco = kIniValue;

    m_LLtuple->Branch("D1True", &D1true, ("D1True/D"));
    m_LLtuple->Branch("D1Rec", &D1Reco, ("D1Rec/D"));
    m_LLtuple->Branch("D2True", &D2true, ("D2True/D"));
    m_LLtuple->Branch("D2Rec", &D2Reco, ("D2Rec/D"));

    // Other branches used by fitter:
    Int_t mectopology = kIniValue;

    // Analysis cut branches:
    m_LLtuple->Branch("reaction",       &mc_intType, "reaction/I");
    m_LLtuple->Branch("mectopology",    &mectopology, "mectopology/I");
    m_LLtuple->Branch("muMomRec",       &muon_P, "muMomRec/D");
    m_LLtuple->Branch("muMomTrue",      &truth_muon_P, "muMomTrue/D");
    m_LLtuple->Branch("muCosThetaRec",  &muon_theta, "muCosThetaRec/D");
    m_LLtuple->Branch("muCosThetaTrue", &truth_muon_theta, "muCosThetaTrue/D");
    m_LLtuple->Branch("pMomRec",        &proton_P, "pMomRec/D");
    m_LLtuple->Branch("pMomTrue",       &truth_proton_P, "pMomTrue/D");
    m_LLtuple->Branch("pCosThetaRec",   &proton_theta, "pCosThetaRec/D");
    m_LLtuple->Branch("pCosThetaTrue",  &truth_proton_theta, "pCosThetaTrue/D");

    // These need to be detemined in the post analysis software:
    // Could add them to reco E.
    Float_t RecoNuEnergy = kIniValue;
    Float_t TrueNuEnergy = kIniValue;

    m_LLtuple->Branch("Enureco",        &RecoNuEnergy, "Enureco/D");
    m_LLtuple->Branch("Enutrue",        &TrueNuEnergy, "Enutrue/D");

    m_LLtuple->Branch("weight",         &wgt, "weight/D");

    // nu_truereac doesn't exsit in this anal
    // m_LLtuple->SetBranchAddress("nu_truereac", &nu_truereac);
    m_LLtuple->Branch("target",             &mc_targetZ, "target/I");

    m_LLtuple->Branch("selpi_mom",          &pi0_P, "selpi_mom/D");
    m_LLtuple->Branch("selpi_truemom",      &truth_pi0_P, "selpitrue_mom/D");
    m_LLtuple->Branch("selpi_costheta",     &pi0_theta, "selpi_costheta/D");
    m_LLtuple->Branch("selpi_truecostheta", &truth_pi0_theta, "selpi_truecostheta/D");
}
