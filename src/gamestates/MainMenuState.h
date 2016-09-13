#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class MainMenuState : public ds::BasicMenuGameState {

public:
	MainMenuState(GameContext* context);
	~MainMenuState();
	int onGUIButton(int button);
	void activate();
private:
	GameContext* _context;
};

