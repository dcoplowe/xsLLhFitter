#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <string>
#include <vector>
// #include <TROOT.h>

class MnvH1D;
class MnvH2D;
class TH1D;

class Sample
{
public:
	// Should simplify this and remove the model bit. 
	// Just make this more of a wrapper for the Minverva histo class.
	enum Type { ModQE = 0, ModQE, ModDis, Det };

	Sample(std::string name, int nbins, double x_low, double x_high, Type type);
	Sample(std::string name, int nbins, double * x_bins, Type type);

	~Sample();

	std::vector<MnvH1D*> GetMnvH1D(){ return m_1Dhists; }
	MnvH1D* GetMnvH1D(int n){ return m_1Dhists[n]; }
	int GetNHists(){ return (int)m_1Dhists.size(); }
	void Fill(double value, double wgt, int model = -1);

	// -------------------------------------------------------- From MnvH1D --------------------------------------------------------
	bool AddLatErrorBand( const std::string& name, const int nhists = -1 );
	bool AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base );
	bool AddLatErrorBandAndFillWithCV( const std::string& name, const int nhists );
	bool AddVertErrorBand( const std::string& name, const int nhists = -1 );
	bool AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base );
	bool AddVertErrorBandAndFillWithCV( const std::string& name, const int nhists );

	bool AddUncorrError( const std::string& name );
	bool AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent = false );
	bool AddUncorrErrorAndFillWithCV( const std::string& name );

	bool AddMissingErrorBandsAndFillWithCV( const MnvH1D& ref );
	bool AddMissingErrorBandsAndFillWithCV( const MnvH2D& ref );

	bool FillLatErrorBand( const std::string& name, const double val, const std::vector<double>& shifts,
		const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand( const std::string& name, const double val, const double * shifts, const double cvweight = 1.0,
		const bool fillcv = true, const double *weights = 0 );
	bool FillLatErrorBand( const std::string& name, const double val, const double shiftDown, const double shiftUp,
		const double cvweight = 1.0, const bool fillcv = true );
	bool FillVertErrorBand( const std::string& name, const double val, const std::vector<double>& weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand( const std::string& name, const double val, const double * weights,
		const double cvweight  = 1.0, double cvWeightFromMe = 1.);
	bool FillVertErrorBand( const std::string& name, const double val, const double weightDown, const double weightUp,
		const double cvweight  = 1.0, double cvWeightFromMe = 1. );
	bool FillUncorrError( const std::string& name, const double val, const double err, const double cvweight = 1.0 );
	// -------------------------------------------------------- END MnvH1D --------------------------------------------------------

private:
	std::vector<MnvH1D*> m_1Dhists;
	Type m_type;
	bool IsModel(int model);
	const char * GetType();
};

#endif

#ifndef __SYSTEMATICSBASE_H__
#define __SYSTEMATICSBASE_H__

#include <string>
#include <vector>
// #include <TROOT.h>

class TFile;
class TTree;
class MnvH1D;

class SystematicsBase
{
public:
	SystematicsBase();
	SystematicsBase(int n_universes, bool verbose);
	SystematicsBase(bool verbose);
	SystematicsBase(std::string in_filename, std::string in_treename, bool verbose);
	~SystematicsBase();
	virtual void Run() = 0;

	void SetNUniverses(int var){ m_Nuniverses = var; }
	void AddSample(std::string name, int nbins, double x_low, double x_high) = 0;
	void AddSample(std::string name, int nbins, double * x_bins) = 0;
	int GetNSamples(){ return (int)m_samples.size(); }

protected:
	bool m_verbose;

	TFile * m_infile;
	TTree * m_intree;

	TFile * m_outfile;
	void SetupOutFile(std::string outfilename);

	std::vector<Sample*> m_samples;
	int m_Nuniverses;
private:
	std::string	m_date;
};

#endif