#pragma once
#include <renderer\MeshBuffer.h>
#include <math\CubicBezierPath.h>
#include <renderer\Scene.h>
#include <math\StraightPath.h>

typedef ds::Array<ID> EnemyArray;

typedef void(*AnimateFunc)(ds::Scene*,ID, float);

void scale_enemy(ds::Scene* scene,ID id, float dt);
void rotate_enemy(ds::Scene* scene, ID id, float dt);

class EnemyMovement {

public:
	EnemyMovement(ds::Scene* scene) : _scene(scene) , _alive(0) {}
	virtual ~EnemyMovement() {}
	virtual void prepare(EnemyArray& array) = 0;
	virtual int tick(EnemyArray& array, float dt) = 0;
	int countAlive() const {
		return _alive;
	}
protected:
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


class Enemies {

public:
	Enemies(ds::Scene* scene,const char* meshName);
	~Enemies();
	bool update(float dt);
	void start(AnimateFunc animateFunction,EnemyMovement* movement);
	void toggle();
	bool isActive() const {
		return _active;
	}
	void setActive(bool active) {
		_active = active;
	}
	bool pickRandomPos(v3* pos);
private:
	AnimateFunc _animateFunction;
	EnemyMovement* _movement;
	EnemyArray _enemies;
	int _alive;
	bool _active;
	float _timer;
	ds::Scene* _scene;
};

