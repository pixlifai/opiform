#ifndef NETWORK_FC_H
#define NETWORK_FC_H

#include "NetworkAbstract.h"

namespace opiform {

	class NetworkFC : public NetworkAbstract {
	public:
		NetworkFC();
		virtual ~NetworkFC();

		virtual NetworkType getNetworkType() const { return NetworkType::FC; };

		virtual void generateNetwork(std::vector<AgentBase *> * apvecAgents);

		virtual bool step();
	};
}

#endif