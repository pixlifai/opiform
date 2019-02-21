#include <random>

#include "RankingModel.h"

using namespace opiform;

namespace {
	std::random_device rd;

	std::mt19937 gen(
#if RANDOM_SEED
		rd()		
#else
		10
#endif //RANDOM_SEED
		);

}

RankingModel::RankingModelFuncMap RankingModel::s_mapRankingModelFuncMap = opiform::RankingModel::RankingModelFuncMap();

//----------------------------------------------------------------------------------------------------------

void RankingModel::wienerProcessFunction(std::vector<AgentCredibility> * apAgentData) {
	std::normal_distribution<double> dis(0.0,1.0);
	const float flsqrtT = sqrt(1.f/365.f);
	for (auto & el : (*apAgentData)) {
		el.second += (dis(gen)*flsqrtT); //Wiener process
	}
}

//----------------------------------------------------------------------------------------------------------

void RankingModel::generalizedWienerProcessFunction(std::vector<AgentCredibility> * apAgentData) {
	std::normal_distribution<double> dis(0.0,1.0);
	const float flsqrtT = sqrt(1.f/365.f);
	for (auto & el : (*apAgentData)) {
		el.second += (0.5 * (1.f/365.f) + 0.3*(dis(gen)*flsqrtT)); //generalized Wiener process
	}
}

//----------------------------------------------------------------------------------------------------------

void RankingModel::moscariniProcessFunction(std::vector<AgentCredibility> * apAgentData) {

	float flMuH, flMuL, flSigma, flDXt, flPt, flDt = 1.f/365.f;

	float s = (flMuH - flMuL) / flSigma;
	float dZt = (1.0 / flSigma) * (flDXt - flPt*flMuH*flDt - (1.0 - flPt)*flMuL*flDt);

	auto dpt = [&]() {
		float pt = pt * (1.f - pt) * s * dZt;
		return pt;
	};

}