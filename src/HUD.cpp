#include "HUD.h"
#include "tweening.h"

HUD::HUD(SpriteBatchBuffer* buffer, Score* score) : _buffer(buffer) , _score(score) , _bombs(2), _minutes(2), _seconds(2) {
	reset();
}

HUD::~HUD() {
}

// ------------------------------------------------------
// reset
// ------------------------------------------------------
void HUD::reset() {
	_minutes.setValue(0, false);
	_seconds.setValue(0, false);
	_bombs.setValue(0, false);
	_timer = 0.0f;
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
			if (nr.scalingTimers[i] >= 0.4f) {
				nr.scaleFlags[i] = 0;
			}
		}
	}
}
// ------------------------------------------------------
// tick
// ------------------------------------------------------
void HUD::tick(float dt) {
	_timer += dt;
	if (_timer > 1.0f) {
		_timer -= 1.0f;
		++_seconds.value;
		if (_seconds.value >= 60) {
			_seconds.value = 0;
			++_minutes.value;
		}
	}
	_minutes.setValue(_minutes.value, false);
	_seconds.setValue(_seconds.value, false);
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
	float width = nr.size * 38.0f;
	p.x = startPos.x - width * 0.5f;
	for (int i = 0; i < nr.size; ++i) {
		float scale = 1.0f;
		if (nr.scaleFlags[i] == 1) {
			scale = tweening::interpolate(tweening::easeOutElastic, 2.5f, 1.0f, nr.scalingTimers[i], 0.4f);
		}
		_buffer->add(p, ds::vec4(nr.items[i] * 38, 190, 38, 30), ds::vec2(1.0f, scale));
		p.x += 38.0f;
	}
}
// ------------------------------------------------------
// render
// ------------------------------------------------------
void HUD::render() {
	
	ds::vec2 p(512, 720);
	renderNumber(_bombs, p);
	p.y = 30.0f;
	renderNumber(_minutes, p);
	p.x += 38.0f * 3.0f;
	renderNumber(_seconds, p);
	
}
