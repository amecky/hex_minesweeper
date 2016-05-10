#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include <particles\ParticleSystem.h>
#include <renderer\QuadBuffer.h>
#include <vector>

struct Bullet {

	v3 position;
	v3 velocity;
	float angle;
};

struct Cube {
	v3 position;
	v3 velocity;
	float angle;
	float timer;
	int state;
	float roll;
};

typedef std::vector<Bullet> Bullets;
typedef std::vector<Cube> Cubes;

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
	void addBullet();
	void addCube();
	void drawGUI();
	void checkCollisions();
	GameContext* _context;
	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	ds::Mesh* _cubes;
	ds::Mesh* _player;
	ds::Mesh* _bullets;
	Bullets _bulletList;
	Cubes _cubeList;
	ds::Rect _cubeTextures[6];
	v3 _playerPos;
	float _playerAngle;
	float _timer;
	int _states[10];
	bool _firing;
	float _fireTimer;
};

