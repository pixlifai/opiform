#ifndef WRITER_H
#define WRITER_H

#include <vector>
#include <memory>

#include <string>

namespace opiform {

	class Utils {

		static std::string s_strFolder;

	public:

		static void setFolder(const std::string & astrFolder);

		static std::string getFolder();

		static void writeAgents(const std::vector<class AgentBase*> & avecAgent, const std::string & astdFile, const char * apchDelim = "\t");

		static void writeNetwork(const std::vector<class AgentBase*> & avecAgents, const std::string & astdFile);
	};
}

#endif