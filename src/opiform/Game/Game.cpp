#include <random>
#include <fstream>
#include <sstream>
#include <queue>

#include "../Utils/PoliticalView.h"

#include "../Utils/Writer.h"
#include "../Agent/AgentBase.h"
#include "../Agent/AgentStubborn.h"
#include "../Agent/AgentInconsistent.h"

#include "../Utils/RankingModel.h"
#include "../Utils/OpinionUpdating.h"
#include "../Utils/Pairing.h"
#include "../Utils/Stats.h"

#include "../Network/NetworkAbstract.h"
#include "ProcessDynamicsBase.h"

#include "../Utils/constants.h"

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

#define AGE 0;

Game::Game(const std::string & astrFileName) : m_strFileName(astrFileName) {
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
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::Logistic, DecisionMaking::logisticFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::InvExp, DecisionMaking::invExpFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::FermiDirac, DecisionMaking::fermiDiracFunction);

	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::DW, OpinionUpdating::DWFunction);
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::kineticFP1, OpinionUpdating::kineticFP1Function);
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::kineticFP2, OpinionUpdating::kineticFP2Function);
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::kineticFP3, OpinionUpdating::kineticFP3Function);

	RankingModel::registerType(RankingModel::RankingModelType::WienerProcess, RankingModel::wienerProcessFunction);
	RankingModel::registerType(RankingModel::RankingModelType::GeneralizedWienerProcess, RankingModel::generalizedWienerProcessFunction);
	RankingModel::registerType(RankingModel::RankingModelType::Moscarini, RankingModel::moscariniProcessFunction);

	Opinion::registerType(Opinion::Euclidian, Opinion::euclidianDist);
	Opinion::registerType(Opinion::Jensen, Opinion::jensenDist);

}

//--------------------------------------------------------------------------------------

bool Game::init(int anSize) {

	gen.seed(10);

	AgentBase::resetCounter();

	//Init agents
	initAgents(anSize);

	Utils::writeAgents(m_vecAgents, Utils::getFolder() + "agents_init.txt");

	return true;
}

//--------------------------------------------------------------------------------------

bool Game::initNetwork(const NetworkType & aNetworkType, NetworkAbstractParams * apNetworkParams) {

	NetworkAbstract * pNet = NetworkAbstract::findAndCreateNetwork(aNetworkType, apNetworkParams);

	if (pNet == nullptr)
		return false;

	bool bRes = pNet->generateNetwork(&m_vecAgents);
	if (bRes == false)
		return false;

	Utils::writeNetwork(m_vecAgents, Utils::getFolder() + "Network.txt");
	pNet->setOpiformModel(apNetworkParams->m_OpinionFormationModelType);

	bRes = NetworkAbstract::isConnected(&m_vecAgents);
	delete pNet;

	return true;
}

//--------------------------------------------------------------------------------------

int Game::getAgents() const {
	return m_vecAgents.size();
}

//--------------------------------------------------------------------------------------

void Game::runGame(const time_t & aTime, const double & adbKappa) {

/*	
	std::ofstream out;
	out.open("./allData.txt");
	for (const auto & agent : m_vecAgents) {
		out << agent->getTopicPosition(Opinion::OpinionTopic_0) << "\t";
	}
*/
	opiform::Pairing * pPairing = opiform::Pairing::create(opiform::Pairing::Irving);
	opiform::OpinionUpdating::OpinionUpdatingFunc * pOpinionUpdating = 
		&opiform::OpinionUpdating::getFunction(opiform::OpinionUpdating::DW);

//	const ProcessDynamicsType ndt = ProcessDynamicsType::Reset;
	ProcessDynamics<ProcessDynamicsType::Continue> processDynamics(&m_vecAgents);

	for (int nTime=0;nTime<aTime;++nTime) {

#if AGE
		std::queue<int> qDeceased = listDeceased();
		processDynamics.update(qDeceased);

#endif //AGE

		//Generate pairs
		Pairing::Pairs vecPairs;
		bool bRes = pPairing->run(&m_vecAgents, vecPairs);

		Opinion::OpinionTopic t = Opinion::getTopic();

		//Go through all pairs
		for (auto & p : vecPairs) {
			AgentBase * pAgent = m_vecAgents[p.first];
			AgentBase * pAgentAdjacent = m_vecAgents[p.second];

			//Store opinions before a change
			Opinion::OpinionPosition opinionAgent = pAgent->getTopicPosition(t);
			Opinion::OpinionPosition opinionAdjacent = pAgentAdjacent->getTopicPosition(t);

			pAgent->updateAdjAgentProfile(pAgentAdjacent, t, opinionAdjacent);
			pAgentAdjacent->updateAdjAgentProfile(pAgent, t, opinionAgent);

			if (pAgent->isAgentCredible(pAgentAdjacent, t, adbKappa) && pAgent->shouldUpdate(t, opinionAdjacent)) {
				Opinion::OpinionPosition op = (*pOpinionUpdating)(opinionAgent, opinionAdjacent);
				pAgent->setOpinionTopicPosition(t, op);
			}

			if (pAgentAdjacent->isAgentCredible(pAgent, t, adbKappa) && pAgentAdjacent->shouldUpdate(t, opinionAgent)) {
				Opinion::OpinionPosition op = (*pOpinionUpdating)(opinionAdjacent, opinionAgent);
				pAgentAdjacent->setOpinionTopicPosition(t, op);
			}
		}

		//Update the data
		updateRanking();

#if AGE
		//Update age
		for (auto & agent : m_vecAgents) {
			agent->setAge(agent->getAge()+1);
		}
#endif //AGE

#if 0
		//Write
		if (nTime % 100000 == 0) {
			string strName = "results_SB_" + to_string(nTime) + ".txt";
			Utils::writeAgents(m_vecAgents, Utils::getFolder() + strName);
		}
#endif //0

/*		out << "\n";
		for (const auto & agent : m_vecAgents) {
			out << agent->getTopicPosition(Opinion::OpinionTopic_0) << "\t";
		}
*/	}

//	out.close();

	delete pPairing;

	//Comment-out for registering last iteration results
	Utils::writeAgents(m_vecAgents, Utils::getFolder() + "finalLine.txt");
}

//---------------------------------------------------------------------------------------------------------------

void Game::initAgents(int anSize) {
	m_vecAgents.reserve(anSize);

	int nTopics = Opinion::getNumTopics();

	std::uniform_real_distribution<double> dis(0.0,1.0);
	std::uniform_int_distribution<int> disAge(mapLifeTable.cbegin()->first, 80 * 12);

	for (int nI = 0; nI < anSize; ++nI) {
		//Set initial topic positions
		Opinion::mapOpinionTopicPosition mapBelieves;

		for (int nT = 0; nT < nTopics; ++nT) {
			mapBelieves[(Opinion::OpinionTopic)nT] = (Opinion::OpinionPosition)dis(gen);
		}

		int nAge = disAge(gen);
		AgentBase * pA = new AgentBase(AgentType::Regular, nAge,0.8, mapBelieves, DecisionMaking::DecisionMakingType::LinearSpread);

		m_vecAgents.push_back(pA);
	}
}

//------------------------------------------------------------------------------------------------------

std::queue<int> Game::listDeceased() {
	//Check deceased
	std::queue<int> qDeceasedIDs;
	int nSize = m_vecAgents.size();
	uniform_real_distribution<double> dis;

	for (int nI=0;nI<nSize; ++nI) {
		AgentBase * pAgent = m_vecAgents[nI];

		//Find appropriate age-group probability from the Table
		int nAge = pAgent->getAge();
		auto it = mapLifeTable.upper_bound(nAge);

		const double * pdbProb = &((--it)->second);

		if (dis(gen) < *pdbProb) {
			qDeceasedIDs.push(nI);
		}
	}

	return qDeceasedIDs;
}

//------------------------------------------------------------------------------------------------------

void Game::updateRanking() {

	const opiform::RankingModel::RankingModelFunc * pRankingModelFunc = 
		&opiform::RankingModel::getFunction(opiform::RankingModel::RankingModelType::GeneralizedWienerProcess);

	for (auto * pAgent : m_vecAgents) {
		std::vector<AgentCredibility> * vecAdjAgentCredibility = pAgent->getAdjAgentCredibility();
		(*pRankingModelFunc)(vecAdjAgentCredibility);
		pAgent->updateCredibility(*vecAdjAgentCredibility);
	}
}