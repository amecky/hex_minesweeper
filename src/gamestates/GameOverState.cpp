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
	char buffer[32];
	sprintf_s(buffer, 32, "%d / %d", _context->markedCorrectly, GAME_MODES[_context->mode].maxBombs);
	dlg->updateText(12, buffer);
	std::string str;
	ds::string::formatTime(_context->playedMinutes, _context->playedSeconds, str);
	dlg->updateText(14, str);

	if (_context->markedCorrectly == GAME_MODES[_context->mode].maxBombs) {
		dlg->updateImage(11, 140, 660, ds::Rect(450, 0, 470, 84));
		if (scoring::hasScored(_context->playedSeconds, _context->playedMinutes, _context->mode, _context->highscore)) {
			LOG << "NEW HIGHSCORE!!!";
		}
	}
	else {
		dlg->updateImage(11, 140, 660, ds::Rect(450, 470, 450, 84));
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
	ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
}


