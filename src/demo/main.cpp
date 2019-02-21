#include <string>
#include <iostream>
#include <chrono>

#include "Utils/Writer.h"

#include "Network/NetworkAbstract.h"

#include "Game/Game.h"

using namespace std;
using namespace opiform;

int main(int argc, char * argv[]) {
	/*
		Params:
		- nAgents:		number of agents used
		- tIerations:	number of iterations per repetition
		- nRepetitions:	number of repetitons per network
		- netType:		type of network
		{
		SW (Watts-Strogatz),
		FC (Fully-connected),
		ER (Erdos–Renyi),
		BA (Barabasi-Albert),
		CM (Communities)
		}

		Additional params:
		1.) output folder = argv[1] or "./" if not given
		*/

	if (argc == 2)
		Utils::setFolder(argv[1]);

	else
		Utils::setFolder("C:/Research/Research/FinitelyLived/Koda/docs/");

	const int nAgents = 1000;
	const time_t tIterations = 50000;
	const int nRepetitions = 30;
	double dbKappa = 0.999999;		// VELIKA KAPPA JE ABSOLUTNA KONSISTENTNOST PRI LINEARNI KOMBINACIJI

	Game::registerStatics();

	auto t0 = std::chrono::system_clock::now();

	for (int nI = 0; nI < nRepetitions; ++nI) {
		string strName = "results_SB_" + to_string(nI) + ".txt";
		Game game(strName.c_str());
		if (game.init(nAgents) == false)
			continue;

		NetworkAbstractParams np;
		np.m_dbConnectionProb = 0.05;
		np.m_dbRewiringProb = 0.05;
		np.m_nConnectedNodes = 10;
		np.m_nNeighbors = 10;
		np.m_OpinionFormationModelType = OpinionFormationModel::DW;
		NetworkType netType = NetworkType::SWAge;

		if (game.initNetwork(netType, &np) == false)
			continue;

		game.runGame(tIterations, dbKappa);
	}

	auto t1 = chrono::system_clock::now();
	std::chrono::duration<double> diff = t1-t0;
	std::cout << diff.count() << std::endl;

	return 0;
}
