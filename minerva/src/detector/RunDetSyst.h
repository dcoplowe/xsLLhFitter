#ifndef __RUNDETSYST_H__
#define __RUNDETSYST_H__

// class DetectorSystematics;

class RunDetSyst
{
public:
	RunDetSyst(){;}
	~RunDetSyst(){;}

	static void MakeMggDist();
	static void MakeFullMgg();
	static void MakeFulldpTT();
// private:

	// DetectorSystematics * syst;
};

#endif