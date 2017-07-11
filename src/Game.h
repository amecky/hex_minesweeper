#pragma once
#include <diesel.h>
#define SPRITE_IMPLEMENTATION
#include <SpriteBatchBuffer.h>

#include "board.h"
#include "hud.h"
#include "GameContext.h"

// ---------------------------------------------------------------
// Game modes
// ---------------------------------------------------------------
enum GameState {
	GM_MENU,
	GM_RUNNING,
	GM_GAMEOVER
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

struct DebugSettings {
	int value;
};


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
};

