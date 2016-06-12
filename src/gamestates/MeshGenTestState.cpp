#include "MeshGenTestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <utils\ObjLoader.h>
#include "..\objects.h"
#include <renderer\graphics.h>
#include <utils\TileMapReader.h>


MeshGenTestState::MeshGenTestState(const char* meshName) : ds::GameState("MeshGenTestState"), _name(meshName) , _mesh(0) {
}


MeshGenTestState::~MeshGenTestState() {
	if (_mesh != 0) {
		delete _mesh;
	}
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void MeshGenTestState::init() {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_camera->setPosition(v3(0, 3, -6), v3(0.0f, 0.0f, 0.1f));
	_camera->resetPitch(DEGTORAD(25.0f));
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	_scene = ds::res::getScene("TestObjects");
	_pressed = false;
	_mesh = new ds::Mesh();
	_grid = new ds::Mesh();
	_grid->load("grid");
	// build squares to show vertices of the selected face
	ds::gen::MeshGen g;
	g.add_cube(v3(0, 0, 0), v3(0.1f, 0.1f, 0.1f));
	ds::Color clrs[] = { ds::Color(255,0,0,255),ds::Color(0,255,0,255),ds::Color(0,0,255,255),ds::Color(255,0,255,255) };
	for (int i = 0; i < 4; ++i) {
		_squares[i] = new ds::Mesh;
		for (int j = 0; j < 6; ++j) {
			g.set_color(j, clrs[i]);
		}
		g.build(_squares[i]);
		_square_ids[i] = _scene->add(_squares[i], v3(i * 1, 0, 0));
		ds::Entity& sqe = _scene->get(_square_ids[i]);
		sqe.active = false;
	}	
	gen.load_text(_name);
	gen.build(_mesh);
	_grid_id = _scene->add(_grid, v3(0.0f, -0.01f, 0.0f));
	ds::Entity& e = _scene->get(_grid_id);
	e.active = false;
	ID id = _scene->add(_mesh, v3(0, 0, 0));
	_mesh->save(_name);

	LOG << "'1' : reload";
	LOG << "'2' : debug";
	LOG << "'3' : toggle grid";
	LOG << "'4' : save mesh";
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int MeshGenTestState::update(float dt) {
	_camera->update(dt);	
	if (ds::input::isMouseButtonPressed(0) && !_pressed) {
		_pressed = true;
		ds::Ray r = graphics::getCameraRay(_camera);
		int selection = gen.intersects(r);
		if (selection != -1) {
			bool selected = gen.select_face(selection);
			_mesh->clear();
			gen.build(_mesh);
			gen.debug_face(selection);
			v3 p[4];
			const ds::gen::Face& f = gen.get_face(selection);
			gen.get_vertices(f, p);
			for (int i = 0; i < 4; ++i) {
				ds::Entity& sqe = _scene->get(_square_ids[i]);
				sqe.position = p[i];
				sqe.active = selected;
			}
		}
	}
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MeshGenTestState::render() {
	_scene->transform();
	_scene->draw();
}

// -------------------------------------------------------
// onChar
// -------------------------------------------------------
int MeshGenTestState::onChar(int ascii) {
	if (ascii == '1') {
		LOG << "reloading " << _name;
		gen.load_text(_name);
		_mesh->clear();
		gen.build(_mesh);
	}
	if (ascii == '2') {
		gen.debug();
	}
	if (ascii == '3') {
		ds::Entity& e = _scene->get(_grid_id);
		e.active = !e.active;
	}
	if (ascii == '4') {
		_mesh->save(_name);
		LOG << "Mesh saved";
	}
	if (ascii == '5') {
		LOG << "camera: " << DBG_V3(_camera->getPosition()) << " target: " << DBG_V3(_camera->getTarget());
	}
	return 0;
}
