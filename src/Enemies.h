#pragma once
#include <renderer\MeshBuffer.h>
#include <math\CubicBezierPath.h>
#include <renderer\Scene.h>
#include <math\StraightPath.h>
#include <data\DynamicSettings.h>

typedef ds::Array<ID> EnemyArray;

// ---------------------------------------
// Virtual enemy animation base class
// ---------------------------------------
class EnemyAnimation {

public:
	EnemyAnimation(ds::Scene* scene) : _scene(scene) {}
	virtual ~EnemyAnimation() {}
	virtual void prepare(EnemyArray& array) = 0;
	virtual void tick(EnemyArray& array, float dt) = 0;
protected:
	ds::Scene* _scene;
};

class ScaleAnimation : public EnemyAnimation{

public:
	ScaleAnimation(ds::Scene* scene) : EnemyAnimation(scene) {}
	virtual ~ScaleAnimation() {}
	void prepare(EnemyArray& array) {}
	void tick(EnemyArray& array, float dt);
};

class RotateAnimation : public EnemyAnimation{

public:
	RotateAnimation(ds::Scene* scene) : EnemyAnimation(scene) {
		_amplitude = ds::settings::addFloat("rotation_amplitude", "Enemy rotation amplitude", 2.0f);
	}
	virtual ~RotateAnimation() {}
	void prepare(EnemyArray& array) {}
	void tick(EnemyArray& array, float dt);
private:
	float* _amplitude;
};

// ---------------------------------------
// Virtual enemy movement base class
// ---------------------------------------
class EnemyMovement {

public:
	EnemyMovement(ds::Scene* scene) : _scene(scene) , _alive(0) {
		_movementTTL = ds::settings::addFloat("movement_ttl", "Enemy movement TTL", 4.0f);
	}
	virtual ~EnemyMovement() {}
	virtual void prepare(EnemyArray& array) = 0;
	virtual int tick(EnemyArray& array, float dt) = 0;
	int countAlive() const {
		return _alive;
	}
protected:
	float* _movementTTL;
	int _alive;
	ds::Scene* _scene;
};

class PathMovement : public EnemyMovement {

public:
	PathMovement(ds::Scene* scene,ds::Path* path) : EnemyMovement(scene) {
		_path = path;
	}
	virtual ~PathMovement() {}
	void prepare(EnemyArray& array) {}
	int tick(EnemyArray& array, float dt);
private:
	ds::Path* _path;
};

// ---------------------------------------
// Enemies
// ---------------------------------------
class Enemies {

public:
	Enemies(ds::Scene* scene,const char* meshName);
	Enemies(ds::Scene* scene, ds::Mesh* mesh);
	~Enemies();
	bool update(float dt);
	void start(EnemyAnimation* animation, EnemyMovement* movement);
	void toggle();
	bool isActive() const {
		return _active;
	}
	void setActive(bool active) {
		_active = active;
	}
	bool pickRandomPos(v3* pos);
private:
	EnemyAnimation* _animation;
	EnemyMovement* _movement;
	EnemyArray _enemies;
	int _alive;
	bool _active;
	float _timer;
	ds::Scene* _scene;
};

