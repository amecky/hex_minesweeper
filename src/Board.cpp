#include "Board.h"
#include "GameContext.h"

Board::Board(SpriteBatchBuffer* sprites) : _sprites(sprites) , _showBombs(false) {
	// closed
	_textures[0] = ds::vec4(40, 0, 40, 44);
	// marked
	_textures[1] = ds::vec4(120, 0, 40, 44);
	// closed
	_textures[2] = ds::vec4(40, 0, 40, 44);
	for (int i = 0; i < 6; ++i) {
		_textures[i + 3] = ds::vec4(i * 40, 50, 40, 44);
	}
	_textures[9] = ds::vec4(80, 0, 40, 44);
}

Board::~Board() {
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void Board::activate(int modeIndex) {
	const GameMode& mode = GAME_MODES[modeIndex];
	_grid.resize(mode.width, mode.height);
	_grid.setOrigin(mode.origin);
	_width = mode.width;
	_height = mode.height;
	_maxBombs = mode.maxBombs;
	_gridOffset = 1;
	switch (modeIndex) {
		case 0: _gridOffset = 0; break;
		case 1: _gridOffset = 1; break;
		case 2: _gridOffset = 2; break;
	}
	fillBombs();
	_marked = 0;
	_markedCorrectly = 0;
	_buttonState[0] = { false, false };
	_buttonState[1] = { false, false };
	_showBombs = false;
}

// -------------------------------------------------------
// open empty tiles
// -------------------------------------------------------
void Board::openEmptyTiles(const Hex& h) {
	Hex n[6];
	int cnt = _grid.neighbors(h, n);
	GridItem& current = _grid.get(h);
	current.state = GIS_OPEN;
	_openTiles[_numOpen++] = h;
	for (int i = 0; i < cnt; ++i) {
		GridItem& item = _grid.get(n[i]);
		if (item.state == 0 && item.adjacentBombs == 0 && !item.bomb) {
			openEmptyTiles(n[i]);
		}
	}
}

// -------------------------------------------------------
// select
// -------------------------------------------------------
bool Board::select() {
	Hex h = _grid.convertFromMousePos();
	if (_grid.isValid(h)) {
		if (ds::isMouseButtonPressed(0) && !_buttonState[0].pressed) {
			_buttonState[0].released = true;
			_buttonState[0].pressed = true;
			GridItem& item = _grid.get(h);
			if (item.state == GIS_CLOSED) {
				// game over
				if (item.bomb) {
					_showBombs = true;
					return false;
				}
				else {
					item.state = GIS_OPEN;
					if (item.adjacentBombs == 0) {
						_numOpen = 0;
						openEmptyTiles(h);
						Hex n[6];
						for (uint32_t i = 0; i < _numOpen; ++i) {
							const Hex& h = _openTiles[i];
							int cnt = _grid.neighbors(h, n);
							for (int j = 0; j < cnt; ++j) {
								GridItem& gi = _grid.get(n[j]);
								if (!gi.bomb) {
									gi.state = GIS_OPEN;
								}
							}
						}
					}
				}
			}
		}
		if (!ds::isMouseButtonPressed(0) && _buttonState[0].pressed) {
			_buttonState[0].pressed = false;
			_buttonState[0].released = false;
		}
		if (ds::isMouseButtonPressed(1) && !_buttonState[1].pressed) {
			_buttonState[1].released = true;
			_buttonState[1].pressed = true;
			GridItem& item = _grid.get(h);
			if (item.state == GIS_CLOSED) {
				if (_marked < _maxBombs) {
					item.state = GIS_MARKED;
					++_marked;
					if (item.bomb) {
						++_markedCorrectly;
					}
				}
			}
			else if (item.state == GIS_MARKED) {
				if (item.bomb) {
					--_markedCorrectly;
				}
				item.state = GIS_CLOSED;
				--_marked;
			}
			if (_markedCorrectly == _maxBombs) {
				return false;
			}
			int left = _maxBombs - _marked;
		}
		if (!ds::isMouseButtonPressed(1) && _buttonState[1].pressed) {
			_buttonState[1].pressed = false;
			_buttonState[1].released = false;
		}
	}
	return true;
}

// -------------------------------------------------------
// fill bombs
// -------------------------------------------------------
void Board::fillBombs() {
	_grid.fill();
	int total = _width * _height;
	Hex* temp = new Hex[total];
	int cnt = 0;
	for (int r = 0; r < _height; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _width - q_offset; q++) {
			temp[cnt++] = Hex(q, r);
		}
	}
	for (int i = 0; i < total; ++i) {
		int idx = ds::random(0, total - 1);
		Hex t = temp[i];
		temp[i] = temp[idx];
		temp[idx] = t;
	}
	for (int i = 0; i < _maxBombs; ++i) {
		_grid.markAsBomb(temp[i]);
	}
	Hex n[6];
	for (int r = 0; r < _height; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _width - q_offset; q++) {
			Hex h = Hex(q, r);
			if (_grid.isValid(h)) {
				int cnt = _grid.neighbors(h, n);
				GridItem& current = _grid.get(h);
				for (int i = 0; i < cnt; ++i) {
					const GridItem& item = _grid.get(n[i]);
					if (item.bomb) {
						++current.adjacentBombs;
					}
				}
			}
		}
	}
	

	delete[] temp;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void Board::render() {
	for (int r = 0; r < _height; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _width - q_offset; q++) {
			Hex h = Hex(q, r);
			if (_grid.isValid(h)) {
				const GridItem& current = _grid.get(h);
				if (_showBombs && current.bomb) {
					_sprites->add(current.position, _textures[1]);
				}
				else {
					if (current.state == GIS_OPEN) {
						_sprites->add(current.position, _textures[current.adjacentBombs + 3]);
					}
					else if (current.state == GIS_MARKED) {
						_sprites->add(current.position, _textures[1]);
					}
					else {
						_sprites->add(current.position, _textures[0]);
					}
				}
			}
		}
	}
}