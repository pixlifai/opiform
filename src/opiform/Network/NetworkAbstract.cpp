#include <memory>
#include <queue>
#include <random>

#include "NetworkFC.h"
#include "NetworkBA.h"
#include "NetworkER.h"
#include "NetworkSW.h"
#include "NetworkCM.h"

#include "NetworkAbstract.h"

using namespace opiform;
using namespace std;

#if __cpp_lib_make_unique >= 201304
// note: this implementation does not disable this overload for array types
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

namespace {
	std::random_device rd;

	std::mt19937 gen(
		rd()
//		10
		);
}


NetworkAbstract::~NetworkAbstract()	{
	int nIndy = -1;
}

unique_ptr< NetworkAbstract> NetworkAbstract::findAndCreateNetwork(const NetworkType & aType, NetworkAbstractParams * apNetworkParams) {

	std::unique_ptr<NetworkAbstract> pNet = 0;

	switch (aType) {
	case NetworkType::BA:
		pNet = make_unique<NetworkBA>(apNetworkParams->m_nConnectedNodes);
		break;
	case NetworkType::ER:
		pNet = make_unique<NetworkER>(apNetworkParams->m_dbConnectionProb);
		break;
	case NetworkType::FC:
		pNet = make_unique<NetworkFC>();
		break;
	case NetworkType::SW:
		pNet = make_unique<NetworkSW>(apNetworkParams->m_nNeighbors,apNetworkParams->m_dbRewiringProb);
		break;
	case NetworkType::CM:
		pNet = make_unique<NetworkCM>(apNetworkParams->m_nNeighbors);
		break;

	default:
		return NULL;
	}

	return pNet;
}

//-------------------------------------------------------------------------------------------------------

void NetworkAbstract::setOpiformModel(const OpinionFormationModel::OpinionFormationModelType & aType) {
	m_OpiformModel = &OpinionFormationModel::getFunction(aType);
}

//-------------------------------------------------------------------------------------------------------

bool NetworkAbstract::isConnected(const std::vector<AgentBase*> * apvecAgents) {

	/*
		A connected network can access any point from any initial point
	*/

	int nSize = apvecAgents->size();

	//Take a random initial node
	std::uniform_int_distribution<> dis(0,nSize-1);
	AgentBase * pA = (*apvecAgents)[dis(gen)];

	queue<AgentBase*> qAgents;
	qAgents.push(pA);
	int nCount = 1;

	vector<int> vecIds(nSize, -1);
	vecIds[pA->getID()] = 1;

	while (!qAgents.empty()) {
		pA = qAgents.front();
		qAgents.pop();

		//Iterate through children
		int nChildren = pA->getNeighborhoodSize();
		for (int nI = 0; nI < nChildren; ++nI) {
			AgentBase * pAC = (AgentBase *)pA->getNeighbor(nI);
			int nIAC = pAC->getID();
			if (vecIds[nIAC] != 1) {
				vecIds[nIAC] = 1;
				qAgents.push(pAC);
				++nCount;
			}
		}
	}

	return (nCount == nSize);
}