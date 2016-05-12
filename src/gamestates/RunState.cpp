#include "RunState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>
#include <utils\ObjLoader.h>

RunState::RunState(GameContext* context) : ds::GameState("RunState"), _context(context) {
	_camera = new ds::FPSCamera(1024, 768);
	_camera->setPosition(v3(0, 8, -21), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(5.0f));
	_camera->resetYAngle();
	graphics::setCamera(_camera);
	_orthoCamera = new ds::OrthoCamera(1024, 768);
	
	_texturedBuffer = ds::res::getMeshBuffer(21);

	_cubes = new ds::Mesh();
	ds::geometrics::createGrid(_cubes, 2.0f, 12, 4, ds::Rect(510, 0, 256, 256) , v3(-12, 0, -4));
	_board = new Board(context->settings, _camera);
}


RunState::~RunState() {
	delete _cubes;
	delete _board;
	delete _camera;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void RunState::init() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void RunState::activate() {
	//_context->resetScore();
	_board->rebuild();
	//_timer = 0.0f;
	//_running = true;
	//_effect->deactivate();
	// FIXME: reset score
	//_context->hud.reset();
	//_context->hud.activate();
	_pressed = false;
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int RunState::update(float dt) {
	v2 mp = ds::input::getMousePosition();
	_camera->update(dt, mp);
	_board->update(dt);
	if (ds::input::isMouseButtonPressed(0) && !_pressed) {
		_pressed = true;
		_board->select();
	}
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void RunState::render() {
	// scene
	graphics::setCamera(_camera);
	graphics::turnOnZBuffer();	
	_texturedBuffer->drawImmediate(_cubes,v3(0,0,0));	
	_board->render();
	// GUI
	drawGUI();	
}

// -------------------------------------------------------
// draw GUI
// -------------------------------------------------------
void RunState::drawGUI() {
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
		_camera->setPosition(v3(0, 2, -12), v3(0, 0, 1));
		_camera->resetYAngle();
	}
	if (gui::Button("Rebuild")) {
		_board->rebuild();
	}
	gui::end();
}
