#include <random>

#include "AgentInconsistent.h"

using namespace opiform;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}


AgentInconsistent::AgentInconsistent(unsigned int anAge, const double& adbMu, const Opinion::mapOpinionTopicPosition & amapBelieves, const DecisionMaking::DecisionMakingType & aDMType) : AgentBase(AgentType::Inconsistent, anAge, adbMu, amapBelieves, aDMType) {
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

	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_dbMu = rhs.m_dbMu;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;

	return *this;
}

//------------------------------------------------------------------------------------------

bool AgentInconsistent::shouldUpdate(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent)	{
	return false; 
}