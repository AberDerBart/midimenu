#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "menuParser.h"

namespace Menu {

	MenuEntry parseJsonEntry(nlohmann::json j){
		MenuEntry entry;
		entry.title = j["title"];
		entry.cmd = j["cmd"];
		return entry;
	}

	Menu parseJson(nlohmann::json j) {
		Menu menu;

		menu.renderMenu = NULL;

		for (auto it = j.begin(); it != j.end(); it++) {
			menu.entries.push_back(parseJsonEntry(*it));
		}
		
		return menu;
	}

	Menu parseFile(std::string fileName) {
		std::cout << "Reading menu file \"" << fileName << "\"" << std::endl;
		std::fstream configStream;
		configStream.open(fileName, std::fstream::in);

		nlohmann::json j;
		configStream >> j;
		configStream.close();

		return parseJson(j);
	}
}	
