#include "Enemies.h"
#include <resources\ResourceContainer.h>

Enemies::Enemies(ds::Scene* scene, const char * meshName) : _scene(scene) {
	for (int i = 0; i < 8; ++i) {
		ID id = _scene->add(meshName, v3(16,2,0));
		ds::Entity& e = _scene->get(id);
		e.active = false;
		e.timer = -i * 0.25f;
		_enemies.push_back(id);
	}
	_active = false;
	_timer = 0.0f;
	_animateFunction = rotate_enemy;
}


Enemies::~Enemies() {
}

void Enemies::update(float dt) {
	_timer += dt;
	if (_active) {
		_movement->tick(_enemies, dt);
		for (int i = 0; i < _enemies.size(); ++i) {
			ID id = _enemies[i];
			//e.timer += dt;
			(*_animateFunction)(_scene, id, dt);
		}
	}
}

void Enemies::start(AnimateFunc animateFunction, EnemyMovement* movement) {
	_animateFunction = animateFunction;
	_movement = movement;
	for (int i = 0; i < _enemies.size(); ++i) {
		ID id = _enemies[i];
		ds::Entity& e = _scene->get(id);
		e.position = v3(16, 2, 0);
		e.rotation = v3(0, 0, 0);
		e.scale = v3(1, 1, 1);
		e.timer = -i * 0.25f;
		//e.moveTimer = 0.0f;
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
void scale_enemy(ds::Scene* scene, ID id, float dt) {
	ds::Entity& e = scene->get(id);
	float s = 0.75f + sin(e.timer * 8.0f) * 0.25f;
	e.scale = v3(s);
}

// ------------------------------------------------
// rotate enemy around x axis
// ------------------------------------------------
void rotate_enemy(ds::Scene* scene, ID id, float dt) {
	ds::Entity& e = scene->get(id);
	e.rotation.x = e.timer * 4.0f;
}

// ------------------------------------------------
// First movement
// ------------------------------------------------
bool FirstMovement::tick(EnemyArray & array, float dt) {
	for (int i = 0; i < array.size(); ++i) {
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		e.timer += dt;
		if (e.timer >= 0.0f && e.timer <= 2.0f) {
			v2 p;
			_path.approx(e.timer * 0.5f, &p);
			e.position = v3(p.x, p.y, 0.0f);
		}
		if (e.timer > 2.0f) {
			//_scene->remove(id);
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
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		e.timer += dt;
		if (e.timer >= 0.0f && e.timer <= 2.0f) {
			v2 p;
			_path.approx(e.timer * 0.5f, &p);
			e.position = v3(p.x, p.y, 0.0f);
		}
		if (e.timer > 2.0f) {
			//_scene->remove(id);
			e.active = false;
			--_alive;
		}
	}

	return false;
}

// ------------------------------------------------
// Second movement
// ------------------------------------------------
bool ThirdMovement::tick(EnemyArray & array, float dt) {
	for (int i = 0; i < array.size(); ++i) {
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		e.timer += dt;
		if (e.timer >= 0.0f && e.timer <= 2.0f) {
			v2 p;
			_path.approx(e.timer * 0.5f, &p);
			e.position = v3(p.x, p.y, 0.0f);
		}
		if (e.timer > 2.0f) {
			//_scene->remove(id);
			e.active = false;
			--_alive;
		}
	}

	return false;
}

// ------------------------------------------------
// Second movement
// ------------------------------------------------
bool PathMovement::tick(EnemyArray & array, float dt) {
	for (int i = 0; i < array.size(); ++i) {
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		e.timer += dt;
		if (e.timer >= 0.0f && e.timer <= 2.0f) {
			v2 p;
			_path->approx(e.timer * 0.5f, &p);
			e.position = v3(p.x, p.y, 0.0f);
		}
		if (e.timer > 2.0f) {
			//_scene->remove(id);
			e.active = false;
			--_alive;
		}
	}

	return false;
}

