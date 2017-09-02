#ifndef AGENT_ABSTRACT_H
#define AGENT_ABSTRACT_H

#include <ostream>
#include <vector>

#include "../Utils/DecisionMaking.h"

namespace opiform {

	typedef enum {
		Regular = 0,
		Inconsistent,
		Stubborn
	} AgentType;

	class AgentBase {
	public:
		AgentBase(
			const AgentType & aType = AgentType::Regular, 
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread, 
			const double & adbOpinion = 0.0,
			const double & adbTreshold=0.0);
		AgentBase(const AgentBase & rhs);
		virtual ~AgentBase() {}

		AgentBase & operator=(const AgentBase & rhs);

		static void resetCounter() { s_nCounter = 0; }

	private:
		static int s_nCounter;

	protected:
		double m_dbOpinion;
		int m_nID;
		AgentType m_Type;
		double m_dbThreshold;
		double m_dbInitialOpinion;
		DecisionMaking::DecisionMakingType m_DecisionMakingType;

		std::vector<const AgentBase*> m_vecNeighborhood;

	public:

		const AgentType & getType() const { return m_Type; }
		int getID() const { return m_nID; }

		void setOpinion(const double & adbOpinion) { m_dbOpinion = adbOpinion; }
		virtual double getOpinion() const { return m_dbOpinion; }

		virtual void setThreshold(const double & adbThreshold) { m_dbThreshold = adbThreshold; }
		double getThreshold() const { return m_dbThreshold; }

		int getNeighborhoodSize() const { return m_vecNeighborhood.size(); }
		const AgentBase * getNeighbor(int anID);

		bool isMember(const AgentBase * apAgent);

		AgentBase * selectInteractingAgent();
		virtual bool shouldUpdate(const double & adbOpinionAdjacent);

		void addAdjacentAgent(const AgentBase * apAgent);
		void replaceAgent(const AgentBase * apAgentOld, const AgentBase * apAgentNew);
		void removeAgent(const AgentBase * apAgentOld);

		friend std::ostream& operator<< (std::ostream& os, const AgentBase & aAgent) {
			os << aAgent.getOpinion();

			return os;
		};

	};

}

#endif