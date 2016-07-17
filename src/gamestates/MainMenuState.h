#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class MainMenuState : public ds::BasicMenuGameState {

public:
	MainMenuState(GameContext* context, ds::Game* game);
	~MainMenuState();
	int onGUIButton(int button);
	int update(float dt) {
		_dialog->tick(dt);
		return 0;
	}
	void activate() {
		_dialog->setNumber(7, 4567);
	}
private:
	GameContext* _context;
};

