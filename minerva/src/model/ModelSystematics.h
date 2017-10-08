#ifndef __MODELSYSTEMATICS_H__
#define __MODELSYSTEMATICS_H__

#include <SystematicsBase.h>
#include <string>

class TGraph;
class TH1D;

class ModelSystematics : public SystematicsBase
{
public:
	ModelSystematics(const int n_genie_shifts = 7, const bool verbose = false);
	~ModelSystematics();

	// bool AddLatErrorBand(const std::string& name, const int reaction_type);
	// bool AddLatErrorBandAndFillWithCV(const std::string& name, const int reaction_type);
	// bool AddVertErrorBand(const std::string& name, const int reaction_type);
	// bool AddVertErrorBandAndFillWithCV(const std::string& name, const int reaction_type);
	// bool AddUncorrError(const std::string& name, const int reaction_type);
	// bool AddUncorrErrorAndFillWithCV(const std::string& name, const int reaction_type);

	bool AddLatErrorBand(const std::string& name, const std::string& reaction_type);
	bool AddLatErrorBandAndFillWithCV(const std::string& name, const std::string& reaction_type);
	bool AddVertErrorBand(const std::string& name, const std::string& reaction_type);
	bool AddVertErrorBandAndFillWithCV(const std::string& name, const std::string& reaction_type);
	bool AddUncorrError(const std::string& name, const std::string& reaction_type);
	bool AddUncorrErrorAndFillWithCV(const std::string& name, const std::string& reaction_type);

	void BuildResponses(const std::string &outfname);

private:
	TGraph * MakeResFunc(const std::vector<TH1D*> & hists, const int bin);

};

#endif