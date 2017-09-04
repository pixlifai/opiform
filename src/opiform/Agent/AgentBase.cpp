#include <random>
#include <algorithm>

#include "../Utils/DecisionMaking.h"

#include "AgentBase.h"

using namespace std;
using namespace opiform;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}

int AgentBase::s_nCounter = 0;

AgentBase::AgentBase(const AgentType & aType, const DecisionMaking::DecisionMakingType & aDMType, const double & adbOpinion, const double & adbTreshold) : m_Type(aType), m_DecisionMakingType(aDMType), m_dbOpinion(adbOpinion), m_dbThreshold(adbTreshold) {

	m_dbInitialOpinion = adbOpinion;

	m_nID = AgentBase::s_nCounter++;
}

//------------------------------------------------------------------------------------

AgentBase::AgentBase(const AgentBase& rhs) {

	this->m_dbOpinion = rhs.m_dbOpinion;
	this->m_dbThreshold = rhs.m_dbThreshold;
	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_dbInitialOpinion = rhs.m_dbInitialOpinion;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;
}

//------------------------------------------------------------------------------------

AgentBase & AgentBase::operator = (const AgentBase& rhs) {
	// check for self-assignment
	if (&rhs == this)
		return *this;

	this->m_dbOpinion = rhs.m_dbOpinion;
	this->m_dbThreshold = rhs.m_dbThreshold;
	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_dbInitialOpinion = rhs.m_dbInitialOpinion;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;

	return *this;
}

//------------------------------------------------------------------------------------

void AgentBase::addAdjacentAgent(const AgentBase* apAgent) {
	if (apAgent == this)
		return;

	m_vecNeighborhood.push_back(apAgent);
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

AgentBase* AgentBase::selectInteractingAgent() {

	if (m_vecNeighborhood.size() == 0)
		return nullptr;

	std::uniform_int_distribution<> dis(0, m_vecNeighborhood.size()-1);
	int nID = dis(gen);

	return (AgentBase*)m_vecNeighborhood[nID];
}

//------------------------------------------------------------------------------------

bool AgentBase::shouldUpdate(const double & adbOpinionAdjacent) {

	DecisionMaking::DecisionMakingFunc * pFunc =
		&DecisionMaking::getFunction(m_DecisionMakingType);

	//1. put both opinions in a relation as to the function
	double dbDistance = (*pFunc)(m_dbOpinion, adbOpinionAdjacent);

	//2. decide as to the decision rule (threshold)
	if (dbDistance < m_dbThreshold)
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------------
