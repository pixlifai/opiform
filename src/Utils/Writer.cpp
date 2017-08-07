#include <vector>
#include <fstream>
#include <iterator>
#include <map>

#include "../Agent/AgentBase.h"

#include "Writer.h"

using namespace std;
using namespace opiform;

void Utils::writeAgents(const std::vector<AgentBase*> & aT, const char * apchFile, const char * apchDelim) {
	std::ofstream out;
	out.open(apchFile, std::ios_base::app);

	for (int nI = 0; nI < aT.size(); ++nI) {
		const AgentBase * pAgent = aT[nI];
		if (pAgent->getType() != AgentType::Stubborn) {
			int nIndy = -1;
		}

		out << pAgent->getOpinion() << "\t";

	}
	out << endl;
	out.close();
}

//----------------------------------------------------------------------------------------------------------------------

void Utils::writeNetwork(const std::vector<AgentBase*> & avecAgents, const char * apchFile) {
	std::ofstream out;
	out.open(apchFile);

	out << "Agent and agent's connections\n";
	for (int nI = 0; nI < avecAgents.size(); ++nI) {

		AgentBase * pAgent = avecAgents[nI];
		out << pAgent->getID() << "\t";
		for (int nJ = 0; nJ < pAgent->getNeighborhoodSize(); ++nJ) {
			out << pAgent->getNeighbor(nJ)->getID() << "\t";
		}
		out << "\n";
	}

	//Collect number of connections per agent
	out << "Connections per agent\nConnections\n";
	for (int nI = 0; nI < avecAgents.size(); ++nI) {
		AgentBase * pAgent = avecAgents[nI];
		out << pAgent->getNeighborhoodSize() << "\n";
	}

	//Collect number of agents per number of connections
	std::map<int, int> m;
	for (int nI = 0; nI < avecAgents.size(); ++nI) {
		AgentBase* pAgent = avecAgents[nI];
		++m[pAgent->getNeighborhoodSize()];
	}

	out << "\n";
	out << "Agents per connections\nConnections Agents\n";
	std::map<int, int>::const_iterator it = m.cbegin();
	for (it; it != m.cend(); ++it) {
		out << it->first << "\t" << it->second << "\n";
	}

	out.close();

}