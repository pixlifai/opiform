#ifndef OPIFORM_DECISION_MAKING_H
#define OPIFORM_DECISION_MAKING_H

#include <map>

namespace opiform {

	class DecisionMaking {
	public:
		typedef double(*DecisionMakingFunc)(const double &, const double &);

		enum DecisionMakingType {
			InvExp = 0,
			Logistic,
			Sigmoid,
			Tanh,
			LinearSpread
		};

		typedef std::map<DecisionMakingType, DecisionMakingFunc> DecisionMakingFuncMap;

		static void registerType(const DecisionMakingType & anType, const DecisionMakingFunc & aFunc) {
			s_mapDecisionMakingFuncMap[anType] = aFunc;
		}

		static DecisionMakingFunc & getFunction(const DecisionMakingType & aFunctionType) {
			return s_mapDecisionMakingFuncMap[aFunctionType];
		}

		static const int size() { return s_mapDecisionMakingFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

		template < class T, class Return = double>
		static inline Return linearSpreadFunction(const T & aTOld, const T & aTNew) {

			return (Return)abs(aTNew - aTOld);

		}

		////------------------------------------------------------------------------------------------------

		template < class T, class Return = double>
		static inline Return tanhFunction(const T & aTOld, const T & aTNew)	{
			T TDiff = abs(aTNew - aTOld);

			Return dbT = exp(-2.f * (Return)TDiff);

			return (Return)((1.f - dbT) / (1.f + dbT));
		};

		////-----------------------------------------------------------------------------------------

		template < class T, class Return = double>
		static inline Return logisticFunction(const T & aTOld, const T & aTNew) {
			T TDiff = abs(aTNew - aTOld);

			return (Return)(TDiff / (1.f + TDiff));
		}

		////------------------------------------------------------------------------------------------

		template < class T, class Return = double>
		static inline Return invExpFunction(const T & aTOld, const T & aTNew) {
			T TDiff = abs(aTNew - aTOld);

			return (Return)(1.f - exp(-TDiff));
		}

		////----------------------------------------------------------------------------------------------------------------------

		template < class T, class Return = double>
		static inline Return sigmoidFunction(const T & aTOld, const T & aTNew) {
			T TDiff = abs(aTNew - aTOld);

			return (Return)(1.f / (1.f + exp(-TDiff)));
		}


	private:
		static DecisionMakingFuncMap s_mapDecisionMakingFuncMap;

	};
}

#endif