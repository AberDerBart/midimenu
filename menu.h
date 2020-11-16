#pragma once
#include <string>
#include <vector>
#include <ncurses.h>
#include <menu.h>

namespace Menu {
	typedef struct {
		std::string title;
		std::string cmd;
		bool killOnRelease;
	} MenuEntry;

	typedef struct {
		std::vector<MenuEntry> entries;
		std::vector<ITEM*> renderItems;
		MENU* renderMenu;
	} Menu;
}
