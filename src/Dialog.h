#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>

namespace font {

	void renderText(const ds::vec2& pos, const char* txt, SpriteBatchBuffer* buffer);

	ds::vec2 textSize(const char* txt);
}


namespace dialog {

	void init(SpriteBatchBuffer* buffer);

	void begin();

	void Image(const ds::vec2& pos, const ds::vec4& rect);

	bool Button(const ds::vec2& pos, const ds::vec4& rect);

	bool Button(const ds::vec2& pos, const ds::vec4& rect, const char* text);

	void Text(const ds::vec2& pos, const char* text);

	void FormattedText(const ds::vec2& pos, const char* fmt, ...);

	void end();

	void shutdown();

}


