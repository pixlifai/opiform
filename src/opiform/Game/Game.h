#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <queue>

namespace opiform {

	class Game	{
	private:
		std::vector <class AgentBase * > m_vecAgents;
		std::string m_strFileName;

	public:
		Game(const std::string & astrFileName);
		~Game();

		bool init(int anSize = 200);
		bool initNetwork(const NetworkType & aNetworkType, struct NetworkAbstractParams * apNetworkParams = 0);
		void runGame(const time_t & aTime, const double & adbKappa);

		static void registerStatics();

		int getAgents() const;

	private:
		void initAgents(int anSize);
		std::queue<int> listDeceased();

		void updateRanking();
	};
}

#endif
