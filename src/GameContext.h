#pragma once
<<<<<<< HEAD
#include <utils\GameTimer.h>
#include <Vector.h>
#include "Highscores.h"
#include <dialogs\GUIDialog.h>
#include "GameSettings.h"
=======
#include <diesel.h>
>>>>>>> origin/2D

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
	ds::vec2 origin;

<<<<<<< HEAD
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
	GameSettings* settings;

	void reset() {
		marked = 0;
		markedCorrectly = 0;
	}
};

=======
	GameMode() : type(GM_EASY), maxBombs(0), width(0), height(0), origin(100, 130) {}
	GameMode(GameModeType t, int m, int w, int h, const ds::vec2& o) : type(t), maxBombs(m), width(w), height(h), origin(o) {}
};

const GameMode GAME_MODES[] = {
	GameMode(GM_EASY, 12, 8, 8, ds::vec2(357, 259)),
	GameMode(GM_MEDIUM, 32, 14, 12, ds::vec2(237, 189)),
	GameMode(GM_HARD, 60, 20, 15, ds::vec2(107,134)),
};
>>>>>>> origin/2D
