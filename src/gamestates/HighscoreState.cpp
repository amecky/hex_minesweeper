#include "HighscoreState.h"
#include <utils\Log.h>

HighscoreState::HighscoreState(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("Highscores", "Highscores", game) , _context(context) {
}


HighscoreState::~HighscoreState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::activate() {
	
	for (int i = 0; i < 3; ++i ) {
		const Highscore& h = _context->highscores[i].get(0);
		std::string str;
		ds::string::formatTime(h.minutes, h.seconds, str);
		_dialog->updateText(20 + i, str.c_str());
	}
}
