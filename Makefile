midimenu: midimenu.cpp
	g++ -o $@ $< -lrtmidi -pthread

.PHONY: install clean uninstall

clean:
	rm -f midimenu

install: midimenu
	cp midimenu /bin/

uninstall:
	rm -f /bin/midimenu
