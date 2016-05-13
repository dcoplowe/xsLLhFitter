//#include "genResponse_test.h"


void genResponse_dpt(string microtreeIn, string rwpath, string dialName, double nom, double err, int topo, int reac) {

  

  // declare variables for fit parameters, names, and values
  vector<string> param_names;
  vector<double> param_values;
  double bestfit;
  double error;
  //double sigma;


  // define inputs
  string microtree = microtreeIn;
  string rwfile_default = rwpath+"rwfile_default.root";
  string rwfile_truth = rwpath+"rwfile_truth.root";
  string flux = "/data/t2k/dolan/xsToolBasedMEC/CC0PiAnl/fluxFiles/nd5_tuned11bv3.2_11anom_run2_fine.root";
  string fluxname = "enu_nd5_tuned11b_numu";

  string outname = Form("topo%d_reac%d.root",topo,reac);
  string outputFileName = "./output/resFunc"+dialName+outname;
  cout << "Output file name is: " << outputFileName << endl; 
  
  
  // set up unfolding variables, signal, and cut
  string mc_variable = "trueDpT";
  string data_variable = "Sum$(recDpT[xstool_throw])";
  string signal, cut, bcut; // these will be defined in the main loop
  //string psCut = "&& (Sum$(selp_mom[xstool_throw])>450) && (Sum$(selmu_mom[xstool_throw])>150) && (Sum$(selp_theta[xstool_throw])>-3.15) && (Sum$(selmu_theta[xstool_throw])>1.57)";
  
  // set up recon binning
  const int nRbins = 9;
  //const double Rbins[nRbins+1] = { 0.0, 0.125, 0.2, 0.28, 0.37, 0.47, 0.6, 0.83, 2.5 };
  const double Rbins[nRbins+1] = { 0.0, 0.08, 0.12, 0.155, 0.2, 0.26, 0.36, 0.51, 1.1, 100.0};

  // set up true binning
  const int nTbins = 9;
  //const double Tbins[nTbins+1] = { 0.0, 0.125, 0.2, 0.28, 0.37, 0.47, 0.6, 0.83, 2.5 };
  const double Tbins[nTbins+1] = { 0.0, 0.08, 0.12, 0.155, 0.2, 0.26, 0.36, 0.51, 1.1, 100.0};

  // You need to provide the number of branches in your HL2 tree
  // And the accum_level you want to cut each one at to get your selected events
  // i.e choosing n in accum_level[0][branch_i]>n
  const int nbranches = 10;
  const int accumToCut[nbranches] =   {7,8,9,8,7,5,4,7,8,7};

  
  // delcare xsBinning, they will be used always the same in the main loop
  xsBinning *binningTru = new xsBinning();
  xsBinning *binningRec = new xsBinning();
  binningTru->addDimension(mc_variable,nTbins,Tbins);
  binningRec->addDimension(data_variable,nRbins,Rbins);
  
  // setup on-the-fly reweighting parameter
  param_names.push_back(dialName);
  bestfit = nom;
  error = err;

  // Total number of topologies
  const int nTopologies = nbranches;
  // Total number of weights
  const int nWeights = 7;
  // reweighted reconstrcted distribution for each weight, true bin, and topology
  TH1 *hreco[nWeights][nTbins][nTopologies];


  for(const int w = 0; w < nWeights; w++){
    for(const int bt = 0; bt < nTbins; bt++){
      int t = topo;
      int r = reac;
      //for(const int t = 0; t < nTopologies; t++){
      
        if(t==0 || t==4 || r==6) continue; // Ignore branches with no proton
        // define xsInput
        xsInputdataHighland2 *inpRwFly = new xsInputdataHighland2();
        inpRwFly->SetupFromHighland(microtree,microtree);
        inpRwFly->SetFlux(flux, fluxname);
        inpRwFly->SetRooTrackerVtxFiles(rwfile_default,rwfile_truth);
        inpRwFly->SetTrueBinning(binningTru);
        inpRwFly->SetReconstructedBinning(binningRec);

        // set cut definition, for each topology
        cut = Form("((Sum$(accum_level[xstool_throw][%d]) > %d) && (mectopology==%d))", t, accumToCut[t], r);

        // set signal definition, for each true bin
        signal = Form("(trueDpT) > %f && (trueDpT) < %f",Tbins[bt],Tbins[bt+1]);
        bcut = Form("((Sum$(trueDpT[xstool_throw]) > %f) && (Sum$(trueDpT[xstool_throw])) < %f)",Tbins[bt],Tbins[bt+1]);

        inpRwFly->SetSignal(signal.c_str());
        inpRwFly->SetCut((cut+" && "+bcut).c_str());

        // define xsReweightParameter
        xsReweightParameter *rwPar = new xsReweightParameter(param_names);
        inpRwFly->AddReweighting(rwPar);


        // set parameter value
        param_values.clear();    
        //param_values.push_back((-3+w)*error/bestfit);
        param_values.push_back((bestfit-1)+((-3+w)*error));
        string name = rwPar->SetParameterValue(param_values);

        // reweighting with the given value
        inpRwFly->GenerateThrow(name,0,true);

        // get reconstructed distribution
        hreco[w][bt][t] = inpRwFly->GetReconstructed();

        // delete xsInput
        delete inpRwFly;
	
      //}
    }
  }
  
    
  //  }

 
  //Write TGraph in the output file
  TFile *output = new TFile(outputFileName.c_str(),"RECREATE");   
  
  for(const int t = 0; t < nTopologies; t++){  
    output->mkdir(Form("topology_%d",t));
  }
  char dir[200];

  TGraph *ReWeight[nRbins][nTbins][nTopologies];
  
  double MA[7];
  for(int w = 0; w < 7; w++){
      MA[w]=bestfit-error*(3-w);
  }  

  for(int br = 0; br<nRbins; br++){//reco kinematics bin
    for(int bt = 0; bt < nTbins; bt++){//true kinematics bin
      //if(fabs(br-bt)>20) continue;  //save memory if reco bin and true bin very far away
      //for(int t = 0; t < nTopologies; t++){//topology
      t=topo;
      r=reac;

	       sprintf(dir,"topology_%d",t);
	       output->cd(dir);
  	     //for(int r=0;r<5;r++){ //reaction
	       char nameHisto[256];
	       sprintf(nameHisto,"RecBin_%d_trueBin_%d_topology_%d_reac_%d",br,bt,t,r);
  	     ReWeight[br][bt][t] = new TGraph(7);
  	     ReWeight[br][bt][t]->SetName(nameHisto);
  	     ReWeight[br][bt][t]->SetTitle(nameHisto);
  	     ReWeight[br][bt][t]->SetMarkerStyle(20);
  	     ReWeight[br][bt][t]->SetMarkerColor(2);
	       for(int w=0;w<nWeights;w++){
	         if(hreco[w][bt][t]->GetBinContent(br+1) !=0 ){
	           ReWeight[br][bt][t]->SetPoint(w,MA[w],hreco[w][bt][t]->GetBinContent(br+1)/hreco[3][bt][t]->GetBinContent(br+1));
	          }
	          else{
	            ReWeight[br][bt][t]->SetPoint(w,MA[w],1);
	          }
	          ReWeight[br][bt][t]->GetYaxis()->SetTitle("weight");
	   
	       }
	       ReWeight[br][bt][t]->Write();
	     //}
     }
   }
  
   output->Close();
  
}
