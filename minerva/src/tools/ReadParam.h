#ifndef __READPARAM__HXX__
#define __READPARAM__HXX__

#include <string>
#include <vector> 
#include <cstdlib> //exit(X)

#include <map>

// Want this class to read in parameters files for the following:
// Read in parameter for plotting w/ symbol, units, binning etc. + options
// Read in general parameters file for things like signal 

const std::string left_brace = "<|";
const std::string right_brace = "|>";
const std::string ignore_sign = "//";

class ReadParam
{
public:
   
    // Fix this to include axis titles in parser.
    ReadParam(){;}
    ~ReadParam(){;}

    static std::vector<std::string> ReadFile(const std::string &infile, const std::string &left_arrow = left_brace,
        const std::string &right_arrow = right_brace, const std::string &ignore = ignore_sign);

    static std::string GetParameterS(const std::string &name, const std::string &infile, const std::string &left_arrow = left_brace, 
        const std::string &right_arrow = right_brace, const bool remove_spaces = true, const std::string &ignore = ignore_sign);

    static int GetParameterI(const std::string &name, const std::string &infile, const std::string &left_arrow = left_brace,
        const std::string &right_arrow = right_brace, const std::string &ignore = ignore_sign);

    static double GetParameterD(const std::string &name, const std::string &infile, const std::string &left_arrow = left_brace,
        const std::string &right_arrow = right_brace, const std::string &ignore = ignore_sign);

    static bool GetParameterB(const std::string &name, const std::string &infile, const std::string &left_arrow = left_brace,
        const std::string &right_arrow = right_brace, const std::string &ignore = ignore_sign);

    static void RemoveArrows(std::string &word, const std::string &left_arrow = left_brace,
        const std::string &right_arrow = right_brace);

    static bool IsNumber(const std::string &name);

    static void GetBinning(const std::string &name, const std::string &infile, int &nbins, double *&binning, const std::string &left_arrow = left_brace, 
        const std::string &right_arrow = right_brace, const std::string &ignore = ignore_sign);

private:
    static std::string GetParameterN(const std::string &name, const std::string &infile, 
        const std::string &left_arrow, const std::string &right_arrow, const std::string &ignore);
    std::string FindOpt(const std::string &name, const std::string &in);

    static const bool m_verbose;
};
#endif