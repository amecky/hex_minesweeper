#pragma once
#include "HexGrid.h"
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include "GameContext.h"

struct ButtonState {
	bool pressed;
	bool released;
};


class Board {

public:
	Board(SpriteBatchBuffer* sprites,GameSettings* settings);
	~Board();
	void fillBombs();
	void render();
	void tick(float dt);
	void activate(int modeIndex);
	void toggleShowBombs() {
		_showBombs = !_showBombs;
	}
	bool select(ds::vec2* opened, int max, int* num);
	int getNumMarked() const {
		return _marked;
	}
	int getMarkedCorrectly() const {
		return _markedCorrectly;
	}
private:
	void openEmptyTiles(const Hex& h);
	GameSettings* _settings;
	ButtonState _buttonState[2];
	SpriteBatchBuffer* _sprites;
	HexGrid _grid;
	ds::vec4 _textures[10];
	int _selected;
	int _width;
	int _height;
	bool _showBombs;
	float _endTimer;
	Hex _current;
	int _hover;
	int _maxBombs;
	int _gridOffset;
	int _marked;
	int _markedCorrectly;
	Hex _openTiles[300];
	int _numOpen;
};

