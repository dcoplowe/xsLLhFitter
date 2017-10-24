#ifndef __DATAINFO_H__
#define __DATAINFO_H__

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

		// const double Pion = 134.9766;// MeV
		// const double PionSq = Pion*Pion;

		const double Muon = 105.6583745;// MeV
		const double MuonSq = Muon*Muon;
	}
}

#endif