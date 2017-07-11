#include "HUD.h"
#include "tweening.h"
#include <math.h>

HUD::HUD(SpriteBatchBuffer* buffer, Score* score, GameSettings* settings) : _buffer(buffer) , _score(score) , _settings(settings), _bombs(2) {
	reset();
}

HUD::~HUD() {
}

// ------------------------------------------------------
// reset
// ------------------------------------------------------
void HUD::reset() {
	_timer.reset();
	_bombs.reset();
	_score->success = false;
	_score->minutes = 0;
	_score->seconds = 0;
	_score->bombsLeft = 0;
}

// ------------------------------------------------------
// scale number
// ------------------------------------------------------
void HUD::scaleNumber(Number& nr, float dt) {
	for (int i = 0; i < nr.size; ++i) {
		if (nr.scaleFlags[i] == 1) {
			nr.scalingTimers[i] += dt;
			if (nr.scalingTimers[i] >= _settings->numberScaleTTL) {
				nr.scaleFlags[i] = 0;
			}
		}
	}
}
// ------------------------------------------------------
// tick
// ------------------------------------------------------
void HUD::tick(float dt) {
	_timer.tick(dt);
	scaleNumber(_bombs, dt);
}

// ------------------------------------------------------
// set pieces
// ------------------------------------------------------
void HUD::setBombs(int pc) {
	_bombs.setValue(pc);
}

void HUD::renderNumber(const Number& nr, const ds::vec2& startPos) {
	ds::vec2 p = startPos;
	float width = nr.size * 38.0f - 38.0f;
	p.x = startPos.x - width * 0.5f;
	for (int i = 0; i < nr.size; ++i) {
		float scale = 1.0f;
		if (nr.scaleFlags[i] == 1) {
			scale = tweening::interpolate(tweening::easeOutElastic, 1.0f + _settings->numberScaleAmplitude, 1.0f, nr.scalingTimers[i], _settings->numberScaleTTL);
			//scale =1.0f + sin(nr.scalingTimers[i] / _settings->numberScaleTTL * ds::PI) * _settings->numberScaleAmplitude;
		}
		_buffer->add(p, ds::vec4(nr.items[i] * 38, 190, 38, 30), ds::vec2(1.0f, scale));
		p.x += 38.0f;
	}
}
// ------------------------------------------------------
// render
// ------------------------------------------------------
void HUD::render() {
	
	ds::vec2 p(512, 725);
	_buffer->add(p, ds::vec4(610, 0, 120, 50));
	renderNumber(_bombs, p);
	p.x -= 38.0f;
	p.y = 40.0f;
	_buffer->add(p, ds::vec4(750, 0, 100, 50));
	renderNumber(_timer.minutes, p);
	p.x += 38.0f * 3.0f;
	_buffer->add(p, ds::vec4(750, 0, 100, 50));
	renderNumber(_timer.seconds, p);
	
}
