#ifndef NETWORK_ABSTRACT_H
#define NETWORK_ABSTRACT_H

#include <vector>
#include <memory>

#include "../Utils/OpinionFormationModels.h"
#include "../Agent/AgentBase.h"

namespace opiform {

	enum NetworkType {
		SW = 0,		//Small-world (Watts-Strogatz)
		FC,			//Fully-connected
		ER,			//Erdos–Renyi
		BA,			//Barabasi-Albert
		CM			//Communities
	};

	struct NetworkAbstractParams {
		int m_nNeighbors /*= 10*/;
		int m_nConnectedNodes;
		double m_dbConnectionProb;
		double m_dbRewiringProb;
		OpinionFormationModel::OpinionFormationModelType m_OpinionFormationModelType;
	};

	class NetworkAbstract {
	public:
		NetworkAbstract() {}
		virtual ~NetworkAbstract() = 0 {
			int nIndy = -1;
		}

		virtual NetworkType getNetworkType() const = 0;

		virtual void generateNetwork(std::vector<AgentBase *> * apvecAgents) = 0;

		static std::unique_ptr<NetworkAbstract> findAndCreateNetwork(const NetworkType & aNetworkType, NetworkAbstractParams * apNetworkParams = 0);

		virtual bool step(std::vector<AgentBase *> * apvecAgents) = 0;
		void setOpiformModel(const OpinionFormationModel::OpinionFormationModelType & aType);

		virtual bool isConnected(const std::vector<AgentBase*> * apvecAgents);

	protected:
		OpinionFormationModel::OpinionFormationModelFunc * m_OpiformModel;
	};
}

#endif