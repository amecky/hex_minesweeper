#include "TestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	//_particles = ds::res::getParticleManager();
	_camera = new ds::FPSCamera(1024, 768);
	_camera->setPosition(v3(0, 2, -12), v3(0, 0, 1));
	graphics::setCamera(_camera);
	_orthoCamera = new ds::OrthoCamera(1024, 768);
	
	_cubes = ds::res::getMesh(21);

	ds::geometrics::createCube(_cubes, ds::Rect(510, 0, 256, 256), v3(-6,-2,-1));
	ds::geometrics::createCube(_cubes, ds::Rect(510, 0, 256, 256), v3(-6, -2, 8));

	for (int i = 0; i < 5; ++i) {
		ds::geometrics::createCube(_cubes, ds::Rect(510, 0, 256, 256),v3(0,-2 + i,0));
	}
	ds::geometrics::createCube(_cubes,ds::Rect(510, 0, 256, 256),v3(-2,0,0), v3(2,2,2));
	ds::geometrics::createCube(_cubes, ds::Rect(510, 0, 256, 256), v3(2, 0, 0), v3(0.5f, 0.5f, 0.5f));
	ds::geometrics::createCube(_cubes, ds::Rect(510, 0, 256, 256), v3(2, 0, 2), v3(0.5f, 0.5f, 0.5f));
	ds::geometrics::createCube(_cubes, ds::Rect(510, 0, 256, 256), v3(2, 0, 4), v3(0.5f, 0.5f, 0.5f));
	_timer = 0.0f;

	for (int i = 0; i < 10; ++i) {
		_states[i] = 0;
	}
}


TestState::~TestState() {
	delete _camera;
}

void TestState::init() {
	
}

int TestState::update(float dt) {
	//_particles->update(dt);
	v2 mp = ds::input::getMousePosition();
	_camera->update(dt, mp);
	_timer += dt;
	if (_states[1] == 1) {
		_cubes->rotateX(_timer);
	}
	if (_states[2] == 1) {
		_cubes->rotateY(_timer);
	}
	if (_states[3] == 1) {
		_cubes->rotateZ(_timer);
	}
	if (_states[0] == 1) {
		v3 p;
		p.x = sin(_timer) * 2.0f;
		_cubes->translate(p);
	}
	if (_states[4] == 1) {
		float s = 0.8f + sin(_timer) * 0.2f;
		_cubes->scale(v3(s, s, s));
	}
	return 0;
}

void TestState::render() {
	graphics::setCamera(_camera);
	graphics::turnOnZBuffer();
	_cubes->draw();
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	v2 pos(10, 760);
	int state = 1;
	gui::start(1, &pos, true);
	gui::begin("Camera", &state);
	char buffer[128];
	sprintf_s(buffer, 128, "%2.2f %2.2f %2.2f", _camera->getPosition().x, _camera->getPosition().y, _camera->getPosition().z);
	gui::Value("Pos", buffer);
	gui::Value("Angle", RADTODEG(_camera->getAngle()));
	if (gui::Button("Reset")) {
		_camera->setPosition(v3(0, 2, -12), v3(0, 0, 1));
		_camera->resetYAngle();
	}
	gui::end();
}

int TestState::onKeyDown(WPARAM virtualKey) {
	return 0;
}

int TestState::onKeyUp(WPARAM virtualKey) {
	return 0;
}

int TestState::onChar(int ascii) {
	if (ascii == '1') {
		if (_states[0] == 0) {
			_states[0] = 1;
		}
		else {
			_states[0] = 0;
			_cubes->translate(v3(0, 0, 0));
		}
	}
	if (ascii == '2') {
		if (_states[1] == 0) {
			_states[1] = 1;
		}
		else {
			_states[1] = 0;
			_cubes->rotateX(0.0f);
		}
	}
	if (ascii == '3') {
		if (_states[2] == 0) {
			_states[2] = 1;
		}
		else {
			_states[2] = 0;
			_cubes->rotateY(0.0f);
		}
	}
	if (ascii == '4') {
		if (_states[3] == 0) {
			_states[3] = 1;
		}
		else {
			_states[3] = 0;
			_cubes->rotateZ(0.0f);
		}
	}
	if (ascii == '5') {
		if (_states[4] == 0) {
			_states[4] = 1;
		}
		else {
			_states[4] = 0;
			_cubes->scale(v3(1, 1, 1));
		}
	}
	return 0;
}
