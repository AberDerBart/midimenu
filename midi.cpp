#include "midi.h"

namespace Midi {
	typedef enum {
		INVALID = -1,
		NOTE_ON = 0x90,
		NOTE_OFF = 0x80
	} MidiCommand;

	RtMidiIn* deviceByPortName(std::string portName) {
		RtMidiIn* midiIn = new RtMidiIn();

		unsigned int nPorts = midiIn->getPortCount();

		for ( unsigned int i=0; i<nPorts; i++ ) {
			int len = portName.compare(0, portName.length(), midiIn->getPortName(i), 0, portName.length());
			if (len == 0) {
				//std::cout << "Opening port #" << i << ": \"" << midiIn->getPortName(i) << "\"" << std::endl;
				midiIn->openPort(i);
				return midiIn;
			}
		}

		delete midiIn;
		return NULL;
	}

	bool isNoteOn(std::vector<unsigned char>* msg) {
		return msg->size() == 3 && (msg->at(0) & 0xf0) == NOTE_ON;
	}

	bool isNoteOff(std::vector<unsigned char>* msg) {
		return msg->size() == 3 && (msg->at(0) & 0xf0) == NOTE_OFF;
	}
	
	int getNote(std::vector<unsigned char>* msg) {
		return msg->at(1);
	}
}
