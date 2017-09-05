#include <random>

#include "../Agent/AgentBase.h"

#include "NetworkFC.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd() /*10*/);
}


NetworkFC::NetworkFC() : NetworkAbstract() {
}

NetworkFC::~NetworkFC() {}


void NetworkFC::generateNetwork(std::vector<AgentBase *> * apvecAgents)	{
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkFC::step(std::vector<AgentBase *> * apvecAgents) {

	int nSize = apvecAgents->size();

	//Iterate through agents
	for (int nI = 0; nI < nSize; ++nI) {

		AgentBase* pAgent = (*apvecAgents)[nI];
		AgentBase* pAgentAdjacent = nullptr;

		std::uniform_int_distribution<> dis(0, apvecAgents->size()-1);
		int nID = dis(gen);

		pAgentAdjacent = (*apvecAgents)[nID];

		if (pAgentAdjacent == nullptr)
			continue;

		//Store opinions before a change
		double dbOpinionAgent = pAgent->getOpinion();
		double dbOpinionAdjacent = pAgentAdjacent->getOpinion();

		double dbMu = 0.2;

		if (pAgent->shouldUpdate(dbOpinionAdjacent))
			pAgent->setOpinion(dbOpinionAgent + dbMu*(dbOpinionAdjacent - dbOpinionAgent));

		if (pAgentAdjacent->shouldUpdate(dbOpinionAgent))
			pAgentAdjacent->setOpinion(dbOpinionAdjacent + dbMu*(dbOpinionAgent - dbOpinionAdjacent));

	}

	return true;
}