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
	int onButtonUp(int button, int x, int y);
	int onButtonDown(int button, int x, int y);
	int onKeyDown(WPARAM virtualKey);
	int onKeyUp(WPARAM virtualKey);
private:
	GameContext* _context;
	//ds::ParticleManager* _particles;
	bool _buttonPressed;
	ds::QuadBuffer* _buffer;
	ds::FPSCamera* _camera;
};

