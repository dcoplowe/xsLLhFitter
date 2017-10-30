#ifndef __DATAINFO_H__
#define __DATAINFO_H__

#include <string>
#include <ReadParam.h>

namespace DataInfo{
	namespace POT{
		const double DATA = 3.33153e+20;
		const double MC = 2.21867e+21;
		const double MEC2P2H = 2.56925e+21;
		const double RATIO = DATA/MC;
		const double RMC2P2H = MC/MEC2P2H;
	}

	namespace Unit{
		const double MeV2GeV = 1e-3;
		const double MeVSq2GeVSq = MeV2GeV*MeV2GeV;
	}

	namespace Mass{
		const double Neutron = 939.5654133;//MeV
		const double NeutronSq = Neutron*Neutron;

		const double Pi0 = 134.9766;// MeV
		const double Pi0Sq = Pi0*Pi0;

		const double Proton = 938.2720813;// MeV
		const double ProtonSq = Proton*Proton;

		// const double Pion = 134.9766;// MeV
		// const double PionSq = Pion*Pion;

		const double Muon = 105.6583745;// MeV
		const double MuonSq = Muon*Muon;
	}

	namespace Opts{
		const std::string base_file = "options/options_files.txt";

		const std::string Run = "options/" + ReadParam::GetParameterS("run", base_file);
		const std::string ReconPars = "options/" + ReadParam::GetParameterS("recon", base_file);
		const std::string ManagePlots = "options/" + ReadParam::GetParameterS("manage_plots", base_file);
		const std::string TransPlots = "options/" + ReadParam::GetParameterS("transverse_plots", base_file);
		const std::string PartiPlots = "options/" + ReadParam::GetParameterS("particle_plots", base_file);
	}
}

#endif