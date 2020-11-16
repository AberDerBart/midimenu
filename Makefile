%.o: %.c
	g++ -c $<

midimenu: menuParser.o main.o config.o midi.o renderMenu.o control.o process.o
	g++ -o $@ $^ -lrtmidi -lncurses -lmenu

.PHONY: install clean uninstall

clean:
	rm -f midimenu *.o

install: midimenu
	install -Dt $(DESTDIR)/bin midimenu

uninstall:
	rm -f $(DESTDIR)/bin/midimenu
