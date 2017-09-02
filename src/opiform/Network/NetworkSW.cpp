#include <random>

#include "../Agent/AgentBase.h"

#include "NetworkSW.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

NetworkSW::NetworkSW(int anNeighbors, const double & adbRewiringProb) : NetworkAbstract(), m_nNeighbors(anNeighbors), m_dbRewiringProb(adbRewiringProb) {
}

//-------------------------------------------------------------------------------------

NetworkSW::~NetworkSW() {
}

//-------------------------------------------------------------------------------------

void NetworkSW::generateNetwork(vector<AgentBase*> * apvecAgents)	{
	gen.seed(10);

	m_pvecAgents = apvecAgents;
	int nSize = apvecAgents->size();

	for (int nI = 0; nI< nSize; ++nI) {

		AgentBase * pAgent = (*apvecAgents)[nI];

		for (int nJ = 1; nJ <= m_nNeighbors >> 1; ++nJ) {
			int nID = nI + nJ; //hlaf connections to each side
			if (nID == nI)
				continue;
			if (nID < 0)
				nID += nSize;

			if (nID >= nSize)
				nID -= nSize;

			AgentBase * pAgentAdj = (*apvecAgents)[nID];
			pAgent->addAdjacentAgent(pAgentAdj);
			pAgentAdj->addAdjacentAgent(pAgent);
		}
	}

	networkRewiring(apvecAgents, m_dbRewiringProb);
}

//-------------------------------------------------------------------------------------

void NetworkSW::networkRewiring(std::vector<AgentBase *> * apvecAgents, const double & adbProb) {
	std::uniform_real_distribution<double> dis;
	int nSize = apvecAgents->size();

	for (int nI = 0; nI < nSize; ++nI) {
		AgentBase* pAgent = (*apvecAgents)[nI];

		//Check rewiring for each connection
		for (int nJ = 0; nJ < pAgent->getNeighborhoodSize(); ++nJ) {
			double dbProb = dis(gen);

			if (dbProb < adbProb) {
				//Rewire
				std::uniform_int_distribution<> uidis(0, nSize - 1);

				//New connection shouldn't be already within the neighborhood
				AgentBase* pAgentReplacement = pAgent;
				while (pAgent->isMember(pAgentReplacement) == true) {
					pAgentReplacement = (*apvecAgents)[uidis(gen)];
				}

				//Make new connections
				pAgentReplacement->addAdjacentAgent(pAgent);
				pAgent->addAdjacentAgent(pAgentReplacement);

				//Remove old connections
				AgentBase* pAgentOld = (AgentBase*)pAgent->getNeighbor(nJ);
				pAgent->removeAgent(pAgentOld);
				pAgentOld->removeAgent(pAgent);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkSW::step() {

	int nSize = m_pvecAgents->size();

	//Iterate through agents
	for (int nI = 0; nI < nSize; ++nI) {

		AgentBase* pAgent = (*m_pvecAgents)[nI];
		int nRes = (*m_OpiformModel)(pAgent);

	}

	return true;
}