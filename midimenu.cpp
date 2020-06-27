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

std::map<int, std::string> mapping;
RtMidiIn* midiin = 0;
std::mutex mtx;
std::thread thread;
pid_t currentPid = 0;

int execProgram(std::string cmd) {
	pid_t pid = fork();
	std::cout << "pid after fork: " << pid << std::endl;
	if (pid) {
		return pid;
	}
	int foo = system(cmd.c_str());
	_exit(0);
}

void killProgram(pid_t pid, int signal = SIGTERM) {
	kill(pid, SIGTERM);
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

void callback(double deltatime, std::vector<unsigned char>* message, void *userData){
	if (currentPid) {
		std::cout << "program still running, pid: " << std::dec << currentPid << std::endl;
		// skip all events when a command is running
		return;
	}

	unsigned int nBytes = message->size();

	std::cout << "Received MIDI message:";
	for(auto it = message->begin(); it != message->end(); it++) {
		std::cout << " " << std::hex << (int) *it;
	}
	std::cout << std::endl;

	// ignore messages that are not 3 bytes long
	if (message->size() < 3) return;
	// ignore any non-"Note on"-messages
	if ((message->at(0) & 0xf0) != 0x90) {
		return;
	}

	int note = message->at(1);

	try{
		std::string command = mapping.at(note);
		std::cout << "Triggering command \"" << command << "\"" << deltatime << std::endl;
		currentPid = execProgram(command);
	} catch (const std::exception& e) {
		std::cout << "Note " << note << " not configured" << std::endl;
	}
}


std::map<int, std::string> readConfig(char* fileName) {
	std::map<int, std::string> map;

	std::cout << "Reading config file \"" << fileName << "\"" << std::endl;
	std::fstream configStream;
	configStream.open(fileName, std::fstream::in);

	nlohmann::json j;
	configStream >> j;
	configStream.close();

	for (auto it = j.begin(); it != j.end(); ++it) {
		try {
			int index = std::stoi(it.key());
			map[index] = it.value();
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

