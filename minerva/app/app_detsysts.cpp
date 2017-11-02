#include <ReadParam.h>
#include <DataInfo.h>

using std::cout;
using std::endl;
using std::string;

const std::string default_Params = "options/run_opts.txt";

int main()//int argc, char *argv[])
{
	int run_type = ReadParam::GetParameterI("run_type", DataInfo::Opts::Run);

	if(run_type == 1) RunDetSyst::MakeMggDist();
	else if(run_type == 2) RunDetSyst::MakeFullMgg();
	else if(run_type == 3) RunDetSyst::MakeFulldpTT();
}