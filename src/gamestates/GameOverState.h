#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class GameOverState : public ds::BasicMenuGameState {

public:
	GameOverState(GameContext* context, ds::Game* game);
	~GameOverState();
	void activate();
private:
	GameContext* _context;
};

