#pragma once
#include <renderer\MeshBuffer.h>
#include <math\CubicBezierPath.h>
#include <renderer\Scene.h>

typedef ds::Array<ID> EnemyArray;

typedef void(*AnimateFunc)(ds::Scene*,ID, float);

void scale_enemy(ds::Scene* scene,ID id, float dt);
void rotate_enemy(ds::Scene* scene, ID id, float dt);

class EnemyMovement {

public:
	EnemyMovement(ds::Scene* scene) : _scene(scene) , _alive(0) {}
	virtual ~EnemyMovement() {}
	virtual void prepare(EnemyArray& array) = 0;
	virtual bool tick(EnemyArray& array, float dt) = 0;
	int countAlive() const {
		return _alive;
	}
protected:
	int _alive;
	ds::Scene* _scene;
};

class FirstMovement : public EnemyMovement {

public:
	FirstMovement(ds::Scene* scene) : EnemyMovement(scene) {
		_path.create(v2(16, 4), v2(4, 12), v2(-4, -12), v2(-16, -4));
		_path.build();
	}
	virtual ~FirstMovement() {}
	void prepare(EnemyArray& array) {}
	bool tick(EnemyArray& array, float dt);
private:
	ds::CubicBezierPath _path;
};

class SecondMovement : public EnemyMovement {

public:
	SecondMovement(ds::Scene* scene) : EnemyMovement(scene) {
		_path.create(v2(16, -4), v2(4, -12), v2(-4, 12), v2(-16, 4));
		_path.build();
	}
	virtual ~SecondMovement() {}
	void prepare(EnemyArray& array) {}
	bool tick(EnemyArray& array, float dt);
private:
	ds::CubicBezierPath _path;
};


class Enemies {

public:
	Enemies(ds::Scene* scene,const char* meshName);
	~Enemies();
	void update(float dt);
	void start(AnimateFunc animateFunction,EnemyMovement* movement);
	void toggle();
private:
	AnimateFunc _animateFunction;
	EnemyMovement* _movement;
	EnemyArray _enemies;
	int _alive;
	bool _active;
	float _timer;
	ds::Scene* _scene;
};

