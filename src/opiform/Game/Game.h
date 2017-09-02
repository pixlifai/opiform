#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>

namespace opiform {

	enum NetworkType;

	class Game	{
	private:
		std::vector <AgentBase * > m_vecAgents;
		const char * m_pchFile;
		std::unique_ptr< NetworkAbstract > m_pNet;

	public:
		Game(const char * apchFileName);
		~Game();

		bool init(const double & adbThresholdLevel, int anSize = 200);
		bool initNetwork(const NetworkType & aNetworkType, struct NetworkAbstractParams * apNetworkParams = 0);
		void runGame(const time_t & aTime = 100000);

		static void registerStatics();

		int getAgents() const;
	};
}

#endif
