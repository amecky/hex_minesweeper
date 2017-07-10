#pragma once
#include "HexGrid.h"
#include <diesel.h>
#include <SpriteBatchBuffer.h>

struct ButtonState {
	bool pressed;
	bool released;
};


class Board {

public:
	Board(SpriteBatchBuffer* sprites);
	~Board();
	void fillBombs();
	void render();
	void activate(int modeIndex);
	void toggleShowBombs() {
		_showBombs = !_showBombs;
	}
	bool select();
	int getNumMarked() const {
		return _marked;
	}
private:
	void openEmptyTiles(const Hex& h);
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

