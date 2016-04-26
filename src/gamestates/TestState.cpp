#include "TestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	_particles = ds::res::getParticleManager();
}


TestState::~TestState() {
}

void TestState::init() {

}

int TestState::update(float dt) {
	_particles->update(dt);
	return 0;
}

void TestState::render() {
	_particles->render();
}

int TestState::onButtonUp(int button, int x, int y) {
	_particles->start(1, v2(x, y));
	return 0;
}