#pragma once
#include <string>

namespace Config {
	extern int note_next;
	extern int note_prev;
	extern int note_select;

	extern std::string portName;

	void parseFile(std::string fileName);
}
