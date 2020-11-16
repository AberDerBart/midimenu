#include "control.h"
#include "midi.h"
#include "config.h"
#include "renderMenu.h"
#include "process.h"
#include <iostream>

void handleMidiCommand(double deltatime, std::vector<unsigned char>* msg, Menu::Menu* menu) {
	Menu::MenuEntry* currentEntry = Menu::getCurrentEntry(menu);
	

	if(!Midi::isNoteOn(msg) && !Midi::isNoteOff(msg)) {
		return;
	}

	int note = Midi::getNote(msg);

	if(!Process::isRunning()) {
		if(note == Config::note_next) {
			if(Midi::isNoteOn(msg)) {
				Menu::nextEntry(menu);
			}
		} else if (note == Config::note_prev) {
			if(Midi::isNoteOff(msg)) {
				Menu::prevEntry(menu);
			}
		} else if (note == Config::note_select) {
			if(Midi::isNoteOn(msg)) {
				Process::run(currentEntry->cmd);
			}
		}
	} else {
		if(note == Config::note_select) {
			if (Midi::isNoteOff(msg)) {
				if (currentEntry->killOnRelease) {
					Process::killProcess();
				}
			} else {
                Process::killProcess();
            }
		}
	}
}
