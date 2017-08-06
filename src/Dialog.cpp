#include "Dialog.h"
#include <vector>
#include <stdarg.h>
#include "tweening.h"
#include "HUD.h"

enum FloatInDirection {
	FID_LEFT,
	FID_RIGHT
};

int floatButton(float time, float ttl, FloatInDirection dir) {
	if (time <= ttl) {
		if (dir == FloatInDirection::FID_LEFT) {
			return tweening::interpolate(tweening::easeOutElastic, -200, 512, time, ttl);
		}
		else {
			return tweening::interpolate(tweening::easeOutElastic, 1020, 512, time, ttl);
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
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 690, 420, 85));
	ds::vec2 p = ds::vec2(512, 500);
	int start = page * 5;
	int end = start + 5;
	for (int i = start; i < end; ++i) {
		dialog::Image(p, ds::vec4(610, 160, 400, 50));
		const Highscore& hs = highscores[mode * 10 + i];
		dialog::FormattedText(p, "%d Name %02d:%02d", (i+1),hs.minutes,hs.seconds);
		p.y -= 60.0f;
	}	
	//
	int dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 180), ds::vec4(0, 300, 304, 50), "MAIN MENU")) {
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
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	if (score.success) {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 420, 325, 70));
	}
	else {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 490, 310, 70));
	}
	dialog::Image(ds::vec2(512, 470), ds::vec4(610, 160, 540, 50));
	dialog::Image(ds::vec2(512, 390), ds::vec4(610, 160, 540, 50));
	dialog::Image(ds::vec2(512, 310), ds::vec4(610, 160, 540, 50));
	dialog::FormattedText(ds::vec2(400, 470), "Bombs left: %d", score.bombsLeft);
	dialog::FormattedText(ds::vec2(400, 390), "Time: %02d:%02d", score.minutes, score.seconds);
	if (score.success) {
		if (score.rank != -1) {
			dialog::FormattedText(ds::vec2(400, 310), "New Highscore - Rank %d", (score.rank + 1));
		}
		else {
			dialog::FormattedText(ds::vec2(400, 310), "%s", "No new highscore");
		}
	}
	else {
		dialog::FormattedText(ds::vec2(400, 310), "%s","No new highscore");
	}
	
	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 230), ds::vec4(0, 368, 304, 50), "REPLAY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 150), ds::vec4(0, 300, 304, 50), "MAIN MENU")) {
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
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 600, 645, 82));

	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 490), ds::vec4(0, 368, 304, 50), "EASY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 410), ds::vec4(0, 368, 304, 50), "MEDIUM")) {
		ret = 2;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 330), ds::vec4(0, 368, 304, 50), "HARD")) {
		ret = 3;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 250), ds::vec4(0, 230, 304, 50), "HIGHSCORES")) {
		ret = 5;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 170), ds::vec4(0, 300, 304, 50), "EXIT")) {
		ret = 4;
	}
	return ret;
}

const float FONT_PADDING = 0.0f;

namespace font {

	// Font starts at 200, 230
	// x-offset / width
	static const ds::vec2 FONT_DEF[] = {
		ds::vec2(1,34),   // A
		ds::vec2(35,29),  // B
		ds::vec2(66,27),  // C
		ds::vec2(94,30),  // D
		ds::vec2(125,27), // E
		ds::vec2(152,28), // F
		ds::vec2(180,29), // G
		ds::vec2(210,29), // H
		ds::vec2(240,12), // I
		ds::vec2(252,17), // J
		ds::vec2(269,28), // K
		ds::vec2(297,28), // L
		ds::vec2(325,39), // M
		ds::vec2(365,31), // N
		ds::vec2(396,29), // O
		ds::vec2(426,30), // P
		ds::vec2(456,29), // Q
		ds::vec2(485,30), // R
		ds::vec2(515,27), // S 
		ds::vec2(541,26), // T
		ds::vec2(567,29), // U
		ds::vec2(596,33), // V
		ds::vec2(629,41), // W
		ds::vec2(670,31), // X
		ds::vec2(701,32), // Y
		ds::vec2(733,27)  // Z
	};


	ds::vec4 get_rect(char c) {
		if (c == ' ') {
			return ds::vec4(0, 100, 20, 25);
		}
		// numbers are stored at different location
		if (c >= 48 && c <= 57) {
			int idx = (int)c - 48;
			return ds::vec4(1 + idx * 30, 940, 30, 25);
		}
		// convert to lower case to upper case
		if (c > 92) {
			c -= 32;
		}
		if (c >= 65 && c <= 90) {
			ds::vec2 fd = FONT_DEF[(int)c - 65];
			return ds::vec4(0.0f + fd.x, 910.0f, fd.y, 25.0f);
		}
		return ds::vec4(0, 100, 20, 25);

	}

	ds::vec2 text_size(const char* txt) {
		int l = strlen(txt);
		ds::vec2 p(0.0f);
		for (int i = 0; i < l; ++i) {
			ds::vec4 r = get_rect(txt[i]);
			p.x += r.z + FONT_PADDING;
			if (r.w > p.y) {
				p.y = r.w;
			}
		}
		return p;
	}
}


namespace dialog {

	struct DrawCall {
		ds::vec2 pos;
		ds::vec4 rect;
	};

	struct GUIContext {
		SpriteBatchBuffer* buffer;
		std::vector<DrawCall> calls;
		bool clicked;
		bool buttonPressed;
	};

	static GUIContext* _guiCtx = 0;

	// -------------------------------------------------------
	// check if mouse cursor is inside box
	// -------------------------------------------------------
	static bool isCursorInside(const ds::vec2& p, const ds::vec2& dim) {
		ds::vec2 mp = ds::getMousePosition();
		if (mp.x < (p.x - dim.x * 0.5f)) {
			return false;
		}
		if (mp.x >(p.x + dim.x * 0.5f)) {
			return false;
		}
		if (mp.y < (p.y - dim.y * 0.5f)) {
			return false;
		}
		if (mp.y >(p.y + dim.y * 0.5f)) {
			return false;
		}
		return true;
	}

	// -------------------------------------------------------
	// handle mouse interaction
	// -------------------------------------------------------
	static bool isClicked(const ds::vec2& pos, const ds::vec2& size) {
		if (_guiCtx->clicked) {
			ds::vec2 p = pos;
			//p.x += size.x * 0.5f;
			if (_guiCtx->clicked && isCursorInside(p, size)) {
				return true;
			}
		}
		return false;
	}

	void init(SpriteBatchBuffer* buffer) {
		_guiCtx = new GUIContext;
		_guiCtx->buffer = buffer;
		_guiCtx->clicked = false;
		_guiCtx->buttonPressed = ds::isMouseButtonPressed(0);
	}

	void begin() {
		_guiCtx->calls.clear();
		if (_guiCtx->clicked) {
			_guiCtx->clicked = false;
		}
		if (ds::isMouseButtonPressed(0)) {
			_guiCtx->buttonPressed = true;
		}
		else {
			if (_guiCtx->buttonPressed) {
				_guiCtx->clicked = true;
			}
			_guiCtx->buttonPressed = false;
		}
	}

	bool Button(const ds::vec2& pos, const ds::vec4& rect) {
		DrawCall call;
		call.pos = pos;
		call.rect = rect;
		_guiCtx->calls.push_back(call);
		ds::vec2 dim = ds::vec2(rect.z, rect.w);
		return isClicked(pos, dim);
	}

	bool Button(const ds::vec2& pos, const ds::vec4& rect, const char* text) {
		DrawCall call;
		call.pos = pos;
		call.rect = rect;
		_guiCtx->calls.push_back(call);
		ds::vec2 dim = ds::vec2(rect.z, rect.w);
		int l = strlen(text);
		ds::vec2 p = pos;
		ds::vec2 size = font::text_size(text);
		p.x = pos.x - size.x * 0.5f;
		float lw = 0.0f;
		for (int i = 0; i < l; ++i) {
			ds::vec4 r = font::get_rect(text[i]);
			p.x += lw * 0.5f + r.z * 0.5f;
			DrawCall call = { p,r };
			_guiCtx->calls.push_back(call);
			lw = r.z;
		}
		return isClicked(pos, dim);
	}

	void Image(const ds::vec2& pos, const ds::vec4& rect) {
		DrawCall call;
		call.pos = pos;
		call.rect = rect;
		_guiCtx->calls.push_back(call);
	}

	void Text(const ds::vec2& pos, const char* text) {
		int l = strlen(text);
		ds::vec2 p = pos;
		ds::vec2 size = font::text_size(text);
		p.x = (1024.0f - size.x) * 0.5f;
		float lw = 0.0f;
		for (int i = 0; i < l; ++i) {
			ds::vec4 r = font::get_rect(text[i]);
			p.x += lw * 0.5f + r.z * 0.5f;
			DrawCall call = { p,r };
			_guiCtx->calls.push_back(call);
			lw = r.z;
		}
	}

	void FormattedText(const ds::vec2& pos, const char* fmt, ...) {
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		ds::vec2 size = font::text_size(buffer);
		ds::vec2 p = pos;
		p.x = (1024.0f - size.x) * 0.5f;
		Text(p, buffer);
		va_end(args);
		
	}

	void end() {
		for (size_t i = 0; i < _guiCtx->calls.size(); ++i) {
			const DrawCall& call = _guiCtx->calls[i];
			_guiCtx->buffer->add(call.pos, call.rect);
		}
	}

	void shutdown() {
		if (_guiCtx != 0) {
			delete _guiCtx;
		}
	}

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
	if (dialog::Button(ds::vec2(dx, 320), ds::vec4(610, 430, 102, 32))) {
		ret = 1;
	}
	dx = 600;
	if (dialog::Button(ds::vec2(dx, 320), ds::vec4(720, 430, 102, 32))) {
		ret = 2;
	}
	return ret;
}