#include "Dialog.h"
#include <vector>
#include <stdarg.h>
#include "tweening.h"
#include "HUD.h"

const ds::vec4 BUTTONS[] = {
	ds::vec4(0, 226, 304, 60), // green button
	ds::vec4(0, 362, 304, 60), // grey button
	ds::vec4(0, 294, 304, 60)  // red button
};

enum FloatInDirection {
	FID_LEFT,
	FID_RIGHT
};

int floatButton(float time, float ttl, FloatInDirection dir) {
	if (time <= ttl) {
		if (dir == FloatInDirection::FID_LEFT) {
			return tweening::interpolate(tweening::easeOutBounce, -200, 512, time, ttl);
		}
		else {
			return tweening::interpolate(tweening::easeOutBounce, 1020, 512, time, ttl);
		}
	}
	return 512;
}

// ---------------------------------------------------------------
// show highscores
// ---------------------------------------------------------------
int showHighscores(float time, float ttl, int mode, Highscore* highscores, int page) {
	int ret = 0;
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutBounce, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 820, 570, 64));
	ds::vec2 p = ds::vec2(512, 500);
	int start = page * 5;
	int end = start + 5;
	for (int i = start; i < end; ++i) {
		dialog::Image(p, ds::vec4(610, 160, 400, 50));
		const Highscore& hs = highscores[mode * 10 + i];
		dialog::FormattedText(p, true, ds::vec2(1.0f), "%d Name %02d:%02d", (i+1),hs.minutes,hs.seconds);
		p.y -= 60.0f;
	}	
	//
	int dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 180), BUTTONS[2], "MAIN MENU")) {
		ret = 1;
	}
	return ret;
}

// ---------------------------------------------------------------
// show game over menu
// ---------------------------------------------------------------
int showGameOverMenu(const Score& score, float time, float ttl) {
	int ret = 0;
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutBounce, 900, 600, time, ttl);
	}
	if (score.success) {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 670, 410, 64));
	}
	else {
		dialog::Image(ds::vec2(512, dy), ds::vec4(420, 670, 394, 64));
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
	
	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 230), BUTTONS[0], "REPLAY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 150), BUTTONS[2], "MAIN MENU")) {
		ret = 2;
	}
	return ret;
}


// ---------------------------------------------------------------
// show main menu
// ---------------------------------------------------------------
int showMainMenu(float time, float ttl) {
	int ret = 0;
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutBounce, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 600, 880, 64));

	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 490), BUTTONS[1], "EASY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 410), BUTTONS[1], "MEDIUM")) {
		ret = 2;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 330), BUTTONS[1], "HARD")) {
		ret = 3;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 250), BUTTONS[0], "HIGHSCORES")) {
		ret = 5;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 170), BUTTONS[2], "EXIT")) {
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
}

void InputDialog::tick(float dt) {
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
		}
		else if (key.value == ds::SpecialKeys::DSKEY_Enter) {
			return false;
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