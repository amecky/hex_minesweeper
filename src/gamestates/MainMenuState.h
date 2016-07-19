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
		_dialog->startTransition(1, v2(1200, 500), 1.0f);
		_dialog->startTransition(2, v2(-200, 400), 1.0f);
		_dialog->startTransition(3, v2(1200, 300), 1.0f);
		_dialog->startTransition(5, v2(-200, 200), 1.0f);
		_dialog->startTransition(4, v2(1200, 100), 1.0f);
	}
private:
	GameContext* _context;
};

