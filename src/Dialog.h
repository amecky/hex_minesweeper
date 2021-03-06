#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include <ds_game_ui.h>

struct Score;
struct Highscore;

enum MenuAnimation {
	MA_BUTTON_SLIDE_LEFT,
	MA_BUTTON_SLIDE_RIGHT,
};

void createAnimations();

float animate(float time, int index);

int showGameOverMenu(const Score& score, float time, float ttl);

int showMainMenu(float time, float ttl);

int showHighscores(float time, float ttl, int mode, Highscore* highscores, int page);

void editAnimation(int index);

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
	bool _showCursor;
	float _timer;
};

