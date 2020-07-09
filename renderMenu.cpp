#include "renderMenu.h"
#include <menu.h>
#include <ncurses.h>

#include <iostream>

void initRender() {
	initscr();
	cbreak();
        noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
}

void waitForF1() {
	int c;
	while((c = getch()) != KEY_F(1)) {}
}

void closeRender() {
	endwin();
}

namespace Menu {
	void prepareRender(Menu* menu) {
		for (auto it = menu->entries.begin(); it != menu->entries.end(); ++it) {
			menu->renderItems.push_back(new_item(it->title.c_str(), NULL));
		}
		menu->renderMenu = new_menu(menu->renderItems.data());
	}

	void clearRender(Menu* menu) {
		unpost_menu(menu->renderMenu);
		free_menu(menu->renderMenu);
		for (auto it = menu->renderItems.begin(); it != menu->renderItems.end(); ++it) {
			free_item(*it);
		}
		menu->renderItems.clear();
	}

	void render(Menu* menu) {
		post_menu(menu->renderMenu);
		refresh();
	}

	void nextEntry(Menu* menu) {
		menu_driver(menu->renderMenu, REQ_DOWN_ITEM);
		refresh();
	}

	void prevEntry(Menu* menu) {
		menu_driver(menu->renderMenu, REQ_UP_ITEM);
		refresh();
	}
}
