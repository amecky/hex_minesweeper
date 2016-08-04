#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class CreditsState : public ds::BasicMenuGameState {

public:
	CreditsState(GameContext* context, ds::Game* game);
	~CreditsState();
	int onGUIButton(int button);
	int update(float dt);
	void activate();
private:
	void toggle(int id);
	GameContext* _context;
	int _current;
	float _timer;
};

