#include "GameOverState.h"
#include <utils\Log.h>
/*
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
	ds::GameTimer* timer = _context->hud->getTimer(3);
	ds::string::formatTime(timer->getMinutes(), timer->getSeconds(), str);
	dlg->updateText(14, str);
	int state = 1;
	if (_context->markedCorrectly == GAME_MODES[_context->mode].maxBombs) {
		state = 2;
		dlg->updateImage(11, 140, 660, ds::Rect(80, 500, 470, 75));
		if (scoring::hasScored(timer->getSeconds(), timer->getMinutes(), _context->mode, _context->highscore)) {
			state = 3;
		}
	}
	else {
		dlg->updateImage(11, 140, 660, ds::Rect(0, 500, 485, 80));
	}
	if (state == 3) {
		dlg->updateText(19, "Congratulations - New highscore");
	}
	else if ( state == 2) {
		dlg->updateText(19, "Try to be faster next time");
	}
	else {
		dlg->updateText(19, "You have unveiled a bomb");
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
	//ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
}


*/