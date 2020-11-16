#include "process.h"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "menu.h"

void sigchldHandler(int);

namespace Process {
	pid_t pid = 0;

	void init() {
		signal(SIGCHLD, sigchldHandler);
	}

	pid_t run(std::string cmd) {
		pid = fork();
		if(pid) {
			return pid;
		}

		char* c_cmd = (char*) cmd.c_str();
		char *const args[4] = { "sh", "-c", c_cmd, NULL };
		int foo = execv("/bin/sh", args);
		_exit(0);
	}

	void killProcess(int signal){
		if(pid) {
			kill(pid, signal);
		}
	}

	bool isRunning() {
		return pid != 0;
	}
}

void sigchldHandler(int signum) {
	if(signum != SIGCHLD) {
		return;
	}

	Process::pid = 0;
}

