#ifndef WRITER_H
#define WRITER_H

#include <vector>
#include <memory>

namespace opiform {

	class Utils {
	public:
		static void writeAgents(const std::vector<class AgentBase*> & avecAgent, const char * apchFile, const char * apchDelim = "\t");

		static void writeNetwork(const std::vector<class AgentBase*> & avecAgents, const char * apchFile);
	};
}

#endif