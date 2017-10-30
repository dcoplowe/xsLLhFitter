#ifndef __DETECTORSYSTEMATICS_CPP__
#define __DETECTORSYSTEMATICS_CPP__

#include <DetectorSystematics.h>
#include <Sample.h>
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TMatrixD.h>
#include <sstream>
#include <ErrorType.h>
#include <TMath.h>

using std::cout;
using std::endl;
using std::string;

const std::string DetectorSystematics::m_ver_name = "";//"_Vert";
const std::string DetectorSystematics::m_lat_name = "";//"_Lat";
const std::string DetectorSystematics::m_uncer_name = "";//"_UnCorEr";

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
	m_HanaHist = 0x0;
	m_errors.clear();
}

DetectorSystematics::~DetectorSystematics()
{
	m_errors.clear();
	if(m_anaHist) delete m_anaHist;
	if(m_HanaHist) delete m_HanaHist;
}

bool DetectorSystematics::AddDefaults()
{
	// These errors have only an upper and lower bound (dim 2/2 universes)
	const int n_wgts = 2;
	bool added = (AddVertErrorBand("MiMisTagTrue",  n_wgts) && AddVertErrorBand("MiMisTagFalse", n_wgts) &&
				  //AddVertErrorBand("MichelTagging", n_wgts) &&
				  AddVertErrorBand("MuonTracking", n_wgts) && AddVertErrorBand("NeutronResponse", n_wgts) &&
		  		  AddVertErrorBand("PionResponse", n_wgts) && AddVertErrorBand("ProtonTracking", n_wgts));
	return added;
}

bool DetectorSystematics::FillDefaults(const std::string& sam_name, const double value, const DetError::Default elist,
	const double cvweight, double cvWeightFromMe)
{						
	bool filled = (FillVertErrorBand(sam_name, "MiMisTagTrue",    value, elist.michel_true,	 cvweight, cvWeightFromMe) &&
				   FillVertErrorBand(sam_name, "MiMisTagFalse",   value, elist.michel_false, cvweight, cvWeightFromMe) &&
				// FillVertErrorBand(sam_name, "MichelTagging",   value, elist.michel, 	 	 cvweight, cvWeightFromMe) &&
				   FillVertErrorBand(sam_name, "MuonTracking",    value, elist.mu_trking, 	 cvweight, cvWeightFromMe) &&
				   FillVertErrorBand(sam_name, "NeutronResponse", value, elist.neutron_res,  cvweight, cvWeightFromMe) &&
				   FillVertErrorBand(sam_name, "PionResponse",    value, elist.pi_res, 		 cvweight, cvWeightFromMe) &&
				   FillVertErrorBand(sam_name, "ProtonTracking",  value, elist.pr_trking, 	 cvweight, cvWeightFromMe) );
	return filled;
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
			if(it->second->AddLatErrorBand(tmp_name, n_universes ) ) counter++;
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
			if(it->second->AddLatErrorBandAndFillWithCV(tmp_name, n_universes )) counter++;
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
		// if(m_verbose)
		cout << "Add Vertical Error Band: " << tmp_name << " with " << n_universes << " universes." <<  endl;

		ErrorType * tmp_et = new ErrorType(tmp_name, n_universes, ErrorType::kVertical);
		m_errors.push_back(tmp_et);
	
		cout << "Adding to sample(s): " << endl;
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		for (; it != m_samples.end(); ++it){ 
			cout << "					  " << it->first;
			it->second->AddError(tmp_et);
			if(it->second->AddVertErrorBand(tmp_name, n_universes )){ 
				counter++;
				cout << " Success ";
			}
			cout << endl;
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
			if(it->second->AddVertErrorBandAndFillWithCV(tmp_name, n_universes )) counter++;
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
		tot_bins += it->second->GetNAnaBins();

	if(m_verbose) cout << "Found to be " << tot_bins << endl;

	m_HanaHist = new TH1D("Analysis_Hist_TH1D", "", tot_bins, 0, tot_bins); 

	int current_bin = 1;

	for(int nn = 0; nn < m_Nsamples; nn++){
		it = m_samples.begin();		
		for (; it != m_samples.end(); ++it){
			Sample * histo = it->second;
			if(histo->GetSampPos() != nn) continue;
			cout << "histo->GetSampPos() = " << histo->GetSampPos() << endl;
			for(int i = histo->GetMinBin(); i < histo->GetMaxBin(); i++){
				m_HanaHist->SetBinContent(current_bin, histo->GetBinContent(i));
				if(includeStat) m_HanaHist->SetBinError(current_bin, histo->GetBinError(i));
				current_bin++;
			}
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

	// Determine normalisation:
	// bool cov_area_normalize = false;
	// bool cov_slice_normalize = false;
	// if(norm.find("A") != std::string::npos) cov_area_normalize = true;
	// else if(norm.find("a") != std::string::npos) cov_area_normalize = true;
	// // This doesn't work how I expected. I think this is wrong: you need to think about the construction of a covariance matrix... idiot.
	// else if(norm.find("S") != std::string::npos) cov_slice_normalize = true;
	// else if(norm.find("S") != std::string::npos) cov_slice_normalize = true;
	// Check that all the samples have the same errors:	

	cout << "Linking errors from individual samples" << endl;

	for(size_t er = 0; er < m_errors.size(); er++){
		ErrorType * er_type = m_errors[er];

		// int er_nhists_tot = 0;
		int er_nhists = 0;
		bool same_universes = true;
		std::vector<double> univ_wgts;

		cout << "Connecting error " << er_type->GetName() << endl;

		// Check that we have a histo with a particular error type:
		std::map<std::string,Sample*>::iterator it= m_samples.begin();
		int counter = 0;
		for (; it != m_samples.end(); ++it){
			MnvH1D * histo = it->second;

			cout << "Getting error from sample " << it->first << endl;

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
				cout << "Found " << nhists << " hists found for error " << er_type->GetName() << " in sample " << it->first << endl;
				// er_nhists_tot += nhists;
				if(nhists > 0 && counter == 0){ 

					er_nhists = nhists;
					counter++;

					cout << "Setting Up Checks: Sample hist numbers are identical (or zero). Require " << er_nhists << " hists" << endl;

					// Get the wgts from this sample (as it will be the same in all sample for this error)
					univ_wgts.clear();
					switch(er_type->GetType()){
						case ErrorType::kLateral:
						case ErrorType::kLateralCV:
							for(int uni = 0; uni < nhists; uni++) univ_wgts.push_back( histo->GetLatErrorBand(er_type->GetName())->GetUnivWgt(uni) );
							break;
						case ErrorType::kVertical:
						case ErrorType::kVerticalCV:
							for(int uni = 0; uni < nhists; uni++) univ_wgts.push_back( histo->GetVertErrorBand(er_type->GetName())->GetUnivWgt(uni) );
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
					cout << "Found sample (" << it->first << ") with no hists!" << endl;
				}
			}
		}

		// Check that we have errors:
		if(er_nhists > 0){

			cout << "Errors looking good! Now building universe histograms" << endl;

			// Lets include this error!!:
			std::vector<TH1D*> new_erhists;

			for(int j = 0; j<er_nhists; j++) {
				std::stringstream ss; ss<<j;
				string temp_name = er_type->GetName() + "_Combined_" + ss.str();
				// cout << j+1 << "/" << er_nhists << ") Making " << temp_name << endl;

				TH1D *temp = (TH1D*)m_HanaHist->Clone(temp_name.c_str());
				temp->Clear();

				// Sample loop:
				int current_bin = 1;

				for(int nn = 0; nn < m_Nsamples; nn++){
					for(it = m_samples.begin(); it != m_samples.end(); ++it){
						Sample * histo = it->second;
						if(histo->GetSampPos() != nn) continue;
						// cout << "histo->GetSampPos() = " << histo->GetSampPos() << " : " << histo->GetName() << endl;
						
						// For computing the integral we want to make sure we are using the correct range:
						int nlowBin = histo->GetMinBin();
						int nhighBin = histo->GetNAnaBins();

						// Area scale is not yet correct, need to understand if the idea is to area noramise the errors to a single sample. 
						// Maybe only ever area normalise by the signal bin.
						int nhists = 0;
						double area_scale = 1.;
						switch(er_type->GetType()){
							case ErrorType::kLateral:
							case ErrorType::kLateralCV:
								area_scale = histo->Integral(nlowBin, nhighBin);
								area_scale *= 1./histo->GetLatErrorBand(er_type->GetName())->GetHists()[j]->Integral(nlowBin, nhighBin);
								nhists = (int)histo->GetLatErrorBand( er_type->GetName() )->GetHists().size();
								break;
							case ErrorType::kVertical:
							case ErrorType::kVerticalCV:
								area_scale = histo->Integral(nlowBin, nhighBin);
								area_scale *= 1./histo->GetVertErrorBand(er_type->GetName())->GetHists()[j]->Integral(nlowBin, nhighBin);
								nhists = (int)histo->GetVertErrorBand( er_type->GetName() )->GetHists().size();
								break;
							default:
							cout << "This shouldn't ever be called..." << endl;
							break;
						}

						// cout << "For sample " << it->first << " with " << nhists << " (er_nhists = " << er_nhists << ") hists containing ";
						// cout << histo->GetNbinsX() << " bins.";
						// if(cov_area_normalize) cout << " Area Norm: " << area_scale;
						// cout << endl;

						for(int bin = histo->GetMinBin(); bin < histo->GetMaxBin(); bin++){
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
							// cout << "Filling global bin " << current_bin << " of " << m_HanaHist->GetNbinsX() << ":";
							// cout << " Sample bin " << bin << " of " << histo->GetNbinsX() << ": " << bin_content << endl;
							// Fill the analhysis bin:
							temp->SetBinContent(current_bin, bin_content);
							current_bin++;
						}//bin loop
					}// samples loop
				}//n_sample counter loop
					// cout << "Finish Filling Universe " << j+1 << " of " << er_nhists << endl;
				new_erhists.push_back(temp);
			} //j<er_nhists loop 
		
			// Taken from the initialisation of Lat/Ver error bands in MnvH1D:
			bool UseSpreadError = (er_nhists == 1) ? true : false;

			cout << "Adding Error to analysis hist" << endl;
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
	cout << "Built analyst hist with full errors" << endl;

	TMatrixD cov =  m_anaHist->GetTotalErrorMatrix(includeStat, asFrac, cov_area_normalize);
	// if(cov_slice_normalize) SliceNorm(cov);
	return cov;
}

void DetectorSystematics::SliceNorm(TMatrixD &cov)
{
	if(cov.GetNrows() == cov.GetNcols()){
		TH2D * tmp = new TH2D(cov);
		tmp = NormalHist(tmp);		
		int lowBin = 0; //cov.GetNrows();
		int highBin = cov.GetNcols();
		cout << "Slice norm: Binning low = " << lowBin << " : high = " << highBin << endl; 
		tmp->Write();
		for(int i = lowBin; i < highBin; ++i ){
			for(int k = lowBin; k < highBin; ++k ){ 
		        //Gettting the the CV value for bin i
				const double cv = tmp->GetBinContent( (i+1) , (k+1) );
				cout << "cov[" << i << "][" << k << "] = " << cv << endl;
				cov[i][k]= cv;
			//	cov[k][i]=cov[i][k];
			}
		}
		delete tmp;
	}
}

TH2D * DetectorSystematics::NormalHist(TH2D * hraw, double thres, bool kmax)
{
    TH2D * hh = static_cast<TH2D*>( hraw->Clone(Form("%sSN",hraw->GetName())) );
    hh->Scale(0);
    
    const int x0 = hh->GetXaxis()->GetFirst();
    const int x1 = hh->GetXaxis()->GetLast();
    const int y0 = hh->GetYaxis()->GetFirst();
    const int y1 = hh->GetYaxis()->GetLast();
    
    double hmax = -1e10;
    double hmin = 1e10;
    double nent = 0;
    for(int ix=x0; ix<=x1; ix++){
        
        //if option "e" is specified, the errors are computed. if option "o" original axis range of the taget axes will be kept, but only bins inside the selected range will be filled.
        
        TH1D * sliceh = hraw->ProjectionY(Form("tmpnormalhist%sx%d", hh->GetName(), ix), ix, ix, "oe");
        const double tot = sliceh->GetEntries();
        
        TH1D * pdfh=0x0;
        
        if(tot>1e-12){
            nent += tot;
            
            double imax = -999;
            
            if(!kmax){
                pdfh = ToPDF(sliceh,"tmp");
            }
            else{
                imax = sliceh->GetBinContent(sliceh->GetMaximumBin());
            }
            
            for(int iy=y0; iy<=y1; iy++){
                const double cont = kmax ? sliceh->GetBinContent(iy)/imax : pdfh->GetBinContent(iy);
                const double ierr = kmax ? sliceh->GetBinError(iy)/imax   : pdfh->GetBinError(iy);
                if(tot>thres && cont>0){
                    hh->SetBinContent(ix, iy, cont);
                    hh->SetBinError(ix,iy, ierr);
                    if(cont>hmax) hmax = cont;
                    if(cont<hmin) hmin = cont;
                }
            }
        }
        
        delete pdfh;
        delete sliceh;
    }
    
    hh->SetEntries(nent);
    hh->SetMinimum(0.99*hmin);
    hh->SetMaximum(1.1*hmax);
    return hh;
}

TH1D * DetectorSystematics::ToPDF(TH1D *hraw, std::string hn){
    const int x0 = 0;
    const int x1 = hraw->GetNbinsX()+1;
    const double tmpnt = hraw->Integral(x0, x1);
    
    TH1D * hist = (TH1D*) hraw->Clone( (hn + std::string(hraw->GetName()) + "pdf").c_str() );
    hist->Scale(0);
    
    for(int ib=x0; ib<=x1; ib++){
        const double bw = hraw->GetBinWidth(ib);
        const double cont = hraw->GetBinContent(ib);
        if(cont<1e-12)
            continue;
        
        //in case of finit number of bins (i.e. eff not always small), Binomial error is more accurate than Poisson error
        const double eff = cont/tmpnt;
        const double pdf = eff/bw;
        
        const double dpdf = TMath::Sqrt(eff*(1-eff)/tmpnt) / bw;
        hist->SetBinContent(ib, pdf);
        hist->SetBinError(ib, dpdf);
    }
    
    hist->SetEntries(tmpnt);
    
    return hist;
}

void DetectorSystematics::MakeBinning(const std::string &out_name)
{
	std::map<std::string,Sample*>::iterator it= m_samples.begin();
	for (; it != m_samples.end(); ++it){ 
		Sample * sam =	it->second;

		std::string tmp_name = out_name;
		if(!tmp_name.empty()) tmp_name += "_";
		tmp_name += Form("sample%d_%s.txt", sam->GetSampPos(), sam->GetName() );

		cout << "File Name: " << tmp_name << endl;

		std::ofstream file;
		file.open(tmp_name.c_str());

		// Check if underlow/overflow is included;

		for(int i = sam->GetMinBin(); i < sam->GetMaxBin(); i++){
			// Fill 1D then 2D: We haven't defined a second dim yet
			// So use p/m 99999999. 
			cout << sam->GetBinLowEdge(i) << " " << sam->GetBinLowEdge(i + 1) << " " << -9999999. << " " << 9999999. << endl;

			file<<std::left;
			file.width(12); file<< sam->GetBinLowEdge(i) <<" ";    
			file.width(12); file<< sam->GetBinLowEdge(i + 1)<<" ";      
			file.width(12); file<< -9999999 << " ";      
			file.width(12); file<< 9999999 << " ";      
			file<<std::endl;

		}


	}

}

#endif