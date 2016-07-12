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
	
	PlayedTime entries[3];
	int cnt = _context->highscore_service.get(entries, 3);
	for (int i = 0; i < cnt; ++i ) {
		std::string str;
		ds::string::formatTime(entries[i].minutes, entries[i].seconds, str);
		_dialog->updateText(20 + i, str.c_str());
	}
}
