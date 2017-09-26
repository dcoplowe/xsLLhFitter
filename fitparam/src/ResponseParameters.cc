#include "ResponseParameters.hh"

#include "TString.h"
#include "TFile.h"

using namespace std;

ResponseParameters::ResponseParameters(){
    Npar = 0;
    m_resfiles.clear();
    m_cov_val.clear();
    m_corr.clear();
    m_correlation = false;
}

ResponseParameters::~ResponseParameters(){
    m_resfiles.clear();
    m_cov_val.clear();
    m_corr.clear();
    m_correlation = false;
}

void ResponseParameters::AddResponse(TString filename, TString param_name, double cov_val){
    TFile * file = new TFile(filename);
    
    m_resfiles.push_back(file);
    pars_name.push_back(Form("%s%d_%s", m_name.c_str(), (int)Npar, param_name.Data()));
    //cout << "Par Name: " << pars_name[(int)Npar] << endl;
    pars_prior.push_back(1);
    pars_step.push_back(0.11);
    pars_limlow.push_back(0.5);
    pars_limhigh.push_back(2.5);
    
    m_cov_val.push_back(cov_val);
    Npar++;
}

void ResponseParameters::AddCorrelation(TString par_name1, TString par_name2, double cov_val){
    m_correlation = true;
    
    int par_name1_pos = -999;
    int par_name2_pos = -999;
    
    for(int i=0; i < (int)pars_name.size(); i++){
        
        TString tmp_string(pars_name[i]);
        
        cout <<"Par: " << i << " " << tmp_string.Data() << endl;
        
        if(tmp_string.Contains(par_name1.Data(), TString::kExact)){
            par_name1_pos = i;
        }
        
        if(tmp_string.Contains(par_name2.Data(), TString::kExact)){
            par_name2_pos = i;
        }
    }
    
    if(par_name2_pos == par_name1_pos && par_name1_pos != -999){
        cout << "ResponseParameters::AddCorrelation : Parameters are the same!" << endl;
    }
    else if(par_name1_pos == -999 || par_name2_pos == -999){
        cout << "ResponseParameters::AddCorrelation : Error could not determine position of parameter(s):";// << endl;
        
        if(par_name1_pos == -999){
            cout << " " << par_name1.Data();
        }
        
        if(par_name2_pos == -999){
            cout << " " << par_name2.Data();
        }
        cout << ". They must be defined before adding correlations." << endl;
        exit(0);
    }
    
    cout << "Adding correlation between " << par_name1.Data() << "(" << par_name1_pos << ") and " << par_name2.Data() << "(" << par_name2_pos << ") with value " << cov_val << endl;
    
    Correlation tmp_cor;
    tmp_cor.i = par_name1_pos;
    tmp_cor.j = par_name2_pos;
    tmp_cor.val = cov_val;
    
    m_corr.push_back(tmp_cor);
}

void ResponseParameters::SetUpParams(std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges){
    cout << "ResponseParameters::SetUpParams" << endl;
    
    cout << "  No. Pars = " << Npar <<endl;
    cout << "No. Priors = " << pars_prior.size() << endl;
    
    TMatrixDSym cov_xsec((int)Npar);
    
    for(int i = 0; i < (int)Npar; i++){
        for(int j = 0; j < (int)Npar; j++){
            
            if(i == j){
                cov_xsec(i,j) = m_cov_val[i];
            }
            else{
                cov_xsec(i,j) = 0.;
            }
        }
    }
    
    if(m_correlation){
        for(int i = 0; i < (int)m_corr.size(); i++){
            Correlation tmp_cor = m_corr[i];
            cov_xsec(tmp_cor.i, tmp_cor.j) = tmp_cor.val;
        }
    }
    
    cov_xsec.Print();
    
    SetCovarianceMatrix(&cov_xsec);
    
    (void)v_D1edges;
    (void)v_D2edges;
    
    cout << "ResponseParameters:: Calling StoreResponseFunctions:" << endl;
    StoreResponseFunctions(m_resfiles, v_D1edges, v_D2edges);
}
