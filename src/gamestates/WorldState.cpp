#include "WorldState.h"
#include <utils\Log.h>
#include <base\InputSystem.h>
#include <renderer\graphics.h>
#include <utils\TileMapReader.h>

WorldState::WorldState() : ds::GameState("WorldState"), _mesh(0) {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_pressed = false;
}

void WorldState::init() {
	_camera->setPosition(v3(12, 6, -9), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(30.0f));
	_camera->setYAngle(DEGTORAD(-45.0f));
	_mesh = new ds::Mesh();
	_scene = ds::res::getScene("TestObjects");
	_buffer = ds::res::getMeshBuffer("TexturedBuffer");
	buildTerrain();
}



WorldState::~WorldState() {
	_objects.destroy_all();
	if (_mesh != 0) {
		delete _mesh;
	}
}

// ------------------------------------------
// build terrain
// ------------------------------------------
void WorldState::buildTerrain() {
	char buffer[32];
	for (int i = 0; i < 12; ++i) {
		sprintf_s(buffer, 32, "tile_%d", i);
		ds::Mesh* m = new ds::Mesh();
		m->load(buffer);
		_objects.push_back(m);
	}
	for (int i = 0; i < 1; ++i) {
		sprintf_s(buffer, 32, "house_%d", i);
		ds::Mesh* m = new ds::Mesh();
		m->load(buffer);
		_objects.push_back(m);
	}
	for (int i = 0; i < 2; ++i) {
		sprintf_s(buffer, 32, "tree_%d", i);
		ds::Mesh* m = new ds::Mesh();
		m->load(buffer);
		_objects.push_back(m);
	}
	ds::TileMapReader reader;
	reader.parse("content\\field.txt");
	float sx = reader.width() * 0.5f - 0.5f;
	float sy = -3.0f;
	float sz = reader.height() * 0.5f + 2;
	for (int y = reader.height() - 1; y >= 0; --y) {
		for (int x = 0; x < reader.width(); ++x) {
			int index = reader.get(x, y);
			ID id = _scene->add(_objects[index], v3(-sx + x, sy, sz - y));
		}
	}
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int WorldState::update(float dt) {
	_camera->update(dt);		
	if (ds::input::isMouseButtonPressed(0) && !_pressed) {
		_pressed = true;
		ds::Ray r = graphics::getCameraRay(_camera);
		ID id = _scene->intersects(r);
	}
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void WorldState::render() {
	// scene
	_scene->draw();
}

int WorldState::onChar(int ascii) {
	return 0;
}
