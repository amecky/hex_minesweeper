#pragma once
#include <utils\GameTimer.h>
#include <Vector.h>
#include "Highscores.h"
#include <dialogs\GUIDialog.h>
#include "GameSettings.h"

enum GameModeType {
	GM_EASY,
	GM_MEDIUM,
	GM_HARD
};

struct GameMode {
	GameModeType type;
	int maxBombs;
	int width;
	int height;
	v2 origin;

	GameMode() : type(GM_EASY), maxBombs(0), width(0), height(0) , origin(0,0) {}
	GameMode(GameModeType t, int m, int w, int h,const v2& o) : type(t), maxBombs(m), width(w), height(h) , origin(o) {}
};

const GameMode GAME_MODES[] = {
	GameMode(GM_EASY, 12, 8, 8, v2(-4, -4)),
	GameMode(GM_MEDIUM, 32, 14, 12, v2(-14, -2)),
	GameMode(GM_HARD, 60, 20, 15, v2(-10,-6)),
};

struct GameContext {

	int mode;
	int marked;
	int markedCorrectly;
	Highscore highscore;
	HighscoreService<PlayedTime, 3> highscore_service;
	ds::GUIDialog* hud;
	GameSettings* settings;

	void reset() {
		marked = 0;
		markedCorrectly = 0;
	}
};

