#ifndef OPIFORM_OPINION_FORMATION_MODELS_H
#define OPIFORM_OPINION_FORMATION_MODELS_H

#include <map>

#include "../Agent/AgentBase.h"

namespace opiform {

	class OpinionFormationModel {
	public:
		typedef int(*OpinionFormationModelFunc)(AgentBase * apAgent);

		enum OpinionFormationModelType {
			DW = 0,			//The Deffuant-Weisbuch model
			HK				//The Hegselmann-Krause model
		};

		typedef std::map<OpinionFormationModelType, OpinionFormationModelFunc> OpinionFormationModelFuncMap;

		static void registerType(const OpinionFormationModelType & anType, const OpinionFormationModelFunc & aFunc) {
			s_mapOpinionFormationModelFuncMap[anType] = aFunc;
		}

		static OpinionFormationModelFunc & getFunction(const OpinionFormationModelType & aFunctionType) {
			return s_mapOpinionFormationModelFuncMap[aFunctionType];
		}

		static const int size() { return s_mapOpinionFormationModelFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

		static inline int DWFunction(AgentBase* apAgent) {

			AgentBase* pAgentAdjacent = nullptr;

			pAgentAdjacent = apAgent->selectInteractingAgent();

			if (pAgentAdjacent == nullptr)
				return 0;

			//Store opinions before a change
			double dbOpinionAgent = apAgent->getOpinion();
			double dbOpinionAdjacent = pAgentAdjacent->getOpinion();

			double dbMu = 0.2;

			if (apAgent->shouldUpdate(dbOpinionAdjacent))
				apAgent->setOpinion(dbOpinionAgent + dbMu*(dbOpinionAdjacent - dbOpinionAgent));

			if (pAgentAdjacent->shouldUpdate(dbOpinionAgent))
				pAgentAdjacent->setOpinion(dbOpinionAdjacent + dbMu*(dbOpinionAgent - dbOpinionAdjacent));

			return 1;
		}

		//------------------------------------------------------------------------------------------------

		static inline int HKFunction(AgentBase* apAgent)	{
			//HEGSELMANN-KRAUSE MODEL
			/*
			https://arxiv.org/pdf/0707.1762.pdf
			*/
			double dbOpinion = 0.0;
			int nCounter = 0;
			for (int nI = 0; nI < apAgent->getNeighborhoodSize(); ++nI) {
				double dbT = apAgent->getNeighbor(nI)->getOpinion();
				if (abs(dbT - apAgent->getOpinion()) < apAgent->getThreshold()) {
					dbOpinion += dbT;
					++nCounter;
				}
			}

			if (nCounter == 0)
				return 0;
			dbOpinion /= (double)nCounter;
			double dbOpinionAgent = apAgent->getOpinion();

			double dbMu = 0.3;

			apAgent->setOpinion(dbOpinionAgent + dbMu*(dbOpinion - dbOpinionAgent));

			return 1;
		};

		//-----------------------------------------------------------------------------------------


	private:
		static OpinionFormationModelFuncMap s_mapOpinionFormationModelFuncMap;

	};
}

#endif