#ifndef NETWORK_CM_H
#define NETWORK_CM_H

#include "NetworkAbstract.h"

#include <vector>

namespace opiform {

	class NetworkCM : public NetworkAbstract {
	public:
		NetworkCM(int anGroupSize = 10);
		virtual ~NetworkCM();

		virtual NetworkType getNetworkType() const { return NetworkType::CM; };

		virtual void generateNetwork(std::vector<AgentBase *> * apvecAgents);

		virtual bool step();

	private:
		int m_nGroupSize;
	};
}

#endif