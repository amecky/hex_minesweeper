#pragma once
#include <utils\Color.h>
#include <math\math_types.h>

const float CELL_SIZE = 1.0f;
const float HALF_CELL_SIZE = 0.5f;
const float STARTX = -10.0f;
const float STARTY = 0.5f;
const int MAX_X = 20;
const int MAX_Y = 12;


enum BoardEvents {
	BE_NONE,
	BE_INVALID_SELECTION,
	BE_SCORE,
	BE_EOL
};

enum GameEvents {
	GE_BASIC,
	GE_SHOW_TEST_GUI,
	GE_HIDE_TEST_GUI,
	GE_REBUILD_BOARD,
	GE_END_GAME,
	GE_TOGGLE_SHOW_BOARD,
	GE_TOGGLE_SHOW_STATES,
	GE_FADE_TO_GRAY,
	GE_TOGGLE_SETTINGS
};

const ds::Color GRASS = ds::Color(184, 203, 98, 255);
const ds::Color CURBS = ds::Color(223, 215, 204, 255);
const ds::Color STREET = ds::Color(151, 144, 138, 255);
