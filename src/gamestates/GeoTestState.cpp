#include "GeoTestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <utils\ObjLoader.h>
#include "..\objects.h"
#include <renderer\graphics.h>
#include <utils\TileMapReader.h>

const int SIZE_X = 8;
const int SIZE_Y = 8;

GeoTestState::GeoTestState() : ds::GameState("GeoTestState") , _name("base_house") {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	_pressed = false;
	//_camera->setPosition(v3(0, -8, -21), v3(0, 0, 1));
	//_camera->resetPitch(DEGTORAD(355.0f));
	//_camera->resetYAngle();
	_mesh = new ds::Mesh();

	_ctx.gen = &gen;
	_ctx.mesh = _mesh;

	_gui = new ds::gen::MeshGenGUI("ortho",&_ctx);

	LOG << " ======================================================== ";
	_scene = ds::res::getScene("TestObjects");
	_buffer = ds::res::getMeshBuffer("TexturedBuffer");
	
	char buffer[32];

	//createHandrail(6.0f, 0.1f, 7 , 0.6f);
	//createCoords();
	//v3 p[] = {v3(-6.0f,-0.6f,6.0f),v3(6.0f,-0.6f,6.0f),v3(6.0f,-0.6f,-6.0f),v3(-6.0f,-0.6f,-6.0f)};
	//v3 p[] = { v3(-1,1,0), v3(1,1,0),v3(1,-1,0),v3(-1,-1,0) };
	/*
	uint16_t f = gen.add_cube(v3(0.0f, 0.0f, 0.0f), v3(1.0f, 0.1f, 1.0f));	
	gen.vsplit_edge(19, 0.4f);
	gen.vsplit_edge(27, 0.33f);
	gen.move_edge(24, v3(0.0f, 0.4f, 0.0f));
	gen.move_edge(26, v3(0.0f, 0.4f, 0.0f));
	uint16_t edges[] = { 25, 29, 4, 17 };
	gen.make_face(edges);
	uint16_t edges2[] = { 27, 19, 12, 31};
	gen.make_face(edges2);
	gen.save_text("roof_0");
	gen.save_bin("roof_0");
	gen.clear();
	*/
	//gen.load_text("roof_0");
	//gen.load_text(_name);
	//gen.scale_face(6, 0.5f);
	/*
	uint16_t slices[45];
	int num = gen.slice(0, 9, 5, slices, 32);
	for (int i = 0; i < 5; ++i) {
		uint16_t ei = gen.get_edge(23 + i * 2, 0);
		gen.move_edge(ei, v3(0.0f, 0.1f, 0.0f));
	}
	for (int i = 0; i < 5; ++i) {
		uint16_t ei = gen.get_edge(32 + i * 2, 0);
		gen.move_edge(ei, v3(0.0f, -0.1f, 0.0f));
	}
	for (int i = 0; i < 3; ++i) {
		uint16_t ei = gen.get_edge(7 + i * 18, 3);
		gen.move_edge(ei, v3(-0.05f, 0.0f, 0.0f));
	}
	for (int i = 0; i < 3; ++i) {
		uint16_t ei = gen.get_edge(7 + i * 18, 1);
		gen.move_edge(ei, v3(0.05f, 0.0f, 0.0f));
	}
	for (int i = 0; i < 3; ++i) {
		uint16_t ei = gen.get_edge(11 + i * 18, 3);
		gen.move_edge(ei, v3(-0.05f, 0.0f, 0.0f));
	}
	for (int i = 0; i < 3; ++i) {
		uint16_t ei = gen.get_edge(11 + i * 18, 1);
		gen.move_edge(ei, v3(0.05f, 0.0f, 0.0f));
	}
	for (int i = 0; i < num + 5; ++i) {
		gen.set_color(i, ds::Color(255, 252, 234, 255));
	}
	uint16_t roof = gen.add_cube(v3(0.0f, 1.05f, 0.0f), v3(1.2f, 0.1f, 1.2f));
	ds::gen::IndexList il;
	gen.find_adjacent_faces(roof, il);
	for (int i = 0; i < il.indices.size(); ++i) {
		gen.set_color(il.indices[i], ds::Color(58, 58, 58, 255));
	}
	//uint16_t re = gen.get_edge_index(il.indices[5], 3);
	uint16_t srf = gen.vsplit_edge(219);
	uint16_t sre = gen.get_edge_index(srf, 0);
	LOG << " SRF: " << srf << " SRE: " << sre;
	gen.move_edge(sre, v3(0.0f, 1.0f, 0.0f));
	gen.save_bin("House_base");
	gen.clear();
	gen.load_bin("House_base");
	ds::TileMapReader colorDefs;
	if (colorDefs.parse("content\\house_colors.txt")) {
		int tiles = colorDefs.height() / 5;
		for (int i = 0; i < tiles; ++i) {
			sprintf_s(buffer, 32, "house_%d", i);
			int offset = i * 45;
			int t = 4;
			for (int j = 0; j < 45; ++j) {
				if (j == 0) {
					t = 4;
				}
				else {
					t = 5 + j;
				}
				if (colorDefs.get(offset + j) == 0) {
					gen.set_color(t, ds::Color(255, 252, 234, 255));
				}
				else if (colorDefs.get(offset + j) == 1) {
					gen.set_color(t, ds::Color(157, 98, 66, 255));
				}
				else if (colorDefs.get(offset + j) == 2) {
					gen.set_color(t, ds::Color(153, 217, 234, 255));
				}
				else if (colorDefs.get(offset + j) == 3) {
					gen.set_color(t, ds::Color(58, 58, 58, 255));
				}
			}
			gen.save_mesh(buffer);
		}
	}
	*/
	//gen.load_text("roof_0");
	//gen.save_mesh("roof_0");
	//gen.clear();
	//gen.load_text("window_0");
	//createWindow(v3(0,0,0));
	/*
	gen.add_cube(v3(0, 0, 0), v3(1.0f,0.6f,1.0f));
	gen.build(_mesh);
	_mesh->load("window_1", v3(-0.25f, 0.0f, -0.54f));
	_mesh->load("window_1", v3(0.25f, 0.0f, -0.54f));
	_mesh->load("roof_0", v3(0.0f, 0.35f, 0.0f));
	ID id = _scene->add(_mesh, v3(0, 0, 0));
	_mesh->buildBoundingBox();
	_mesh->save("house_0");
	*/
	//buildTestTerrain();
	//buildTerrain();
	gen.add_cube(v3(0, 0, 0), v3(1.0f, 0.1f, 1.0f));
	gen.add_cube(v3(0.0f, 0.35f, 0.0f), v3(1.0f, 0.6f, 1.0f));
	//gen.slice(0, 3);
	gen.scale_face(10, 0.1f);
	gen.add_cube(v3(0.0f, 0.70f, 0.0f), v3(1.2f, 0.1f, 1.2f));
	gen.hsplit_edge(64, 0.45f);
	gen.hsplit_edge(72, 0.05f);
	gen.move_edge(65, v3(0.0f, 0.4f, 0.0f));
	gen.move_edge(79, v3(0.0f, 0.4f, 0.0f));
	gen.hsplit_edge(68, 0.45f);
	//gen.move_edge(81, v3(0.0f, 0.4f, 0.0f));
	gen.debug_colors();
	//gen.load_text(_name);
	gen.build(_mesh);
	ID id = _scene->add(_mesh, v3(0, 0, 0));
	_mesh->save(_name);
}



GeoTestState::~GeoTestState() {
	_objects.destroy_all();
	delete _gui;
	delete _mesh;
}

// ------------------------------------------
// build terrain
// ------------------------------------------
void GeoTestState::buildTerrain() {
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
	ds::TileMapReader reader;
	reader.parse("content\\field.txt");
	float sx = reader.width() * 0.5f - 0.5f;
	float sy = -3.0f;
	float sz = reader.height() * 0.5f + 2;
	for (int y = reader.height() - 1; y >= 0; --y) {
		for (int x = 0; x < reader.width(); ++x) {
			int index = reader.get(x, y);
			if (index > 11) {
				sy = -2.65f;
			}
			else {
				sy = -3.0f;
			}
			ID id = _scene->add(_objects[index], v3(-sx + x, sy, sz - y));
			_ids.push_back(id);
		}
	}
}

// ------------------------------------------
// build TEST terrain
// ------------------------------------------
void GeoTestState::buildTestTerrain() {
	ds::Mesh* m = new ds::Mesh();
	m->load("tile_0");
	_objects.push_back(m);
	float sx = 3.0f * 0.5f - 0.5f;
	float sy = 3.0f * 0.5f + 2;
	for (int y = 3 - 1; y >= 0; --y) {
		for (int x = 0; x < 3; ++x) {
			_scene->add(m, v3(-sx + x, -3, sy - y));
		}
	}
}

void GeoTestState::createHandrail(float length, float griderSize, int segments, float height) {
	uint16_t faces[16];
	int cnt = 0;
	faces[cnt++] = gen.add_cube(v3(0, 0, 0), v3(length, griderSize, griderSize), v3(DEGTORAD(45.0f), 0.0f, 0.0f));
	int s = (segments - 1) / 2;
	float il = (length - griderSize * 4.0f);
	float step = il * 0.5f / s;	
	for (int i = -s; i < s + 1; ++i) {
		faces[cnt++] = gen.add_cube(v3(step * i, -height * 0.5f - griderSize * 0.5f, 0.0f), v3(griderSize, height, griderSize));
	}
	for (int i = 0; i < cnt; ++i) {
		gen.set_color(faces[i], ds::Color(128, 128, 128, 255));
	}
	//gen.save_bin("Test.data");
	//gen.save_text("Test.txt");
	gen.save_mesh("handrail");
}

void GeoTestState::createCoords() {
	// coordsystem
	uint16_t faces[6];
	gen.add_cube(v3(0.5f, 0.0f, 0.0f), v3(1.0f, 0.1f, 0.1f), faces);
	for (int i = 0; i < 6; ++i) {
		gen.set_color(faces[i], ds::Color(255, 0, 0, 255));
	}
	gen.add_cube(v3(0.05f, 0.5f, 0.0f), v3(0.1f, 0.9f, 0.1f), faces);
	for (int i = 0; i < 6; ++i) {
		gen.set_color(faces[i], ds::Color(0, 255, 0, 255));
	}
	gen.add_cube(v3(0.05f, 0.0f, 0.5f), v3(0.1f, 0.1f, 0.9f), faces);
	for (int i = 0; i < 6; ++i) {
		gen.set_color(faces[i], ds::Color(0, 0, 255, 255));
	}
}

void GeoTestState::createGriderBox(ds::gen::MeshGen* gen, float dim, float griderSize) {
	float size = dim;
	float hs = size * 0.5f;
	float w = griderSize;
	float hw = w * 0.5f;
	gen->add_cube(v3(-hs + hw, 0.0f,-hs + hw), v3(w, size, w));
	gen->add_cube(v3( hs - hw, 0.0f,-hs + hw), v3(w, size, w));
	gen->add_cube(v3(-hs + hw, 0.0f, hs - hw), v3(w, size, w));
	gen->add_cube(v3( hs - hw, 0.0f, hs - hw), v3(w, size, w));

	gen->add_cube(v3(0.0f, hs - hw, hs - hw), v3(size - 2.0f * w, w, w));
	gen->add_cube(v3(0.0f,-hs + hw,-hs + hw), v3(size - 2.0f * w, w, w));
	gen->add_cube(v3(0.0f, hs - hw,-hs + hw), v3(size - 2.0f * w, w, w));
	gen->add_cube(v3(0.0f,-hs + hw, hs - hw), v3(size - 2.0f * w, w, w));

	gen->add_cube(v3( hs - hw,-hs + hw, 0.0f), v3(w, w, size - 2.0f * w));
	gen->add_cube(v3( hs - hw, hs - hw, 0.0f), v3(w, w, size - 2.0f * w));
	gen->add_cube(v3(-hs + hw,-hs + hw, 0.0f), v3(w, w, size - 2.0f * w));
	gen->add_cube(v3(-hs + hw, hs - hw, 0.0f), v3(w, w, size - 2.0f * w));
}
// -------------------------------------------------------
// update
// -------------------------------------------------------
int GeoTestState::update(float dt) {
	_camera->update(dt);	
	_gui->handleClick(_camera);
	/*
	if (ds::input::isMouseButtonPressed(0) && !_pressed) {
		_pressed = true;
		ds::Ray r = graphics::getCameraRay(_camera);
		ID id = _scene->intersects(r);
	}
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	*/
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void GeoTestState::render() {
	// scene
	_scene->draw();
	_gui->drawGUI();
}

int GeoTestState::onChar(int ascii) {
	if (ascii == 'r') {
		gen.load_text(_name);
		//gen.debug();
		_mesh->clear();
		gen.build(_mesh);
	}
	return 0;
}

void GeoTestState::drawGUI() {
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	v2 pos(10, 760);
	int state = 1;
	gui::start(1, &pos, true);
	gui::begin("Camera", &state);
	char buffer[128];
	sprintf_s(buffer, 128, "%2.2f %2.2f %2.2f", _camera->getPosition().x, _camera->getPosition().y, _camera->getPosition().z);
	gui::Value("Pos", buffer);
	gui::Value("Yaw", RADTODEG(_camera->getAngle()));
	gui::Value("Pitch", RADTODEG(_camera->getPitch()));
	if (gui::Button("Reset")) {
		_camera->setPosition(v3(0, 0, -10), v3(0, 0, 1));
		_camera->resetYAngle();
	}
	gui::end();
}

// -------------------------------------------
// Create street tiles
// -------------------------------------------
void GeoTestState::createStreets() {
	char buffer[32];
	ds::TileMapReader colorDefs;
	if (colorDefs.parse("content\\tile_colors.txt")) {
		int tiles = colorDefs.height() / 5;
		for (int i = 0; i < tiles; ++i) {
			gen.clear();
			uint16_t f = gen.add_cube(v3(0.0f, 0.0f, 0.0f), v3(1.0f, 0.1f, 1.0f));
			uint16_t slices[45];
			int num = gen.slice(4, 5, slices, 32);
			gen.move_edge(25, v3(-0.1f, 0.0f, 0.0f));
			gen.move_edge(65, v3(-0.1f, 0.0f, 0.0f));
			gen.move_edge(105, v3(-0.1f, 0.0f, 0.0f));

			gen.move_edge(35, v3(0.1f, 0.0f, 0.0f));
			gen.move_edge(75, v3(0.1f, 0.0f, 0.0f));
			gen.move_edge(115, v3(0.1f, 0.0f, 0.0f));

			gen.move_edge(42, v3(0.0f, 0.0f, 0.1f));
			gen.move_edge(50, v3(0.0f, 0.0f, 0.1f));
			gen.move_edge(58, v3(0.0f, 0.0f, 0.1f));

			gen.move_edge(62, v3(0.0f, 0.0f, -0.1f));
			gen.move_edge(70, v3(0.0f, 0.0f, -0.1f));
			gen.move_edge(78, v3(0.0f, 0.0f, -0.1f));

			gen.set_color(0, ds::Color(157, 98, 66, 255));
			gen.set_color(1, ds::Color(157, 98, 66, 255));
			gen.set_color(2, ds::Color(157, 98, 66, 255));
			gen.set_color(3, ds::Color(157, 98, 66, 255));
			gen.set_color(5, ds::Color(157, 98, 66, 255));
			gen.recalculate_normals();

			sprintf_s(buffer, 32, "tile_%d", i);
			int offset = i * 25;
			for (int j = 0; j < num; ++j) {
				if (colorDefs.get(offset + j) == 0) {
					gen.set_color(slices[j], ds::Color(184, 203, 98, 255));
				}
				else if (colorDefs.get(offset + j) == 1) {
					gen.set_color(slices[j], ds::Color(223, 215, 204, 255));
				}
				if (colorDefs.get(offset + j) == 2) {
					gen.set_color(slices[j], ds::Color(151, 144, 138, 255));
				}
			}
			gen.save_text(buffer);
			gen.save_mesh(buffer);
		}
	}
}

void GeoTestState::createWindow(const v3& center) {
	ds::gen::MeshGen wgen;
	float size = 0.4f;
	float hs = size * 0.5f;
	float w = 0.04f;
	float hw = w * 0.5f;
	wgen.add_cube(v3(center.x - hs + hw, 0.0f, center.z), v3(w, size, w));
	wgen.add_cube(v3(center.x + hs - hw, 0.0f, center.z), v3(w, size, w));
	wgen.add_cube(v3(center.x, hs - hw, center.z), v3(size - 2.0f * w, w, w));
	wgen.add_cube(v3(center.x, -hs + hw, center.z), v3(size - 2.0f * w, w, w));
	v3 p[] = {
		v3(center.x - hs + hw * 2.0f,hs -hw,center.z + hw),
		v3(center.x + hs - hw * 2.0f,hs - hw,center.z + hw),
		v3(center.x + hs - hw * 2.0f,-hs +hw,center.z + hw),
		v3(center.x - hs + hw * 2.0f,-hs + hw,center.z + hw) };
	uint16_t window = wgen.add_face(p);
	for (int i = 0; i < wgen.num_faces(); ++i) {
		wgen.set_color(i, ds::Color(157, 98, 66, 255));
	}
	wgen.set_color(window, ds::Color(196, 215, 213, 255));
	wgen.save_mesh("window_1");
}