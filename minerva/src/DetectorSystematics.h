#ifndef __DETECTORSYSTEMATICS_H__
#define __DETECTORSYSTEMATICS_H__

#include <SystematicsBase.h>

class DetectorSystematics : public SystematicsBase
{
public:
	DetectorSystematics(std::string in_filename, std::string in_treename, std::string out_filename, bool verbose = true);
	~DetectorSystematics();

	void Run();

private:

};

#endif