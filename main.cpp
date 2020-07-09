#include <functional>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "menuParser.h"
#include "config.h"
#include "midi.h"
#include "version.h"
#include "renderMenu.h"
#include "control.h"

void callback(double deltatime, std::vector<unsigned char>* msg, void* userData) {
	Menu::Menu* menu = (Menu::Menu*) userData;
	handleMidiCommand(deltatime, msg, menu);
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "usage:" << std::endl;
		std::cerr << argv[0] << " [configfile] [menufile]" << std::endl;
		return -1;
	}

	std::cout << "MidiMenu v" << MIDIMENU_VERSION << std::endl;

	// read config
	Config::parseFile(std::string(argv[1]));
	// read menu
	Menu::Menu menu = Menu::parseFile(std::string(argv[2]));
	
	// prepare rendering
	initRender();
	Menu::prepareRender(&menu);
	Menu::render(&menu);
	
	// initialize midi device
	RtMidiIn* midiIn = Midi::deviceByPortName(Config::portName);
	if (!midiIn) {
		std::cerr << "No MIDI input device matching the name \"" << Config::portName << "\" found";
		return -2;
	}

	midiIn->setCallback(callback, &menu);
	
	// wait
	while(true) {
		sleep(1);
	}

	// clean up
	Menu::clearRender(&menu);
	closeRender();
	delete midiIn;
	return 0;
}
