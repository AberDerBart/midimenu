#pragma once
#include <rtmidi/RtMidi.h>

namespace Midi {
	RtMidiIn* deviceByPortName(std::string portName);

	bool isNoteOn(std::vector<unsigned char>* msg);
	bool isNoteOff(std::vector<unsigned char>* msg);
	int getNote(std::vector<unsigned char>* msg);
}
