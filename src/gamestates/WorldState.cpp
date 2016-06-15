#include "WorldState.h"
#include <utils\Log.h>
#include <base\InputSystem.h>
#include <renderer\graphics.h>
#include <utils\TileMapReader.h>

WorldState::WorldState() : ds::GameState("WorldState"), _mesh(0) {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_pressed = false;
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	_world = new TinyWorld(WORLD_SIZE);
	_switch = false;
	_selectedCell = p2i(-1, -1);
	_buffer = ds::res::getMeshBuffer("ColouredBuffer");
}

void WorldState::init() {
	_camera->setPosition(v3(0, 2, -5), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(30.0f));
	_camera->setYAngle(DEGTORAD(-45.0f));
	_mesh = new ds::Mesh();
	_scene = ds::res::getScene("World");
	loadObjects();	
	p2i hp[] = { p2i(2, 5), p2i(0, 1), p2i(10, 10), p2i(4,5),p2i(8,5) };
	for (int i = 0; i < 5; ++i) {
		_world->addHouse(hp[i]);
	}	
	_world->addForrest(3, 8, 3);
	_world->addPowerPlant(p2i(0, 2));
	_world->addPowerPlant(p2i(3, 5));
	_world->addPowerPlant(p2i(1, 5));
	_world->add(p2i(6, 5),TileType::WM_WOOD_PLANT);
	_world->add(p2i(5, 5), TileType::WM_WAREHOUSE);
	_world->add(p2i(0, 2), TileType::WT_TREE);
	_world->add(p2i(0, 3), TileType::WT_TREE);
	_world->add(p2i(0, 4), TileType::WT_TREE);
	_world->connect(hp[0], hp[2]);
	_world->connect(hp[1], hp[2]);
	_world->connect(hp[3], hp[4]);
	_world->rebuildStreets();
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
// activate
// ------------------------------------------
void WorldState::activate() {
	_camera->setPosition(v3(0, 5, -3), v3(0, 0, 0));
	_camera->resetPitch(DEGTORAD(30.0f));
	_camera->resetYaw(0.0f);
}

// ------------------------------------------
// load objects
// ------------------------------------------
void WorldState::loadObjects() {
	char buffer[32];
	for (int i = 0; i < 16; ++i) {
		sprintf_s(buffer, 32, "tile_%d", i);
		loadObject(buffer);
	}
	loadObject("tower");
	for (int i = 0; i < 2; ++i) {
		sprintf_s(buffer, 32, "tree_%d", i);
		loadObject(buffer);
	}
	loadObject("power_plant");
	loadObject("wood_plant");
	loadObject("warehouse");
}

// ------------------------------------------
// load object
// ------------------------------------------
void WorldState::loadObject(const char* name) {
	ds::Mesh* mp = new ds::Mesh();
	mp->load(name);
	_objects.push_back(mp);
}

// ------------------------------------------
// build terrain
// ------------------------------------------
void WorldState::buildTerrain() {
	float sx = WORLD_SIZE / 2.0f;// -8.0f;
	float sz = WORLD_SIZE / 2.0f;// -8.0f;
	for (int z = 0; z < WORLD_SIZE; ++z) {
		for (int x = 0; x < WORLD_SIZE; ++x) {
			const Tile& t = _world->get(x, z);
			WorldTile& wt = _tiles[x + z * WORLD_SIZE];
			wt.coord = p2i(x, z);
			switch (t.type) {
				case WT_EMPTY: wt.id = _scene->addStatic(_objects[0], v3(-sx + x, -3.0f, sz + z)); break;
				case WT_HOUSE: wt.id = _scene->addStatic(_objects[16], v3(-sx + x, -3.0f, sz + z)); break;
				case WM_POWER_PLANT: wt.id = _scene->addStatic(_objects[19], v3(-sx + x, -3.0f, sz + z)); break;
				case WM_WOOD_PLANT: wt.id = _scene->addStatic(_objects[20], v3(-sx + x, -3.0f, sz + z)); break;
				case WM_WAREHOUSE: wt.id = _scene->addStatic(_objects[21], v3(-sx + x, -3.0f, sz + z)); break;
				case WT_STREET: wt.id = _scene->addStatic(_objects[t.index], v3(-sx + x, -3.0f, sz + z)); break;
				case WT_TREE: wt.id = _scene->addStatic(_objects[18], v3(-sx + x, -3.0f, sz + z)); break;
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
		if (id == INVALID_ID) {
			_selectedCell = p2i(-1, -1);
		}
		else {
			int total = WORLD_SIZE * WORLD_SIZE;
			for (int i = 0; i < total; ++i) {
				if (_tiles[i].id == id) {
					LOG << "selected at " << _tiles[i].coord.x << " / " << _tiles[i].coord.y;
					const Tile& t = _world->get(_tiles[i].coord);
					LOG << "Building: " << t.type;
					_selectedCell = _tiles[i].coord;
				}
			}
		}
	}
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	if (_switch) {
		_switch = false;
		return 1;
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void WorldState::render() {
	ZoneTracker("WSR");
	_scene->draw();
	drawGUI();
}

// -------------------------------------------------------
// drawGUI
// -------------------------------------------------------
void WorldState::drawGUI() {
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	v2 pos(10, 760);
	int state = 1;
	gui::start(1, &pos, true);
	gui::begin("World", &state);
	gui::InputVec3("Light", _buffer->getLightPos());
	gui::beginGroup();
	if (gui::Button("Editor")) {
		_switch = true;
	}
	if (gui::Button("House")) {
		if (_selectedCell.x != -1 && _selectedCell.y != -1) {
			const Tile& t = _world->get(_selectedCell);
			if (t.type == TileType::WT_EMPTY) {
				float sx = WORLD_SIZE / 2.0f;// -8.0f;
				float sz = WORLD_SIZE / 2.0f;// -8.0f;
				WorldTile& wt = _tiles[_selectedCell.x + _selectedCell.y * WORLD_SIZE];
				_scene->remove(wt.id);
				wt.id = _scene->addStatic(_objects[16], v3(-sx + _selectedCell.x, -3.0f, sz + _selectedCell.y));
			}
		}
	}
	gui::endGroup();
	gui::end();
}

// ------------------------------------------
// onChar
// ------------------------------------------
int WorldState::onChar(int ascii) {
	if (ascii == '1') {
		if (_selectedCell.x != -1 && _selectedCell.y != -1) {
			Tile& t = _world->get(_selectedCell);
			if (t.type == TileType::WT_EMPTY) {
				float sx = WORLD_SIZE / 2.0f;// -8.0f;
				float sz = WORLD_SIZE / 2.0f;// -8.0f;
				WorldTile& wt = _tiles[_selectedCell.x + _selectedCell.y * WORLD_SIZE];
				_scene->remove(wt.id);
				wt.id = _scene->addStatic(_objects[16], v3(-sx + _selectedCell.x, -3.0f, sz + _selectedCell.y));
				t.type = TileType::WT_HOUSE;
			}
		}
	}
	return 0;
}
