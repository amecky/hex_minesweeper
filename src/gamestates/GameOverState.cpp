#include "GameOverState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

GameOverState::GameOverState(ds::DialogManager* gui, GameContext* context) : ds::GameState("GameOver"), _context(context), _gui(gui) {
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	_gui->activate("GameOver");	
	
	ds::GUIDialog* dlg = _gui->get("GameOver");
	std::string str;
	ds::string::formatInt(_context->markedCorrectly, str);
	dlg->updateText(12, str);
	ds::string::formatTime(_context->playedMinutes, _context->playedSeconds, str);
	dlg->updateText(14, str);

	if (_context->markedCorrectly == GAME_MODES[_context->mode].maxBombs) {
		dlg->updateImage(11, 140, 660, ds::Rect(680, 0, 470, 84));
	}
	else {
		dlg->updateImage(11, 140, 660, ds::Rect(680, 470, 450, 84));
	}
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::deactivate() {
	_gui->deactivate("GameOver");
}

// --------------------------------------------
// update
// --------------------------------------------
int GameOverState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int GameOverState::onGUIButton(ds::DialogID dlgID, int button) {	
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void GameOverState::render() {
}


