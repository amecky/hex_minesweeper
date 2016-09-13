#include "MainMenuState.h"
#include <core\log\Log.h>
#include <core\base\EventStream.h>

const float MAIN_MENU_TRANSITION = 0.4f;

MainMenuState::MainMenuState(GameContext* context) : ds::BasicMenuGameState("MainMenu", "MainMenu"), _context(context) {
	_dialog->registerCallback(this);
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// click
// --------------------------------------------
int MainMenuState::onGUIButton(int button) {
	if (button == 4) {
		ds::events::send(ds::InternalEvents::ENGINE_SHUTDOWN);
	}
	return button;
}


void MainMenuState::activate() {
	_dialog->activate();
	_dialog->startTransition(1, v2(1200, 470), MAIN_MENU_TRANSITION);
	_dialog->startTransition(5, v2(-200, 300), MAIN_MENU_TRANSITION);
	_dialog->startTransition(4, v2(1200, 200), MAIN_MENU_TRANSITION);
}