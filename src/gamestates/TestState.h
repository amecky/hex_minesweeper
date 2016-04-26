#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include <particles\ParticleSystem.h>

class TestState : public ds::GameState {

public:
	TestState(GameContext* context);
	~TestState();
	void init();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
private:
	GameContext* _context;
	ds::ParticleManager* _particles;
};

