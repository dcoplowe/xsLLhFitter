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
#include <TRandom3.h>

#include "TDatime.h"

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
    
    //Binning and input/output data files:
    string fsel         = inputDir + "llFormat/neutP6BWA_nall_data_170316_llFormat.root";
    string fakeData     = inputDir + "llFormat/neutP6BWA_nall_data_170316_llFormat.root";
    
    string fxsecbinning = inputDir + "inputs/dpTT_symvarbinning_nb11_m250_p250.txt";
    
    TDatime time;
    string date = Form("_%.2d%.2d%.2d", time.GetDay(), time.GetMonth(), (time.GetYear()-2000));
    
    string fnameout     = inputDir + "fitResult/cc1P1PiOutput_new" + date + ".root";
    
    double potD     = 5.734; //in units 10^21 Neut Air
    double potMC    = 5.734; //in units 10^21 Neut Air

    int myseed        = 1019;
    double regparam = 0.0;
    
    //Data type choice:
    int datatype = 2;
    int ntoys = 1;
    
    //Systematics:
    //Det:
    string detcov_binning = inputDir + "inputs/dpTT_symvarbinning_nb11_m250_p250_syst.txt";
    string fdetcov_fine   = inputDir + "inputs/det_syst_nb11_m250_p250_syst.root";  //USING NEW BINNING DET MATRIX
    string fdetcov        = inputDir + "inputs/det_syst_nb11_m250_p250_syst.root";  //USING NEW BINNING DET MATRIX
    
    //Flux:
    string ffluxcov       = inputDir + "inputs/flux_covariance_banff_13av1.1.root";
    
    //get command line options
    char cc;
    while((cc = getopt(argc, argv, "i:e:b:o:n:N:s:f:r:c:d:t:h:l:")) != -1)
    {
        switch(cc)
        {
            case 'i': //selected events
                fsel = optarg;
                break;
            case 'e': //flux covariance matrix
                ffluxcov = optarg;
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
            case 's': //random myseed
                myseed = atoi(optarg);
                break;
            case 'f': //fake data file
                fakeData = optarg;
                break;
            case 'r': //regularisation param
                regparam = atof(optarg);
                break;
            case 'c':
                detcov_binning = optarg;
                break;
            case 'd': //det covariance matrix
                fdetcov = optarg;
                break;
            case 't':
                datatype = atoi(optarg);
                break;
            case 'l': 
                ntoys = atoi(optarg);
                break;
            case 'h': //help
                std::cout << "USAGE: "
                << argv[0] << " OPTIONS:" << std::endl
                << "-i : \tset MC location" << std::endl
                << "-e : \tset flux covariances location" << std::endl
                << "-b : \tset xec bin definitions" << std::endl
                << "-o : \tset name of output file" << std::endl
                << "-n : \tset POT for data in units 10**19" << std::endl
                << "-N : \tset POT for MC in units 10**19" << std::endl
                << "-s : \tset random seed" << std::endl
                << "-f : \tset (fake) data location" << std::endl
                << "-r : \tset regularisation parameter" << std::endl
                << "-c : \tset syst. binning" << std::endl
                << "-d : \tset detector covariances location" << std::endl
                << "-t : \tset datatype 1 2 or 3" << std::endl;
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
    cout<<"Set and opened outfile."<<endl;
    
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
    if(!fdata->IsOpen()){
        cout << "Could not find file: " << fakeData << endl;
        cout << "Please check... Exiting" << endl;
        return 0;
    }
    
    TTree *tdata = (TTree*)(fdata->Get("selectedEvents"));
    
    /************************* Get Samples from Fake Data **************************/
    
    // Add analysis samples:
    
    vector<AnaSample*> samples;
    AnySample sam1(0, "muTPC-pTPC-piTPC",v_D1edges, v_D2edges,tdata);
    sam1.SetNorm(potD/potMC);
    samples.push_back(&sam1);
    
    /************************* Get MC Data **************************/
    //read MC events
    
    //Check if the file exists first:
    TFile *mctmpdata = new TFile(TString(fsel));
    if(!mctmpdata->IsOpen()){
        cout << "Could not find file: " << fsel << endl;
        cout << "Please check... Exiting" << endl;
        return 0;
    }
    mctmpdata->Close();
    delete mctmpdata;

    anyTreeMC selTree(fsel.c_str());
    cout << "Reading and collecting events" << endl;
    selTree.GetEvents(samples);
    //get brakdown by reaction
    cout << "Getting sample breakdown by reaction" << endl;
    for(size_t s=0;s<samples.size();s++){
        ((AnySample*)(samples[s]))->GetSampleBreakdown(fout,"nominal");
    }
    
    cout<<"nominal data done"<<endl;
    
    /*************************************************************************************************************************/
    /*************************************************************************************************************************/
    /*******************************************                                   *******************************************/
    /*******************************************          THE SYSTEMATICS          *******************************************/
    /*******************************************                                   *******************************************/
    /*************************************************************************************************************************/
    /*************************************************************************************************************************/
    
    /*****************************************************************************************************************/
    /*********************************           DETECTOR SYSTS.         *********************************************/
    /*****************************************************************************************************************/
    //setup D1,D2 bins, starting param values and covm for det syst --------------
    
    //We require two detector covariances: 1) Course 2) fine. which is used in the fit for
    
    TFile * findetcov = new TFile(fdetcov.c_str(), "READ"); //contains flux and det. systematics info
    
    TMatrixDSym *cov_det_in   = (TMatrixDSym*)findetcov->Get("covMat_norm");//Check name is correct
    
    TMatrixDSym cov_det(cov_det_in->GetNrows());
    
    for(size_t m=0; m<cov_det_in->GetNrows(); m++){
        for(size_t k=0; k<cov_det_in->GetNrows(); k++){
            cov_det(m, k) = (*cov_det_in)(m,k);
        }
    }
    
    findetcov->Close();
    
    cov_det.SetTol(1e-200);
    cov_det.Print();
    
    double det = cov_det.Determinant();
    if(abs(det) < 1e-200){
        cout << "Warning, det cov matrix is non invertable. Det is:" << endl;
        cout << det << endl;
        return 0;
    }
    
    
    //Throw this code away later (24/05/16)
    
    int ndetcovmatele = 11;//11;//Should this be the size of the number of bins?
    
    TString tmpstring(detcov_binning.c_str());
    string bin_choice;
    
    if(tmpstring.Contains("dpTT_symvarbinning_nb11_m250_p250_overflow_syst.txt", TString::kIgnoreCase)){
        ndetcovmatele = 7;
        bin_choice = "nb11_m250_p250_overflow";
    }
    else if(tmpstring.Contains("dpTT_symvarbinning_nb11_m250_p250_syst.txt", TString::kIgnoreCase)){
        ndetcovmatele = 5;
        bin_choice = "nb11_m250_p250";
    }
    else if(tmpstring.Contains("dpTT_symvarbinning_nb9_m300_p300_overflow_syst.txt", TString::kIgnoreCase)){
        ndetcovmatele = 7;
        bin_choice = "nb9_m300_p300_overflow";
    }
    else if(tmpstring.Contains("dpTT_symvarbinning_nb9_m300_p300_syst.txt", TString::kIgnoreCase)){
        ndetcovmatele = 5;
        bin_choice = "nb9_m300_p300";
    }
    
    double * arr = new double[ndetcovmatele];
    
    for(int i=0; i<ndetcovmatele; i++){ arr[i]=(1.0);}
    
    TVectorD* det_weights = new TVectorD(ndetcovmatele, arr);
    
    delete arr;
    
    det_weights->Print();
    /******************************* Fine Covariance Only used for Fake Data *************************************/
    
    TFile * findetcov_fine = TFile::Open(fdetcov_fine.c_str()); //contains flux and det. systematics info
    
    TMatrixDSym *cov_det_in_fine   = (TMatrixDSym*)findetcov_fine->Get("covMat_norm");
    
    TMatrixDSym cov_det_fine(cov_det_in_fine->GetNrows());
    for(size_t m=0; m<cov_det_in_fine->GetNrows(); m++){
        for(size_t k=0; k<cov_det_in_fine->GetNrows(); k++){
            cov_det_fine(m, k) = (*cov_det_in_fine)(m,k);
        }
    }
    
    cov_det_fine.SetTol(1e-200);
    cov_det_fine.Print();
    double det_fine = cov_det_fine.Determinant();
    
    if(abs(det_fine) < 1e-200){
        cout << "Warning, det cov matrix with fine binning is non invertable. Det is:" << endl;
        cout << det_fine << endl;
        return 0;
    }
    
    findetcov_fine->Close();
    
    /*****************************************************************************************************************/
    /*********************************       FLUX UNCERTAINTY INPUTS     *********************************************/
    /*****************************************************************************************************************/
    
    //setup enu bins and covm for flux    //get info on the priors
    TFile *finfluxcov = TFile::Open(ffluxcov.c_str()); //contains flux and det. systematics info
    /*************************************** FLUX *****************************************/
    
    TAxis *nd_numu_bins = (TAxis*)finfluxcov->Get("nd5_numode_numu_bins");
    TMatrixDSym *cov_flux_in   = (TMatrixDSym*)finfluxcov->Get("total_flux_cov");
    TMatrixDSym cov_flux(nd_numu_bins->GetNbins());
    vector<double> enubins;
    enubins.push_back(nd_numu_bins->GetBinLowEdge(1));
    for(int i=0;i<nd_numu_bins->GetNbins();i++)
    {
        enubins.push_back(nd_numu_bins->GetBinUpEdge(i+1));
        for(int j=0;j<nd_numu_bins->GetNbins();j++)
        {
            cov_flux(i, j) = (*cov_flux_in)(i,j);
        }
    }
    
    finfluxcov->Close();
    
    
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
    
    int nbins_2d_temp = v_D2edges.size();
    vector<double> tmp_d2_vec;
    for(int i=0;i<nbins_2d_temp;i++){
        if (tmp_d2_vec.size()==0) tmp_d2_vec.push_back(v_D2edges[i].first);
        else{
            if (tmp_d2_vec.back()!=v_D2edges[i].first) tmp_d2_vec.push_back(v_D2edges[i].first);
        }
    }
    tmp_d2_vec.push_back(v_D2edges[nbins_2d_temp-1].second);
    
    int npars_D1 = v_D1edges.size();
    int npars_D2 = tmp_d2_vec.size()-1;
    
    cout << "There are " << npars_D1 << " 1d bins and " << npars_D2 << " d2 bins" << endl;
    
    /******************* Get Middle Bin for Abs Xsec *****************/
    //Want to get the middle bin and set it as the only free parameter:
    
    int mid_point = (npars_D1 - npars_D2)/(2*npars_D2);
    
    std::vector<int> free_par;
    
    cout << "Midpoint per bin assuming the same binning in D1: " << mid_point << " No. bins in D1 " << npars_D1 << endl;
    
    free_par.push_back(mid_point);
    for(int i=1; i < npars_D2; i++){
        free_par.push_back( (mid_point + npars_D1/npars_D2));
    }
    
    for(int i = 0; i < (int)free_par.size(); i++){
        cout << "D2 Bin " << i+1 <<  ": Middle bin " << free_par[i] << endl;
    }
    
    if(npars_D1 % 2 == 0 && free_par.size() == 1){
        cout << "ccqefit::ERROR : Even Number of bins. Analysis requires odd no. of bins" << endl;
        return 0;
    }
    else{
        cout << "Two Dim fit" << endl;
    }
    
   // return 0;
    
    //****************************************************************************************************//
    //*****************************      Feed Systematics To Fitter     **********************************//
    //****************************************************************************************************//
    
    //******************************* Detector Systematic Parameters     ******************************* //
    /*DetParameters detpara(detcov_binning.c_str(), det_weights, samples, "par_detAve");
    detpara.SetCovarianceMatrix(&cov_det);
    //detpara.InitEventMap(samples);
    fitpara.push_back(&detpara);*/
    
    //Not needed for now as it is required for fake data only
    // DetParameters detpara_fine("../inputs/dptbinning.txt",det_weights_fine, samples, "par_detFine");
    // detpara_fine.SetCovarianceMatrix(&cov_det_fine);
    // detpara_fine.InitEventMap(samples);
    // fitpara.push_back(&detpara_fine);
    
    //*********************************       FLUX UNCERTAINTY  ***************************************** //
    FluxParameters fluxpara(enubins);
    fluxpara.SetCovarianceMatrix(&cov_flux);
    fluxpara.InitEventMap(samples);
    fitpara.push_back(&fluxpara);
    
    //return 0;
    //*********************************** Response Functions  ******************************************* //
    //Check which ones don't work by themselves: --Works ... LOOK INTO.
    
    //XsecParameters xsecpara;//Currently uncertainty on all errors are 100%...
    //xsecpara.SetCovarianceMatrix(&cov_xsec);
    //xsecpara.StoreResponseFunctions(responsefunctions, v_D1edges, v_D2edges);
    
    //These are very low stats. samples that make it into the selection.
   /* xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_CA5RES_" + bin_choice + "_topo0_reac11_tar1.root" ,"H_CA5RES", 0.01412); //--Works
    xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_MaNFFRES_" + bin_choice + "_topo0_reac11_tar1.root" ,"H_MaNFFRES", 0.02493); //--Works
    xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_BgSclRES_" + bin_choice + "_topo0_reac11_tar1.root" ,"H_BgSclRES", 0.02367); //--Works
    xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNIWG2012a_cccohE0_" + bin_choice + "_topo0_reac21_tar1.root" ,"H_cccohE0", 1.); //--Works, error matrix is 100%
    xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNIWG2012a_dismpishp_" + bin_choice + "_topo0_reac26_tar1.root","H_dismpishp", 0.16);//--Works
   */ //These are the more dominate modes entering as background.
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_MaNFFRES_" + bin_choice + "_topo0_reac11_tar999.root" ,"Bkg_11_MaNFFRES", 0.02493); //--Works
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_BgSclRES_" + bin_choice + "_topo0_reac11_tar999.root" ,"Bkg_11_BgSclRES", 0.02367); //--Works, error matrix is 100%
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_CA5RES_" + bin_choice + "_topo0_reac11_tar999.root" ,"Bkg_11_CA5RES", 0.01412); //--Works
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_MaNFFRES_" + bin_choice + "_topo0_reac13_tar999.root" ,"Bkg_13_MaNFFRES", 0.02493);
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_BgSclRES_" + bin_choice + "_topo0_reac13_tar999.root" ,"Bkg_13_BgSclRES", 0.02367);
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNXSec_CA5RES_" + bin_choice + "_topo0_reac13_tar999.root" ,"Bkg_13_CA5RES", 0.01412);
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNIWG2012a_cccohE0_" + bin_choice + "_topo0_reac21_tar999.root" ,"Bkg_cccohE0", 1.);
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNIWG2012a_dismpishp_" + bin_choice + "_topo0_reac26_tar999.root" ,"Bkg_dismpishp", 0.16);
  //  xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNIWG2012a_nccohE0_" + bin_choice + "_topo0_reac41_tar999.root" ,"Bkg_nccohE0", 0.09);
    //xsecpara.AddResponse(inputDir + "inputs/responseFuncs/resFuncNIWG2012a_ncotherE0_" + bin_choice + "_topo0_reac41_tar999.root" ,"Bkg_ncotherE0", 0.09);
    
    //xsecpara.AddCorrelation("Bkg_ncotherE0", "Bkg_13_CA5RES", 0.1);
    //Perhaps I should make the Bkg resonance include both res. types modes for analysis.
    
    //xsecpara.SetUpParams(v_D1edges, v_D2edges);
    //xsecpara.InitEventMap(samples);
    //fitpara.push_back(&xsecpara);
    
 //   return 0;

    TRandom3 seed;
    
    double chi2_tot = 0.;

    TH1D Chi2Dist("Chi2Dir",";#chi^{2};Counts",500, 0., 100.);

    std::vector<TH1D*> par_pulls;
    std::vector<TH1D*> par_bias;
    std::vector<TH1D*> par_error;

    /*********************** Initialise Fitter ***********************/
    for(int i = 0; i < ntoys; i++){
        // cout << "SEED = " << (ntoys != 1 ? seed.Rndm() : myseed) << endl; Does this 
        XsecFitter xsecfit( (ntoys != 1 ? seed.Rndm() : myseed) );
        // Include my seed here too.

        xsecfit.SetFreePar(free_par, npars_D1);//Specific for the abs Xsec -- Should also fix central bins of energy dep. xsec!

        //init w/ para vector
        std::cout << "initialising fitter with regularisaion param " << regparam << std::endl;
        xsecfit.InitFitter(fitpara, regparam);
        //set frequency to save output
        //return 0;
        xsecfit.SetSaveMode(fout, 1);
        //do fit: 1 = generate toy dataset from nuisances (WITH stat fluct)
        //        2 = fake data from MC or real data (+ stat fluct)
        //        3 = no nuisance sampling only stat fluctuation
        xsecfit.Fit(samples, 1);
        // cout << "Changes Made" << endl;

        TMatrixDSym * covI = xsecfit.GetFitCovarianceI();

        // Calculate Chi2 dist:
        double chi2 = 0.;
        for(size_t npars_i = 0; npars_i < xsecfit.GetFitResults().size(); npars_i++){
            for(size_t npars_j = 0; npars_j < xsecfit.GetFitResults().size(); npars_j++){
               chi2 += ((xsecfit.GetFitResults()[npars_i] - xsecfit.GetToyInputs()[npars_i])*(xsecfit.GetFitResults()[npars_j] - xsecfit.GetToyInputs()[npars_j]))*( (*covI)(npars_i,npars_j) );
           }
       }
       // cout << "chi2 = " << chi2 << endl;
       Chi2Dist.Fill(chi2);

       chi2_tot += chi2;

       // Produce pull plots for free params:
       if(i == 0){
            for(size_t npar = 0; npar < xsecfit.GetFitResults().size(); npar++){
                par_pulls.push_back( new TH1D(Form("par%d_pull", (int)npar), ";Pull;Counts", 500, -5., 5.) );
                par_bias.push_back( new TH1D(Form("par%d_bias", (int)npar),  ";Bias;Counts", 500, -5., 5.) );
                par_error.push_back( new TH1D(Form("par%d_error", (int)npar),";Error;Counts", 200, 0, 2.) );
            }
       }

       TMatrixDSym * cov = xsecfit.GetFitCovariance();

       for(size_t npar = 0; npar < xsecfit.GetFitResults().size(); npar++){
            double bias = (xsecfit.GetFitResults()[npar] - xsecfit.GetToyInputs()[npar]);
            double error = TMath::Sqrt( (*cov)(npar, npar) );
            double pull = bias/error;
            par_bias[npar]->Fill(bias);
            par_error[npar]->Fill(error);
            par_pulls[npar]->Fill(pull);
       }
    }

    fout->cd();
    Chi2Dist.Write();
    for(size_t npar = 0; npar < par_pulls.size(); npar++){ 
        par_pulls[npar]->Write();
        par_error[npar]->Write();
        par_bias[npar]->Write();
    }
    fout->Close();
    par_pulls.clear();

    cout << "*****************************************************************************************" << endl;
    cout << "|                                    Finished Fit                                       |"<<endl << endl;
    cout << "*****************************************************************************************" << endl;
    
    return 0;
}
