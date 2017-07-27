#pragma once
#include <diesel.h>
#define SPRITE_IMPLEMENTATION
#include <SpriteBatchBuffer.h>
#include <ds_imgui.h>
#include "board.h"
#include "hud.h"
#include "GameContext.h"
#include "Dialog.h"
// ---------------------------------------------------------------
// Game modes
// ---------------------------------------------------------------
enum GameState {
	GM_MENU,
	GM_RUNNING,
	GM_GAMEOVER,
	GM_HIGHSCORES
};

// ---------------------------------------------------------------
// DebugPanel
// ---------------------------------------------------------------
struct DebugPanel {
	char key;
	bool pressed;
	bool active;
	int state;
};


// ---------------------------------------------------------------
// Game
// ---------------------------------------------------------------
class Game {

public:
	Game();
	~Game();
	bool isRunning() const {
		return _running;
	}
	void tick(float dt);
	void render();
private:
	int handleScore();
	void handleDebugInput(DebugPanel* panel);
	void renderDebugPanel();
	bool _running;
	SpriteBatchBuffer* _spriteBuffer;
	GameSettings _settings;
	Board* _board;
	Score _score;
	HUD* _hud;
	GameState _mode;
	int _selectedMode;
	DebugPanel _debugPanel;
	float _menuTimer;
	Highscore _highscores[30];
	ds::vec2 _debugScore;
	int _page;
	float _pageTimer;
	InputDialog _inputDialog;
	bool _inputActive;
	char _playerName[10];

	int _dummy; // FIXME: remove after debugging
	p2i _dialogPos; // FIXME: remove after debugging
	ds::Color _dummyColor; // FIXME: RAD
};

