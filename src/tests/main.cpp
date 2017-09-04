/*
	This section contains some ad-hoc procedures
*/

#include <string>
#include <fstream>

#include "Network/NetworkAbstract.h"

#include "Game/Game.h"

using namespace std;
using namespace opiform;

#include <map>
#include <sstream>
#include <algorithm>

int testAppearance(int argc, char * argv[]) {

	std::ofstream out;
	out.open("FCConv.txt");

	const int c_intAdjFactor = pow(10, 5);

	for (int nI = 0; nI <= 50; ++nI) {
		std::ifstream in;
		in.open("results_" + to_string(nI) + ".txt");
		if (in.is_open() == false)
			continue;

		while (in.eof() == false) {
			string str;
			double nBest[3] = { 0.0, 0.0, 0.0 };
			while (getline(in, str)) {
				std::map<double, int> mapOpinions;
				double dbOpinion;

				char * pchData = &str[0];
				char * pch;

				pch = strtok(pchData, "\t");
				while (pch != NULL)	{
					dbOpinion = stof(pch);
					dbOpinion = floor(dbOpinion*c_intAdjFactor + 0.5) / c_intAdjFactor;
					++mapOpinions[dbOpinion];
					pch = strtok(NULL, "\t");
				}

				int nSize = (mapOpinions.size() > 3) ? mapOpinions.size() : 3;

				std::vector<std::pair<double, int> > lst(nSize, make_pair(0.0, 0));

				map<double, int>::const_iterator cit = mapOpinions.cbegin();
				int nI;
				for (cit, nI = 0; cit != mapOpinions.cend(); ++cit, ++nI)
					lst[nI] = *cit;
				sort(lst.begin(), lst.end(), [](
					std::pair<double, int> a, std::pair<double, int> b)-> bool {
				
					return a.second > b.second;
				});

//				nBest[0] += lst[0].second;
//				nBest[1] += lst[1].second;
//				nBest[2] += lst[2].second;

				out << lst[0].second << "\t";
			}
			out << "\n";

//			nBest[0] = (abs(nBest[0]) < 0.0001) ? 0 : nBest[0] / 50.0;
//			nBest[1] = (abs(nBest[1]) < 0.0001) ? 0 : nBest[1] / 50.0;
//			nBest[2] = (abs(nBest[2]) < 0.0001) ? 0 : nBest[2] / 50.0;

//			out << nBest[0] << '\t' << nBest[1] << '\t' << nBest[2]<< "\n";

		}
		in.close();
	}
	out.close();

	return 0;
}

//---------------------------------------------------------------------------------------------------

int testConvergence(int argc, char * argv[]) {

	vector<int> nvecRes;
	std::ofstream out;
	out.open("res.txt");

	for (int nI = 0; nI <= 50; ++nI) {
		std::ifstream in;
		in.open("results_" + to_string(nI) + ".txt");
		if (in.is_open() == false)
			continue;

		int nRes = 0;
		while (in.eof() == false) {
			string str;
			int nLines = 0;
			int nUniform = 0;
			while (getline(in, str)) {
				++nLines;
				char * pchData = &str[0];
				char * pch;

				bool bTrue = true;

				pch = strtok(pchData, "\t");
				double dbOpinion = stof(pch);
				while (pch != NULL)	{
					double dbT = stof(pch);
					if (abs(dbT - dbOpinion) > 0.000001) {
						bTrue = false;
						break;
					}
					pch = strtok(NULL, "\t");
				}

				if (bTrue) {
					//Stop
//					cout << nI << "\n";
//					in.close();
					++nUniform;
				}
			}
			out << nUniform << "\n";
		}
		in.close();
	}
	out.close();

	return 0;
}

//==================================================================================================

int main(int argc, char * argv[]) {

//	testConvergence(argc, argv);
	testAppearance(argc, argv);

	return 0;
}