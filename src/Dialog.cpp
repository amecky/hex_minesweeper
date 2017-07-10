#include "Dialog.h"
#include <vector>
#include <stdarg.h>
#include "Constants.h"

namespace font {

	void renderText(const ds::vec2& pos, const char* txt, SpriteBatchBuffer* buffer) {
		int l = strlen(txt);
		ds::vec2 p = pos;
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			if (idx >= 0 && idx < 127) {
				const ds::vec4& r = FONT_RECTS[idx];
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
			if (idx >= 0 && idx < 127) {
				const ds::vec4& r = FONT_RECTS[idx];
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
		_guiCtx->buttonPressed = false;
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
			if (idx >= 0 && idx < 127) {
				ds::vec4 r = FONT_RECTS[idx];
				r.r += 640.0f;
				r.g -= 130.0f;
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
