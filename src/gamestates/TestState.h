#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include <particles\ParticleSystem.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include "..\Enemies.h"

struct Bullet {

	v3 position;
	v3 velocity;
	float angle;
};

struct Cube {
	v3 position;
	v3 velocity;
	v3 scale;
	v3 rotation;
	float angle;
	float timer;
	int state;
	float roll;
};

typedef std::vector<Bullet> Bullets;
typedef std::vector<Enemies*> EnemyList;
typedef std::vector<EnemyMovement*> MovementList;
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
	void drawGUI();
	void checkCollisions();
	void readPathInformations();
	GameContext* _context;
	ds::SpriteBuffer* _sprites;
	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	ds::MeshBuffer* _colouredBuffer;
	ds::MeshBuffer* _texturedBuffer;
	Bullets _bulletList;
	v3 _playerPos;
	float _playerAngle;
	float _timer;
	int _states[10];
	bool _firing;
	float _fireTimer;
	bool _pressed;
	EnemyList _enemies;
	AnimateFunc _animation;
	MovementList _movements;
	int _activeMovement;
	int _activeEnemies;
	ds::Array<ds::Path*> _paths;
	ds::Scene* _scene;
};

