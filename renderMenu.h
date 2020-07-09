#pragma once

#include "menu.h"

void initRender();
void closeRender();

void waitForF1();

namespace Menu {
	void prepareRender(Menu*);
	void clearRender(Menu*);

	void render(Menu*);

	void nextEntry(Menu*);
	void prevEntry(Menu*);

	MenuEntry* getCurrentEntry(Menu*);
}
