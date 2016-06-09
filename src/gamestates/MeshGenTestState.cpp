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
	_pressed = false;
	_mesh = new ds::Mesh();
	_grid = new ds::Mesh();
	_grid->load("grid");
	_scene = ds::res::getScene("TestObjects");
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
			gen.select_face(selection);
			_mesh->clear();
			gen.build(_mesh);
			gen.debug_face(selection);
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
