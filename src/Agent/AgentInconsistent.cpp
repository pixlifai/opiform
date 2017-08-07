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

	AgentInconsistent::operator=(rhs);

	return *this;
}

//------------------------------------------------------------------------------------------

double AgentInconsistent::getOpinion() const { 
	std::uniform_real_distribution<> dis;
	((AgentInconsistent *) this)->m_dbOpinion = dis(gen);

	return m_dbOpinion;
}