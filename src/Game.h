#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include <ds_imgui.h>
#include <ds_game_ui.h>
#include "board.h"
#include "hud.h"
#include "GameContext.h"
#include "Dialog.h"
#include "particles.h"

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
	int _pageMode;
	InputDialog _inputDialog;
	bool _inputActive;
	char _playerName[10];
	p2i _dialogPos;
	int _logPanelState;
	StarParticleSettings _starSettings;
	Particles* _particles;

	int _dbgAnimIndex;
	int _dbgShowStarsPanel;
	int _dbgShowAnimationPanel;
};

