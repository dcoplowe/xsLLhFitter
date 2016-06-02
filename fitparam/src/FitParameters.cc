#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>

#include "FitParameters.hh"

using namespace std;

bool FPDEBUG = true;

FitParameters::FitParameters(const char *fname, const char *name)
{
    m_name    = name;
    hasCovMat = false;
    
    //ccqe_recode = 0; //defunct as it stands, signal is defined manually below
    
    //get the binning from a file
    SetBinning(fname);
    
    //parameter names & prior values
    for(size_t i=0;i<Npar;i++)
    {
        pars_name.push_back(Form("%s%d", m_name.c_str(), (int)i));
        pars_prior.push_back(1.0); //all weights are 1.0 a priori
        pars_step.push_back(0.1);
        pars_limlow.push_back(0.0);
        pars_limhigh.push_back(10.0);
    }
}

FitParameters::~FitParameters()
{;}

void FitParameters::SetBinning(const char *fname)
{
    ifstream fin(fname);
    assert(fin.is_open());
    string line;
    AnyBin bin;
    while (getline(fin, line))
    {
        stringstream ss(line);
        double D1_1, D1_2, D2_1, D2_2;
        if(!(ss>>D2_1>>D2_2>>D1_1>>D1_2))
        {
            cerr<<"Bad line format: "<<endl
            <<"     "<<line<<endl;
            continue;
        }
        bin.D1low    = D1_1;
        bin.D1high   = D1_2;
        bin.D2low  = D2_1;
        bin.D2high = D2_2;
        m_bins.push_back(bin);
    }
    fin.close();
    Npar = m_bins.size();
    
    cout<<endl<<"CC1P1Pi binning:"<<endl;
    for(size_t i = 0;i<m_bins.size();i++)
    {
        cout<<setw(3)<<i
        <<setw(10)<<m_bins[i].D2low
        <<setw(10)<<m_bins[i].D2high
        <<setw(10)<<m_bins[i].D1low
        <<setw(10)<<m_bins[i].D1high<<endl;
    }
    cout<<endl;
}

int FitParameters::GetBinIndex(double D1, double D2)
{
    int binn = BADBIN;
    for(size_t i=0;i<m_bins.size();i++)
    {
        if(D1>=m_bins[i].D1low && D1<m_bins[i].D1high && D2>=m_bins[i].D2low && D2<m_bins[i].D2high)  binn = (int)i;
    }
    return binn;
}


// initEventMap
void FitParameters::InitEventMap(std::vector<AnaSample*> &sample)
{
    m_evmap.clear();
    
    //loop over events to build index map
    for(size_t s=0;s<sample.size();s++)
    {
        vector<int> row;
        //int count = 0;
        for(int i=0;i<sample[s]->GetN();i++)
        {
            AnaEvent *ev = sample[s]->GetEvent(i);
            int code = PASSEVENT; // -1 by default
            
            // SIGNAL DEFINITION TIME
            // Warning, important hard coding up ahead:
            // This is where your signal is actually defined, i.e. what you want to extract an xsec for
            // N.B In Sara's original code THIS WAS THE OTHER WAY AROUND i.e. this if statement asked what was NOT your signal
            // Bare that in mind if you've been using older versions of the fitter.
            //cout << "Tar = " << ev->GetTarget() << " Reac = " << ev->GetNuReaction() << " dpTT = " << ev->GetTrueD1trk() << endl;
            
            if( ev->GetTarget() == 1 && ev->GetNuReaction() == 11 && TMath::Abs(ev->GetTrueD1trk()) < 1e-5) //pass if  CC1P1Pi Hydrogen
            {
                //get event true D1 and D2
                //count++;
                double D1 = ev->GetTrueD1trk();
                double D2 = ev->GetTrueD2trk();
                int binn   = GetBinIndex(D1, D2);
                //cout << "EVENT " << count << endl;
                //cout << "BadBin : " << binn << endl;
                if(binn == BADBIN)// && FPDEBUG)
                {
                    cout<<"WARNING: "<<m_name<<" D1 = "<<D1<<" D2 = "<<D2<<" fall outside bin ranges"<<endl;
                    cout<<"        This event will be ignored in analysis."<<endl;
                }
                row.push_back(binn);
            }
            else{
                row.push_back(code);
                continue;
            }
            
        }
        m_evmap.push_back(row);
    }
}

// EventWeghts
void FitParameters::EventWeights(std::vector<AnaSample*> &sample,
                                 std::vector<double> &params)
{
    if(m_evmap.empty()) //build an event map
    {
        cout<<"Need to build event map index for "<<m_name<<endl;
        InitEventMap(sample);
    }
    
    for(size_t s=0;s<sample.size();s++)
    {
        for(int i=0;i<sample[s]->GetN();i++)
        {
            AnaEvent *ev = sample[s]->GetEvent(i);
            ReWeight(ev, s, i, params);
        }
    }
}


void FitParameters::ReWeight(AnaEvent *event, int nsample, int nevent, std::vector<double> &params)
{
    if(m_evmap.empty()) //need to build an event map first
    {
        cout<<"Need to build event map index for "<<m_name<<endl;
        return;
    }
    
    int binn = m_evmap[nsample][nevent];
    
    //skip event if not Signal
    if(binn == PASSEVENT){
        return;
    }
    
    //cout << "SIGNAL" << endl;
    
    // If bin fell out of valid ranges, pretend the event just didn't happen:
    if(binn == BADBIN) event->AddEvWght(0.0); 
    else
    {
        if(binn>(int)params.size())
        {
            cerr<<"ERROR: number of bins "<<m_name<<" does not match num of param"<<endl;
            event->AddEvWght(0.0);
        }
        event->AddEvWght(params[binn]);
        //cout << "ReWeight param " << binn << endl;
        //cout << "Weight is " << params[binn] << endl;
    }
}
