#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include <particles\ParticleSystem.h>
#include <renderer\QuadBuffer.h>

class TestState : public ds::GameState {

public:
	TestState(GameContext* context);
	~TestState();
	void init();
	int update(float dt);
	void render();
	int onKeyDown(WPARAM virtualKey);
	int onKeyUp(WPARAM virtualKey);
	int onChar(int ascii);
private:
	GameContext* _context;
	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	ds::Mesh* _cubes;
	float _timer;
	int _states[10];
};

