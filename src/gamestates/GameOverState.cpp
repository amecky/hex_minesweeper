#include "GameOverState.h"
#include <utils\Log.h>

GameOverState::GameOverState(GameContext* context) : ds::BasicMenuGameState("GameOver", 16), _context(context) {
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	char buffer[32];
	sprintf_s(buffer, 32, "%d / %d", _context->markedCorrectly, GAME_MODES[_context->mode].maxBombs);
	_dialog->updateText(12, buffer);
	std::string str;
	ds::GameTimer* timer = _context->hud->getTimer(3);
	ds::string::formatTime(timer->getMinutes(), timer->getSeconds(), str);
	_dialog->updateText(14, str.c_str());
	int state = 1;
	if (_context->markedCorrectly == GAME_MODES[_context->mode].maxBombs) {
		state = 2;
		_dialog->updateImage(11, 140, 660, ds::Rect(80, 500, 470, 75));
		if (scoring::hasScored(timer->getSeconds(), timer->getMinutes(), _context->mode, _context->highscore)) {
			state = 3;
		}
	}
	else {
		_dialog->updateImage(11, 140, 660, ds::Rect(0, 500, 485, 80));
	}
	if (state == 3) {
		_dialog->updateText(19, "Congratulations - New highscore");
	}
	else if ( state == 2) {
		_dialog->updateText(19, "Try to be faster next time");
	}
	else {
		_dialog->updateText(19, "You have unveiled a bomb");
	}
	
}
