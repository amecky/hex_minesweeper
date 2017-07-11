#pragma once
#include <diesel.h>

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

	GameMode() : type(GM_EASY), maxBombs(0), width(0), height(0), origin(100, 130) {}
	GameMode(GameModeType t, int m, int w, int h, const ds::vec2& o) : type(t), maxBombs(m), width(w), height(h), origin(o) {}
};

const GameMode GAME_MODES[] = {
	GameMode(GM_EASY, 12, 8, 8, ds::vec2(357, 259)),
	GameMode(GM_MEDIUM, 32, 14, 12, ds::vec2(237, 189)),
	GameMode(GM_HARD, 60, 20, 15, ds::vec2(107,134)),
};

struct GameSettings {

	float wiggleScale;
	float wiggleTTL;

	float numberScaleAmplitude;
	float numberScaleTTL;

	float menuTTL;
};