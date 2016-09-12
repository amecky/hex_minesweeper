#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

const float SELECTION_MENU_TRANSITION = 0.4f;

class SelectionState : public ds::BasicMenuGameState {

public:
	SelectionState(GameContext* context);
	~SelectionState();
	int onGUIButton(int button);
	int update(float dt);
	void activate();
	void deactivate();
	void entering(ID id);
	void leaving(ID id);
private:
	GameContext* _context;
};

