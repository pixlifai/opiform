#include <string>
#include <iostream>
#include <fstream>

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

	const int nAgents = 100;
	const time_t tIterations = 5;
	const int nRepetitions = 50;

	int nTreshold = 0;
	int nEnd = 1;

	Game::registerStatics();

	while (nTreshold <= nEnd)	{
		for (int nI = 0; nI < nRepetitions; ++nI) {
			string strName = "results_" + to_string(nTreshold) + ".txt";
			Game game(strName.c_str());
			if (game.init((double)nTreshold/100, nAgents) == false)
				continue;

			NetworkAbstractParams np;
			np.m_dbConnectionProb = 0.05;
			np.m_dbRewiringProb = 0.05;
			np.m_nConnectedNodes = 10;
			np.m_nNeighbors = 10;
			np.m_OpinionFormationModelType = OpinionFormationModel::DW;
			NetworkType netType = NetworkType::FC;

			if (game.initNetwork(netType, &np) == false)
				continue;

			game.runGame(tIterations);
		}
		nTreshold += 1;
	}

	return 0;
}