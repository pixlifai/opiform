#ifndef OPIFORM_RANKING_MODEL_H
#define OPIFORM_RANKING_MODEL_H

#include "types.h"

namespace opiform {

	class RankingModel {
	public:
		typedef void(*RankingModelFunc)(std::vector<AgentCredibility> *);

		enum RankingModelType {
			WienerProcess = 0,
			GeneralizedWienerProcess,
			Moscarini
		};

		typedef std::map<RankingModelType, RankingModelFunc> RankingModelFuncMap;

		static void registerType(const RankingModelType & anType, const RankingModelFunc & aFunc) {
			s_mapRankingModelFuncMap[anType] = aFunc;
		}

		static RankingModelFunc & getFunction(const RankingModelType & aFunctionType) {
			return s_mapRankingModelFuncMap[aFunctionType];
		}

		static const int size() { return s_mapRankingModelFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

		static void wienerProcessFunction(std::vector<AgentCredibility> * apAgentData);

		//------------------------------------------------------------------------------------------------

		static void generalizedWienerProcessFunction(std::vector<AgentCredibility> * apAgentData);

		//----------------------------------------------------------------------------------------------------------------------

		static void moscariniProcessFunction(std::vector<AgentCredibility> * apAgentData);

		//----------------------------------------------------------------------------------------------------------------------

	private:
		static RankingModelFuncMap s_mapRankingModelFuncMap;

	};
}

#endif