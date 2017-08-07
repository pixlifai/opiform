#include <fstream>
#include <sstream>
#include <string>

#include "../Network/NetworkAbstract.h"

#include "../Game/Game.h"

using namespace std;
using namespace opiform;

int readData() {
	ifstream in;
	in.open("C:/Projekti/Matjaz/CompEcon/results_0.txt");

	string str;
	int nI = 0;
	while (in.eof() == false) {
		while (std::getline(in, str)) {

			ofstream out;
			out.open("C:/Projekti/Matjaz/CompEcon/res_" + std::to_string(nI) + ".txt");
			out << str;
			out.close();
			++nI;
		}
	}

	return 0;
}

int main(int argc, char * argv[]) {
	if (argc != 3) {

	}

#if 0
	readData();

#else
	int nAgents = 10000;
	time_t tIterations = 10000000;

	int nTreshold =
		100;
//		atoi(argv[1]);
	int nEnd =
		100;
//		atoi(argv[2]);

	Game::registerStatics();

	while (nTreshold <= nEnd)	{
		for (int nI = 0; nI < 1; ++nI) {
			string strName = "C:/temp/results_" + to_string(nTreshold) + ".txt";
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
#endif
	return 0;
}