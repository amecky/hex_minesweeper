#include "MainMenuState.h"
#include <utils\Log.h>
#include <base\EventStream.h>

MainMenuState::MainMenuState(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("MainMenu", "MainMenu", game), _context(context) {
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// click
// --------------------------------------------
int MainMenuState::onGUIButton(int button) {
	LOG << "button: " << button;
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
	if (button == 4) {
		ds::events::send(ds::InternalEvents::ENGINE_SHUTDOWN);
	}
	return button;
}

