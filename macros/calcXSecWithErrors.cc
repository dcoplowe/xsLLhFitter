/******************************************************
 
 Code to take output of the fitter and produce a corrected
 Nevents spectrum compared to the MC and fake data truth
 
 Author: Stephen Dolan
 Date Created: Jan 2016
 
 ******************************************************/


#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <TCanvas.h>
#include <TH1F.h>
#include <TTree.h>
#include <TString.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TMath.h>
#include "TLegend.h"

#include "TH1I.h"

//using namespace std;

void BWNorm(TH1D * h1 ){
    
    int nbins = h1->GetNbinsX();

    for (int i=1; i < nbins + 1; i++) {
        double tmp_m =  (h1->GetBinContent(i)/h1->GetBinWidth(i));
        double tmp_e = (h1->GetBinError(i)/h1->GetBinWidth(i));
        //tmp->SetBinContent(i, h1->GetBinWidth(i));
        //printf("Hist: %s : Bin %d: (%f p/m %f)/%f = %f p/m %f\n", h1->GetName(), i, h1->GetBinContent(i), h1->GetBinError(i), h1->GetBinWidth(i), tmp_m, tmp_e);
        h1->SetBinContent(i, tmp_m);
        h1->SetBinError(i,tmp_e);
    }
}

void GeV2MeV(TH1D *&h1){
    if(!h1){
        printf("myrootfunc::Error GeV2MeV: Could not find TH1D\n");
        return;
    }
    TString histname = h1->GetName();
    printf("GeV2MeV******\n Name: %s\n",histname.Data());
    //Int_t nbins = h1->GetNbinsX();
    const int nbins = h1->GetXaxis()->GetNbins();
    const TArrayD * x_bins = h1->GetXaxis()->GetXbins();
    
    Double_t * array = x_bins->GetArray();
    
    for(int i=0; i < nbins + 1; i++){
        printf("Bin %d : %f ",(i+1), array[i]);

        array[i] *= 1000;
        printf("MeV: %f\n",array[i]);
        
    }
    
    TH1D *tmph = new TH1D("tmph","",nbins,array);
    for(Int_t b=1; b < nbins+1; b++){
        tmph->SetBinContent(b,h1->GetBinContent(b));
        tmph->SetBinError(b,h1->GetBinError(b));
    }
    h1->Reset();
    h1 = (TH1D*)tmph->Clone(histname.Data());
    delete tmph;
}

void calcXsecWithErrors (TString mcFilename = "/Users/davidcoplowe/work/t2k/signal_extraction/llFormat/neutP6BWA_nall_data_170316_llFormat.root", TString fitResultFilename = "/Users/davidcoplowe/work/t2k/signal_extraction/fitResult/data_25_05_16/neutP6BWA_nall_data_170316_varbinning_nb9_m300_p300_OF_re11_tar1_dpTT05_fit_25_05_16.root", TString fakeDataFilename = "/Users/davidcoplowe/work/t2k/signal_extraction/llFormat/neutP6BWA_nall_data_170316_llFormat.root", TString outputFileName = "/Users/davidcoplowe/work/t2k/signal_extraction/testing/plotting_style_neutP6BWA_nall_data_170316_varbinning_nb9_m300_p300_OF_re11_tar1_dpTT05_xsec_15_09_16.root", TString finalIter = ""){
   
    TFile* mcFile = new TFile(mcFilename);
    TFile* fitResultFile = new TFile(fitResultFilename);
    TFile* fakeDataFile = new TFile(fakeDataFilename);
    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    
    TTree* selEvtsTree = (TTree*) mcFile->Get("selectedEvents");//Same as Dolan  -- this is the input filename used in analysis
    TTree* selEvtsFakeDataTree = (TTree*) fakeDataFile->Get("selectedEvents");//Same as Dolan -- this is the input filename used in analysis
    
    TH1D* fitParamValues = (TH1D*) fitResultFile->Get("paramhist_parpar_ccqe_result"); //This is the results file from the fit
    TH1D* fitParamErrors = (TH1D*) fitResultFile->Get("paramerrhist_parpar_ccqe_result"); //This is the results file from the fit
    TH1D* histForBinning = (TH1D*) fitResultFile->Get("muTPC-pTPC-piTPC_RecPmu_cc1pi+_nominal");//This is the results file from the fit
    // This isn't a useful distribution for me as it is topological and the true interaction I am able to separate
    
    TH1I * iterations = (TH1I*)fitResultFile->Get("iterations");
    if(iterations){
        int niter = iterations->GetBinContent(1);
        printf("niter : %d\n",niter);
        finalIter.Clear();
        finalIter.Append(Form("%d",niter));
        printf("Final Iteraction = %s\n", finalIter.Data());
    }
    
    if(finalIter.EqualTo("")){
        printf("calcXsecWithErrors::ERROR : No iteraction declared.\n");
        exit(0);
    }

    const int nbinsAuto = histForBinning->GetXaxis()->GetNbins();
    const TArrayD* binsAuto = histForBinning->GetXaxis()->GetXbins();
    
    
    TH1D* totalSpectMC_sam0 = (TH1D*) fitResultFile->Get("evhist_sam0_iter0_mc");
    TH1D* totalSpectFit_sam0 = (TH1D*) fitResultFile->Get("evhist_sam0_iter"+finalIter+"_pred");  //Need to insert correct iter number
    TH1D* totalSpectFD_sam0 = (TH1D*) fitResultFile->Get("evhist_sam0_iter0_data");
    
    TH1D* totalSpectMC_sam1 = (TH1D*) fitResultFile->Get("evhist_sam1_iter0_mc");
    TH1D* totalSpectFit_sam1 = (TH1D*) fitResultFile->Get("evhist_sam1_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam1 = (TH1D*) fitResultFile->Get("evhist_sam1_iter0_data");
    if(!totalSpectMC_sam1) totalSpectMC_sam1 = new TH1D("NullSam1","NullSam1",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam1) totalSpectFit_sam1 = new TH1D("NullSam1","NullSam1",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam1) totalSpectFD_sam1 = new TH1D("NullSam1","NullSam1",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam2 = (TH1D*) fitResultFile->Get("evhist_sam2_iter0_mc");
    TH1D* totalSpectFit_sam2 = (TH1D*) fitResultFile->Get("evhist_sam2_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam2 = (TH1D*) fitResultFile->Get("evhist_sam2_iter0_data");
    if(!totalSpectMC_sam2) totalSpectMC_sam2 = new TH1D("NullSam2","NullSam2",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam2) totalSpectFit_sam2 = new TH1D("NullSam2","NullSam2",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam2) totalSpectFD_sam2 = new TH1D("NullSam2","NullSam2",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam3 = (TH1D*) fitResultFile->Get("evhist_sam3_iter0_mc");
    TH1D* totalSpectFit_sam3 = (TH1D*) fitResultFile->Get("evhist_sam3_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam3 = (TH1D*) fitResultFile->Get("evhist_sam3_iter0_data");
    if(!totalSpectMC_sam3) totalSpectMC_sam3 = new TH1D("NullSam3","NullSam3",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam3) totalSpectFit_sam3 = new TH1D("NullSam3","NullSam3",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam3) totalSpectFD_sam3 = new TH1D("NullSam3","NullSam3",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam4 = (TH1D*) fitResultFile->Get("evhist_sam4_iter0_mc");
    TH1D* totalSpectFit_sam4 = (TH1D*) fitResultFile->Get("evhist_sam4_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam4 = (TH1D*) fitResultFile->Get("evhist_sam4_iter0_data");
    if(!totalSpectMC_sam4) totalSpectMC_sam4 = new TH1D("NullSam4","NullSam4",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam4) totalSpectFit_sam4 = new TH1D("NullSam4","NullSam4",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam4) totalSpectFD_sam4 = new TH1D("NullSam4","NullSam4",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam5 = (TH1D*) fitResultFile->Get("evhist_sam5_iter0_mc");
    TH1D* totalSpectFit_sam5 = (TH1D*) fitResultFile->Get("evhist_sam5_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam5 = (TH1D*) fitResultFile->Get("evhist_sam5_iter0_data");
    if(!totalSpectMC_sam5) totalSpectMC_sam5 = new TH1D("NullSam5","NullSam5",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam5) totalSpectFit_sam5 = new TH1D("NullSam5","NullSam5",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam5) totalSpectFD_sam5 = new TH1D("NullSam5","NullSam5",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_sam6 = (TH1D*) fitResultFile->Get("evhist_sam6_iter0_mc");
    TH1D* totalSpectFit_sam6 = (TH1D*) fitResultFile->Get("evhist_sam6_iter"+finalIter+"_pred");
    TH1D* totalSpectFD_sam6 = (TH1D*) fitResultFile->Get("evhist_sam6_iter0_data");
    if(!totalSpectMC_sam6) totalSpectMC_sam6 = new TH1D("NullSam6","NullSam6",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFit_sam6) totalSpectFit_sam6 = new TH1D("NullSam6","NullSam6",nbinsAuto,binsAuto->GetArray());
    if(!totalSpectFD_sam6) totalSpectFD_sam6 = new TH1D("NullSam6","NullSam6",nbinsAuto,binsAuto->GetArray());
    
    TH1D* totalSpectMC_allSam = new TH1D("totalSpectMC_allSam","totalSpectMC_allSam",nbinsAuto,binsAuto->GetArray());
    TH1D* totalSpectFit_allSam = new TH1D("totalSpectFit_allSam","totalSpectFit_allSam",nbinsAuto,binsAuto->GetArray());
    TH1D* totalSpectFD_allSam = new TH1D("totalSpectFD_allSam","totalSpectFD_allSam",nbinsAuto,binsAuto->GetArray());
    
    totalSpectMC_allSam->Add(totalSpectMC_sam0,totalSpectMC_sam1);
    totalSpectMC_allSam->Add(totalSpectMC_sam2);
    totalSpectMC_allSam->Add(totalSpectMC_sam3);
    totalSpectMC_allSam->Add(totalSpectMC_sam4);
    totalSpectMC_allSam->Add(totalSpectMC_sam5);
    totalSpectMC_allSam->Add(totalSpectMC_sam6);
    
    totalSpectFit_allSam->Add(totalSpectFit_sam0,totalSpectFit_sam1);
    totalSpectFit_allSam->Add(totalSpectFit_sam2);
    totalSpectFit_allSam->Add(totalSpectFit_sam3);
    totalSpectFit_allSam->Add(totalSpectFit_sam4);
    totalSpectFit_allSam->Add(totalSpectFit_sam5);
    totalSpectFit_allSam->Add(totalSpectFit_sam6);
    
    totalSpectFD_allSam->Add(totalSpectFD_sam0,totalSpectFD_sam1);
    totalSpectFD_allSam->Add(totalSpectFD_sam2);
    totalSpectFD_allSam->Add(totalSpectFD_sam3);
    totalSpectFD_allSam->Add(totalSpectFD_sam4);
    totalSpectFD_allSam->Add(totalSpectFD_sam5);
    totalSpectFD_allSam->Add(totalSpectFD_sam6);

    //dptbinsV1
    //const int nbins = 15;
    //const double bins[nbins+1] =  {0.00, 0.065, 0.1, 0.125, 0.15, 0.18,0.21, 0.24, 0.275, 0.315, 0.37, 0.425, 0.495, 0.59, 0.75, 2.0};
    //dptbinscoarseV1
    //const int nbins = 8;
    //const double bins[nbins+1] =  {0.00, 0.065, 0.1, 0.15, 0.21, 0.275, 0.37, 0.495, 2.0};
    //dptbinscoarseV2
    //const int nbins = 10;
    //const double bins[nbins+1] =  {0.00, 0.065, 0.095, 0.122, 0.15, 0.182, 0.225, 0.295, 0.4, 0.54, 2.0};
    //const int nbins = 5;
    //const double bins[nbins+1] =  {0.00, 0.095, 0.15, 0.225, 0.4, 2.0};
    //const int nbins = 1;
    //const double bins[nbins+1] =  {0.00, 2.0};
    
    
    int low = 0;
    int nobins = nbinsAuto;
    
    TArrayD * binning;// = new TArrayD(nbinsAuto-2);
    
    if(fitResultFilename.Contains("OF")){
        low = 1;
        nobins = nbinsAuto - 2;// + 1;
        binning = new TArrayD( (int)(nbinsAuto - 1) );
        int number = 0;
     
        for(int i = low; i < nbinsAuto; i++){
            printf("i = %d : number = %d : p = %f\n", i, number, binsAuto->At(i));
            binning->SetAt(binsAuto->At(i), number);
            number++;
        }
        
        printf("Min = %f, Max = %f\n", binning->At(0), binning->At(nobins));
        totalSpectMC_allSam->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectFit_allSam->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectFD_allSam->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        
        
        totalSpectMC_sam0->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectFit_sam0->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectFD_sam0->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectMC_sam1->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectFit_sam1->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        totalSpectFD_sam1->GetXaxis()->SetRangeUser(binning->At(0), binning->At(nobins));
        
    }
    else{
        binning = new TArrayD(*binsAuto);
    }
    
    for(int i = 0; i < binning->GetSize(); i++){
        printf("Bin %d Value: %f\n", i, binning->At(i));
    }
    
    TH1D * mcSpect = new TH1D("mcSpect","mcSpect", nobins, binning->GetArray());
    TH1D * fakeDataTrueSpect = new TH1D("fakeDataTrueSpect","fakeDataTrueSpect", nobins,binning->GetArray());
    TH1D * mcRecoSig = new TH1D("mcRecoSig","", nobins, binning->GetArray());
    
    //TH1D* mcSpect = new TH1D("mcSpect","mcSpect",nbinsAuto,binsAuto->GetArray());
    //TH1D* fakeDataTrueSpect = new TH1D("fakeDataTrueSpect","fakeDataTrueSpect",nbinsAuto,binsAuto->GetArray());
    
    //Find out if the files has reweighted the signal: This is determined by the name of the file.
    if(fakeDataFilename.Contains("reweight") || fakeDataFilename.Contains("wght")){
        printf("REWEIGHTED\n");
        selEvtsFakeDataTree->Draw("D1True/1000>>fakeDataTrueSpect", "(cutBranch==0 && nu_truereac == 11 && target==1 && TMath::Abs(D1True/1000) < 1e-5)");
        /*TH1F * shist = (TH1F*)fakeDataFile->Get("scale");
        if(shist) {
            double scale = (double)(shist->GetBinContent(1));
            fakeDataTrueSpect->Scale(scale);
            int testn = (int)scale*(fakeDataTrueSpect->Integral());
            printf("SIGNAL : %d\n",testn);
        }*/
    }
    else selEvtsFakeDataTree->Draw("D1True/1000>>fakeDataTrueSpect", "cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5");
    
    selEvtsTree->Draw("D1True/1000>>mcSpect", "(cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5)");
    
    selEvtsTree->Draw("D1Rec/1000>>mcRecoSig", "(cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5)");
    //selEvtsFakeDataTree->Draw("D1True/1000>>fakeDataTrueSpect", "cutBranch==0 && nu_truereac ==11 && target==1 && TMath::Abs(D1True/1000) < 1e-5");
    
    TH1D* fitSpect = new TH1D("fitSpect","fitSpect", nobins, binning->GetArray());
    //TH1D* fitSpect = new TH1D("fitSpect","fitSpect",nbinsAuto,binsAuto->GetArray());
    
    //for(int i=1;i<=nbinsAuto;i++){
    //    fitSpect->SetBinContent( i,( (mcSpect->GetBinContent(i))*(fitParamValues->GetBinContent(i)) ) );
    //    fitSpect->SetBinError( i,( (mcSpect->GetBinContent(i))*(fitParamErrors->GetBinContent(i)) ) );
    //}
    
    for(int i=1;i<=nobins;i++){
        fitSpect->SetBinContent( i,( (mcSpect->GetBinContent(i))*(fitParamValues->GetBinContent(i + low)) ) );
        fitSpect->SetBinError( i,( (mcSpect->GetBinContent(i))*(fitParamErrors->GetBinContent(i + low))) );
        printf("BIN %d : Entries = %d, Error = %f\n", i, fitSpect->GetBinContent(i), fitSpect->GetBinError(i) );
    }
    
    GeV2MeV(totalSpectMC_allSam);
    GeV2MeV(totalSpectFD_allSam);
    GeV2MeV(fitSpect);
    GeV2MeV(totalSpectFit_allSam);
    GeV2MeV(fakeDataTrueSpect);
    GeV2MeV(mcSpect);
    GeV2MeV(mcRecoSig);
    
    TH1D * hlist[6] = {totalSpectFit_allSam, totalSpectFit_allSam, totalSpectFD_allSam, mcSpect, fakeDataTrueSpect, fitSpect};
    
    int xmax = 0;
    for(int i = 0; i < 6; i++){
        TH1D * tmphist = hlist[i];
        if(i == 0) xmax = tmphist->GetBinContent(tmphist->GetMaximumBin());
        int tmp_maxcand = tmphist->GetBinContent(tmphist->GetMaximumBin()) +  tmphist->GetBinError(tmphist->GetMaximumBin());
        
        if(xmax < tmp_maxcand) xmax = tmp_maxcand;
        //printf("%s : Underflow = %f, Overflow = %f\n",tmphist->GetName(),tmphist->GetBinContent(0), tmphist->GetBinContent(tmphist->GetNbinsX()+1));
    }
    
    fitSpect->SetLineColor(kBlack);//
    fitSpect->SetLineWidth(2);
    fitSpect->SetLineStyle(2);//Fitted signal
    fitSpect->SetMarkerStyle(26);//26 or 32
    fitSpect->SetMarkerColor(kBlack);//26 or 32
    fitSpect->SetMarkerSize(2);

    mcSpect->SetLineColor(kOrange+10);
    mcSpect->SetLineWidth(2);
    mcSpect->SetLineStyle(2);
    mcSpect->SetMarkerStyle(22);//26 or 32
    //mcSpect->SetMarkerColor(kGray+2);
    
    mcRecoSig->SetLineWidth(2);
    mcRecoSig->SetLineStyle(7);
    mcRecoSig->SetLineColor(kGray+2);
    
    fakeDataTrueSpect->SetLineColor(kGreen+2);
    fakeDataTrueSpect->SetLineWidth(2);
    fakeDataTrueSpect->SetLineStyle(2);
    
    fitSpect->Write();
    mcSpect->Write();
    mcRecoSig->Write();
    fakeDataTrueSpect->Write();
    
    totalSpectFit_sam0->SetLineColor(kRed);
    totalSpectFit_sam0->SetLineWidth(2);
    totalSpectMC_sam0->SetLineColor(kAzure+2);
    totalSpectMC_sam0->SetLineWidth(2);
    totalSpectFD_sam0->SetLineColor(kOrange-2);
    totalSpectFD_sam0->SetLineWidth(2);
    
    totalSpectFit_sam1->SetLineColor(kRed);
    totalSpectFit_sam1->SetLineWidth(2);
    
    totalSpectMC_sam1->SetLineColor(kAzure+2);
    totalSpectMC_sam1->SetLineWidth(2);
    totalSpectFD_sam1->SetLineColor(kOrange-2);
    totalSpectFD_sam1->SetLineWidth(2);
    
    totalSpectFit_allSam->SetLineColor(kGray);//Fitted dpTT dist.
    totalSpectFit_allSam->SetLineWidth(2);
    
    totalSpectMC_allSam->SetLineColor(kAzure+2);
    totalSpectMC_allSam->SetLineWidth(2);
    
    totalSpectFD_allSam->SetLineColor(kOrange-2);
    totalSpectFD_allSam->SetLineWidth(2);
    totalSpectFD_allSam->SetMarkerStyle(23);
    totalSpectFD_allSam->SetMarkerColor(kOrange-2);

    totalSpectMC_sam0 ->Write();
    totalSpectFit_sam0->Write();
    totalSpectFD_sam0 ->Write();
    totalSpectMC_sam1 ->Write();
    totalSpectFit_sam1->Write();
    totalSpectFD_sam1 ->Write();
    
    TCanvas* canv = new TCanvas("Signal Comp","Signal Comp");
    fitSpect->Draw();
    mcSpect->Draw("same");
    fakeDataTrueSpect->Draw("sameHIST");
    canv->Write();
    
    TCanvas* canv2 = new TCanvas("Sample0 Comp","Sample0 Comp");
    totalSpectFit_sam0->Draw();
    totalSpectMC_sam0->Draw("same");
    totalSpectFD_sam0->Draw("sameE");
    canv2->Write();
    
    TCanvas* canv3 = new TCanvas("Sample1 Comp","Sample1 Comp");
    totalSpectFit_sam1->Draw();
    totalSpectMC_sam1->Draw("same");
    totalSpectFD_sam1->Draw("sameE");
    canv3->Write();
    
    TCanvas* canv4 = new TCanvas("All Sample Comp","All Sample Comp");
    totalSpectFit_allSam->Draw("HIST");
    totalSpectMC_allSam->Draw("sameHIST");
    totalSpectFD_allSam->Draw("sameE"); 
    canv4->Write();
    
    //Final canvas for showing distributions:
    
    //Get the true and fitted signal events:
    int sigfit = (int)(fitSpect->GetBinContent(fitSpect->GetMaximumBin()));
    int sigerror = (int)(fitSpect->GetBinError(fitSpect->GetMaximumBin()));
    int sigtrue = (int)(fakeDataTrueSpect->GetBinContent(fakeDataTrueSpect->GetMaximumBin()));
    int mc_signal = (int)mcSpect->GetBinContent(mcSpect->GetMaximumBin());
    
    BWNorm(totalSpectMC_allSam);
    BWNorm(totalSpectFD_allSam);
    BWNorm(fitSpect);
    BWNorm(totalSpectFit_allSam);
    BWNorm(fakeDataTrueSpect);
    BWNorm(mcSpect);
    BWNorm(mcRecoSig);
    
    xmax = 0;
    for(int i = 0; i < 6; i++){
        TH1D * tmphist = hlist[i];
        if(i == 0) xmax = tmphist->GetBinContent(tmphist->GetMaximumBin());
        int tmp_maxcand = tmphist->GetBinContent(tmphist->GetMaximumBin()) +  tmphist->GetBinError(tmphist->GetMaximumBin());
        
        if(xmax < tmp_maxcand) xmax = tmp_maxcand;
        //printf("%s : Underflow = %f, Overflow = %f\n",tmphist->GetName(),tmphist->GetBinContent(0), tmphist->GetBinContent(tmphist->GetNbinsX()+1));
    }
    
    double leg51_x = 0.5;
    double leg51_y = 0.6;
    TLegend * leg51 = new TLegend(leg51_x,leg51_y,leg51_x+0.3,leg51_y+0.1);
    leg51->SetHeader("Pre-fit Inputs");
    leg51->AddEntry(totalSpectMC_allSam,"MC Reco.","l");//Pre-fit dpTT dist.
    leg51->AddEntry(totalSpectFD_allSam,"Fake Data (7x10^{21} POT)","pl");//Fake Data
    
    double leg52_x = 0.5;
    double leg52_y = leg51_y+0.1;
    TLegend * leg52 = new TLegend(leg52_x,leg52_y,leg52_x+0.3,leg52_y+0.1);
    leg52->SetHeader("Fitted Output");
    leg52->AddEntry(fitSpect,"Signal","pl");
    leg52->AddEntry(totalSpectFit_allSam,"Fitted Reco.","l");
    
    double leg53_x = 0.5;
    double leg53_y = leg52_y+0.1;
    TLegend * leg53 = new TLegend(leg53_x,leg53_y,leg53_x+0.3,leg53_y+0.1);
    leg53->SetHeader("True Dist.");
    leg53->AddEntry(fakeDataTrueSpect,"Fake Data Signal","l");
    leg53->AddEntry(mcSpect,Form("MC Signal (%d)",(int)mcSpect->GetBinContent(mcSpect->GetMaximumBin())),"l");
    
    double leg54_x = 0.5;
    double leg54_y = leg53_y+0.1;
    TLegend * leg54 = new TLegend(leg54_x,leg54_y,leg54_x+0.3,leg54_y+0.1);
    leg54->SetHeader("Signal");
    leg54->AddEntry((TObject*)0,Form("True %d",sigtrue),"");
    leg54->AddEntry((TObject*)0,Form("Fitted %d #pm %d",sigfit, sigerror),"");
    
    TCanvas* canv5 = new TCanvas("All Truth and Rec","All Truth and Rec");
    fitSpect->GetXaxis()->SetTitle("#delta#it{p}_{TT} (MeV/c)");
    fitSpect->GetYaxis()->SetTitle("Counts/Bin Width");
    fitSpect->GetYaxis()->SetRangeUser(0, 1.1*xmax);
    fitSpect->Draw("E");
    mcSpect->Draw("sameHIST");
    fakeDataTrueSpect->Draw("sameHIST");
    totalSpectFit_allSam->Draw("sameHIST");
    totalSpectMC_allSam->Draw("sameHIST");
    totalSpectFD_allSam->Draw("sameE");
    leg51->Draw();
    leg52->Draw();
    leg53->Draw();
    leg54->Draw();
    canv5->Write();
    
    double leg61_x = 0.19;
    double leg61_y = 0.57;
    TLegend * leg61 = new TLegend(leg61_x,leg61_y,leg61_x+0.29,leg61_y+0.2);
    //leg61->SetHeader("Pre-fit Inputs");
    leg61->AddEntry(totalSpectMC_allSam,"MC Reco.","l");//Pre-fit dpTT dist.
    leg61->AddEntry(totalSpectFD_allSam,"Fake Data (7x10^{21} POT)","pl");//Fake Data
    //leg61->AddEntry(mcSpect,Form("MC Signal (%d)",mc_signal),"l");
    leg61->SetFillStyle(0);
    
    double leg62_x = 0.69;
    double leg62_y = 0.57;
    TLegend * leg62 = new TLegend(leg62_x,leg62_y,leg62_x+0.29,leg62_y+0.15);
    //leg62->SetHeader("Pre-fit Inputs");
    leg62->AddEntry(mcSpect,Form("MC Signal (%d)", mc_signal),"l");
    leg62->AddEntry(mcRecoSig,"MC Reco. Signal","l");
    leg62->SetFillStyle(0);
    
    double leg63_x = 0.19;
    double leg63_y = 0.67;
    TLegend * leg63 = new TLegend(leg63_x,leg63_y,leg63_x+0.29,leg63_y+0.2);
    leg63->SetHeader("Pre-fit Inputs");
    leg63->SetFillStyle(0);
    
    TLatex * text_1 = new TLatex(40, xmax*0.95, "#font[72]{T2K Work In Progress}");
    text_1->SetTextSize(0.04);
    
    TCanvas* canv6 = new TCanvas("PreFit Rec and Truth","PreFit Rec and Truth");
    mcSpect->GetXaxis()->SetTitle("#delta#it{p}_{TT} (MeV/c)");
    mcSpect->GetYaxis()->SetTitle("Counts/Bin Width");
    mcSpect->GetYaxis()->SetTitleOffset(0.9);
    mcSpect->GetYaxis()->SetRangeUser(0, 1.07*xmax);
    mcSpect->Draw("HIST");
    mcRecoSig->Draw("sameHIST");
    totalSpectMC_allSam->Draw("sameHIST");
    totalSpectFD_allSam->Draw("sameE");
    leg61->Draw();
    leg62->Draw();
    leg63->Draw();
    text_1->Draw();
    canv6->Write();
    
    double leg71_x = 0.58;
    double leg71_y = 0.62;
    TLegend * leg71 = new TLegend(leg71_x,leg71_y,leg71_x+0.3,leg71_y+0.2);
    leg71->SetHeader("Signal");
    leg71->AddEntry(fakeDataTrueSpect,Form("True %d",sigtrue),"l");
    leg71->AddEntry(fitSpect,Form("Fitted %d #pm %d",sigfit, sigerror),"pl");
    leg71->SetFillStyle(0);
    
    double leg72_x = 0.21;
    double leg72_y = 0.61;
    TLegend * leg72 = new TLegend(leg72_x,leg72_y,leg72_x+0.32,leg72_y+0.22);
    leg72->AddEntry(totalSpectFit_allSam,"Fitted Reco.","l");
    leg72->AddEntry(totalSpectFD_allSam,"Fake Data (7x10^{21} POT)","pl");
    leg72->SetFillStyle(0);
    
    TLatex * text_2 = new TLatex(-280, xmax*0.98, "#font[72]{T2K Work In Progress}");
    text_2->SetTextSize(0.04);
    
    TCanvas* canv7 = new TCanvas("PostFit Rec and Truth","PostFit Rec and Truth");
    fitSpect->GetXaxis()->SetTitle("#delta#it{p}_{TT} (MeV/c)");
    fitSpect->GetYaxis()->SetTitle("Counts/Bin Width");
    fitSpect->GetYaxis()->SetTitleOffset(0.8);
    fitSpect->GetYaxis()->SetRangeUser(0, 1.07*xmax);
    fitSpect->Draw("E");
    fakeDataTrueSpect->Draw("sameHIST");
    totalSpectFit_allSam->Draw("sameHIST");
    totalSpectFD_allSam->Draw("sameE");
    leg71->Draw();
    leg72->Draw();
    text_2->Draw();
    canv7->Write();
    
}
