#include <random>

#include "../Agent/AgentBase.h"

#include "NetworkER.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

NetworkER::NetworkER(const double & adbConnectionProb) : NetworkAbstract(), m_dbConnectionProb(adbConnectionProb) {
}

NetworkER::~NetworkER() {}


void NetworkER::generateNetwork(std::vector<AgentBase*> * apvecAgents)	{

	m_pvecAgents = apvecAgents;
	int nSize = apvecAgents->size();

	double dbC = 1.5;
	const double dbProb = dbC * (1.0 / nSize) * log(nSize);

	std::uniform_real_distribution<double> dis;

	for (int nI = 0; nI< nSize; ++nI) {

		AgentBase* pAgent = (*apvecAgents)[nI];

		for (int nJ = 0; nJ < nSize; ++nJ) {
			if (dbProb > dis(gen) && nI != nJ) {
				AgentBase* pAgentAdjacent = (*apvecAgents)[nJ];

				bool bRes = pAgent->isMember(pAgentAdjacent);
				if (bRes == false)
					pAgent->addAdjacentAgent(pAgentAdjacent);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkER::step() {

	int nSize = m_pvecAgents->size();

	//Iterate through agents
	for (int nI = 0; nI < nSize; ++nI) {

		AgentBase * pAgent = (*m_pvecAgents)[nI];
		int nRes = (*m_OpiformModel)(pAgent);

	}

	return true;
}