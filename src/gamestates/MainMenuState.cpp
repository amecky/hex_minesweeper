#include "MainMenuState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

MainMenuState::MainMenuState(ds::DialogManager* gui, GameContext* context) : ds::GameState("MainMenu"), _context(context), _gui(gui) {
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainMenuState::activate() {
	_gui->activate("Test");	
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainMenuState::deactivate() {
	_gui->deactivate("Test");
}

// --------------------------------------------
// update
// --------------------------------------------
int MainMenuState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int MainMenuState::onGUIButton(ds::DialogID dlgID, int button) {
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

// --------------------------------------------
// render
// --------------------------------------------
void MainMenuState::render() {
	ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
}


