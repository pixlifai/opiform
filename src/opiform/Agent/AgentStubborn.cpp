#include "AgentStubborn.h"

using namespace opiform;

AgentStubborn::AgentStubborn(const DecisionMaking::DecisionMakingType & aDMType, const double & adbOpinion) : AgentBase(AgentType::Stubborn, aDMType, adbOpinion, 1.0) {
}

//------------------------------------------------------------------------------------------

AgentStubborn::AgentStubborn(const AgentStubborn & rhs)	{
	this->m_dbOpinion = rhs.m_dbOpinion;
	this->m_dbThreshold = rhs.m_dbThreshold;
	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_dbInitialOpinion = rhs.m_dbInitialOpinion;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;
}

//------------------------------------------------------------------------------------------

AgentStubborn::~AgentStubborn() {
}

//------------------------------------------------------------------------------------------

AgentStubborn & AgentStubborn::operator=(const AgentStubborn & rhs) {
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

bool AgentStubborn::shouldUpdate(const double & adbOpinionAdjacent) {
	return false;
}
