#include "Enemies.h"
#include <resources\ResourceContainer.h>

Enemies::Enemies(const char * meshName) {
	_meshBuffer = ds::res::getMeshBuffer("ColouredBuffer");
	_mesh = ds::res::getMesh(meshName);
	for (int i = 0; i < 8; ++i) {
		Enemy e;
		e.position = v3(16, 2, 0);
		e.rotation = v3(0, 0, 0);
		e.scale = v3(1, 1, 1);
		e.timer = -i * 0.25f;
		e.moveTimer = 0.0f;
		e.active = true;
		_enemies.push_back(e);
	}
	_active = false;
	_timer = 0.0f;
	_animateFunction = rotate_enemy;
}


Enemies::~Enemies() {
}

void Enemies::draw() {
	for (int i = 0; i < _enemies.size(); ++i) {
		const Enemy& e = _enemies[i];
		if (e.active) {
			_meshBuffer->drawImmediate(_mesh, e.position, e.scale, e.rotation, ds::Color(255, 128, 0, 255));
		}
	}
}

void Enemies::update(float dt) {
	_timer += dt;
	if (_active) {
		_movement->tick(_enemies, dt);
		for (int i = 0; i < _enemies.size(); ++i) {
			Enemy& e = _enemies[i];
			e.timer += dt;
			(*_animateFunction)(&e, dt);
		}
	}
}

void Enemies::start(AnimateFunc animateFunction, EnemyMovement* movement) {
	_animateFunction = animateFunction;
	_movement = movement;
	for (int i = 0; i < _enemies.size(); ++i) {
		Enemy& e = _enemies[i];
		e.position = v3(16, 2, 0);
		e.rotation = v3(0, 0, 0);
		e.scale = v3(1, 1, 1);
		e.timer = -i * 0.25f;
		e.moveTimer = 0.0f;
		e.active = true;
	}
	_movement->prepare(_enemies);
	_alive = _enemies.size();
	_active = true;
}

void Enemies::toggle() {
	_active = !_active;
}

// ------------------------------------------------
// scale enemy
// ------------------------------------------------
void scale_enemy(Enemy * e, float dt) {
	float s = 0.75f + sin(e->timer * 8.0f) * 0.25f;
	e->scale = v3(s);
}

// ------------------------------------------------
// rotate enemy around x axis
// ------------------------------------------------
void rotate_enemy(Enemy * e, float dt) {
	e->rotation.x = e->timer * 4.0f;
}

// ------------------------------------------------
// First movement
// ------------------------------------------------
bool FirstMovement::tick(EnemyArray & array, float dt) {
	for (int i = 0; i < array.size(); ++i) {
		Enemy& e = array[i];
		e.timer += dt;
		if (e.timer >= 0.0f && e.moveTimer <= 1.0f) {
			e.moveTimer += dt * 0.5f;
			v2 p;
			_path.get(e.moveTimer, &p);
			e.position = v3(p.x, p.y, 0.0f);
		}
		if (e.moveTimer > 1.0f) {
			e.active = false;
			--_alive;
		}
	}

	return false;
}

// ------------------------------------------------
// Second movement
// ------------------------------------------------
bool SecondMovement::tick(EnemyArray & array, float dt) {
	for (int i = 0; i < array.size(); ++i) {
		Enemy& e = array[i];
		e.timer += dt;
		if (e.timer >= 0.0f && e.moveTimer <= 1.0f) {
			e.moveTimer += dt * 0.5f;
			v2 p;
			_path.get(e.moveTimer, &p);
			e.position = v3(p.x, p.y, 0.0f);
		}
		if (e.moveTimer > 1.0f) {
			e.active = false;
			--_alive;
		}
	}

	return false;
}

