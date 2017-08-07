#ifndef AGENT_STUBBORN_H
#define AGENT_STUBBORN_H

#include "AgentBase.h"

namespace opiform {

	class AgentStubborn : public AgentBase {
	public:
		AgentStubborn(
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread, 
			const double & adbOpinion = 0.0);
//		AgentStubborn(const AgentStubborn & rhs);

		virtual ~AgentStubborn();
//		AgentStubborn & operator=(const AgentStubborn & rhs);

		virtual void setThreshold(const double & ) { }
		virtual bool shouldUpdate(const double & adbOpinionAdjacent) override;
	};
}

#endif