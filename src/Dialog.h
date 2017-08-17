#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include "Animation.h"

struct Score;
struct Highscore;

int showGameOverMenu(const Score& score, float time, float ttl);

int showMainMenu(float time, float ttl, const KeyFrameAnimation& buttonAnimation);

int showHighscores(float time, float ttl, int mode, Highscore* highscores, int page);

namespace font {

	ds::vec4 get_rect(char c);

	//void renderText(const ds::vec2& pos, const char* txt, SpriteBatchBuffer* buffer);

	ds::vec2 text_size(const char* txt);
}


namespace dialog {

	void init(SpriteBatchBuffer* buffer);

	void begin();

	void Image(const ds::vec2& pos, const ds::vec4& rect);

	bool ImageButton(const ds::vec2& pos, const ds::vec4& rect, const ds::vec2& scale = ds::vec2(1.0f));

	bool Button(const ds::vec2& pos, const ds::vec4& rect, const char* text, const ds::vec2& scale = ds::vec2(1.0f));

	void Text(const ds::vec2& pos, const char* text);

	void FormattedText(const ds::vec2& pos, const char* fmt, ...);

	void end();

	void shutdown();

}

class InputDialog {

public:
	InputDialog();
	void reset(const char* name);
	void tick(float dt);
	int render();
	const char* getName() const {
		return _name;
	}
private:
	bool handleTextInput();
	int _caretPos;
	char _name[20];
	int _index;
};

