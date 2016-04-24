#include "MainMenuState.h"
#include <utils\Log.h>

MainMenuState::MainMenuState(GameContext* context) : ds::BasicMenuGameState("MainMenu",15), _context(context) {
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// click
// --------------------------------------------
int MainMenuState::onGUIButton(int button) {
	if (button == 1) {
		_context->mode = 0;
		return 1;
	}
	if (button == 2) {
		_context->mode = 1;
		return 1;
	}
	if (button == 3) {
		_context->mode = 2;
		return 1;
	}
	return button;
}

