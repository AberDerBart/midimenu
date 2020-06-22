midimenu: midimenu.cpp
	g++ -o $@ $< -lrtmidi -pthread
