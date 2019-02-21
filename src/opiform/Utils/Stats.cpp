#include <vector>
#include <algorithm>

#include "Opinion.h"
#include "../Agent/AgentBase.h"

#include "Stats.h"

using namespace opiform;
using namespace std;

bool Stats::analyze(std::vector<AgentBase*> & avecAgents, std::vector<Statistics> & avecStatsOut) {
	int nSizeTopics = Opinion::getNumTopics();
	avecStatsOut.reserve(nSizeTopics);

	vector<double> vecdbMean(nSizeTopics, 0.0);
	vector<double> vecdbM2(nSizeTopics, 0.0);
	vector < map<int, int> > vecmapCounts(nSizeTopics);

	int nSize = 0;
	for (AgentBase * el : avecAgents) {
		if (el->getType() != Regular)
			continue;

		Opinion::mapOpinionTopicPosition * pBelieves = el->getBelieves();

		++nSize;
		Opinion::mapOpinionTopicPosition::const_iterator cit = pBelieves->cbegin(), citEnd = pBelieves->cend();
		for (int nI = 0; cit != citEnd; ++nI, ++cit) {
			//Var Knuth - one-pass
			double n = (double)cit->second;
			double dbDelta = n - vecdbMean[nI];
			vecdbMean[nI] += dbDelta / (double)nSize;
			vecdbM2[nI] += dbDelta*(n - vecdbMean[nI]);

			map<int, int> * pM = &vecmapCounts[nI];
			++(*pM)[cit->second];
		}
	}

	//Var Knuth
	std::transform(vecdbM2.begin(), vecdbM2.end(), vecdbM2.begin(),
		[&](double & adbEl) -> double { return adbEl / (double)(nSize - 1); });

	for (int nI = 0; nI < nSizeTopics; ++nI) {

		Statistics s;
		s.m_dbMean = vecdbMean[nI];
		s.m_dbVar= vecdbM2[nI];
		avecStatsOut.push_back(s);
	}
	return true;
}

//-----------------------------------------------------------------------------------------------

double Stats::covar(const Opinion::mapOpinionTopicPosition & aOTP1, const Opinion::mapOpinionTopicPosition & aOTP2) {
	if (aOTP1.size() != aOTP2.size())
		return -1;

	double dbMeanx = 0.0;
	double dbMeany = 0.0;
	double dbC = 0.0;
	int nCount = 0.0;

	for (auto X = aOTP1.cbegin(), Y = aOTP2.cbegin(); X != aOTP1.cend(); ++X, ++Y) {
		++nCount;
		double dx = X->second - dbMeanx;
		dbMeanx += dx / (double)nCount;
		dbMeany += (Y->second - dbMeany) / (double)nCount;
		dbC += dx * (Y->second - dbMeany);
	}

	double population_covar = dbC / nCount;
	double sample_covar = dbC / (nCount - 1);

	return sample_covar;
}

//------------------------------------------------------------------------------------------------------

double Stats::varKnuth(const std::deque<double> & aData) {

	if (aData.size() < 2)
		return -1.0;

	int nCount = 0;
	double dbMean = 0.0;
	double dbM2 = 0.0;

	for (const auto & el : aData) {
		++nCount;
		double dbDelta = el - dbMean;
		dbMean += dbDelta / nCount;
		double dbDelta2 = el - dbMean;
		dbM2 += dbDelta*dbDelta2;
	}

	return dbM2 / (nCount - 1);
}