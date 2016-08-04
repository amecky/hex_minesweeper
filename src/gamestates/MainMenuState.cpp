#include "MainMenuState.h"
#include <utils\Log.h>
#include <base\EventStream.h>

MainMenuState::MainMenuState(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("MainMenu", "MainMenu", game), _context(context) {
	_dialog->registerCallback(this);
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// click
// --------------------------------------------
int MainMenuState::onGUIButton(int button) {
	LOG << "button: " << button;
	if (button == 4) {
		ds::events::send(ds::InternalEvents::ENGINE_SHUTDOWN);
	}
	return button;
}

void MainMenuState::entering(ID id) {
	LOG << "entering: " << id;
}

void MainMenuState::leaving(ID id) {
	LOG << "leaving: " << id;
}
