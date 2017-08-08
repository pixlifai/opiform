#include <string>
#include <iostream>

#include "../Utils/Writer.h"

#include "../Network/NetworkAbstract.h"

#include "../Game/Game.h"

using namespace std;
using namespace opiform;

int main(int argc, char * argv[]) {
	std::cout << "Usage:\n "
		<< argv[0] << " <path/to/output/result/folder/>"
		<< argv[0];

	if (argc == 2) {
		Utils::setFolder(argv[1]);
	}

	const int nAgents = 10000;
	const time_t tIterations = 500000;
	const int nRepetitions = 50;

	int nTreshold = 100;
	int nEnd = 100;

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

			if (game.initNetwork(NetworkType::CM, &np) == false)
				continue;

			game.runGame(tIterations);
		}
		nTreshold += 1;
	}

	return 0;
}