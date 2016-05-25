#pragma once
#include "..\GameContext.h"
#include <gamestates\GameState.h>
#include <particles\ParticleSystem.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include "..\Enemies.h"
#include "..\Stars.h"
#include "..\Waves.h"
#include "..\Upgrades.h"

struct Bullet {
	ID id;
	v3 position;
	v3 velocity;
	float angle;
	int type;
	float timer;
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
typedef std::vector<EnemyAnimation*> AnimationList;
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
	void moveEnemies(float dt);
	void addBullet();
	void addEnemyBullet(const v3& pos);
	void drawGUI();
	void checkCollisions();
	void readPathInformations();
	void startWave();
	GameContext* _context;
	ds::SpriteBuffer* _sprites;
	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	v3 _playerPos;
	float _playerAngle;
	float _timer;
	int _states[10];
	bool _firing;
	float _fireTimer;
	bool _pressed;
	EnemyList _enemies;
	AnimationList _animations;
	MovementList _movements;
	int _activeMovement;
	int _activeAnimation;
	int _activeEnemies;
	ds::Array<ds::Path*> _paths;
	ds::Scene* _scene;
	ds::Scene* _objects;
	ds::Mesh* _bulletMesh;
	ID _player;
	Stars _stars;
	Waves _waves;
	Upgrades _upgrades;
	int _wavesIndex;
	float _enemyShootTimer;
	float _currentShootDelay;
};

