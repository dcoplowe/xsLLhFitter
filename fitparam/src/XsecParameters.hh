//////////////////////////////////////////////////////////
//
//  Xsec modeling parameters
//
//
//
//  Created: Oct 2013
//  Modified:
//
//////////////////////////////////////////////////////////

#ifndef __XsecParameters_hh__
#define __XsecParameters_hh__

//#include "AnaFitParameters.hh"
#include "ResponseParameters.hh"

#include <TFile.h>
#include <TGraph.h>

// Hard coding of sample and raction types

/*struct Correlation
{
    int i;
    int j;
    double val;
};*/


// Sample types - should match whats in ccqefit.cc
enum SampleTypes { muTPCpTPCpiTPC = 0 };
// Reaction and backgrounds
// the indices should match with trees
// case below is mectopology from HL2 v1r15 cc0pi
enum ReactionTypes { ReRec11 = 11, ReRec13 = 13, ReCCMPi = 21, ReCCDIS = 26, ReNCMPi = 41, ReNCDIS = 46, ReUND = 99 };

enum TargetTypes { Hydrogen = 1, Other = 999 };//Did we need to add carbon?

struct XsecBin
{
    double recoD1low, recoD1high;
    double trueD1low, trueD1high;
    double recoD2low, recoD2high;
    double trueD2low, trueD2high;
    
    SampleTypes topology;
    ReactionTypes reaction;
    TargetTypes target;
    
    std::vector<TGraph*> respfuncs;
    std::vector<bool> resexists;
};

class XsecParameters : public ResponseParameters
{
public:
 
    XsecParameters(const char *name = "par_xsec");
    ~XsecParameters();
    
    void StoreResponseFunctions(std::vector<TFile*> respfuncs, std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges);
    
    void InitEventMap(std::vector<AnaSample*> &sample);
    void EventWeights(std::vector<AnaSample*> &sample, std::vector<double> &params);
    void ReWeight(AnaEvent *event, int nsample, int nevent, std::vector<double> &params);
    //void AddResponse(TString filename, TString param_name, double cov_val);
    //void SetUpParams(std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges);
    //void AddCorrelation(TString par_name1, TString par_name2, double cov_val);
    
private:
    int GetBinIndex(SampleTypes sampletype, int reac, int tar, double D1reco, double D1true, double D2reco, double D2true);
    
    std::vector<XsecBin> m_bins;
    
    //std::vector<TFile*> m_resfiles;
    //std::vector<double> m_cov_val;
    
    //bool m_correlation;
    //std::vector<Correlation> m_corr;
    
};

#endif
