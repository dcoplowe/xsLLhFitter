#include "TransverseTools.h"

#ifndef __TRANSVERSETOOLS__CXX
#define __TRANSVERSETOOLS__CXX

#include "TVector3.h"
#include "TMath.h"
#include <iostream>
// #include <Kernel/MinervaPhysicalConstants.h> //MinervaUnits

#ifndef EPSILON
#define EPSILON  1e-10
#endif

using namespace std;

// This may not work:
Double_t TransverseTools::m_PDP_x = 0.231135;
Double_t TransverseTools::m_PDP_y = 45.368069;
Double_t TransverseTools::m_PDP_z = -766.384058;
 
const Double_t TransverseTools::m_Theta = -0.0582977560;
const Double_t TransverseTools::m_XOffset = 0.2486;
const Double_t TransverseTools::m_YOffset = 60.350;
const Double_t TransverseTools::m_ZOffset = -1022.74;

TransverseTools::TransverseTools(Double_t pdp_x, Double_t pdp_y, Double_t pdp_z) {
    //    m_PDP = new TVector3(m_PDP_x, m_PDP_y, m_PDP_z);
    m_PDP_x = pdp_x; 
    m_PDP_y = pdp_y;
    m_PDP_z = pdp_z;
}

TransverseTools::~TransverseTools(){}

Double_t TransverseTools::GetDPTTRec(Double_t vtx[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom)// const
{
    const TVector3 * nudir = GetNuDirRec(vtx);
    Double_t value = GetDPTTBase(nudir, mumom, prmom,pimom);
    delete nudir;
    return value;
}

Double_t TransverseTools::GetDPTTRec(std::vector<Double_t> vtx, const TVector3 *& mumom, const TVector3 *& prmom, 
const TVector3 *& pimom)// const
{
    Double_t vertex[3] = { vtx[0], vtx[1], vtx[2] };
    return GetDPTTRec(vertex, mumom, prmom, pimom);
}

Double_t TransverseTools::GetDPTTSim(Double_t vtx[], Double_t pdp[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom)// const
{
    const TVector3 * nudir = GetNuDirSim(vtx, pdp);
    Double_t value = GetDPTTBase(nudir, mumom, prmom,pimom);
    delete nudir;
    return value;
}

Double_t TransverseTools::GetDPTTSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, const TVector3 *& mumom, const TVector3 *& prmom,
 const TVector3 *& pimom)// const
{
    Double_t tmp_vtx[3] = { vtx[0], vtx[1], vtx[2] };
    Double_t tmp_pdp[3] = { pdp[0], pdp[1], pdp[2] };
    return GetDPTTSim(tmp_vtx, tmp_pdp, mumom, prmom, pimom);
}

Double_t TransverseTools::GetDPTTDir(Double_t dir[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom)
{
    const TVector3 * nudir = new TVector3(dir[0], dir[1], dir[2]);
    Double_t value = GetDPTTBase(nudir, mumom, prmom,pimom);
    delete nudir;
    return value;
}

Double_t TransverseTools::GetDPTTDir(std::vector<Double_t> dir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom)
{
    Double_t direction[3] = { dir[0], dir[1], dir[2] };
    return GetDPTTDir(direction, mumom, prmom, pimom);
}

Double_t TransverseTools::GetDPTTBase(const TVector3 *&nudir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom)// const
{
    TVector3 tmp1_vec = nudir->Cross(*mumom);
    tmp1_vec *= 1/tmp1_vec.Mag();   
    TVector3 sum_vec = *prmom + *pimom;
    return sum_vec.Dot(tmp1_vec);
}

TVector3 * TransverseTools::SetDPT(const TVector3 *& ptmuon, const TVector3 *& ptproton, const TVector3 *& ptpion)// const
{
    //ptmuon and ptproton already in the same plain which is perpendicular to the neutrino and already in a near back-to-back 
    //configuration
    TVector3 tmpd = (*ptmuon) + (*ptproton) + (*ptpion);
    TVector3 tmp_had = (*ptproton) + (*ptpion);
    
    Double_t phi = TMath::ACos( ptmuon->Dot(tmp_had)*(-1)/(ptmuon->Mag()*tmp_had.Mag()) );
    
    Double_t theta = TMath::ACos( tmpd.Dot(*ptmuon)*(-1)/(tmpd.Mag()*ptmuon->Mag())  );
    TVector3 * deltapt = new TVector3();
    deltapt->SetMagThetaPhi(tmpd.Mag(),theta, phi);
    return deltapt;
}

TVector3 * TransverseTools::GetVecT(const TVector3 *& refdir, const TVector3 *& mom)// const
{
    //w.r.t. beam direction   
    if(!refdir){
        cout << "TransverseTools::GetVecT refdir null" << endl;
        exit(1);
    }
    TVector3 vRotated(*mom);
    vRotated.Rotate(TMath::Pi(), *refdir);
    TVector3 *vt = new TVector3( (*mom - vRotated)*0.5 );
    return vt;
}

TVector3 * TransverseTools::GetPTRec(Double_t vtx[], const TVector3 *& mom)// const
{    
    const TVector3 * nudir = GetNuDirRec(vtx);
    TVector3 * tmp = GetVecT(nudir, mom);
    delete nudir;
    return tmp;
}

TVector3 * TransverseTools::GetPTRec(std::vector<Double_t> vtx, const TVector3 *& mom)
{
    Double_t tmp_vtx[3] = { vtx[0], vtx[1], vtx[2] };
    return GetPTRec(tmp_vtx, mom);
}

TVector3 * TransverseTools::GetPTSim(Double_t vtx[], Double_t pdp[], const TVector3 *& mom)// const
{    
    const TVector3 * nudir = GetNuDirSim(vtx, pdp);  
    TVector3 * tmp = GetVecT(nudir, mom);
    delete nudir;
    return tmp;
}

TVector3 * TransverseTools::GetPTSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, const TVector3 *& mom)
{
    Double_t tmp_vtx[3] = { vtx[0], vtx[1], vtx[2] };
    Double_t tmp_pdp[3] = { pdp[0], pdp[1], pdp[2] };
    return GetPTSim(tmp_vtx, tmp_pdp, mom);
}

TVector3 * TransverseTools::GetPTDir(Double_t dir[], const TVector3 *& mom)
{
    const TVector3 * nudir = new TVector3(dir[0], dir[1], dir[2]);  
    TVector3 * tmp = GetVecT(nudir, mom);
    delete nudir;
    return tmp;
}

TVector3 * TransverseTools::GetPTDir(std::vector<Double_t> dir, const TVector3 *& mom)
{
    Double_t vec[3] = { dir[0], dir[1], dir[2] };
    return GetPTDir(vec, mom);
}

TVector3 * TransverseTools::GetTransVarsRec(Double_t vtx[], const TVector3 *& mumom, const TVector3 *& prmom,
 const TVector3 *& pimom, Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)// const
{
    const TVector3 * nudir = GetNuDirRec(vtx);
    return GetTransVarsBase(nudir, mumom, prmom, pimom, dpTT, dpTMag, dalphaT, dphiT);
}

TVector3 * TransverseTools::GetTransVarsRec(std::vector<Double_t> vtx, const TVector3 *& mumom, const TVector3 *& prmom,
 const TVector3 *& pimom, Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)// const
{
    Double_t vec[3] = { vtx[0], vtx[1], vtx[2] };
    return GetTransVarsRec(vec, mumom, prmom, pimom, dpTT, dpTMag, dalphaT, dphiT);
}

TVector3 * TransverseTools::GetTransVarsDir(Double_t dir[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)
{
    const TVector3 * nudir = new TVector3(dir[0], dir[1], dir[2]);
    TVector3 * tmp = GetTransVarsBase(nudir, mumom, prmom, pimom, dpTT, dpTMag, dalphaT, dphiT);
    return tmp;
}           
    
TVector3 * TransverseTools::GetTransVarsDir(std::vector<Double_t> dir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom,
                                 Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)
{
    Double_t vec[3] = { dir[0], dir[1], dir[2] };
    return GetTransVarsDir(vec, mumom, prmom, pimom, dpTT, dpTMag, dalphaT, dphiT);
}           

TVector3 * TransverseTools::GetNuDirBase(const TVector3 *& vtx, const TVector3 *& PDP)// const
{
    //If PDP exists make sure to rotote to minerva coords: -- May not need to do this
    TVector3 * nup1local = new TVector3(*vtx);
    (*nup1local) *= 0.001;//in meters (default mm)
    if( PDP->Mag() < EPSILON || nup1local->Mag() < EPSILON ){
        cout << "TransverseTools::CalcNuDir bad input " << PDP->Mag() << " " << nup1local->Mag() << endl;
        return 0x0;
    }
    TVector3 *nuDirCalc = new TVector3( (*nup1local) - (*PDP) );
    (*nuDirCalc) *= 1./nuDirCalc->Mag();
    delete nup1local;
    return nuDirCalc;
}

TVector3 * TransverseTools::GetNuDirRec(Double_t vtx[])// const
{
    //If PDP exists make sure to rotote to minerva coords: -- May not need to do this
    const TVector3 * VTX = new TVector3(vtx[0], vtx[1], vtx[2]);
    const TVector3 * PDP = new TVector3(m_PDP_x, m_PDP_y, m_PDP_z);
    TVector3 * tmp = GetNuDirBase(VTX, PDP);
    delete VTX;
    delete PDP;
    return tmp;
}

TVector3 * TransverseTools::GetNuDirRec(std::vector<Double_t> vtx)
{    
    Double_t tmp_vtx[3] = { vtx[0], vtx[1], vtx[2] };
    return GetNuDirRec(tmp_vtx);
}

TVector3 * TransverseTools::NuMiToMin(Double_t nu_NuParentDecPoint[])// const
{
    // This converts from Numi (beam) coordinates to Minerva coordinates.
    //pl output in [m]
    TVector3 pg(nu_NuParentDecPoint);
    pg *= 0.001; //[mm] -> [m]
    const Double_t tmpx = pg.X();
    const Double_t tmpy = pg.Y();
    const Double_t tmpz = pg.Z();
    const Double_t xmnv = tmpx + m_XOffset;
    const Double_t ymnv =  TMath::Cos(m_Theta)*tmpy + TMath::Sin(m_Theta)*tmpz + m_YOffset;
    const Double_t zmnv = -TMath::Sin(m_Theta)*tmpy + TMath::Cos(m_Theta)*tmpz + m_ZOffset;
    return new TVector3(xmnv, ymnv, zmnv);
}

TVector3 * TransverseTools::GetNuDirSim(Double_t vtx[], Double_t pdp[])// const
{
    // May need to delete these after use? 
    const TVector3 * PDP = NuMiToMin(pdp);
    const TVector3 * VTX = new TVector3(vtx[0], vtx[1], vtx[2]);
    TVector3 * tmp = GetNuDirBase(VTX, PDP);
    delete VTX;
    delete PDP;
    return tmp;
}

TVector3 * TransverseTools::GetNuDirSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp)// const
{   
    Double_t tmp_vtx[3] = { vtx[0], vtx[1], vtx[2] };
    Double_t tmp_pdp[3] = { pdp[0], pdp[1], pdp[2] };   
    return GetNuDirSim(tmp_vtx, tmp_pdp);
}

TVector3 * TransverseTools::GetTransVarsSim(Double_t vtx[], Double_t pdp[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)// const;
{
    const TVector3 * nudir = GetNuDirSim(vtx, pdp);
    return GetTransVarsBase(nudir, mumom, prmom, pimom, dpTT, dpTMag, dalphaT, dphiT);
}

TVector3 * TransverseTools::GetTransVarsSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, const TVector3 *& mumom, const TVector3 *& prmom, 
    const TVector3 *& pimom, Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)// const;
{
    Double_t tmp_vtx[3] = { vtx[0], vtx[1], vtx[2] };
    Double_t tmp_pdp[3] = { pdp[0], pdp[1], pdp[2] };
    return GetTransVarsSim(tmp_vtx, tmp_pdp, mumom, prmom, pimom, dpTT, dpTMag, dalphaT, dphiT);
}

TVector3 * TransverseTools::GetTransVarsBase(const TVector3 *& nudir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT)
{
    const TVector3 * mupT = GetVecT(nudir, mumom);
    const TVector3 * prpT = GetVecT(nudir, prmom);
    const TVector3 * pipT = GetVecT(nudir, pimom);
    
    TVector3 * deltapt = SetDPT(mupT, prpT, pipT);
    
    dpTMag  = deltapt->Mag();
    dalphaT = (deltapt->Theta())*TMath::RadToDeg();
    dphiT   = (deltapt->Phi())*TMath::RadToDeg();
    dpTT    = GetDPTTBase(nudir, mumom, prmom, pimom);
    
    delete nudir;
    delete mupT;
    delete prpT;
    delete pipT;

    return deltapt;
}

void TransverseTools::RotateToMIN(Double_t &py, Double_t &pz)//MinervaUnits::numi_beam_angle_rad (-0.085887) == m_Theta;
{
   Double_t py_prime =  TMath::Cos(m_Theta)*py + TMath::Sin(m_Theta)*pz;
   Double_t pz_prime = -TMath::Sin(m_Theta)*py + TMath::Cos(m_Theta)*pz;
   py = py_prime;
   pz = pz_prime;
}

void TransverseTools::RotateToNuMi(Double_t &py, Double_t &pz)
{
   Double_t py_prime = TMath::Cos( m_Theta )*py - TMath::Sin( m_Theta )*pz;
   Double_t pz_prime = TMath::Sin( m_Theta )*py + TMath::Cos( m_Theta )*pz;
   py = py_prime;
   pz = pz_prime;
}

Double_t TransverseTools::GetThetaWRTBeamRec(Double_t vtx[], Double_t x, Double_t y, Double_t z)
{
    TVector3 * DIR = GetNuDirRec(vtx);
    TVector3 * VEC = new TVector3(x, y, z);
    return ThetaWRTBeamBase(DIR, VEC);
}

Double_t TransverseTools::GetThetaWRTBeamRec(std::vector<Double_t> vtx, Double_t x, Double_t y, Double_t z)
{
    Double_t tmp_vtx[3] = {vtx[0], vtx[1], vtx[2]};
    return GetThetaWRTBeamRec(tmp_vtx, x, y, z);
}

Double_t TransverseTools::GetThetaWRTBeamSim(Double_t vtx[], Double_t pdp[], Double_t x, Double_t y, Double_t z)
{
    TVector3 * DIR = GetNuDirSim(vtx, pdp);
    TVector3 * VEC = new TVector3(x, y, z);
    return ThetaWRTBeamBase(DIR, VEC);
}

Double_t TransverseTools::GetThetaWRTBeamSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, Double_t x, Double_t y, Double_t z)
{
   Double_t tmp_vtx[3] = {vtx[0], vtx[1], vtx[2]};
   Double_t tmp_pdp[3] = {pdp[0], pdp[1], pdp[2]};
   return GetThetaWRTBeamSim(tmp_vtx, tmp_pdp, x, y, z);
}

Double_t TransverseTools::GetThetaWRTBeamDir(Double_t dir[], Double_t x, Double_t y, Double_t z)
{
    TVector3 * DIR = new TVector3(dir[0], dir[1], dir[2]);
    TVector3 * VEC = new TVector3(x, y, z);
    return ThetaWRTBeamBase(DIR, VEC);
}

Double_t TransverseTools::GetThetaWRTBeamDir(std::vector<Double_t> dir, Double_t x, Double_t y, Double_t z)
{
    Double_t tmp_dir[3] = {dir[0], dir[1], dir[2]};
    return GetThetaWRTBeamDir(tmp_dir, x, y, z);
}

Double_t TransverseTools::ThetaWRTBeamBase(const TVector3 * DIR, const TVector3 * VEC)
{
    Double_t mag = DIR->Mag() * VEC->Mag();
    Double_t dot = DIR->Dot(*VEC);
    dot *= 1/mag;
    return TMath::ACos(dot);
}

Double_t TransverseTools::GetPhiWRTBeamRec(Double_t vtx[], Double_t x, Double_t y, Double_t z)
{
    TVector3 * DIR = GetNuDirRec(vtx);
    TVector3 * VEC = new TVector3(x, y, z);
    return PhiWRTBeamBase(DIR, VEC);
}

Double_t TransverseTools::GetPhiWRTBeamRec(std::vector<Double_t> vtx, Double_t x, Double_t y, Double_t z)
{
    Double_t tmp_vtx[3] = {vtx[0], vtx[1], vtx[2]};
    return GetPhiWRTBeamRec(tmp_vtx, x, y, z);
}

Double_t TransverseTools::GetPhiWRTBeamSim(Double_t vtx[], Double_t pdp[], Double_t x, Double_t y, Double_t z)
{
    TVector3 * DIR = GetNuDirSim(vtx, pdp);
    TVector3 * VEC = new TVector3(x, y, z);
    return PhiWRTBeamBase(DIR, VEC);
}

Double_t TransverseTools::GetPhiWRTBeamSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, Double_t x, Double_t y, Double_t z)
{
   Double_t tmp_vtx[3] = {vtx[0], vtx[1], vtx[2]};
   Double_t tmp_pdp[3] = {pdp[0], pdp[1], pdp[2]};
   return GetPhiWRTBeamSim(tmp_vtx, tmp_pdp, x, y, z);
}

Double_t TransverseTools::GetPhiWRTBeamDir(Double_t dir[], Double_t x, Double_t y, Double_t z)
{
    TVector3 * DIR = new TVector3(dir[0], dir[1], dir[2]);
    TVector3 * VEC = new TVector3(x, y, z);
    return PhiWRTBeamBase(DIR, VEC);
}

Double_t TransverseTools::GetPhiWRTBeamDir(std::vector<Double_t> dir, Double_t x, Double_t y, Double_t z)
{
    Double_t tmp_dir[3] = {dir[0], dir[1], dir[2]};
    return GetPhiWRTBeamDir(tmp_dir, x, y, z);
}

Double_t TransverseTools::PhiWRTBeamBase(const TVector3 * DIR, const TVector3 * VEC)
{
    //This is independent of the DIR Z direction and in fact the VEC's z-direction.
    VEC->X()/VEC->Y();

    Double_t mag = DIR->Mag() * VEC->Mag();
    Double_t dot = DIR->Dot(*VEC);
    dot *= 1/mag;
    return TMath::ACos(dot);
}

#endif

#ifndef __DRAWINGTOOLS__CXX
#define __DRAWINGTOOLS__CXX

#include <PlotUtils/MnvH2D.h>
#include <PlotUtils/MnvH1D.h>
using namespace PlotUtils;
#include <TF1.h>

// const double DrawingTools::range = 3.;

MnvH2D * DrawingTools::NormalHist(MnvH2D * hraw, double thres, bool kmax)
{
    MnvH2D * hh = static_cast<MnvH2D*>( hraw->Clone(Form("%sSN",hraw->GetName())) );
    hh->Scale(0);
    
    const int x0 = hh->GetXaxis()->GetFirst();
    const int x1 = hh->GetXaxis()->GetLast();
    const int y0 = hh->GetYaxis()->GetFirst();
    const int y1 = hh->GetYaxis()->GetLast();
    
    double hmax = -1e10;
    double hmin = 1e10;
    double nent = 0;
    for(int ix=x0; ix<=x1; ix++){
        
        //if option "e" is specified, the errors are computed. if option "o" original axis range of the taget axes will be kept, but only bins inside the selected range will be filled.
        
        TH1D * sliceh = hraw->ProjectionY(Form("tmpnormalhist%sx%d", hh->GetName(), ix), ix, ix, "oe");
        const double tot = sliceh->GetEntries();
        
        TH1D * pdfh=0x0;
        
        if(tot>1e-12){
            nent += tot;
            
            double imax = -999;
            
            if(!kmax){
                pdfh = ToPDF(sliceh,"tmp");
            }
            else{
                imax = sliceh->GetBinContent(sliceh->GetMaximumBin());
            }
            
            for(int iy=y0; iy<=y1; iy++){
                const double cont = kmax ? sliceh->GetBinContent(iy)/imax : pdfh->GetBinContent(iy);
                const double ierr = kmax ? sliceh->GetBinError(iy)/imax   : pdfh->GetBinError(iy);
                if(tot>thres && cont>0){
                    hh->SetBinContent(ix, iy, cont);
                    hh->SetBinError(ix,iy, ierr);
                    if(cont>hmax) hmax = cont;
                    if(cont<hmin) hmin = cont;
                }
            }
        }
        
        delete pdfh;
        delete sliceh;
    }
    
    hh->SetEntries(nent);
    hh->SetMinimum(0.99*hmin);
    hh->SetMaximum(1.1*hmax);
    return hh;
}

TH2D * DrawingTools::NormalHist(TH2D * hraw, double thres, bool kmax)
{
    TH2D * hh = static_cast<TH2D*>( hraw->Clone(Form("%sSN",hraw->GetName())) );
    hh->Scale(0);
    
    const int x0 = hh->GetXaxis()->GetFirst();
    const int x1 = hh->GetXaxis()->GetLast();
    const int y0 = hh->GetYaxis()->GetFirst();
    const int y1 = hh->GetYaxis()->GetLast();
    
    double hmax = -1e10;
    double hmin = 1e10;
    double nent = 0;
    for(int ix=x0; ix<=x1; ix++){
        
        //if option "e" is specified, the errors are computed. if option "o" original axis range of the taget axes will be kept, but only bins inside the selected range will be filled.
        
        TH1D * sliceh = hraw->ProjectionY(Form("tmpnormalhist%sx%d", hh->GetName(), ix), ix, ix, "oe");
        const double tot = sliceh->GetEntries();
        
        TH1D * pdfh=0x0;
        
        if(tot>1e-12){
            nent += tot;
            
            double imax = -999;
            
            if(!kmax){
                pdfh = ToPDF(sliceh,"tmp");
            }
            else{
                imax = sliceh->GetBinContent(sliceh->GetMaximumBin());
            }
            
            for(int iy=y0; iy<=y1; iy++){
                const double cont = kmax ? sliceh->GetBinContent(iy)/imax : pdfh->GetBinContent(iy);
                const double ierr = kmax ? sliceh->GetBinError(iy)/imax   : pdfh->GetBinError(iy);
                if(tot>thres && cont>0){
                    hh->SetBinContent(ix, iy, cont);
                    hh->SetBinError(ix,iy, ierr);
                    if(cont>hmax) hmax = cont;
                    if(cont<hmin) hmin = cont;
                }
            }
        }
        
        delete pdfh;
        delete sliceh;
    }
    
    hh->SetEntries(nent);
    hh->SetMinimum(0.99*hmin);
    hh->SetMaximum(1.1*hmax);
    return hh;
}

MnvH1D * DrawingTools::MakeResidual(std::string name, std::string var_symbol, int nbins, double low, double high)
{
    return new MnvH1D(name.c_str(), (";1 - (" + var_symbol + "^{recon})/(" + var_symbol + "^{true})").c_str(), nbins, low, high);
}

TH1D * DrawingTools::MakeResidualROOT(std::string name, std::string var_symbol, int nbins, double low, double high)
{
    return new TH1D(name.c_str(), (";1 - (" + var_symbol + "^{recon})/(" + var_symbol + "^{true})").c_str(), nbins, low, high);
}

void DrawingTools::FillResidual(MnvH1D * hist, double reco_val, double true_val, double weight)
{
    double value = 1 - reco_val/true_val;
    hist->Fill(value, weight);
}

TH1D * DrawingTools::ToPDF(TH1D *hraw, std::string hn){
    const int x0 = 0;
    const int x1 = hraw->GetNbinsX()+1;
    const double tmpnt = hraw->Integral(x0, x1);
    
    TH1D * hist = (TH1D*) hraw->Clone( (hn + std::string(hraw->GetName()) + "pdf").c_str() );
    hist->Scale(0);
    
    for(int ib=x0; ib<=x1; ib++){
        const double bw = hraw->GetBinWidth(ib);
        const double cont = hraw->GetBinContent(ib);
        if(cont<1e-12)
            continue;
        
        //in case of finit number of bins (i.e. eff not always small), Binomial error is more accurate than Poisson error
        const double eff = cont/tmpnt;
        const double pdf = eff/bw;
        
        const double dpdf = TMath::Sqrt(eff*(1-eff)/tmpnt) / bw;
        hist->SetBinContent(ib, pdf);
        hist->SetBinError(ib, dpdf);
    }
    
    hist->SetEntries(tmpnt);
    
    return hist;
}

MnvH1D * DrawingTools::ToPDF(MnvH1D *hraw, std::string hn){
    const int x0 = 0;
    const int x1 = hraw->GetNbinsX()+1;
    const double tmpnt = hraw->Integral(x0, x1);
    
    MnvH1D * hist = (MnvH1D*) hraw->Clone( (hn + std::string(hraw->GetName()) + "pdf").c_str() );
    hist->Scale(0);
    
    for(int ib=x0; ib<=x1; ib++){
        const double bw = hraw->GetBinWidth(ib);
        const double cont = hraw->GetBinContent(ib);
        if(cont<1e-12)
            continue;
        
        //in case of finit number of bins (i.e. eff not always small), Binomial error is more accurate than Poisson error
        const double eff = cont/tmpnt;
        const double pdf = eff/bw;
        
        const double dpdf = TMath::Sqrt(eff*(1-eff)/tmpnt) / bw;
        hist->SetBinContent(ib, pdf);
        hist->SetBinError(ib, dpdf);
    }
    
    hist->SetEntries(tmpnt);
    
    return hist;
}

double DrawingTools::GetFWHM(TH1D * hist, bool approx)
{
    int bin1 = hist->FindFirstBinAbove(hist->GetMaximum()/2);
    int bin2 = hist->FindLastBinAbove(hist->GetMaximum()/2);
    double FWHM = hist->GetBinCenter(bin2) - hist->GetBinCenter(bin1);
    return FWHM;
}

double DrawingTools::GetWidth(const TH1D * hist)
{
    TH1D * tmp = static_cast<TH1D*>(hist->Clone("cauchy_fit")); 
    // hist->Get
    Double_t min = tmp->GetXaxis()->GetXmin();
    Double_t max = tmp->GetXaxis()->GetXmax();
    Double_t mean = (Double_t)tmp->GetMean();
    Double_t rms = (Double_t)tmp->GetRMS();
    Double_t integral = (Double_t)tmp->Integral();

    cout << "Inputs: Range = " << min << " < x < " << max;
    cout << " mean = " << mean << " rms = " << rms;
    cout << " integral = " << integral << endl;

    TF1 * cauchy = new TF1("cauchy","([2]*[1])/(TMath::Pi()*([1]*[1] + (x-[0])*(x-[0]) ) )", min, max);
    cauchy->SetParameter(0, mean );
    cauchy->SetParameter(1, rms  );
    cauchy->SetParameter(2, integral );
    Int_t success = tmp->Fit(cauchy,"RLN", "", min, max);
    
    double value = tmp->GetRMS();
    if(success == 0){
        value = cauchy->GetParameter(1);
    }
    delete tmp;
    delete cauchy;

    return value;
}

#endif