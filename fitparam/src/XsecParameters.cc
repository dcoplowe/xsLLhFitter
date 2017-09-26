#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>

#include "XsecParameters.hh"

using namespace std;

//ctor
XsecParameters::XsecParameters(const char *name) 
{
    m_name = name;
    Npar=0;
    
    //m_correlation = false;
    //Now we want to define the xsec parameters in the fit. This needs to be in the same order as the response functions (splines)
    //are fed into the fitter. This is a bad fit od hard coding and should be improved at some point.
}

//dtor
XsecParameters::~XsecParameters()
{;}

// So much upsetting hard coding here, sorry!
// So below I've hacked in a fix to avoid the FSI params looking at cut branch (sample) 0 or 4
// (since these have no protons)
// and reaction 6 has been made to be reaaction 7 (since reaction 6 is not defined in mectopology HL2 cat)

// The dummy response function thing is also a bit of a hack. This allows the occasional job to fail when making
// resp functions and the fitter still runs but this is not good and should be delt with


// store response functions in vector of Xsec "bins" (Ereco, Etrue, reac, topo)
void XsecParameters::StoreResponseFunctions(vector<TFile*> respfuncs, std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges)
{
    
    cout << "XsecParameters::StoreResponseFunctions" << endl;
    
    double dummyx[7]={-1,-0.66,-0.33,0,0.33,0.66,1};
    double dummyy[7]={1,1,1,1,1,1,1};
    int    dummyn=7;
    
    //Now in the response production we want to set some definitions for the
    //target too. now we should be consistant with our implementation of both
    //the target and reaction. Should check that the reactions that splines a
    //are produced for are in here.
    
    int targ_list[2] = {1, 999};
    int reac_list[7] = {11, 13, 21, 26, 41, 46, 99};
    
    cout << "Inputing Response Functions..." << endl;
    
    //In each bin: We want to we want to get the splines assocaited to that reaction and topology
    
    for ( int sam_iter = 0; sam_iter < 1; sam_iter++){

        SampleTypes sampletype = static_cast <SampleTypes> (sam_iter);
        
        for(int tar_iter = 0; tar_iter < 2; tar_iter++){
    
            TargetTypes targettype = static_cast<TargetTypes>(targ_list[tar_iter]);
    
            for(int reac_iter = 0; reac_iter < 7; reac_iter++){
                
                ReactionTypes reactype = static_cast<ReactionTypes>(reac_list[reac_iter]);
                
                bool good_reac = false;
                
                if(targ_list[tar_iter] == 1){
                    switch (reac_list[reac_iter]) {
                        case 11: good_reac = true; break;
                        case 21: good_reac = true; break;
                        case 26: good_reac = true; break;
                        case 99: good_reac = true; break;
                        default: good_reac = false; break;
                    }
                }
                else if(targ_list[tar_iter] == 999){
                    switch (reac_list[reac_iter]) {
                        case 11: good_reac = true; break;
                        case 13: good_reac = true; break;
                        case 21: good_reac = true; break;
                        case 26: good_reac = true; break;
                        case 41: good_reac = true; break;
                        case 99: good_reac = true; break;
                        default: good_reac = false; break;
                    }
                }
                
                if(!good_reac) continue;
                
                //cout<<"reading response functions for topology "<<stInt<<"  reaction "<<rtInt<<endl;
                int nccqebins=v_D1edges.size();
                for(int br=0;br<nccqebins;br++){//reco kinematics bin
                    //cout<<"reading rewighting function for reco bin "<<br<<endl;
                    for(int bt=0;bt<nccqebins;bt++){//true kinematics bin
                        //cout<<"reading rewighting function for true bin "<<bt<<endl;
                        XsecBin bin;
                        bin.recoD1low = v_D1edges[br].first;
                        bin.recoD1high = v_D1edges[br].second;
                        bin.trueD1low = v_D1edges[bt].first; //same binning for reco and true
                        bin.trueD1high = v_D1edges[bt].second;
                        bin.recoD2low = v_D2edges[br].first;
                        bin.recoD2high = v_D2edges[br].second;
                        bin.trueD2low = v_D2edges[bt].first; //same binning for reco and true
                        bin.trueD2high = v_D2edges[bt].second;
                        bin.topology = sampletype;
                        bin.reaction = reactype;
                        bin.target = targettype;
                        
                        if(fabs(br-bt)<21) {  //save memory if reco bin and true bin very far away
                            for(int i=0; i< (int)Npar; i++){
                                char name[200];
                                
                                sprintf(name,"topology_%d/RecBin%d_TrueBin%d_top%d_reac%d_tar%d",sam_iter, br, bt, sam_iter, reac_list[reac_iter], targ_list[tar_iter]);
                                //sprintf(name,"topology_%d/RecBin_%d_trueBin_%d_topology_%d_reac_%d",stInt,br,bt,stInt,rtInt);
                                //cout<<respfuncs[i]->GetName()<<" "<<name<<endl;
                                TGraph* g=(TGraph*)respfuncs[i]->Get(name);
                                bool tmp_resexists = false;
                                
                                if(!g){
                                    //      cout << "Warning, creating dummy respfunc, param: " << i << " " << name << endl;
                                    //     cout << "getchar to cont" << endl;
                                    //getchar();
                                    g = NULL;// new TGraph(dummyn, dummyx, dummyy);
                                }
                                else{
                                    tmp_resexists = true;
                                    g->SetName(name);
                                    if((g->GetY())[3]!=1.0){
                                        cout << "Warning: altering xsec nominal param: " << i << " " << name << endl;
                                    }
                                }
                                
                                bin.resexists.push_back(tmp_resexists);
                                bin.respfuncs.push_back(g);
                            }
                        }
                        
                        m_bins.push_back(bin);
                    }
                }
            }
        }
    }
}

// --
int XsecParameters::GetBinIndex(SampleTypes sampletype, int reac, int tar, double D1reco, double D1true, double D2reco, double D2true)
{
    //Cast the reaction type and target as ReactionTypes and TargetTypes:
    int tmp_reactype = 99;
    
    switch (reac) {
        case 11: tmp_reactype = 11; break;
        case 13: tmp_reactype = 13; break;
        case 21: tmp_reactype = 21; break;
        case 26: tmp_reactype = 26; break;
        case 41: tmp_reactype = 41; break;
        case 46: tmp_reactype = 46; break;
        default: tmp_reactype = 99; break;
    }
    
    ReactionTypes reactype = static_cast<ReactionTypes>(tmp_reactype);
    
    int tmp_target = 999;
    switch (tar) {
        case 1 :  tmp_target = 1;   break;
        default: tmp_target = 999; break;
    }
    
    TargetTypes target = static_cast<TargetTypes>(tmp_target);
    
    int binn = BADBIN;
    for(size_t i=0;i<m_bins.size();i++){
        
        if(target == Hydrogen && reactype == ReRec11 && D1true < 1e-5) continue;
        
        if(m_bins[i].topology == sampletype && m_bins[i].reaction == reactype && m_bins[i].target == target &&
           (D1reco > m_bins[i].recoD1low) && (D1reco  < m_bins[i].recoD1high)  &&
           (D2reco  > m_bins[i].recoD2low) && (D2reco  < m_bins[i].recoD2high) &&
           (D1true > m_bins[i].trueD1low) && (D1true  < m_bins[i].trueD1high)  &&
           (D2true  > m_bins[i].trueD2low) && (D2true  < m_bins[i].trueD2high)){
            binn = (int)i;
            break;
        }
        
    }
    
    cout<<"topology "<<sampletype<<" : reaction "<<reactype << " : target "<< target <<endl;
    cout<<"recoP "<<D1reco<<"  trueP "<<D1true<<"    recoD2 "<<D2reco<<"  trueD2 "<<D2true<<endl;
    cout<<"BIN "<<binn<<endl<<endl;
    
    return binn;
}

// initEventMap
void XsecParameters::InitEventMap(std::vector<AnaSample*> &sample)
{
    if(m_bins.empty())
    {
        cout<<"Need to build map of response functions for "<<m_name<<" ... exiting ..."<<endl;
        exit(-1);
    }
    m_evmap.clear();
    
    int counter = 0;
    //loop over events to build index map
    for(size_t s=0;s<sample.size();s++)
    {
        vector<int> row;
        for(int i=0;i<sample[s]->GetN();i++)
        {
            AnaEvent *ev = sample[s]->GetEvent(i);
            //skip reactions not prepared in response function
            /*int code = PASSEVENT;
             if(ev->GetReaction() == AntiNu ||
             ev->GetReaction() == OutFGD)
             {
             row.push_back(code);
             continue;
             }*/
            //get event info
            int binn = GetBinIndex(static_cast<SampleTypes>(ev->GetSampleType()), ev->GetNuReaction(), ev->GetTarget(), ev->GetRecD1trk(),ev->GetTrueD1trk(),ev->GetRecD2trk(),ev->GetTrueD2trk());
            /*if(binn == BADBIN)
            {
                cout<<"WARNING: "<<m_name<<" event "
                <<" fall outside bin ranges"<<endl;
                cout<<" This event will be ignored in analysis."
                <<endl;
                ev->Print();
                counter++;
            }*/
            
            row.push_back(binn);
        }//event loop
        m_evmap.push_back(row);
    }//sample loop
    
    //cout << "Number of bad events " << counter << endl;
}


// EventWeghts
void XsecParameters::EventWeights(std::vector<AnaSample*> &sample, std::vector<double> &params)
{
    for(size_t s=0;s<sample.size();s++)
    {
        for(int i=0;i<sample[s]->GetN();i++)
        {
            AnaEvent *ev = sample[s]->GetEvent(i);
            ReWeight(ev, s, i, params);
        }
    }
}

// ReWeight
void XsecParameters::ReWeight(AnaEvent *event, int nsample, int nevent, std::vector<double> &params)
{
    if(m_evmap.empty()) //need to build an event map first
    {
        cout<<"Need to build event map index for "<<m_name<<endl;
        return;
    }
    
    int binn = m_evmap[nsample][nevent];
    
    if(binn == PASSEVENT) return;
    if(binn == BADBIN)
        event->AddEvWght(0.0); //skip!!!!
    else
    {
        std::vector <TGraph*> respfuncs = m_bins[binn].respfuncs;
        std::vector <bool> tmp_resexists = m_bins[binn].resexists;
        
        cout<<"SIZE : " << respfuncs.size() << " ::: EXPECTED " << Npar << endl;
        double weight=1;
        if(respfuncs.size()>0){ //needed because there are missing reponse functions when reco very different from true (to save memory)
            int tmp_counts = 0;
            for(uint i=0; i<Npar; i++){
                //This assumes that the response functions are always the same as the response functions.
                cout << "Par " << i << " : ";
                if(tmp_resexists[i]){
                    cout << "Found " << tmp_counts;
                    tmp_counts++;
                    weight = weight*(respfuncs[i]->Eval(params[i]));
                    if(weight!=1)
                    cout<<"reweighting using weight "<<weight<<"  from bin "<<binn<<endl;
                }
                cout << "" << endl;
            }
            
        }
        
        double prevWght = event->GetEvWght();
        event->AddEvWght(weight);
        double newWght = event->GetEvWght();
        
        //For debugging reweighting:
        // if(prevWght!=newWght){
        //   cout << "Reweighted! From " << prevWght << " to " << newWght << endl;
        //   getchar();
        // }
    }
}

/*void XsecParameters::AddResponse(TString filename, TString param_name, double cov_val){
 
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
 }*/

/*void XsecParameters::SetUpParams(std::vector<std::pair <double,double> > v_D1edges, std::vector<std::pair <double,double> > v_D2edges){
 
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
 
 this->StoreResponseFunctions(m_resfiles, v_D1edges, v_D2edges);
 }*/

/*void XsecParameters::AddCorrelation(TString par_name1, TString par_name2, double cov_val){
 
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
        cout << "XsecParameters::AddCorrelation : Parameters are the same!" << endl;
    }
    else if(par_name1_pos == -999 || par_name2_pos == -999){
        cout << "XsecParameters::AddCorrelation : Error could not determine position of parameter(s):";// << endl;
        
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
}*/