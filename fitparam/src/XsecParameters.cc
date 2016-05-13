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
 
  //More hard coding:
  //Here we need to set the FSI params that we're going to use
  //and the step info for MINUIT.
  //One day this will be done in the main ...

  // Sara's old parameters for CC0Pi:

  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 1,"CCoth"));
  // pars_prior.push_back(0.0);
  // pars_step.push_back(0.4/5);
  // pars_limlow.push_back(-1.2);
  // pars_limhigh.push_back(1.2);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 2,"PilessDcy"));
  // pars_prior.push_back(0.0);
  // pars_step.push_back(0.2/5);
  // pars_limlow.push_back(-0.6);
  // pars_limhigh.push_back(0.6);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 3,"CC1piE0"));
  // pars_prior.push_back(1.1);
  // pars_step.push_back(0.43/5.);
  // pars_limlow.push_back(-1.40);
  // pars_limhigh.push_back(2.44);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 4,"CC1piE1"));
  // pars_prior.push_back(1.0);
  // pars_step.push_back(0.40/5.);
  // pars_limlow.push_back(-0.2);
  // pars_limhigh.push_back(2.2);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 5,"CCCohE0"));
  // pars_prior.push_back(1);
  // pars_step.push_back(1./5.);
  // pars_limlow.push_back(-2);
  // pars_limhigh.push_back(4);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 6,"NCoth"));
  // pars_prior.push_back(1.0);
  // pars_step.push_back(0.3/5);
  // pars_limlow.push_back(0.1);
  // pars_limhigh.push_back(1.9);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 7,"NC1pi0E0"));
  // pars_prior.push_back(0.96);
  // pars_step.push_back(0.43/5.);
  // pars_limlow.push_back(-0.33);
  // pars_limhigh.push_back(2.25);
  // pars_name.push_back(Form("%s%d%s", m_name.c_str(), 8,"NC1piE0"));
  // pars_prior.push_back(1.0);
  // pars_step.push_back(0.3/5.);
  // pars_limlow.push_back(0.1);
  // pars_limhigh.push_back(1.9);

  Npar=9;

  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 0,"CA5"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5); 
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 1,"MANFFRES"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 2,"BgRES"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 3,"CCNUE_0"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 4,"DISMPISHP"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 5,"CCCOH_C_0"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 6,"NCCOH_0"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 7,"NCOTHER_0"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
  pars_name.push_back(Form("%s%d%s", m_name.c_str(), 8,"EB_C"));
  pars_prior.push_back(1.00);
  pars_step.push_back(0.11);
  pars_limlow.push_back(0.5);
  pars_limhigh.push_back(2.5);
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
void XsecParameters::StoreResponseFunctions(vector<TFile*> respfuncs, std::vector<std::pair <double,double> > v_D1edges, 
              std::vector<std::pair <double,double> > v_D2edges)
{
  double dummyx[7]={-1,-0.66,-0.33,0,0.33,0.66,1};
  double dummyy[7]={1,1,1,1,1,1,1};
  int    dummyn=7;

  for ( int stInt = 0; stInt < 8; stInt++ ){
    if((stInt==0) || (stInt==4)) continue; // Ignore branches with no proton
    SampleTypes sampletype = static_cast <SampleTypes> (stInt);
    for ( int rtInt = 0; rtInt < 7; rtInt++){
      if(rtInt==6) continue; // Hack to deal with missing mectopo6
      ReactionTypes reactype = static_cast<ReactionTypes>(rtInt);
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
          if(fabs(br-bt)<21) {  //save memory if reco bin and true bin very far away
            for(uint i=0; i<Npar; i++){
              char name[200];
              sprintf(name,"topology_%d/RecBin_%d_trueBin_%d_topology_%d_reac_%d",stInt,br,bt,stInt,rtInt);
              //cout<<respfuncs[i]->GetName()<<" "<<name<<endl;
              TGraph* g=(TGraph*)respfuncs[i]->Get(name);
              //cout<<g<<endl;
              if(!g){
                if(i!=8 && br!=8 && bt!=8){ // Ignore Eb issues for the moment
                  cout << "Warning, creating dummy respfunc, param: " << i << " " << name << endl;
                  cout << "getchar to cont" << endl;
                  getchar();
                }
                g = new TGraph(dummyn, dummyx, dummyy);
              }            
              g->SetName(name);
              if((g->GetY())[3]!=1.0){
                cout << "Warning: altering xsec nominal param: " << i << " " << name << endl;
              }  
              bin.respfuncs.push_back(g);
            }
          }
          m_bins.push_back(bin);
        }
      }
    }
  }

  /*for(size_t j=0; j<m_bins.size();j++){
    cout<<j<<" topology: "<<m_bins[j].topology<<"  reaction: "<<m_bins[j].reaction
  <<"  recoP: "<<m_bins[j].recoD1low<<"-"<<m_bins[j].recoD1high
  <<"  trueP: "<<m_bins[j].trueD1low<<"-"<<m_bins[j].trueD1high
  <<"  recoD2: "<<m_bins[j].recoD2low<<"-"<<m_bins[j].recoD2high
  <<"  trueD2: "<<m_bins[j].trueD2low<<"-"<<m_bins[j].trueD2high<<endl;
    if(m_bins[j].respfuncs.size()>0)
  cout<<" response function name "<<m_bins[j].respfuncs[0]->GetName()<<endl;
      else
  cout<<" no response function"<<endl;
  }*/
  
}

// --
int XsecParameters::GetBinIndex(SampleTypes sampletype, ReactionTypes reactype, 
				double D1reco, double D1true, double D2reco, double D2true)
{
  int binn = BADBIN;
  for(size_t i=0;i<m_bins.size();i++)
  {
    if(m_bins[i].topology == sampletype && m_bins[i].reaction == reactype &&
	  (D1reco > m_bins[i].recoD1low) && (D1reco  < m_bins[i].recoD1high)  &&
	  (D2reco  > m_bins[i].recoD2low) && (D2reco  < m_bins[i].recoD2high) &&
	  (D1true > m_bins[i].trueD1low) && (D1true  < m_bins[i].trueD1high)  &&
	  (D2true  > m_bins[i].trueD2low) && (D2true  < m_bins[i].trueD2high)){
	    binn = (int)i;
	    break;
	  }
  }
  /*cout<<"topology "<<sampletype<<"  reaction "<<reactype<<endl;
  cout<<"recoP "<<D1reco<<"  trueP "<<D1true<<"    recoD2 "<<D2reco<<"  trueD2 "<<D2true<<endl;
  cout<<"BIN "<<binn<<endl<<endl;*/
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
	    int binn = GetBinIndex(static_cast<SampleTypes>(ev->GetSampleType()),
				                     static_cast<ReactionTypes>(ev->GetReaction()),
				                     ev->GetRecD1trk(),ev->GetTrueD1trk(),ev->GetRecD2trk(),ev->GetTrueD2trk());
	    if(binn == BADBIN) 
	    {
	      cout<<"WARNING: "<<m_name<<" event "
		    <<" fall outside bin ranges"<<endl;
	      cout<<" This event will be ignored in analysis."
	      <<endl;
	      ev->Print();
	    }
	    row.push_back(binn); 
	  }//event loop
    m_evmap.push_back(row);
  }//sample loop
}


// EventWeghts
void XsecParameters::EventWeights(std::vector<AnaSample*> &sample, 
				  std::vector<double> &params)
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
void XsecParameters::ReWeight(AnaEvent *event, int nsample, int nevent,
			      std::vector<double> &params)
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
    vector <TGraph*> respfuncs = m_bins[binn].respfuncs;
    double weight=1;
    if(respfuncs.size()>0){ //needed because there are missing reponse functions when reco very different from true (to save memory)
  	  for(uint i=0; i<Npar; i++){
  	    weight = weight*(respfuncs[i]->Eval(params[i]));
  	    //if(weight!=1)
  	    //cout<<"reweighting using weight "<<weight<<"  from bin "<<binn<<endl;
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


