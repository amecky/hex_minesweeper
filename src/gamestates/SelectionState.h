#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>

class SelectionState : public ds::BasicMenuGameState {

public:
	SelectionState(GameContext* context);
	~SelectionState();
	int onGUIButton(int button);
	void activate();
	void deactivate();
	void entering(ID id);
	void leaving(ID id);
private:
	GameContext* _context;
};

