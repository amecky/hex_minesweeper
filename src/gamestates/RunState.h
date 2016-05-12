#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include <particles\ParticleSystem.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include "..\Board.h"

class RunState : public ds::GameState {

public:
	RunState(GameContext* context);
	~RunState();
	void init();
	int update(float dt);
	void render();
	void activate();
private:
	void drawGUI();
	GameContext* _context;

	Board* _board;

	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	ds::MeshBuffer* _texturedBuffer;
	ds::Mesh* _cubes;
	bool _pressed;
};

