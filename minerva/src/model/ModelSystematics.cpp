#ifndef __MODELSYSTEMATICS_CPP__
#define __MODELSYSTEMATICS_CPP__

#include <ModelSystematics.h>

#include <iostream>
#include <TGraph.h>
#include <TH1D.h>
#include <TFile.h>
#include <cassert>
#include <PlotUtils/MnvH1D.h> 

using std::cout;
using std::endl;
using std::string;

using namespace PlotUtils;

ModelSystematics::ModelSystematics(const int n_genie_shifts, const bool verbose) :
	SystematicsBase(n_genie_shifts, verbose)
{
	cout << "ModelSystematics::ModelSystematics()" << endl;
}

ModelSystematics::~ModelSystematics()
{

}

bool ModelSystematics::AddLatErrorBand(const std::string& name, const std::string &reaction_type)
{
	int counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		if(it->first.find(reaction_type) == string::npos) continue;
		if(it->second->AddLatErrorBand(name, m_Nuniverses) ) counter++;
	}
	return (counter == m_Nuniverses);
}

bool ModelSystematics::AddLatErrorBandAndFillWithCV(const std::string& name, const std::string &reaction_type)
{
	int counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		if(it->first.find(reaction_type) == string::npos) continue;
		if(it->second->AddLatErrorBandAndFillWithCV(name, m_Nuniverses) ) counter++;
	}
	return (counter == m_Nuniverses);
}

bool ModelSystematics::AddVertErrorBand(const std::string& name, const std::string &reaction_type)
{
	int counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		if(it->first.find(reaction_type) == string::npos) continue;
		if(it->second->AddVertErrorBand(name, m_Nuniverses) ) counter++;
	}
	return (counter == m_Nuniverses);
}

bool ModelSystematics::AddVertErrorBandAndFillWithCV(const std::string& name, const std::string &reaction_type)
{
	int counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		if(it->first.find(reaction_type) == string::npos) continue;
		if(it->second->AddVertErrorBandAndFillWithCV(name, m_Nuniverses) ) counter++;
	}
	return (counter == m_Nuniverses);
}

bool ModelSystematics::AddUncorrError(const std::string& name, const std::string &reaction_type)
{
	int counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		if(it->first.find(reaction_type) == string::npos) continue;
		if(it->second->AddUncorrError(name, m_Nuniverses) ) counter++;
	}
	return (counter == m_Nuniverses);
}

bool ModelSystematics::AddUncorrErrorAndFillWithCV(const std::string& name, const std::string &reaction_type)
{
	int counter = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		if(it->first.find(reaction_type) == string::npos) continue;
		if(it->second->AddUncorrErrorAndFillWithCV(name, m_Nuniverses) ) counter++;
	}
	return (counter == m_Nuniverses);
}

TGraph * ModelSystematics::MakeResFunc(const std::vector<TH1D*> & hists, const int bin)
{
	// Although we may have 7 genie weight to avoid seg. faulting let's not rely on 
	// m_Nuniverse;
	int nhists = (int)hists.size();
	Double_t * x = new Double_t[nhists];
	Double_t * y = new Double_t[nhists];
	// For now assume the middle bin is nom. and we only have 7 bins
	// if(nhists % 2 == 0){
	// }
	double cv = hists[3]->GetBinContent(bin);
	for(int i = 0; i < nhists; i++){
		x[i] = i;
		y[i] = hists[i]->GetBinContent(bin)/cv;
	}
	return new TGraph(nhists, x, y);
}

void ModelSystematics::BuildResponses(const std::string &outfname)
{

	TFile ofile(outfname.c_str(), "RECREATE");
	assert(ofile);
	ofile.cd();

	string name = "";
	string error = "";
	std::vector<std::string> error_list;
	std::vector<TH1D*> hist_list;
	int ntbins = 0;

	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for(; it != m_samples.end(); it++){
		MnvH1D * sam = it->second;
		// Finish this tomorrow:
		// For each sam retrieve all the systs and make a response function
		for(int type = 0; type < 2; type++){
			if(type == 0) error_list = sam->GetLatErrorBandNames();
			else error_list = sam->GetVertErrorBandNames();

			for(size_t ners = 0; ners < error_list.size(); ners++){
				error = error_list[ners];
				
				if(type == 0) hist_list = sam->GetLatErrorBand(error)->GetHists();
				else hist_list = sam->GetVertErrorBand(error)->GetHists();

				// Now build this response functions:
				// Need the bins from each sample
				// We include under and overflow bins here (may be 1);
				for(int tbins = 0; tbins < ntbins + 2; tbins++){
					name = Form("%s_%s_b%.3n_nb%.3n", it->first.c_str(), error.c_str(), tbins, ntbins);
					TGraph * res = MakeResFunc(hist_list, bin);
					res->SetName( name.c_str() );
					res->Write();
					delete res;
				}
				hist_list.clear();
			}
			error_list.clear();
		}
	}
	ofile.Close();
}

#endif