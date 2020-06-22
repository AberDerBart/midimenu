midimenu: midimenu.cpp
	g++ -o $@ $< -lrtmidi -pthread

.PHONY: install clean uninstall

clean:
	rm -f midimenu

install: midimenu
	install -Dt $(DESTDIR)/bin midimenu

uninstall:
	rm -f $(DESTDIR)/bin/midimenu
