#include "WorldState.h"
#include <utils\Log.h>
#include <base\InputSystem.h>
#include <renderer\graphics.h>
#include <utils\TileMapReader.h>
#include <io\FileRepository.h>
#include <io\BinaryFile.h>

WorldState::WorldState() : ds::GameState("WorldState"), _mesh(0) , _selectionMesh(0) {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_pressed = false;
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	_world = new TinyWorld(WORLD_SIZE);
	_switch = false;
	_selectedCell = p2i(-1, -1);
	_buffer = ds::res::getMeshBuffer("ColouredBuffer");
	_meshID = 200;
	_gridPos = p2i(WORLD_SIZE / 2, WORLD_SIZE / 2);
	int total = WORLD_SIZE * WORLD_SIZE;
	for (int i = 0; i < total; ++i) {
		_tiles[i].height = 0.0f;		
	}
}

void WorldState::init() {
	_camera->setPosition(v3(0, 2, -5), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(30.0f));
	_camera->setYAngle(DEGTORAD(-45.0f));
	_mesh = new ds::Mesh();
	_scene = ds::res::getScene("World");
	_selectionMesh = new ds::Mesh;
	ds::gen::MeshGen gn;
	v3 p[] = { v3(-0.5f,0.0f,0.5f),v3(0.5f,0.0f,0.5f),v3(0.5f,0.0f,-0.5f),v3(-0.5f,0.0f,-0.5f) };
	gn.add_face(p);
	gn.set_color(0, ds::Color(128, 128, 128, 255));
	gn.build(_selectionMesh);
	_selectionID = _scene->add(_selectionMesh, v3(0, 0, 0),ds::DrawMode::IMMEDIATE);
	//parseFile();

}



WorldState::~WorldState() {
	delete _world;
	if (_selectionMesh != 0) {
		delete _selectionMesh;
	}
	_objects.destroy_all();
	if (_mesh != 0) {
		delete _mesh;
	}
}

void WorldState::parseFile() {
	int size = -1;
	const char* txt = ds::repository::load("content\\world.txt", &size);
	if (size != -1) {
		ds::Tokenizer t;
		t.parse(txt);
		//assert(t.size() % 4 == 0);
		int total = t.size() / 4;
		for ( int i = 0; i < total; ++i ) {
			const ds::Token& td = t.get(i * 4);
			const ds::Token& tx = t.get(i * 4 + 1);
			const ds::Token& ty = t.get(i * 4 + 2);
			LOG << i << " td: " << td.value << " tx: " << tx.value << " ty: " << ty.value;
			float sx = WORLD_SIZE / 2.0f;
			float sz = WORLD_SIZE / 2.0f;
			ds::Mesh* m = ds::res::getMesh(td.value);
			ds::AABBox box = m->boundingBox;
			float h = box.extent.y * 0.5f;
			int idx = tx.value + ty.value * WORLD_SIZE;
			WorldTile& t = _tiles[idx];
			h = t.height;
			t.id = _scene->addStatic(m, v3(-sx + tx.value, h, sz + ty.value));
			t.height += box.extent.y * 2.0f;
			LOG << "t.height: " << t.height << " h: " << h;
		}
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
// build terrain
// ------------------------------------------
void WorldState::buildTerrain() {
	/*
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
				case WM_CITY_HALL: wt.id = _scene->addStatic(_objects[22], v3(-sx + x, -3.0f, sz + z)); break;
				case WT_STREET: wt.id = _scene->addStatic(_objects[t.index], v3(-sx + x, -3.0f, sz + z)); break;
				case WT_TREE: wt.id = _scene->addStatic(_objects[18], v3(-sx + x, -3.0f, sz + z)); break;
			}			
		}
	}
	*/
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
					_selectedCell = _tiles[i].coord;
					float sx = WORLD_SIZE / 2.0f;
					float sz = WORLD_SIZE / 2.0f;
					ds::Entity& e = _scene->get(_selectionID);
					e.position = v3(-sx + _selectedCell.x, -2.99f, sz + _selectedCell.y);
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
	_scene->transform();
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
	gui::endGroup();
	gui::InputInt("ID", &_meshID);
	gui::Input("Pos", &_gridPos);
	if (gui::Button("Add")) {
		TileCommand tc;
		tc.id = _meshID;
		tc.coord = _gridPos;
		process(tc);
		_commands.push_back(tc);
	}
	gui::beginGroup();
	if (gui::Button("Save")) {
		save();
	}
	if (gui::Button("Load")) {
		load();
	}
	gui::endGroup();
	gui::end();
}

// ------------------------------------------
// save
// ------------------------------------------
void WorldState::save() {
	ds::BinaryFile bf;
	bf.open("world", ds::FileMode::WRITE);
	bf.write(_commands.size());
	for (uint32_t i = 0; i < _commands.size(); ++i) {
		const TileCommand& tc = _commands[i];
		bf.write(tc.id);
		bf.write(tc.coord.x);
		bf.write(tc.coord.y);
	}
}

// ------------------------------------------
// load
// ------------------------------------------
void WorldState::load() {
	ds::BinaryFile bf;
	bf.open("world", ds::FileMode::READ);
	int size = 0;
	bf.read(&size);
	for (int i = 0; i < size; ++i) {
		TileCommand tc;
		bf.read(&tc.id);
		bf.read(&tc.coord.x);
		bf.read(&tc.coord.y);
		_commands.push_back(tc);
		process(tc);
	}
}

// ------------------------------------------
// process TileCommand
// ------------------------------------------
void WorldState::process(const TileCommand& command) {
	float sx = WORLD_SIZE / 2.0f;
	float sz = WORLD_SIZE / 2.0f;
	ds::Mesh* m = ds::res::getMesh(command.id);
	ds::AABBox box = m->boundingBox;
	float h = box.extent.y * 0.5f;
	int idx = command.coord.x + command.coord.y * WORLD_SIZE;
	WorldTile& t = _tiles[idx];
	h = t.height;
	t.id = _scene->addStatic(m, v3(-sx + command.coord.x, h, sz + command.coord.y));
	t.height += box.extent.y * 2.0f;
	LOG << "t.height: " << t.height << " h: " << h;
}

// ------------------------------------------
// onChar
// ------------------------------------------
int WorldState::onChar(int ascii) {	
	return 0;
}
