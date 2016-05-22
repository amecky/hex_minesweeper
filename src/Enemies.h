#pragma once
#include <renderer\MeshBuffer.h>
#include <math\CubicBezierPath.h>

struct Enemy {
	float timer;
	v3 position;
	v3 scale;
	v3 rotation;
	float moveTimer;
	bool active;
};

typedef ds::Array<Enemy> EnemyArray;

typedef void(*AnimateFunc)(Enemy*, float);

void scale_enemy(Enemy* e, float dt);
void rotate_enemy(Enemy* e, float dt);

class EnemyMovement {

public:
	EnemyMovement() : _alive(0) {}
	virtual ~EnemyMovement() {}
	virtual void prepare(EnemyArray& array) = 0;
	virtual bool tick(EnemyArray& array, float dt) = 0;
	int countAlive() const {
		return _alive;
	}
protected:
	int _alive;
};

class FirstMovement : public EnemyMovement {

public:
	FirstMovement() : EnemyMovement() {
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
	SecondMovement() : EnemyMovement() {
		_path.create(v2(16, -4), v2(4, 12), v2(-4, -12), v2(-16, 4));
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
	Enemies(const char* meshName);
	~Enemies();
	void draw();
	void update(float dt);
	void start(AnimateFunc animateFunction,EnemyMovement* movement);
	void toggle();
private:
	AnimateFunc _animateFunction;
	EnemyMovement* _movement;
	ds::MeshBuffer* _meshBuffer;
	ds::Mesh* _mesh;
	ds::Array<Enemy> _enemies;
	int _alive;
	bool _active;
	float _timer;
};

