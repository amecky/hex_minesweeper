#pragma once
#include "..\GameContext.h"
#include <base\GameState.h>

class TestGameState : public ds::GameState {

public:
	TestGameState(ds::DialogManager* gui, GameContext* context);
	~TestGameState();
	int update(float dt);
	void render();
	int onGUIButton(ds::DialogID dlgID, int button);
	void activate();
	void deactivate();
private:
	GameContext* _context;
	ds::DialogManager* _gui;
};


