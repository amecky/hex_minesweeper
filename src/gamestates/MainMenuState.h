#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

const float MAIN_MENU_TRANSITION = 0.4f;

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
		_dialog->startTransition(1, v2(1200, 500), MAIN_MENU_TRANSITION);
		_dialog->startTransition(5, v2(-200, 200), MAIN_MENU_TRANSITION);
		_dialog->startTransition(4, v2(1200, 100), MAIN_MENU_TRANSITION);
	}
private:
	GameContext* _context;
};

