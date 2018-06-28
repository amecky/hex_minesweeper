#include "Dialog.h"
#include <vector>
#include <stdarg.h>
#include "tweening.h"
#include "HUD.h"
#include "animation.h"
#include <ds_imgui.h>

const ds::vec4 BUTTONS[] = {
	ds::vec4(0, 226, 304, 60), // green button
	ds::vec4(0, 362, 304, 60), // grey button
	ds::vec4(0, 294, 304, 60)  // red button
};

enum FloatInDirection {
	FID_LEFT,
	FID_RIGHT
};

float floatButton(float time, float ttl, FloatInDirection dir) {
	if (time <= ttl) {
		if (dir == FloatInDirection::FID_LEFT) {
			return tweening::interpolate(tweening::easeOutBounce, -200, 512, time, ttl);
		}
		else {
			return tweening::interpolate(tweening::easeOutBounce, 1020, 512, time, ttl);
		}
	}
	return 512.0f;
}

// ---------------------------------------------------------------
// show highscores
// ---------------------------------------------------------------
int showHighscores(float time, float ttl, int mode, Highscore* highscores, int page) {
	int ret = 0;
	float dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutBounce, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512.0f, dy), ds::vec4(0, 820, 570, 64));

	dialog::Image(ds::vec2(512,530), ds::vec4(610, 160, 440, 50));
	if (mode == 0) {
		dialog::Text(ds::vec2(512, 530), "Easy");
	}
	else if (mode == 1) {
		dialog::Text(ds::vec2(512, 530), "Medium");
	}
	if (mode == 2) {
		dialog::Text(ds::vec2(512, 530), "Hard");
	}
	ds::vec2 p = ds::vec2(512, 470);
	int start = page * 5;
	int end = start + 5;
	for (int i = start; i < end; ++i) {
		dialog::Image(p, ds::vec4(610, 160, 440, 50));
		const Highscore& hs = highscores[mode * 10 + i];
		dialog::FormattedText(p, true, ds::vec2(1.0f), "%-2d. %-10s %02d:%02d", (i+1),hs.name,hs.minutes,hs.seconds);
		p.y -= 60.0f;
	}	
	//
	float dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 160.0f), BUTTONS[2], "MAIN MENU")) {
		ret = 1;
	}
	return ret;
}

// ---------------------------------------------------------------
// show game over menu
// ---------------------------------------------------------------
int showGameOverMenu(const Score& score, float time, float ttl) {
	int ret = 0;
	float dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutBounce, 900, 600, time, ttl);
	}
	if (score.success) {
		dialog::Image(ds::vec2(512.0f, dy), ds::vec4(0, 670, 410, 64));
	}
	else {
		dialog::Image(ds::vec2(512.0f, dy), ds::vec4(420, 670, 394, 64));
	}
	dialog::Image(ds::vec2(512, 470), ds::vec4(610, 160, 540, 50));
	dialog::Image(ds::vec2(512, 390), ds::vec4(610, 160, 540, 50));
	dialog::Image(ds::vec2(512, 310), ds::vec4(610, 160, 540, 50));
	dialog::FormattedText(ds::vec2(400, 470), true, ds::vec2(1.0f), "Bombs left: %d", score.bombsLeft);
	dialog::FormattedText(ds::vec2(400, 390), true, ds::vec2(1.0f), "Time: %02d:%02d", score.minutes, score.seconds);
	if (score.success) {
		if (score.rank != -1) {
			dialog::FormattedText(ds::vec2(400, 310), true, ds::vec2(1.0f), "New Highscore - Rank %d", (score.rank + 1));
		}
		else {
			dialog::FormattedText(ds::vec2(400, 310), true, ds::vec2(1.0f), "%s", "No new highscore");
		}
	}
	else {
		dialog::FormattedText(ds::vec2(400, 310), true, ds::vec2(1.0f), "%s","No new highscore");
	}
	
	float dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 230.0f), BUTTONS[0], "REPLAY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 150.0f), BUTTONS[2], "MAIN MENU")) {
		ret = 2;
	}
	return ret;
}

static Animation animations[32];

void createAnimations() {
	anim::build_simple_animation(&animations[0],900.0f, 600.0f, 1.2f);
	animations[0].tweening = tweening::easeOutBounce;
	anim::build_simple_animation(&animations[1], -200.0f, 512.0f, 1.2f);
	animations[1].tweening = tweening::easeOutBounce;
	anim::build_simple_animation(&animations[2], 1020.0f, 512.0f, 1.2f);
	animations[2].tweening = tweening::easeOutBounce;
	anim::add_step(&animations[3], 0.0f, 0.6f);
	anim::add_step(&animations[3], 1.0f, 1.0f);
	anim::add_step(&animations[3], 1.1f, 1.2f);
	anim::add_step(&animations[3], 1.2f, 0.8f);
	anim::add_step(&animations[3], 1.3f, 1.1f);
	anim::add_step(&animations[3], 1.4f, 0.9f);
	anim::add_step(&animations[3], 1.5f, 1.0f);
}

float animate(float time, int index) {
	return anim::interpolate(&animations[index], time);
}

void editAnimation(int index) {
	if (index != -1) {
		Animation& anim = animations[index];
		for (int i = 0; i < anim.count; ++i) {
			ds::vec2 tmp(anim.steps[i], anim.values[i]);
			gui::pushID(i);
			if (gui::Input("Step", &tmp)) {
				anim.steps[i] = tmp.x;
				anim.values[i] = tmp.y;
			}
			gui::popID();
		}
	}
}

// ---------------------------------------------------------------
// show main menu
// ---------------------------------------------------------------
int showMainMenu(float time, float ttl) {
	int ret = 0;
	dialog::Image(ds::vec2(512.0f, animate(time, 0)), ds::vec4(0, 600, 880, 64));

	float scale = animate(time, 3);
	ds::vec2 vc(scale);

	float dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 490.0f), BUTTONS[1], "EASY", vc)) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 410.0f), BUTTONS[1], "MEDIUM", vc)) {
		ret = 2;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 330.0f), BUTTONS[1], "HARD", vc)) {
		ret = 3;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 250.0f), BUTTONS[0], "HIGHSCORES", vc)) {
		ret = 5;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 170.0f), BUTTONS[2], "EXIT", vc)) {
		ret = 4;
	}
	dialog::Image(ds::vec2(512, 30), ds::vec4(0, 800, 600, 14));
	return ret;
}

// -------------------------------------------------------
// Input dialog
// -------------------------------------------------------
InputDialog::InputDialog() {
	sprintf(_name, "%s", "Name");
	_caretPos = 4;
}

void InputDialog::reset(const char* name) {
	sprintf(_name, "%s", name);
	_showCursor = true;
	_timer = 0.0f;
}

void InputDialog::tick(float dt) {
	_timer += dt;
	if (_timer >= 0.4f) {
		_timer -= 0.4f;
		_showCursor = !_showCursor;
	}
}

bool InputDialog::handleTextInput() {
	int len = strlen(_name);
	for (int i = 0; i < ds::getNumInputKeys(); ++i) {
		const ds::InputKey& key = ds::getInputKey(i);
		if (key.type == ds::IKT_SYSTEM) {
			if (key.value == ds::SpecialKeys::DSKEY_Backspace) {
				if (_caretPos > 0) {
					if (_caretPos < len) {
						memmove(_name + _caretPos - 1, _name + _caretPos, len - _caretPos);
					}
					--_caretPos;
					--len;
					_name[len] = '\0';

				}
			}
			if (key.value == ds::SpecialKeys::DSKEY_Enter) {
				return false;
			}
		}
		else {
			if (len < 10) {
				if ((key.value > 47 && key.value < 128) || key.value == '.' || key.value == '-') {
					if (len < 32) {
						if (_caretPos < len) {
							memmove(_name + _caretPos + 1, _name + _caretPos, len - _caretPos);
						}
						_name[_caretPos] = key.value;
						++len;
						++_caretPos;

					}
				}
			}
		}
		_name[len] = '\0';
	}
	return true;
}

int InputDialog::render() {
	int ret = 0;
	ds::vec2 boxPos(270, 384);
	dialog::Image(boxPos, ds::vec4(610, 220, 100, 200));
	boxPos.x += 140.0f;
	dialog::Image(boxPos, ds::vec4(720, 220, 180, 200));
	boxPos.x += 180.0f;
	dialog::Image(boxPos, ds::vec4(720, 220, 180, 200));
	boxPos.x += 140.0f;
	dialog::Image(boxPos, ds::vec4(890, 220, 100, 200));
	
	dialog::Text(ds::vec2(400, 440), "Please enter your name");

	dialog::Image(ds::vec2(512, 380), ds::vec4(610,160,400,40));
	if (_showCursor) {
		ds::vec2 size = dialog::text_size(_name);
		ds::vec2 p(512, 375);
		p.x = (1024.0f - size.x) * 0.5f + size.x + 15.0f;
		dialog::Image(p, ds::vec4(200, 0, 20, 2));
	}
	if (!handleTextInput()) {
		return 1;
	}
	dialog::Text(ds::vec2(512, 380), _name);

	// FIXME: blinking cursor

	int dx = 400;
	if (dialog::Button(ds::vec2(dx, 320), ds::vec4(610, 430, 148, 32))) {
		ret = 1;
	}
	dx = 600;
	if (dialog::Button(ds::vec2(dx, 320), ds::vec4(770, 430, 148, 32))) {
		ret = 2;
	}
	return ret;
}