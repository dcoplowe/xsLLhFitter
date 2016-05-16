/******************************************************
 
 This is the main for the fitter. In the long run I hope
 to make it work like the xsTool macros: you set all your
 inputs here and then can avoid delving into the fitters
 backend. For the moment this isn't completely the case
 for example a lot of the systematics specifics are in
 the appropriate systematic parameters methods.
 
 
 ******************************************************/


#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <TCanvas.h>
#include <TH1F.h>

#include "FluxParameters.hh"
#include "FluxParameters_norm.hh"
#include "DetParameters.hh"
#include "FSIParameters.hh"
#include "NuclFSIParameters.hh"
#include "XsecParameters.hh"
#include "FitParameters.hh"
#include "XsecFitter.hh"
#include "anyTreeMC.hh"
#include "AnySample.hh"


using namespace std;

int main(int argc, char *argv[])
{
    
    char const * xslf_env = getenv("XSLLINPUTDIR");
    if(!xslf_env){
        std::cerr << "[ERROR]: environment variable \"XSLLINPUTDIR\" not set. "
        "Cannot determine source tree location." << std::endl;
        return 1;
    }
    string inputDir = std::string(xslf_env);// + "/inputs";
    string fsel     = inputDir + "llFormat/neutP6BWA_nall_data_180316_allsyst_llFormat.root";
    string fakeData     = inputDir + "llFormat/neutP6BWA_nall_data_180316_allsyst_llFormat.root";
    string ffluxcov     = inputDir + "inputs/flux_covariance_banff_13av1.1.root";
    string fdetcov_fine = inputDir + "inputs/det_syst_11bins_pm250_dpTT.root";
    string fdetcov = inputDir + "inputs/detCovMatCoarse.root";
    string fxsecbinning = inputDir + "inputs/dpTT_symvarbinning_nb11_m250_p250.txt";
    string fnameout = inputDir + "fitResult/cc1P1PiOutput.root";
    //double potD     = 372.67;   //in units of 10^19 GENIE Air
    //double potD     = 349.15;   //in units of 10^19 NEUT Water
    double potD     = 331.6; //in units 10^19 Neut Air
    double potMC    = 331.6; //in units 10^19 Neut Air
    int seed        = 1019;
    double regparam = 0.0;
    
    //get command line options
    char cc;
    while((cc = getopt(argc, argv, "i:e:b:o:n:N:s:h:f:r:")) != -1)
    {
        switch(cc)
        {
            case 'i': //selected events
                fsel = optarg;
                break;
            case 'e': //flux covariance matrix
                ffluxcov = optarg;
                break;
            case 'd': //det covariance matrix
                fdetcov = optarg;
                break;
            case 'b': //binning for xsec weights
                fxsecbinning = optarg;
                break;
            case 'o': //output file
                fnameout = optarg;
                break;
            case 'n': //data POT
                potD = atof(optarg);
                break;
            case 'N': //MC POT
                potMC = atof(optarg);
                break;
            case 's': //random seed
                seed = atoi(optarg);
                break;
            case 'f': //fake data file
                fakeData = optarg;
                break;
            case 'r': //regularisation param
                regparam = atof(optarg);
                break;
            case 'h': //help
                std::cout << "USAGE: "
                << argv[0] << " OPTIONS:" << std::endl
                << "-i : \tset MC location" << std::endl
                << "-e : \tset flux covariances location" << std::endl
                << "-d : \tset detector covariances location" << std::endl
                << "-b : \tset xec bin definitions" << std::endl
                << "-o : \tset name of output file" << std::endl
                << "-n : \tset POT for data in units 10**19" << std::endl
                << "-N : \tset POT for MC in units 10**19" << std::endl
                << "-s : \tset random seed" << std::endl
                << "-f : \tset (fake) data location" << std::endl
                << "-r : \tset regularisation parameter" << std::endl;
                return 0;
                break;
            default:
                return 1;
        }
    }
    
    cout << "*****************************************************************************************" << endl;
    cout << "|                                     Running Fit                                       |"<<endl << endl;
    cout << "*****************************************************************************************" << endl;
    cout << "                                   Input Parameters "<<endl << endl;
    cout << "        Fake Data File: " << fakeData << endl;
    cout << "      Monte Carlo File: " << fsel << endl << endl;
    cout << "    Target Output File: " << fnameout << endl << endl;
    cout << "          Binning File: " << fxsecbinning << endl << endl;
    cout << "         Fake Data POT: " << potD << " MC POT: "<<potMC << " :: Scale Factor FD/MC = " << (potD/potMC) << endl << endl;
    cout << "|          ************************** Systematics **************************            |" << endl << endl;
    cout << "       Flux Covariance: " << ffluxcov << endl;
    cout << "   Detector Covariance: " << fdetcov << endl;
    cout << "*****************************************************************************************" << endl;
    
    /************************* Set Output File name ******************************/
    
    TFile *fout = TFile::Open(fnameout.c_str(), "RECREATE");
    cout<<"file open"<<endl;
    
    /************************ Deterimine Binning Info ****************************/
    
    std::vector<std::pair<double, double> > v_D1edges;
    std::vector<std::pair<double, double> > v_D2edges;
    std::vector<std::pair<double, double> > v_D1edges_Dummy;
    std::vector<std::pair<double, double> > v_D2edges_Dummy;
    ifstream fin(fxsecbinning.c_str());
    assert(fin.is_open());
    string line;
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
        v_D1edges.push_back(make_pair(D1_1,D1_2));
        v_D2edges.push_back(make_pair(D2_1,D2_2));
        v_D1edges_Dummy.push_back(make_pair(D1_1-9999.0,D1_2-9999.0));
        v_D2edges_Dummy.push_back(make_pair(D2_1-9999.0,D2_2-9999.0));
    }
    fin.close();
    
    /*************************      Get Fake Data     ******************************/
    
    // Setup data trees
    
    TFile *fdata = new TFile(TString(fakeData));
    TTree *tdata = (TTree*)(fdata->Get("selectedEvents"));
    
    /************************* Get Samples from Fake Data **************************/
    
    // Add analysis samples:
    
    vector<AnaSample*> samples;
    AnySample sam1(0, "muTPC-pTPC-piTPC",v_D1edges, v_D2edges,tdata);
    sam1.SetNorm(potD/potMC);
    samples.push_back(&sam1);
    
    /************************* Get MC Data **************************/
    //read MC events

    anyTreeMC selTree(fsel.c_str());
    cout << "Reading and collecting events" << endl;
    selTree.GetEvents(samples);
    //get brakdown by reaction
    cout << "Getting sample breakdown by reaction" << endl;
    for(size_t s=0;s<samples.size();s++){
        ((AnySample*)(samples[s]))->GetSampleBreakdown(fout,"nominal");
    }
    
    cout<<"nominal data done"<<endl;
    
    /************************* FITTER SETTINGS ***********************/
    //In the bit below we choose which params are used in the fit
    //For stats only just use fit params
    //**********************************************************
    
    //define fit param classes
    vector<AnaFitParameters*> fitpara;
    //fitpara.SetFluxHisto(h_flux);
    
    //Fit parameters
    FitParameters sigfitpara(fxsecbinning.c_str());
    sigfitpara.InitEventMap(samples);
    fitpara.push_back(&sigfitpara);
    
    cout<<"fit parameters done"<<endl;
    
    /******************* Get Middle Bin for Abs Xsec *****************/
    //Want to get the middle bin and set it as the only free parameter:
    int npars = v_D1edges.size();
    if(npars % 2 == 0){
        cout << "ccqefit::ERROR : Even Number of bins. Analysis requires odd no. of bins" << endl;
        exit(0);
    }
    
    int free_par = (npars-1)/2;
    cout << "Free Parameter Number: " << free_par << endl;
    
    
    /*********************** Initialise Fitter ***********************/
    XsecFitter xsecfit(seed);
    xsecfit.SetFreePar(free_par);//Specific for the abs Xsec

    //init w/ para vector
    std::cout << "initialising fitter with regularisaion param " << regparam << std::endl;
    xsecfit.InitFitter(fitpara, regparam);
    //set frequency to save output
    xsecfit.SetSaveMode(fout, 1);
    
    //do fit: 1 = generate toy dataset from nuisances (WITH stat fluct)
    //        2 = fake data from MC or real data (+ stat fluct)
    //        3 = no nuisance sampling only stat fluctuation
    xsecfit.Fit(samples, 2);
    
    fout->Close();
    
    cout << "*****************************************************************************************" << endl;
    cout << "|                                    Finished Fit                                       |"<<endl << endl;
    cout << "*****************************************************************************************" << endl;
    
    return 0;
}
