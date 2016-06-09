#include "WorldState.h"
#include <utils\Log.h>
#include <base\InputSystem.h>
#include <renderer\graphics.h>
#include <utils\TileMapReader.h>

const int WORLD_SIZE = 6;

WorldState::WorldState() : ds::GameState("WorldState"), _mesh(0) {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_pressed = false;
	_world = new TinyWorld(WORLD_SIZE);
}

void WorldState::init() {
	_camera->setPosition(v3(12, 6, -9), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(30.0f));
	_camera->setYAngle(DEGTORAD(-45.0f));
	_mesh = new ds::Mesh();
	_scene = ds::res::getScene("TestObjects");
	loadObjects();	
	_world->addHouse(2, 5);
	
	//_world->addHouse(10, 7);
	//_world->addHouse(12, 12);
	//_world->addForrest(3, 3, 3);
	v2 wp[36];
	int num = _world->connect(0, 1, 5, 4, wp, 36);
	for (int i = 1; i < num - 1; ++i) {
		v2 p = wp[i];
		_world->addStreet(p.x, p.y);
	}
	_world->addHouse(0, 1);
	_world->addHouse(5, 4);
	buildTerrain();
}



WorldState::~WorldState() {
	delete _world;
	_objects.destroy_all();
	if (_mesh != 0) {
		delete _mesh;
	}
}

// ------------------------------------------
// build terrain
// ------------------------------------------
void WorldState::loadObjects() {
	char buffer[32];
	for (int i = 0; i < 16; ++i) {
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
}

void WorldState::buildTerrain() {
	float sx = WORLD_SIZE / 2.0f;// -8.0f;
	float sz = WORLD_SIZE / 2.0f;// -8.0f;
	for (int z = 0; z < WORLD_SIZE; ++z) {
		for (int x = 0; x < WORLD_SIZE; ++x) {
			const Tile& t = _world->get(x, z);
			switch (t.type) {
				case WT_EMPTY: _scene->addStatic(_objects[0], v3(sx + x, -3.0f, sz + z)); break;
				case WT_HOUSE: _scene->addStatic(_objects[16], v3(sx + x, -3.0f, sz + z)); break;
				case WT_STREET: _scene->addStatic(_objects[1], v3(sx + x, -3.0f, sz + z)); break;
				case WT_TREE: _scene->addStatic(_objects[18], v3(sx + x, -3.0f, sz + z)); break;
			}
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
	ZoneTracker("WSR");
	_scene->draw();
}

int WorldState::onChar(int ascii) {
	return 0;
}
