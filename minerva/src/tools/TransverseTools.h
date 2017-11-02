#ifndef __TRANSVERSETOOLS__H
#define __TRANSVERSETOOLS__H

#include "TROOT.h"

class TVector3;
// class vector;//Is this good?
#include <vector>

class TransverseTools {
    
public:                 
    TransverseTools(Double_t pdp_x = 0.231135, Double_t pdp_y = 45.368069, Double_t pdp_z = -766.384058);
    ~TransverseTools();
    
    TVector3 * GetNuDirRec(Double_t vtx[]);// const;
    TVector3 * GetNuDirRec(std::vector<Double_t> vtx);// const;
    
    TVector3 * GetNuDirSim(Double_t vtx[], Double_t pdp[]);// const;
    TVector3 * GetNuDirSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp);// const;
    
    Double_t GetDPTTRec(Double_t vtx[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);// const;
    Double_t GetDPTTRec(std::vector<Double_t> vtx, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);// const;

    Double_t GetDPTTSim(Double_t vtx[], Double_t pdp[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);
    Double_t GetDPTTSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);
    
    Double_t GetDPTTDir(Double_t dir[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);
    Double_t GetDPTTDir(std::vector<Double_t> dir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);

    TVector3 * GetVecT(const TVector3 *& refdir, const TVector3 *& mom);// const;
    
    TVector3 * GetPTSim(Double_t vtx[], Double_t pdp[], const TVector3 *& mom);// const;
    TVector3 * GetPTSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, const TVector3 *& mom);// const;

    TVector3 * GetPTRec(Double_t vtx[], const TVector3 *& mom);// const;
    TVector3 * GetPTRec(std::vector<Double_t> vtx, const TVector3 *& mom);// const;

    TVector3 * GetPTDir(Double_t dir[], const TVector3 *& mom);// const;
    TVector3 * GetPTDir(std::vector<Double_t> dir, const TVector3 *& mom);// const;

    TVector3 * SetDPT(const TVector3 *& ptmuon, const TVector3 *& ptproton, const TVector3 *& ptpion);// const;
    
    TVector3 * GetTransVarsRec(Double_t vtx[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);// const;
    
    TVector3 * GetTransVarsRec(std::vector<Double_t> vtx, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom,
                                 Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);// const;

    TVector3 * GetTransVarsDir(Double_t dir[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);// const;
    
    TVector3 * GetTransVarsDir(std::vector<Double_t> dir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom,
                                 Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);// const;

    //Use these for checking reco:
    TVector3 * GetTransVarsSim(Double_t vtx[], Double_t pdp[], const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);// const;
    
    TVector3 * GetTransVarsSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom,
                                 Double_t &dpTT, Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);// const;

    Double_t GetThetaWRTBeamRec(Double_t vtx[], Double_t x, Double_t y, Double_t z);
    Double_t GetThetaWRTBeamRec(std::vector<Double_t> vtx, Double_t x, Double_t y, Double_t z);

    Double_t GetPhiWRTBeamRec(Double_t vtx[], Double_t x, Double_t y, Double_t z);
    Double_t GetPhiWRTBeamRec(std::vector<Double_t> vtx, Double_t x, Double_t y, Double_t z);

    Double_t GetThetaWRTBeamSim(Double_t vtx[], Double_t pdp[], Double_t x, Double_t y, Double_t z);
    Double_t GetThetaWRTBeamSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, Double_t x, Double_t y, Double_t z);

    Double_t GetPhiWRTBeamSim(Double_t vtx[], Double_t pdp[], Double_t x, Double_t y, Double_t z);
    Double_t GetPhiWRTBeamSim(std::vector<Double_t> vtx, std::vector<Double_t> pdp, Double_t x, Double_t y, Double_t z);

    Double_t GetThetaWRTBeamDir(Double_t dir[], Double_t x, Double_t y, Double_t z);
    Double_t GetThetaWRTBeamDir(std::vector<Double_t> dir, Double_t x, Double_t y, Double_t z);

    Double_t GetPhiWRTBeamDir(Double_t vtx[], Double_t x, Double_t y, Double_t z);
    Double_t GetPhiWRTBeamDir(std::vector<Double_t> vtx, Double_t x, Double_t y, Double_t z);

    void RotateToNuMi(Double_t &y, Double_t &z);
    void RotateToMIN(Double_t &y, Double_t &z);
    
private:
    static Double_t m_PDP_x;
    static Double_t m_PDP_y;
    static Double_t m_PDP_z;
        
    static const Double_t m_Theta;// = -0.0582977560;
    static const Double_t m_XOffset;// = 0.2486;
    static const Double_t m_YOffset;// = 60.350;
    static const Double_t m_ZOffset;// = -1022.74;
    
    TVector3 * NuMiToMin(Double_t nu_NuParentDecPoint[]);
    TVector3 * GetNuDirBase(const TVector3 *& vtx, const TVector3 *& PDP);
    TVector3 * GetTransVarsBase(const TVector3 *& nudir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom, Double_t &dpTT,
                                 Double_t &dpTMag, Double_t &dalphaT, Double_t &dphiT);
    Double_t GetDPTTBase(const TVector3 *&nudir, const TVector3 *& mumom, const TVector3 *& prmom, const TVector3 *& pimom);

    Double_t PhiWRTBeamBase(const TVector3 * DIR, const TVector3 * VEC);
    Double_t ThetaWRTBeamBase(const TVector3 * DIR, const TVector3 * VEC);
};

#endif

#ifndef __DRAWINGTOOLS__HXX
#define __DRAWINGTOOLS__HXX

#include <PlotUtils/MnvH2D.h>
#include <PlotUtils/MnvH1D.h>
#include <string.h>
class TH1D;
class TH2D;

const double drawingtools_range = 0.5;
const int drawingtools_nbins = 49;

class DrawingTools {
public:
    DrawingTools(){;}

    // static const double range = 3.;
    static PlotUtils::MnvH2D * NormalHist(PlotUtils::MnvH2D * hraw, double thres = 0, bool kmax=true);
    static PlotUtils::MnvH1D * MakeResidual(std::string name, std::string var_symbol,
        int nbins = drawingtools_nbins, double low = -drawingtools_range, double high = drawingtools_range);

    static TH2D * NormalHist(TH2D * hraw, double thres = 0, bool kmax=true);
    static TH1D * MakeResidualROOT(std::string name, std::string var_symbol, int nbins = drawingtools_nbins, 
        double low = -drawingtools_range, double high = drawingtools_range);

    static void FillResidual(PlotUtils::MnvH1D * hist, double reco_val, double true_val, double weight = 1.);

    static TH1D * ToPDF(TH1D * hraw, std::string hn = "");
    static PlotUtils::MnvH1D * ToPDF(PlotUtils::MnvH1D *hraw, std::string hn = "");

    static double GetFWHM(TH1D * hist, bool approx = true);
    static double GetWidth(const TH1D * hist);//Don't want to mess around with the original hist

};

#endif
