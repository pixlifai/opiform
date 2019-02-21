#ifndef AGENT_ABSTRACT_H
#define AGENT_ABSTRACT_H

#include <ostream>
#include <vector>
#include <map>
#include <deque>

#include "../Utils/types.h"
#include "../Utils/DecisionMaking.h"
#include "../Utils/Opinion.h"

namespace opiform {

	typedef enum {
		Regular = 0,
		Inconsistent,
		Stubborn
	} AgentType;

	typedef std::map <Opinion::OpinionTopic, std::map<int /*AdjAgentID*/, std::deque < double > > > AdjacentAgentProfile;

	class AgentBase {
	public:
		AgentBase(
			const AgentType & aType,
			unsigned int aunAge,
			const double & adbMu = 0.2,
			const Opinion::mapOpinionTopicPosition & amapBelieves = Opinion::mapOpinionTopicPosition(),
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread);
		AgentBase(const AgentBase & rhs);
		virtual ~AgentBase() {}

		AgentBase & operator=(const AgentBase & rhs);

		static void resetCounter() { s_nCounter = 0; }
		
		inline void resetAdjacentAgentProfiles() { std::swap(m_mapAdjacentAgentProfiles,opiform::AdjacentAgentProfile()); }

	private:
		static int s_nCounter;

	protected:
		unsigned int m_unAge;
		int m_nID;
		AgentType m_Type;
		DecisionMaking::DecisionMakingType m_DecisionMakingType;

		std::vector<const AgentBase*> m_vecNeighborhood;
		double m_dbMu;

		AdjacentAgentProfile m_mapAdjacentAgentProfiles;
		std::vector<AgentCredibility> m_vecAdjAgentCredibility;


	public:

		const AgentType & getType() const { return m_Type; }
		int getID() const { return m_nID; }

		int getNeighborhoodSize() const { return m_vecNeighborhood.size(); }
		const AgentBase * getNeighbor(int anID);

		bool isMember(const AgentBase * apAgent);

		virtual void updateAdjAgentProfile(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent);

		std::vector<AgentCredibility> * getAdjAgentCredibility() {return &m_vecAdjAgentCredibility;}
		void updateCredibility(const std::vector<AgentCredibility> & avecAdjAgentCredibility);

		bool isAgentCredible(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const double & adbKappa);

		virtual AgentBase * selectInteractingAgent(const Opinion::OpinionTopic & aTopic);
		virtual bool shouldUpdate(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent);

		void addAdjacentAgent(const AgentBase * apAgent);
		void replaceAgent(const AgentBase * apAgentOld, const AgentBase * apAgentNew);
		void removeAgent(const AgentBase * apAgentOld);

		void setAge(const unsigned int & aunAge) { m_unAge = aunAge; }
		int getAge() const { return m_unAge; }


		Opinion::mapOpinionTopicPosition m_mapBelieves; //Position on particular topic
		void setOpinionTopicPosition(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aPosition) {
			m_mapBelieves[aTopic] = aPosition;
		}
		Opinion::mapOpinionTopicPosition * getBelieves() { return &m_mapBelieves; }
		void setBelieves(const Opinion::mapOpinionTopicPosition & amapBelieves);
		inline Opinion::OpinionPosition getTopicPosition(const Opinion::OpinionTopic & aTopic) { return m_mapBelieves[aTopic]; };

		inline double getMu() const { return m_dbMu; }


		friend std::ostream& operator<< (std::ostream& os, const AgentBase & aAgent) {

			os << aAgent.getAge() << '\t';
			const Opinion::mapOpinionTopicPosition * pmapOTP = ((AgentBase *)&aAgent)->getBelieves();

			Opinion::mapOpinionTopicPosition::const_iterator cit = pmapOTP->cbegin(), citEnd = --pmapOTP->cend();
			for (cit; cit != citEnd; ++cit) {
				os << cit->second << '\t';
			}
			os << citEnd->second;

			return os;
		};

	};

}

#endif