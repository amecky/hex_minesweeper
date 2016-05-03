#include "TestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	//_particles = ds::res::getParticleManager();
	_buffer = ds::res::getQuadBuffer(20);
	_camera = new ds::FPSCamera(1024, 768);
	_camera->setPosition(v3(0, 2, -12), v3(0, 0, 1));
	graphics::setCamera(_camera);
	_buttonPressed = false;
}


TestState::~TestState() {
	delete _camera;
}

void TestState::init() {

}

int TestState::update(float dt) {
	//_particles->update(dt);
	v2 mp;
	graphics::getMousePosition(&mp);
	_camera->update(dt, mp, _buttonPressed);
	return 0;
}

void TestState::render() {
	ds::Texture t = math::buildTexture(ds::Rect(510, 0, 256, 256));
	_buffer->begin();
	_buffer->draw(v3(-0.5f, 0.5f, 0.0f), t.getUV(0));
	_buffer->draw(v3(0.5f, 0.5f, 0.0f), t.getUV(1));
	_buffer->draw(v3(0.5f, -0.5f, 0.0f), t.getUV(2));
	_buffer->draw(v3(-0.5f, -0.5f, 0.0f), t.getUV(3));

	_buffer->draw(v3(0.5f,  0.5f, 0.0f), t.getUV(0));
	_buffer->draw(v3(0.5f,  0.5f, 1.0f), t.getUV(1));
	_buffer->draw(v3(0.5f, -0.5f, 1.0f), t.getUV(2));
	_buffer->draw(v3(0.5f, -0.5f, 0.0f), t.getUV(3));

	_buffer->draw(v3(-0.5f, 0.5f, 1.0f), t.getUV(0));
	_buffer->draw(v3( 0.5f, 0.5f, 1.0f), t.getUV(1));
	_buffer->draw(v3( 0.5f, 0.5f, 0.0f), t.getUV(2));
	_buffer->draw(v3(-0.5f, 0.5f, 0.0f), t.getUV(3));

	_buffer->draw(v3(-0.5f, 0.5f, 1.0f), t.getUV(0));
	_buffer->draw(v3(-0.5f, 0.5f, 0.0f), t.getUV(1));
	_buffer->draw(v3(-0.5f,-0.5f, 0.0f), t.getUV(2));
	_buffer->draw(v3(-0.5f,-0.5f, 1.0f), t.getUV(3));


	_buffer->end();
	//_particles->render();
}

int TestState::onButtonUp(int button, int x, int y) {
	_buttonPressed = false;
	return 0;
}

int TestState::onButtonDown(int button, int x, int y) {
	_buttonPressed = true;
	return 0;
}

int TestState::onKeyDown(WPARAM virtualKey) {
	return 0;
}

int TestState::onKeyUp(WPARAM virtualKey) {
	return 0;
}
