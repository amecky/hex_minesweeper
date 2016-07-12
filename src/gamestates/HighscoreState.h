#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class HighscoreState : public ds::BasicMenuGameState {

public:
	HighscoreState(GameContext* context, ds::Game* game);
	~HighscoreState();
	void activate();
private:
	GameContext* _context;
};

