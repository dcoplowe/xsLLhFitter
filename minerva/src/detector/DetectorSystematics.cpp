#ifndef __DETECTORSYSTEMATICS_CPP__
#define __DETECTORSYSTEMATICS_CPP__

#include <DetectorSystematics.h>
#include <Sample.h>
#include <iostream>
#include <TH1D.h>
#include <TMatrixD.h>
#include <sstream>
#include <ErrorType.h>

using std::cout;
using std::endl;
using std::string;

const std::string DetectorSystematics::m_ver_name = "_Vert";
const std::string DetectorSystematics::m_lat_name = "_Lat";
const std::string DetectorSystematics::m_uncer_name = "_UnCorEr";

DetectorSystematics::DetectorSystematics(bool verbose) : SystematicsBase(verbose), m_counter(-1), m_anaHist_set(false)
{ 
	cout << "DetectorSystematics::DetectorSystematics(bool verbose)" << endl;
	m_errors.clear();
	m_anaHist = 0x0;
	m_HanaHist = 0x0;
}

DetectorSystematics::DetectorSystematics(int n_universes, bool verbose) : SystematicsBase(n_universes, verbose), m_counter(-1),
	m_anaHist_set(false)
{
	cout << "DetectorSystematics::DetectorSystematics(int n_universes, bool verbose)" << endl;
	m_anaHist = 0x0;
	m_errors.clear();
}

DetectorSystematics::~DetectorSystematics()
{
	m_errors.clear();
	if(m_anaHist) delete m_anaHist;
}

bool DetectorSystematics::AddLatErrorBand(const std::string& name, const int n_universes, const std::string &fill_samples)
{
	size_t counter = 0;
	string tmp_name = name + m_lat_name;
	if(IsUniqueError(tmp_name)){ 
		ErrorType * tmp_et = new ErrorType(tmp_name, n_universes, ErrorType::kLateral);
		m_errors.push_back(tmp_et);
		
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			it->second->AddError(tmp_et);
			if(it->second->AddLatErrorBand(tmp_name, (n_universes == -999 ? m_Nuniverses : n_universes) ) ) counter++;
		}
		(void)fill_samples;
	}
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddLatErrorBandAndFillWithCV(const std::string& name, const int n_universes, const std::string &fill_samples)
{
	size_t counter = 0;
	
	string tmp_name = name + m_lat_name;
	if(IsUniqueError(tmp_name)){
		ErrorType * tmp_et = new ErrorType(tmp_name, n_universes, ErrorType::kLateralCV); 
		m_errors.push_back(tmp_et);
		
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			it->second->AddError(tmp_et);
			if(it->second->AddLatErrorBandAndFillWithCV(tmp_name, (n_universes == -999 ? m_Nuniverses : n_universes) )) counter++;
		}
		(void)fill_samples;
	}
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddVertErrorBand(const std::string& name, const int n_universes, const std::string &fill_samples)
{
	size_t counter = 0;

	string tmp_name = name + m_ver_name;
	if(IsUniqueError(tmp_name)){ 
		ErrorType * tmp_et = new ErrorType(tmp_name, n_universes, ErrorType::kVertical);
		m_errors.push_back(tmp_et);
	
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			it->second->AddError(tmp_et);
			if(it->second->AddVertErrorBand(tmp_name, (n_universes == -999 ? m_Nuniverses : n_universes) )) counter++;
		}
		(void)fill_samples;
	}
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddVertErrorBandAndFillWithCV(const std::string& name, const int n_universes, const std::string &fill_samples)
{
	size_t counter = 0;

	string tmp_name = name + m_ver_name;
	if(IsUniqueError(tmp_name)){ 
		ErrorType * tmp_et = new ErrorType(tmp_name, n_universes, ErrorType::kVerticalCV);
		m_errors.push_back(tmp_et);
	
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			it->second->AddError(tmp_et);
			if(it->second->AddVertErrorBandAndFillWithCV(tmp_name, (n_universes == -999 ? m_Nuniverses : n_universes) )) counter++;
		}
		(void)fill_samples;
	}
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddUncorrError(const std::string& name, const std::string &fill_samples)
{
	size_t counter = 0;

	string tmp_name = name + m_uncer_name;
	if(IsUniqueError(tmp_name)){ 
		ErrorType * tmp_et = new ErrorType(tmp_name, -999, ErrorType::kUnCorError);
		m_errors.push_back(tmp_et);

		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			it->second->AddError(tmp_et);
			if(it->second->AddUncorrError(tmp_name)) counter++;
		}
		(void)fill_samples;
	}
	return (counter == m_samples.size());
}

bool DetectorSystematics::AddUncorrErrorAndFillWithCV(const std::string& name, const std::string &fill_samples)
{
	size_t counter = 0;

	string tmp_name = name + m_uncer_name;
	if(IsUniqueError(tmp_name)){ 
		ErrorType * tmp_et = new ErrorType(tmp_name, -999, ErrorType::kUnCorErrorCV);
		m_errors.push_back(tmp_et);

		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			it->second->AddError(tmp_et);
			if(it->second->AddUncorrErrorAndFillWithCV(tmp_name)) counter++;
		}
		(void)fill_samples;
	}
	return (counter == m_samples.size());
}

bool DetectorSystematics::IsUniqueError(const std::string &name)
{
	bool is_unique = true;
	for(size_t i = 0; i < m_errors.size(); i++){
		if(m_errors[i]->GetName().compare(name) == 0){ 
			is_unique = false;
			break;
		}
	}
	return is_unique; 
}


ErrorType * DetectorSystematics::FindError(const std::string &name)
{
	for(size_t i = 0; i < m_errors.size(); i++){
		ErrorType * tmp = m_errors[i];//->GetName();
		if(tmp->GetName().compare(name) == 0) return tmp;
	}
	return new ErrorType(); 
}

void DetectorSystematics::Prepare()
{
	// if(!m_anaHist_set){
	// 	int nbins = 0;
	// 	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	// 	for (; it != m_samples.end(); ++it){ 
	// 		// Include under/overflow bins
	// 		nbins += it->second->GetXaxis()->GetNbins() + 2;
	// 	}
		
	// 	m_anaHist = new MnvH1D(Form("analysis_bins_n%d", nbins), "", nbins, 0., nbins + 1);
	// 	// Tell samples which analysis bins they are to fill:
	// 	for(size_t i =0; i < m_errors.size(); i++){
	// 		ErrorType ee = m_errors[i];
	// 		if(ee->GetType() == ErrorType::kLateral) m_anaHist->AddLatErrorBand(ee.GetName(), ee.GetNUniverses());
	// 		else if(ee->GetType() == ErrorType::kLateralCV) m_anaHist->AddLatErrorBandAndFillWithCV(ee.GetName(), ee.GetNUniverses());
	// 		else if(ee->GetType() == ErrorType::kVertical) m_anaHist->AddVertErrorBand(ee.GetName(), ee.GetNUniverses());
	// 		else if(ee->GetType() == ErrorType::kVerticalCV) m_anaHist->AddVertErrorBandAndFillWithCV(ee.GetName(), ee.GetNUniverses());
	// 		else if(ee->GetType() == ErrorType::kUnCorError) m_anaHist->AddUncorrError(ee.GetName());
	// 		else if(ee->GetType() == ErrorType::kUnCorErrorCV) m_anaHist->AddUncorrErrorAndFillWithCV(ee.GetName());
	// 		else{
	// 			cout << __FILE__ << ":" << __LINE__ << " : Unkown Error Type" << endl;
	// 		}
	// 	}
	// 	m_anaHist_set = true;
	// }
}

void DetectorSystematics::BuildAnaHist(const bool includeStat)
{
	// Get total number of bins from samples:

	if(m_verbose) cout << "Destroying analysis hist, if one exists" << endl;
	if(m_HanaHist){
		delete m_HanaHist;
		m_HanaHist = 0x0;
		if(m_verbose) cout << "Found and Destoyed m_HanaHist!" << endl;
	}

	if(m_verbose){ 
		cout << "Build Analysis Hist" << endl;
		cout << "Determining number of bins... " << endl;
	}
	int tot_bins = 0;
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it)
		tot_bins += it->second->GetNbinsX();

	if(m_verbose) cout << "Found to be " << tot_bins << endl;

	m_HanaHist = new TH1D("Analysis_Hist_TH1D", "", tot_bins, 0, tot_bins); 

	int current_bin = 1;
	it = m_samples.begin();
	for (; it != m_samples.end(); ++it){
		MnvH1D * histo = it->second;
		for(int i = 0; i < histo->GetNbinsX(); i++){
			m_HanaHist->SetBinContent(current_bin, histo->GetBinContent(i+1));
			if(includeStat) m_HanaHist->SetBinError(current_bin, histo->GetBinError(i+1));
			current_bin++;
		}
	}
   // Create an MnvH1D out of the combined histo
	if(m_anaHist){ 
		delete m_anaHist;
		m_anaHist = 0x0;
		if(m_anaHist) cout << "Found and Destoyed m_HanaHist!" << endl;
	}

	m_anaHist = new MnvH1D(*m_HanaHist);
}

TMatrixD DetectorSystematics::GetCovMatrix(const bool includeStat, const bool asFrac, const bool cov_area_normalize)
{
	if(!m_anaHist){
		cout << __FILE__ << ":" << __LINE__ << " : Warning : No histogram made. Building using " << m_samples.size() << " samples." << endl;
		BuildAnaHist(includeStat);

		if(!m_anaHist){
			cout << __FILE__ << ":" << __LINE__ << " : Error : Could not building n sample histogram." << endl;
			exit(0);
		}
	}

	// Check that all the samples have the same errors:	
	for(size_t er = 0; er < m_errors.size(); er++){
		ErrorType * er_type = m_errors[er];

		// int er_nhists_tot = 0;
		int er_nhists = 0;
		bool same_universes = true;
		std::vector<double> univ_wgts;

		// Check that we have a histo with a particular error type:
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		int counter = 0;
		for (; it != m_samples.end(); ++it){
			MnvH1D * histo = it->second;

			// GetLatErrorBand( const std::string& name );
			// MnvVertErrorBand* GetVertErrorBand( const std::string& name );

			// We know which type of error we have from error type so could sort this out into
			// cases for the various error types. could be easy
			int nhists = 0;// histo->GetHists().size();
			bool has_error = false;
			switch(er_type->GetType()){
				case ErrorType::kLateral:
				case ErrorType::kLateralCV:
					if( histo->HasLatErrorBand( er_type->GetName() ) ){ 
						has_error = true;
						nhists = (int)histo->GetLatErrorBand( er_type->GetName() )->GetHists().size();
					}
					break;
				case ErrorType::kVertical:
				case ErrorType::kVerticalCV:
					if( histo->HasVertErrorBand( er_type->GetName() ) ){ 
						has_error = true;
						nhists = (int)histo->GetVertErrorBand( er_type->GetName() )->GetHists().size();
					}
					break;
				case ErrorType::kUnCorError:
				case ErrorType::kUnCorErrorCV:
					if( histo->HasUncorrError( er_type->GetName() ) ){
						has_error = true;
						er_nhists = 1;
						counter++;
					}
					break;
				default:
					cout << "No handling for error: " << er_type->GetName() << endl;
					break;
			}

			if( has_error ){
				// int nhists = histo->GetHists().size();
				// er_nhists_tot += nhists;
				if(nhists > 0 && counter == 0){ 
					er_nhists = nhists;
					counter++;

					// Get the wgts from this sample (as it will be the same in all sample for this error)
					univ_wgts.clear();
					switch(er_type->GetType()){
						case ErrorType::kLateral:
						case ErrorType::kLateralCV:
							for(int uni = 0; uni < er_nhists; uni++) univ_wgts.push_back( histo->GetLatErrorBand(er_type->GetName())->GetUnivWgt(uni) );
							break;
						case ErrorType::kVertical:
						case ErrorType::kVerticalCV:
							for(int uni = 0; uni < er_nhists; uni++) univ_wgts.push_back( histo->GetVertErrorBand(er_type->GetName())->GetUnivWgt(uni) );
							break;
						// case ErrorType::kUnCorError:
						// case ErrorType::kUnCorErrorCV:
						// 	break;
						default:
							cout << "No handling for error: " << er_type->GetName() << endl;
						break;
					}
				}
				// check that the samples have the same number of hists (or zero for no universes):
				if( nhists != er_nhists && nhists != 0){
					cout << __FILE__ << ":" << __LINE__ << " Error : Sample has different number of histograms.";
					cout << " Expect " << er_nhists << " observe " << nhists << endl; 
					exit(0);
				}

				if(nhists != er_nhists){// could also do nhists == 0
					// For the case where we have a sample with no error of type
					same_universes = false;
				}
			}
		}

		// Check that we have errors:
		if(er_nhists > 0){
			// Lets include this error!!:
			std::vector<TH1D*> new_erhists;

			for(int j = 0; j<er_nhists; j++) {
				std::stringstream ss; ss<<j;
				string temp_name = er_type->GetName() + "_Combined_" + ss.str();

				TH1D *temp = (TH1D*)m_HanaHist->Clone(temp_name.c_str());
				temp->Clear();

				// Sample loop:
				int current_bin = 1;
				for(it = m_samples.begin(); it != m_samples.end(); ++it){
					MnvH1D * histo = it->second;
					
					// Area scale is not yet correct, need to understand if the idea is to area noramise the errors to a single sample. 
					// Maybe only ever area normalise by the signal bin.
					int nhists = 0;
					double area_scale = 1.;
					switch(er_type->GetType()){
						case ErrorType::kLateral:
						case ErrorType::kLateralCV:
							area_scale = histo->Integral()/histo->GetLatErrorBand(er_type->GetName())->GetHists()[j]->Integral();
							nhists = (int)histo->GetLatErrorBand( er_type->GetName() )->GetHists().size();
							break;
						case ErrorType::kVertical:
						case ErrorType::kVerticalCV:
							area_scale = histo->Integral()/histo->GetVertErrorBand(er_type->GetName())->GetHists()[j]->Integral();
							nhists = (int)histo->GetVertErrorBand( er_type->GetName() )->GetHists().size();
							break;
						default:
						cout << "This shouldn't evetr be called..." << endl;
						break;
					}

					for(int bin = 1; bin < histo->GetNbinsX() + 1; bin++){
						double bin_content = 0.;
						if(nhists != 0){
							switch(er_type->GetType()){
								case ErrorType::kLateral:
								case ErrorType::kLateralCV:
									bin_content = histo->GetLatErrorBand(er_type->GetName())->GetHists()[j]->GetBinContent(bin);
									break;
								case ErrorType::kVertical:
								case ErrorType::kVerticalCV:
									bin_content = histo->GetVertErrorBand(er_type->GetName())->GetHists()[j]->GetBinContent(bin);
									break;
								default:
									cout << "This shouldn't evetr be called..." << endl;
									break;
							}
							if(cov_area_normalize) bin_content *= area_scale;
						}
						else if(er_type->GetType() == ErrorType::kUnCorError || er_type->GetType() == ErrorType::kUnCorErrorCV){
							if(histo->HasUncorrError( er_type->GetName() ) ) 
								bin_content = histo->GetUncorrError( er_type->GetName() )->GetBinContent(bin);
							else
								bin_content = 0.;
						}
						else bin_content = histo->GetBinContent(bin);
				
						// Fill the analhysis bin:
						temp->SetBinContent(current_bin, bin_content);
						current_bin++;
					}//bin loop
				}// samples loop
				new_erhists.push_back(temp);
			} //j<er_nhists loop 
		
			// Taken from the initialisation of Lat/Ver error bands in MnvH1D:
			bool UseSpreadError = (er_nhists == 1) ? true : false;

			// Now fill the analysis histogram with the errors determined from the samples:
			switch(er_type->GetType()){
				case ErrorType::kLateral:
				case ErrorType::kLateralCV:
					m_anaHist->AddLatErrorBand(er_type->GetName(), new_erhists);
					m_anaHist->GetLatErrorBand(er_type->GetName())->SetUseSpreadError(UseSpreadError);
	     			// Set universe weights; does not work if universes are different for two histograms
					if(same_universes) {
						for(int j = 0; j < er_nhists; j++)
							m_anaHist->GetLatErrorBand( er_type->GetName() )->SetUnivWgt(j, univ_wgts[j] );
					}
					break;
				case ErrorType::kVertical:
				case ErrorType::kVerticalCV:
					m_anaHist->AddVertErrorBand( er_type->GetName(),  new_erhists);
					m_anaHist->GetVertErrorBand(er_type->GetName())->SetUseSpreadError(UseSpreadError);
	     			// Set universe weights; does not work if universes are different for two histograms
					if(same_universes) {
						for(int j = 0; j<er_nhists; j++)
							m_anaHist->GetVertErrorBand(er_type->GetName())->SetUnivWgt(j, univ_wgts[j]);
					}
					break;
				case ErrorType::kUnCorError:
				case ErrorType::kUnCorErrorCV:
					if(new_erhists.size() == 1) m_anaHist->AddUncorrError(er_type->GetName(), new_erhists[0]);
					else{
						cout << __FILE__ << ":" << __LINE__ << " Error : Could not add uncorrelated error to analysis hist. Wrong size: " << new_erhists.size() << endl; 
						exit(0);
					}
					break;
				default:
					cout << "This shouldn't evetr be called..." << endl;
					break;
			}
			new_erhists.empty();
		} //er_nhists > 0	
	} //m_errors loop
	return m_anaHist->GetTotalErrorMatrix(includeStat, asFrac, cov_area_normalize);
}
#endif