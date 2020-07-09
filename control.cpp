#include "control.h"
#include "midi.h"
#include "config.h"
#include "renderMenu.h"
#include <iostream>

void handleMidiCommand(double deltatime, std::vector<unsigned char>* msg, Menu::Menu* menu) {
	if(Midi::isNoteOn(msg)) {
		int note = Midi::getNote(msg);

		if(note == Config::note_next) {
			Menu::nextEntry(menu);
		} else if (note == Config::note_prev) {
			Menu::prevEntry(menu);
		}
	}
}
