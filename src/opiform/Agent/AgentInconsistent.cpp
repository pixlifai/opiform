#include <random>

#include "AgentInconsistent.h"

using namespace opiform;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}


AgentInconsistent::AgentInconsistent(const DecisionMaking::DecisionMakingType & aDMType, const double & adbOpinion, const double & adbTreshold) : AgentBase(AgentType::Inconsistent, aDMType, adbOpinion, adbTreshold) {
}

//------------------------------------------------------------------------------------------

AgentInconsistent::AgentInconsistent(const AgentInconsistent & rhs) : AgentBase(rhs) {
}

//------------------------------------------------------------------------------------------

AgentInconsistent::~AgentInconsistent() {
}

//------------------------------------------------------------------------------------------

AgentInconsistent& AgentInconsistent::operator=(const AgentInconsistent& rhs) {
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

//------------------------------------------------------------------------------------------

double AgentInconsistent::getOpinion() const { 
	std::uniform_real_distribution<> dis;
	((AgentInconsistent *) this)->m_dbOpinion = dis(gen);

	return m_dbOpinion;
}

void AgentInconsistent::setThreshold(const double &) { }

bool AgentInconsistent::shouldUpdate(const double & adbOpinionAdjacent) { 
	return false; 
};