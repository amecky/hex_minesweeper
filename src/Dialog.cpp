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
	dialog::begin();
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 700, 400, 80));
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
	if (dialog::Button(ds::vec2(dx, 180), ds::vec4(0, 300, 300, 50), "MAIN MENU")) {
		ret = 1;
	}
	dialog::end();
	return ret;
}

// ---------------------------------------------------------------
// show game over menu
// ---------------------------------------------------------------
int showGameOverMenu(const Score& score, float time, float ttl) {
	int ret = 0;
	dialog::begin();
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	if (score.success) {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 420, 310, 60));
	}
	else {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 490, 300, 60));
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
	if (dialog::Button(ds::vec2(dx, 230), ds::vec4(0, 368, 300, 50), "REPLAY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 150), ds::vec4(0, 300, 300, 50), "MAIN MENU")) {
		ret = 2;
	}
	dialog::end();
	return ret;
}

// ---------------------------------------------------------------
// show main menu
// ---------------------------------------------------------------
int showMainMenu(float time, float ttl) {
	int ret = 0;
	dialog::begin();
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 600, 640, 70));

	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 450), ds::vec4(0, 368, 300, 50), "EASY")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 370), ds::vec4(0, 368, 300, 50), "MEDIUM")) {
		ret = 2;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 290), ds::vec4(0, 368, 300, 50), "HARD")) {
		ret = 3;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 210), ds::vec4(0, 368, 300, 50), "HIGHSCORES")) {
		ret = 5;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 130), ds::vec4(0, 300, 300, 50), "EXIT")) {
		ret = 4;
	}
	dialog::end();
	return ret;
}

const static int FONT_WIDTH[] = {
	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
	22,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
	 8,  9,	13, 19, 16, 28, 19,  7, 12, 12, 16, 19,  8, 11,  8, 15, 
	16, 16, 16,	16, 16, 16, 16, 16, 16, 16,  9,  9, 19, 19, 19, 14, 
	21, 17, 17, 16,	18, 15, 14, 18, 18, 12, 12, 17, 14, 21, 19, 19, 
	16, 19, 17, 16,	15, 18, 17, 25, 17, 16, 15, 12, 15, 12, 19, 16, 
	16, 15, 15, 13,	15, 15,  9, 15, 16,  8,  9, 15,  8, 23, 16, 15, 
	15, 15, 11, 13, 10,	16, 14, 22, 15, 14, 13, 16, 12, 16, 19, 22, 
	16, 22,  7, 16, 13,	23, 16, 16, 16, 39, 16, 12, 25, 22, 15, 22, 
	22,  7,  7, 13, 13,	16, 16, 22, 16, 21, 13, 12, 24, 22, 13, 16, 
	 8,  9, 16, 16, 16,	16, 12, 16, 16, 21, 13, 19, 19, 11, 21, 16, 
	13, 19, 13, 13, 16,	16, 16,  8, 16, 13, 13, 19, 26, 26, 26, 14, 
	17, 17, 17, 17, 17,	17, 24, 16, 15, 15, 15, 15, 12, 12, 12, 12, 
	18, 19, 19, 19,	19, 19, 19, 19, 19, 18, 18, 18, 18, 16, 16, 16, 
	15, 15, 15,	15, 15, 15, 22, 13, 15, 15, 15, 15,  8,  8,  8,  8, 
	15, 16, 15,	15, 15, 15, 15, 19, 15, 16, 16, 16, 16, 14, 15, 14

};

namespace font {

	void renderText(const ds::vec2& pos, const char* txt, SpriteBatchBuffer* buffer) {
		int l = strlen(txt);
		ds::vec2 p = pos;
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			int y = idx / 18;
			int x = idx - y * 18;
			if (idx >= 0 && idx < 127) {
				ds::vec4 r = ds::vec4(420 + x * 28, 670 + y * 28, FONT_WIDTH[idx+32], 28);
				buffer->add(p, r);
				p.x += r.z + 6.0f;
			}
		}
	}

	ds::vec2 textSize(const char* txt) {
		int l = strlen(txt);
		ds::vec2 p(0.0f);
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			int y = idx / 18;
			int x = idx - y * 18;
			if (idx >= 0 && idx < 127) {
				ds::vec4 r = ds::vec4(420 + x * 28, 670 + y * 28, FONT_WIDTH[idx+32], 28);
				p.x += r.z + 6.0f;
				if (r.w > p.y) {
					p.y = r.w;
				}
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
		ds::vec2 size = font::textSize(text);
		p.x = pos.x - size.x * 0.5f;
		for (int i = 0; i < l; ++i) {
			int idx = (int)text[i] - 32;
			int y = idx / 18;
			int x = idx - y * 18;
			if (idx >= 0 && idx < 127) {
				ds::vec4 r = ds::vec4(420 + x * 28, 670 + y * 28, FONT_WIDTH[idx+32], 26);			
				DrawCall call;
				call.pos = p;
				call.rect = r;
				_guiCtx->calls.push_back(call);
				p.x += r.z + 6.0f;
			}
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
		ds::vec2 size = font::textSize(text);
		p.x = (1024.0f - size.x) * 0.5f;
		for (int i = 0; i < l; ++i) {
			int idx = (int)text[i] - 32;
			int y = idx / 18;
			int x = idx - y * 18;
			if (idx >= 0 && idx < 127) {
				ds::vec4 r = ds::vec4(420 + x * 28, 670 + y * 28, FONT_WIDTH[idx+32], 26);
				DrawCall call;
				call.pos = p;
				call.rect = r;
				_guiCtx->calls.push_back(call);
				p.x += r.z + 6.0f;
			}
		}
	}

	void FormattedText(const ds::vec2& pos, const char* fmt, ...) {
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		ds::vec2 size = font::textSize(buffer);
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
