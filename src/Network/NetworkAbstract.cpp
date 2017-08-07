#include <memory>

#include "NetworkFC.h"
#include "NetworkBA.h"
#include "NetworkER.h"
#include "NetworkSW.h"
#include "NetworkCM.h"

#include "NetworkAbstract.h"

using namespace opiform;
using namespace std;

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