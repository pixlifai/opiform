#ifndef AGENT_INCONSISTENT_H
#define AGENT_INCONSISTENT_H

#include "AgentBase.h"

/*
	An inconsistent agent randomly changes personal opinion at each occasion
*/

namespace opiform {

	class AgentInconsistent : public AgentBase {
	public:
		AgentInconsistent(
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread,
			const double & adbOpinion = 0.0,
			const double & adbTreshold = 0.0);
		AgentInconsistent(const AgentInconsistent & rhs);

		virtual ~AgentInconsistent();
		AgentInconsistent & operator=(const AgentInconsistent& rhs);

		virtual double getOpinion() const;
	};
}

#endif