#include <random>

#include "../Utils/Writer.h"
#include "../Agent/AgentBase.h"
#include "../Agent/AgentStubborn.h"
#include "../Agent/AgentInconsistent.h"

#include "../Network/NetworkAbstract.h"

#include "Game.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;

	std::mt19937 gen(
#if RANDOM_SEED
		rd()		
#else
		10
#endif //RANDOM_SEED
		);
}

Game::Game(const char * apchFileName) : m_pchFile(apchFileName) {

}

//--------------------------------------------------------------------------------------

Game::~Game() {
	std::vector<AgentBase*>::iterator it = m_vecAgents.begin();
	while (it != m_vecAgents.end()) {
		delete *it;
		(*it) = NULL;
		++it;
	}
}

//--------------------------------------------------------------------------------------

void Game::registerStatics() {
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::LinearSpread, DecisionMaking::linearSpreadFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::Tanh, DecisionMaking::tanhFunction);

	OpinionFormationModel::registerType(OpinionFormationModel::OpinionFormationModelType::DW, OpinionFormationModel::DWFunction);
	OpinionFormationModel::registerType(OpinionFormationModel::OpinionFormationModelType::HK, OpinionFormationModel::HKFunction);
}

//--------------------------------------------------------------------------------------

bool Game::init(const double & adbThresholdLevel, int anSize) {

	gen.seed(10);
	AgentBase::resetCounter();

	m_vecAgents.reserve(anSize);

	std::uniform_real_distribution<> dis;
	std::uniform_real_distribution<double> ddis(0.35, 0.45);

	for (int nI = 0; nI < anSize; ++nI) {
		double dbValue = dis(gen);
		AgentBase * pA = new AgentBase(AgentType::Regular, DecisionMaking::DecisionMakingType::LinearSpread, dbValue, adbThresholdLevel);

		m_vecAgents.push_back(pA);
	}

#if 0
	//Add stubborn agents
	for (int nI = 0; nI < 20; ++nI) {
		double dbValue = 0.0;
		AgentStubborn * pA = new AgentStubborn(DecisionMaking::DecisionMakingType::LinearSpread, dbValue);
		m_vecAgents.push_back(pA);
	}

#endif//0

//	Utils::writeAgents(m_vecAgents, m_pchFile, "\t");

	return true;
}

//--------------------------------------------------------------------------------------

bool Game::initNetwork(const NetworkType & aNetworkType, NetworkAbstractParams * apNetworkParams) {

	m_pNet = NetworkAbstract::findAndCreateNetwork(aNetworkType, apNetworkParams);

	if (m_pNet == nullptr)
		return false;
	
	m_pNet->generateNetwork(&m_vecAgents);
	Utils::writeNetwork(m_vecAgents, "Network.txt");
	m_pNet->setOpiformModel(apNetworkParams->m_OpinionFormationModelType);

	return true;
}

//--------------------------------------------------------------------------------------

int Game::getAgents() const {
	return m_vecAgents.size();
}

//--------------------------------------------------------------------------------------

void Game::runGame(const time_t & aTime) {

	//Run the process
	for (int nTime = 0; nTime < aTime; ++nTime) {
		bool bRes = m_pNet->step();
	}
	Utils::writeAgents(m_vecAgents, m_pchFile);

}

//---------------------------------------------------------------------------------------------------------------
