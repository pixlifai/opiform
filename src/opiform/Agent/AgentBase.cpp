#include <random>
#include <algorithm>
#include <iostream>

#include <map>

#include "../Utils/DecisionMaking.h"
#include "../Utils/Stats.h"

#include "AgentBase.h"

using namespace std;
using namespace opiform;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}

int AgentBase::s_nCounter = 0;

AgentBase::AgentBase(
	const AgentType & aType,
	unsigned int aunAge,
	const double & adbMu,
	const Opinion::mapOpinionTopicPosition & amapBelieves, const DecisionMaking::DecisionMakingType & aDMType) : 
m_unAge(aunAge), m_dbMu(adbMu),  m_Type(aType), m_DecisionMakingType(aDMType), m_mapBelieves(amapBelieves) {

	m_nID = AgentBase::s_nCounter++;
}

//------------------------------------------------------------------------------------

AgentBase::AgentBase(const AgentBase& rhs) {

	this->m_unAge = rhs.m_unAge;
	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_dbMu = rhs.m_dbMu;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;
	this->m_mapBelieves = rhs.m_mapBelieves;
}

//------------------------------------------------------------------------------------

AgentBase & AgentBase::operator = (const AgentBase& rhs) {
	// check for self-assignment
	if (&rhs == this)
		return *this;

	this->m_unAge = rhs.m_unAge;
	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_dbMu = rhs.m_dbMu;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;
	this->m_mapBelieves = rhs.m_mapBelieves;

	return *this;
}

//------------------------------------------------------------------------------------

void AgentBase::addAdjacentAgent(const AgentBase* apAgent) {
	if (apAgent == this)
		return;

	std::uniform_real_distribution<float> dis;
	m_vecNeighborhood.push_back(apAgent);
	m_vecAdjAgentCredibility.push_back(AgentCredibility(apAgent->getID(),dis(gen)));
}

//------------------------------------------------------------------------------------

void AgentBase::replaceAgent(const AgentBase* apAgentOld, const AgentBase* apAgentNew) {
	if (apAgentOld == 0 || apAgentNew == 0)
		return;

	std::replace(m_vecNeighborhood.begin(), m_vecNeighborhood.end(), apAgentOld, apAgentNew);
}

//------------------------------------------------------------------------------------

void AgentBase::removeAgent(const AgentBase* apAgentOld) {

	auto itAgent = find(m_vecNeighborhood.begin(), m_vecNeighborhood.end(), apAgentOld);
	if (itAgent != m_vecNeighborhood.end())
		m_vecNeighborhood.erase(itAgent);
}

//------------------------------------------------------------------------------------

const AgentBase * AgentBase::getNeighbor(int anID) {
	if (anID < 0 || anID >= m_vecNeighborhood.size())
		return 0;

	return m_vecNeighborhood[anID];
}

//------------------------------------------------------------------------------------

bool AgentBase::isMember(const AgentBase* apAgent) {
	if (apAgent == this)
		return true;

	if (apAgent == 0)
		return false;

	bool bRes = (std::find(m_vecNeighborhood.cbegin(), m_vecNeighborhood.cend(), apAgent) != m_vecNeighborhood.end());
	return bRes;
}

//------------------------------------------------------------------------------------

void AgentBase::updateAdjAgentProfile(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent) {
	if (apAgent == nullptr)
		return;

	std::map<int, std::deque < double > > * pMap = &m_mapAdjacentAgentProfiles[aTopic];
	std::deque < double > * pdeqTemp = &(*pMap)[apAgent->getID()];

#if 0
	pdeqTemp->push_front(aOpinionAdjacent);

	if (pdeqTemp->size() > 5)
		pdeqTemp->pop_back();

#else

	if (pdeqTemp->size() < 3) {
		pdeqTemp->push_front(aOpinionAdjacent);
		return;
	}

	//Keep only first, prior last and new last opinion
	swap((*pdeqTemp)[1], (*pdeqTemp)[2]);
	(*pdeqTemp)[2] = aOpinionAdjacent;

#endif //0
}

//------------------------------------------------------------------------------------

bool AgentBase::isAgentCredible(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const double & adbKappa) {

	if (apAgent == nullptr)
		return false;

	std::map<int, std::deque < double > > * pMap = &m_mapAdjacentAgentProfiles[aTopic];
	std::deque < double > * pdeqTemp = &(*pMap)[apAgent->getID()];

	std::uniform_real_distribution<float> dis(0.f, 1.f);
	//Lacks full information
	if (pdeqTemp->size() < 3) {
		return false;	// CREDIBLE FROM THE START
//		return ((dis(gen) > 0.5f) ? true : false);
	}

	else {
		double dbFirst = (*pdeqTemp)[0];
		double dbBeforeLast = (*pdeqTemp)[1];
		double dbLast = (*pdeqTemp)[2];

		// EQUATION 2 - CREDIBILITY
		double dbRes = (adbKappa*abs(dbLast - dbFirst) + (1.0 - adbKappa)*abs(dbLast - dbBeforeLast));

//		return false;		// KREDIBILNOST
		return ((dbRes < 0.1) ? true : false);		// KREDIBILNOST - theta
//		return ((dbRes < dis(gen)) ? true : false);
	}
}

//------------------------------------------------------------------------------------

AgentBase* AgentBase::selectInteractingAgent(const Opinion::OpinionTopic & aTopic) {

	if (m_vecNeighborhood.size() == 0)
		return nullptr;

	std::uniform_int_distribution<> dis(0, m_vecNeighborhood.size() - 1);
	return (AgentBase*)m_vecNeighborhood[dis(gen)];

#if 0
	//select a neighbor that behaved the most consistently
	auto topicProfiles = m_mapAdjacentAgentProfiles.find(aTopic);
	if (topicProfiles == m_mapAdjacentAgentProfiles.end()) {
		std::uniform_int_distribution<> dis(0, m_vecNeighborhood.size() - 1);
		return (AgentBase*)m_vecNeighborhood[dis(gen)];
	}

	struct AgentVar {
		int nID;
		double dbVar;
	};

	vector<AgentVar> vecVars;
	for (auto const & elAgent : topicProfiles->second) {
		double dbVar = Stats::varKnuth(elAgent.second);
		if (dbVar < 0)
			continue;

		vecVars.push_back({ elAgent.first, dbVar });
	}

	if (vecVars.size() < 5) {
		std::uniform_int_distribution<> dis(0, m_vecNeighborhood.size() - 1);
		return (AgentBase*)m_vecNeighborhood[dis(gen)];
	}
	else {

		//Check for multiple "first-best" and select one of them
		std::sort(vecVars.begin(), vecVars.end(), [](const AgentVar & a, const AgentVar & b)-> bool {
			return a.dbVar < b.dbVar;
		});

		int nI = 1;
		int nSize = vecVars.size();
		double dbT = vecVars[0].dbVar;
		while (nI != nSize && vecVars[nI].dbVar == dbT) {
			++nI;
		}

		//
		int nID = this->getID();
		if (nID == 5) {
			for (int nI = 0; nI < vecVars.size(); ++nI) {
				std::cout << vecVars[nI].dbVar << " ";
			}
			std::cout << endl;
		}

		//

		uniform_real_distribution<double> dis;
		int nId = nI * dis(gen);

		int nBest = vecVars[nId].nID;

		for (const AgentBase * el : m_vecNeighborhood) {
			if (el->getID() == nBest)
				return (AgentBase*)el;
		}
	}

	return nullptr;
#endif //0
}

//------------------------------------------------------------------------------------

bool AgentBase::shouldUpdate(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent) {

	DecisionMaking::DecisionMakingFunc * pFunc =
		&DecisionMaking::getFunction(m_DecisionMakingType);

	Opinion::OpinionPosition opinion = getTopicPosition(aTopic);
	//1. put both opinions in a relation as to the function
	double dbDistance = (*pFunc)(opinion, aOpinionAdjacent);

	uniform_real_distribution<double> dis;

	if (dbDistance < 0.6)			// BOUNDED CONFIDENCE
//	if (dbDistance < dis(gen))
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------------

void AgentBase::setBelieves(const Opinion::mapOpinionTopicPosition & amapBelieves) {
	//Initial believes of an agent are set at random
	m_mapBelieves.swap(Opinion::mapOpinionTopicPosition());
	m_mapBelieves = amapBelieves;
}

//-------------------------------------------------------------------------------------

void AgentBase::updateCredibility(const std::vector<AgentCredibility> & avecAdjAgentCredibility) {
	
	m_vecAdjAgentCredibility = avecAdjAgentCredibility;

	std::sort(
		m_vecAdjAgentCredibility.begin(),
		m_vecAdjAgentCredibility.end(),
		[](const AgentCredibility & el1, const AgentCredibility & el2) {return el1.second > el2.second;}
	);

}