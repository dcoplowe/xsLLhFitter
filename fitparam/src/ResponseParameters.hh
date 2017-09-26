#ifndef RESPONSEPARAMETERS_H
#define RESPONSEPARAMETERS_H

#include "AnaFitParameters.hh"

struct Correlation
{
    int i;
    int j;
    double val;
};

//Forward declared classes:
class TString;
class TFile;

class ResponseParameters : public AnaFitParameters {
public:

    ResponseParameters();
    ~ResponseParameters();
    
    void AddResponse(TString filename, TString param_name, double cov_val);
    void AddCorrelation(TString par_name1, TString par_name2, double cov_val);
    void SetUpParams(std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges);
    
    virtual void StoreResponseFunctions(std::vector<TFile*> respfuncs, std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges)=0;
    
private:
    
    std::vector<TFile*> m_resfiles;
    std::vector<double> m_cov_val;
    
    bool m_correlation;
    std::vector<Correlation> m_corr;
    
};

#endif
