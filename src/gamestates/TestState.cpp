#include "TestState.h"
#include <core\log\Log.h>
#include <core\base\EventStream.h>

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
	/*
	ds::ColorModuleData* cm = (ds::ColorModuleData*)_particles->getParticleSystem(1)->getData(ds::PM_COLOR);
	cm->color = ds::Color(192, 0, 0, 255);
	cm = (ds::ColorModuleData*)_particles->getParticleSystem(2)->getData(ds::PM_COLOR);
	cm->color = ds::Color(255, 0, 0, 255);
	cm = (ds::ColorModuleData*)_particles->getParticleSystem(3)->getData(ds::PM_COLOR);
	cm->color = ds::Color(255, 64, 0, 255);
	*/
	_particles->start(1, v2(x, y));
	_particles->start(2, v2(x, y));
	_particles->start(3, v2(x, y));
	return 0;
}