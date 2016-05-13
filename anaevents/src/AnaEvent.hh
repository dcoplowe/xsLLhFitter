//////////////////////////////////////////////////////////
//
//  A container with subset of necessary event
//  information for CCQE analysis
//
//
//  Created: Thu Jun  6 11:28:13 CEST 2013
//  Modified:
//
//////////////////////////////////////////////////////////
#ifndef __AnaEvent_hh__
#define __AnaEvent_hh__

#include <iostream>

#include <TMath.h>

class AnaEvent
{
public:
    AnaEvent(Long64_t evid) //unique event id
    {
        m_evid       = evid;
        m_evtype     = 0;
        m_reaction   = -1;
        m_sample     = -1;
        m_trueEnu    = -999.0;
        m_recEnu     = -999.0;
        m_trueD1trk   = -999.0;
        m_trueThtrk  = -999.0;
        m_trueD2trk = -999.0;
        m_recD1trk    = -999.0;
        m_reD2trk   = -999.0;
        m_recD2trk  = -999.0;
        m_wght       = 1.0;
        m_wghtMC       = 1.0;
        
        //dpTT specific:
        m_target     = -999;
        m_nureaction = -999;
        
        // New kinematic variables always included for phase space cuts
        m_muMomRec = -999.0;
        m_muMomTrue = -999.0;
        m_muCosThetaRec = -999.0;
        m_muCosThetaTrue = -999.0;
        
        m_pMomRec = -999.0;
        m_pMomTrue = -999.0;
        m_pCosThetaRec = -999.0;
        m_pCosThetaTrue = -999.0;
        
        m_piMomRec = -999.0;
        m_piMomTrue = -999.0;
        m_piCosThetaRec = -999.0;
        m_piCosThetaTrue = -999.0;
        
    }
    ~AnaEvent(){;}
    
    //Set/Get methods
    void SetEvType(int val){ m_evtype = val; }
    int GetEvType(){ return m_evtype; }
    
    void SetReaction(int val){ m_reaction = val; }
    int GetReaction(){ return m_reaction; }
    
    void SetSampleType(int val){ m_sample = val; }
    int GetSampleType(){ return m_sample; }
    
    Long64_t GetEvId(){ return m_evid; }
    
    void SetTrueEnu(double val) {m_trueEnu = val;}
    double GetTrueEnu(){ return m_trueEnu; }
    
    void SetRecEnu(double val){ m_recEnu = val; }
    double GetRecEnu(){ return m_recEnu; }
    
    void SetTrueD1trk(double val){ m_trueD1trk = val; }
    double GetTrueD1trk(){ return m_trueD1trk; }
    
    void SetRecD1trk(double val){ m_recD1trk = val; }
    double GetRecD1trk(){ return m_recD1trk; }
    
    void SetTrueThtrk(double val)
    {
        m_trueThtrk  = val;
        m_trueD2trk = TMath::Cos(m_trueThtrk);
    }
    void SetTrueD2trk(double val)
    {
        m_trueD2trk = val;
        m_trueThtrk  = TMath::ACos(m_trueD2trk);
    }
    double GetTrueThtrk(){ return m_trueThtrk; }
    double GetTrueD2trk(){ return m_trueD2trk; }
    
    //dpTT Specific:
    void SetTarget(int val){ m_target = val; }
    int GetTarget(){ return m_target; }
    
    void SetNuReaction(int val){ m_nureaction = val; }
    int GetNuReaction(){ return m_nureaction; }
    
    void SetReD2trk(double val)
    {
        m_reD2trk  = val;
        m_recD2trk = TMath::Cos(m_reD2trk);
    }
    void SetRecD2trk(double val)
    {
        m_recD2trk = val;
        m_reD2trk  = TMath::ACos(m_recD2trk);
    }
    double GetReD2trk(){ return m_reD2trk; }
    double GetRecD2trk(){ return m_recD2trk; }
    
    void SetEvWght(double val){ m_wght  = val; }
    void SetEvWghtMC(double val){ m_wghtMC  = val; }
    void AddEvWght(double val){ m_wght *= val; }
    double GetEvWght(){ return m_wght; }
    double GetEvWghtMC(){ return m_wghtMC; }
    
    // New kinematic variables always included for phase space cuts
    
    void SetpMomRec(double val){ m_pMomRec = val; }
    void SetpMomTrue(double val){ m_pMomTrue = val; }
    void SetpCosThetaRec(double val){ m_pCosThetaRec = val; }
    void SetpCosThetaTrue(double val){ m_pCosThetaTrue = val; }

    void SetmuMomRec(double val){ m_muMomRec = val; }
    void SetmuMomTrue(double val){ m_muMomTrue = val; }
    void SetmuCosThetaRec(double val){ m_muCosThetaRec = val; }
    void SetmuCosThetaTrue(double val){ m_muCosThetaTrue = val; }
    
    void SetpiMomRec(double val){ m_piMomRec = val; }
    void SetpiMomTrue(double val){ m_piMomTrue = val; }
    void SetpiCosThetaRec(double val){ m_piCosThetaRec = val; }
    void SetpiCosThetaTrue(double val){ m_piCosThetaTrue = val; }
    
    double GetpMomRec(){ return m_pMomRec; }
    double GetpMomTrue(){ return m_pMomTrue; }
    double GetpCosThetaRec(){ return m_pCosThetaRec; }
    double GetpCosThetaTrue(){ return m_pCosThetaTrue; }
    
    double GetmuMomRec(){ return m_muMomRec; }
    double GetmuMomTrue(){ return m_muMomTrue; }
    double GetmuCosThetaRec(){ return m_muCosThetaRec; }
    double GetmuCosThetaTrue(){ return m_muCosThetaTrue; }
    
    double GetpiMomRec(){ return m_piMomRec; }
    double GetpiMomTrue(){ return m_piMomTrue; }
    double GetpiCosThetaRec(){ return m_piCosThetaRec; }
    double GetpiCosThetaTrue(){ return m_piCosThetaTrue; }
    
    
    void Print()
    {
        std::cout<<"Event ID "<<m_evid<<std::endl
        <<"Reaction        "<<GetReaction()<<std::endl
        <<"Sample          "<<GetSampleType()<<std::endl
        <<"True energy     "<<GetTrueEnu()<<std::endl
        <<"Recon energy    "<<GetRecEnu()<<std::endl
        <<"True track D1  "<<GetTrueD1trk()<<std::endl
        <<"Recon track D1 "<<GetRecD1trk()<<std::endl
        <<"True track D2  "<<GetTrueD2trk()<<std::endl
        <<"Recon track D2 "<<GetRecD2trk()<<std::endl
        <<"True track th   "<<GetTrueThtrk()<<std::endl
        <<"Recon track th  "<<GetReD2trk()<<std::endl
        <<"Event weight    "<<GetEvWght()<<std::endl
        <<"Event weight MC  "<<GetEvWghtMC()<<std::endl
        <<"Recon proton momentum  " <<GetpMomRec() <<std::endl
        <<"True proton momentum   " <<GetpMomTrue() <<std::endl
        <<"Recon pion momentum  " <<GetpiMomRec() <<std::endl
        <<"True pion momentum   " <<GetpiMomTrue() <<std::endl
        <<"Recon muon momentum    " <<GetmuMomRec() <<std::endl
        <<"True muon momentum     " <<GetmuMomTrue() <<std::endl
        <<"Recon muon cos theta   " <<GetmuCosThetaRec() <<std::endl
        <<"True muon cos theta    " <<GetmuCosThetaTrue() <<std::endl
        <<"Recon Proton Cos Theta " <<GetpCosThetaRec() <<std::endl
        <<"True Proton Cos Theta  " <<GetpCosThetaTrue() <<std::endl
        <<"Recon Pion Cos Theta " <<GetpiCosThetaRec() <<std::endl
        <<"True Pion Cos Theta  " <<GetpiCosThetaTrue() <<std::endl;
    }
    
private:
    Long64_t m_evid;     //unique event id
    int m_evtype;        //0 - MC, 1 - Data event
    int m_reaction;      //reaction type
    int m_sample;        //sample type (aka topology)
    double m_trueEnu;    //true nu energy
    double m_recEnu;     //recon nu energy
    double m_trueD1trk;   //true D1
    double m_trueThtrk;  //Old, do not use
    double m_trueD2trk; //true D2
    double m_recD1trk;    //recon D1
    double m_reD2trk;   //Old, do not use
    double m_recD2trk;  //recon D2
    double m_wght;       //event weight
    double m_wghtMC;       //event weight from original MC
    
    //dpTT specific:
    int m_target;        //neutrino target
    int m_nureaction;    //true neutrino reaction
    
    // New kinematic variables always included for phase space cuts
    double        m_piMomRec;
    double        m_piMomTrue;
    double        m_piCosThetaRec;
    double        m_piCosThetaTrue;
    
    double        m_pMomRec;
    double        m_pMomTrue;
    double        m_pCosThetaRec;
    double        m_pCosThetaTrue;
    
    double        m_muMomRec;
    double        m_muMomTrue;
    double        m_muCosThetaRec;
    double        m_muCosThetaTrue;
    
};

#endif
