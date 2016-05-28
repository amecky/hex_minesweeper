#include "Enemies.h"
#include <resources\ResourceContainer.h>

Enemies::Enemies(ds::Scene* scene, const char * meshName) : _scene(scene) {
	for (int i = 0; i < 8; ++i) {
		ID id = _scene->add(meshName, v3(16,2,0));
		ds::Entity& e = _scene->get(id);
		e.active = false;
		e.timer = -i * 0.25f;
		e.color = ds::Color(255, 0, 0, 255);
		_enemies.push_back(id);
	}
	_active = false;
	_timer = 0.0f;
	_animation = 0;
}

Enemies::Enemies(ds::Scene * scene, ds::Mesh * mesh) : _scene(scene) {
	for (int i = 0; i < 8; ++i) {
		ID id = _scene->add(mesh, v3(16, 2, 0));
		ds::Entity& e = _scene->get(id);
		e.active = false;
		e.timer = -i * 0.25f;
		e.color = ds::Color(255, 0, 0, 255);
		_enemies.push_back(id);
	}
	_active = false;
	_timer = 0.0f;
	_animation = 0;
}


Enemies::~Enemies() {
}

bool Enemies::update(float dt) {
	_timer += dt;
	if (_active) {
		if (_movement != 0) {
			_alive = _movement->tick(_enemies, dt);
		}
		if (_animation != 0) {
			_animation->tick(_enemies, dt);
		}
	}
	return _alive > 0;
}

bool Enemies::pickRandomPos(v3* pos) {
	if (_active && !_enemies.empty()) {
		int r = math::random(0, _enemies.size() - 1);
		ID id = _enemies[r];
		const ds::Entity& e = _scene->get(id);
		*pos = e.position;
		return true;
	}
	return false;
}

void Enemies::start(EnemyAnimation* animation, EnemyMovement* movement) {
	_animation = animation;
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
// rotate enemy around x axis
// ------------------------------------------------
void ScaleAnimation::tick(EnemyArray& array, float dt) {
	for (int i = 0; i < array.size(); ++i) {
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		float s = 0.75f + sin(e.timer * 8.0f) * 0.25f;
		e.scale = v3(s);
	}
}

// ------------------------------------------------
// rotate enemy around x axis
// ------------------------------------------------
void RotateAnimation::tick(EnemyArray& array, float dt) {
	float amplitude = *_amplitude;
	for (int i = 0; i < array.size(); ++i) {
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		e.rotation.x = e.timer * amplitude;
	}
}

// ------------------------------------------------
// PathMovement
// ------------------------------------------------
int PathMovement::tick(EnemyArray & array, float dt) {
	int alive = 0;
	float ttl = *_movementTTL;
	float invTTL = 1.0f / ttl;
	for (int i = 0; i < array.size(); ++i) {
		ID id = array[i];
		ds::Entity& e = _scene->get(id);
		e.timer += dt;
		if (e.timer >= 0.0f && e.timer <= ttl) {
			v2 p;
			_path->approx(e.timer * invTTL, &p);
			e.position = v3(p.x, p.y, 0.0f);
			++alive;
		}
		if (e.timer > ttl) {
			//_scene->remove(id);
			e.active = false;
		}
	}

	return alive;
}

