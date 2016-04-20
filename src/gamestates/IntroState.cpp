#include "IntroState.h"
/*
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

IntroState::IntroState(GameContext* context) : ds::GameState("Intro"), _context(context) {
	ds::SparkleEffectSettings settings;
	settings.gap = 1.0f;
	settings.max_x = 66;
	settings.max_y = 28;
	settings.square_size = 10;
	settings.startScale = 1.0f;
	settings.endScale = 0.2f;
	settings.ttl = 3.0f;
	settings.velocity = 60.0f;
	settings.velocityVariance = 20.0f;
	settings.warmupTimer = 2.0f;
	_effect = new ds::IntroEffect(settings);
}


IntroState::~IntroState() {
	delete _effect;
}

// --------------------------------------------
// activate
// --------------------------------------------
void IntroState::activate() {
	_timer = 0.0f;
	_effect->start(512, 384, ds::Rect(0, 0, 512, 210), 64, 42, 1.0f, 512.0f);
	ds::sprites::setTexture(1);
}

// --------------------------------------------
// activate
// --------------------------------------------
void IntroState::deactivate() {
	ds::sprites::setTexture(0);
}

// --------------------------------------------
// update
// --------------------------------------------
int IntroState::update(float dt) {
	_effect->update(dt);
	_timer += dt;
	if (_timer > 6.0f) {
		return 1;
	}
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int IntroState::onGUIButton(ds::DialogID dlgID, int button) {
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void IntroState::render() {
	
	_effect->render();
}


*/