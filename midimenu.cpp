#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include <rtmidi/RtMidi.h>
#include <nlohmann/json.hpp>

typedef struct {
	std::string cmd;
	bool killOnRelease;
} NoteConfig;

std::map<int, NoteConfig> mapping;
RtMidiIn* midiin = 0;
std::mutex mtx;
std::thread thread;
pid_t currentPid = 0;

typedef enum {
	INVALID = -1,
	NOTE_ON = 0x90,
	NOTE_OFF = 0x80
} MidiCommand;

int execProgram(std::string cmd) {
	pid_t pid = fork();
	if (pid) {
		std::cout << "child process pid: " << std::dec << pid << std::endl;
		return pid;
	}
	std::cout << "starting program \"" << cmd << "\"" << ", pid: " << pid << std::endl;


	char* c_cmd = (char*) cmd.c_str();
	char *const args[4] = { "sh", "-c", c_cmd, NULL };
	int foo = execv("/bin/sh", args);
	_exit(0);
}

void killProgram(pid_t pid, int signal = SIGTERM) {
	if (pid) {
		std::cout << "killing program with pid " << std::dec << pid << std::endl;
		kill(pid, signal);
	} else {
		std::cout << "No program to kill" << std::endl;
	}
}

void signalHandler(int signum) {
	if (signum == SIGCHLD) {
		currentPid = 0;
	}
	if (signum == SIGTERM || signum == SIGKILL) {
		if (currentPid) {
			killProgram(currentPid, signum);
		}
		exit(1);
	}
}

void handleNoteOn(NoteConfig cfg) {
	if (currentPid) {
		std::cout << "program still running, pid: " << std::dec << currentPid << std::endl;
		return;
	}
	currentPid = execProgram(cfg.cmd);
}

void handleNoteOff(NoteConfig cfg) {
	if (cfg.killOnRelease){
		killProgram(currentPid, SIGTERM);
		return;
	}
}

void callback(double deltatime, std::vector<unsigned char>* message, void *userData){
	std::cout << "Received MIDI message:";
	for(auto it = message->begin(); it != message->end(); it++) {
		std::cout << " " << std::hex << (int) *it;
	}
	std::cout << std::endl;


	// ignore messages that are not 3 bytes long
	if (message->size() < 3) return;

	// ignore non-note-on/off messages
	MidiCommand command = (MidiCommand) (message->at(0) & 0xf0);
	if (command != NOTE_ON && command != NOTE_OFF) {
		std::cout << "ignoring non-note-on/off message." << std::endl;
		return;
	}

	// get configuration for triggered note
	int note = message->at(1);
	NoteConfig noteConfig;
	try{
		noteConfig = mapping.at(note);
	} catch (const std::exception& e) {
		std::cout << "Note " << note << " not configured" << std::endl;
		return;
	}

	switch(command) {
		case NOTE_ON:
			handleNoteOn(noteConfig);
			break;
		case NOTE_OFF:
			handleNoteOff(noteConfig);
			break;
		default:
			std::cout << "Command not supported";
			break;
	}
}

NoteConfig parseNoteConfig(nlohmann::json j) {
	NoteConfig retn;
	retn.cmd = j["cmd"];
	retn.killOnRelease = j["killOnRelease"] == "true";

	return retn;
}

std::map<int, NoteConfig> readConfig(char* fileName) {
	std::map<int, NoteConfig> map;

	std::cout << "Reading config file \"" << fileName << "\"" << std::endl;
	std::fstream configStream;
	configStream.open(fileName, std::fstream::in);

	nlohmann::json j;
	configStream >> j;
	configStream.close();

	for (auto it = j.begin(); it != j.end(); ++it) {
		try {
			int index = std::stoi(it.key());
			map[index] = parseNoteConfig(it.value());
		} catch (const std::exception& e) {
			std::cout << "Error converting key \"" << it.key() << "\", skipping." << std::endl;
		}
	}

	return map;
}

int main(int argc, char** argv) {
	signal(SIGCHLD, signalHandler);
	try {
		midiin = new RtMidiIn();
	} catch (RtMidiError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	unsigned int nPorts = midiin->getPortCount();
	std::cout << "There are " << nPorts << " ports available." << std::endl;

	std::string portName;
	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = midiin->getPortName(i);
		}
		catch ( RtMidiError &error ) {
			error.printMessage();
			delete midiin;
			exit(EXIT_FAILURE);
		}
		std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
	}
	
	if (argc < 3) {
		std::cout << "Too few arguments" << std::endl;
		std::cout << "usage: " << argv[0] << " <portname> <config file>" << std::endl;
		delete midiin;
		exit(EXIT_FAILURE);
	}

	std::string portname(argv[1]);

	for (unsigned int i=0; i<nPorts; i++) {
		int len = portname.compare(0, portname.length(), midiin->getPortName(i), 0, portname.length());
		if (len == 0) {
			std::cout << "Opening port #" << i << ": \"" << midiin->getPortName(i) << "\"" << std::endl;
			midiin->openPort(i);
			break;
		}
	}

	if (!midiin->isPortOpen()) {
		std::cout << "Could not find port: \"" << portname << "\"" << std::endl;
		delete midiin;
		exit(EXIT_FAILURE);
	}

	mapping = readConfig(argv[2]);

	midiin->setCallback(&callback);
	midiin->ignoreTypes(false, false, false);

	std::cout << "Reading MIDI input ..." << std::endl;
	while(true){
		sleep(1000000);
	}

	midiin->cancelCallback();
	return 0;
}

