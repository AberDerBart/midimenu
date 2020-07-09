#pragma once

#include <string>
#include <sys/types.h>
#include <signal.h>

namespace Process {
	void init();

	pid_t run(std::string cmd);
	void killProcess(int signal = SIGTERM);

	bool isRunning();
}
