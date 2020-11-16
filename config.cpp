#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "config.h"

namespace Config {
	int note_next = -1;
	int note_prev = -1;
	int note_select = -1;

	std::string portName;

	void parse(nlohmann::json j) {
		note_next = j["next"];
		note_prev = j["prev"];
		note_select = j["select"];
		portName = j["portName"];
	}

	void parseFile(std::string fileName) {
		std::cout << "Reading config file \"" << fileName << "\"" << std::endl;
		std::fstream configStream;
		configStream.open(fileName, std::fstream::in);

		nlohmann::json j;
		configStream >> j;
		configStream.close();

		return parse(j);
	}
}
