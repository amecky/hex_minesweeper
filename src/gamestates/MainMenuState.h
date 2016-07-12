#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class MainMenuState : public ds::BasicMenuGameState {

public:
	MainMenuState(GameContext* context, ds::Game* game);
	~MainMenuState();
	int onGUIButton(int button);
private:
	GameContext* _context;
};

