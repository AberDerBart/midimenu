%.o: %.c
	g++ -c $<

midimenu: midimenu.o
	g++ -o $@ $< -lrtmidi

.PHONY: install clean uninstall

clean:
	rm -f midimenu

install: midimenu
	install -Dt $(DESTDIR)/bin midimenu

uninstall:
	rm -f $(DESTDIR)/bin/midimenu
