#include "MainGameState.h"
#include <sprites\SpriteBatch.h>
#include <Vector.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_selected = -1;
	_maxBombs = 60;
	
}


MainGameState::~MainGameState() {
	
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void MainGameState::init() {
	
	ds::assets::load("hud", _context->hud, ds::CVT_HUD);
}

// -------------------------------------------------------
// fill bombs
// -------------------------------------------------------
void MainGameState::fillBombs() {
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
		int idx = ds::math::random(0, total - 1);
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
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	LOG << "game mode: " << _context->mode;
	const GameMode& mode = GAME_MODES[_context->mode];
	_grid.resize(mode.width, mode.height);
	_grid.setOrigin(mode.origin);
	_width = mode.width;
	_height = mode.height;
	_maxBombs = mode.maxBombs;
	fillBombs();
	_context->marked = 0;
	_context->markedCorrectly = 0;
	_context->hud->setTimer(0, 0, 0);
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
}
// -------------------------------------------------------
// open empty tiles
// -------------------------------------------------------
void MainGameState::openEmptyTiles(const Hex& h) {
	Hex n[6];
	int cnt = _grid.neighbors(h, n);
	GridItem& current = _grid.get(h);
	current.state = 1;
	for (int i = 0; i < cnt; ++i) {
		GridItem& item = _grid.get(n[i]);
		if (item.state == 0 && item.adjacentBombs == 0) {
			openEmptyTiles(n[i]);
		}
		else if (item.state == 0) {
			item.state = 1;
		}
	}
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	Hex h = _grid.convertFromMousePos();
	if (_grid.isValid(h)) {
		// right button -> mark cell or remove mark
		if (button == 1) {			
			GridItem& item = _grid.get(h);
			if (item.state == 0) {
				if (_context->marked < _maxBombs) {
					item.state = 2;
					++_context->marked;
					if (item.bomb) {
						++_context->markedCorrectly;
					}
				}
			}
			else if (item.state == 2) {
				if (item.bomb) {
					--_context->markedCorrectly;
				}
				item.state = 0;
				--_context->marked;
			}

			if (_context->markedCorrectly == _maxBombs) {
				return 1;
			}
			int left = _maxBombs - _context->marked;
			_context->hud->setCounterValue(0, left);
		}
		// left button
		else {
			GridItem& item = _grid.get(h);
			if (item.state == 0) {
				if (item.bomb) {
					return 1;
				}
				item.state = 1;
				if (item.adjacentBombs == 0) {
					openEmptyTiles(h);
				}
			}			
		}
	}
	return 0;
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
int MainGameState::update(float dt) {
	_context->hud->update(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
	for (int i = 0; i < _grid.size(); ++i) {
		const GridItem& item = _grid.get(i);
		// marked
		if (item.state == 2) {
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(0, 120, 40, 44)));
		}
		// opened
		else if (item.state == 1) {
			int offset = item.adjacentBombs * 40;
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(50, offset, 40, 44)));
		}
		// closed
		else {
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(0, 40, 40, 44)));
		}
	}
	_context->hud->render();
	
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'e') {
		return 1;
	}
	if (ascii == 'r') {
		fillBombs();
	}
	return 0;
}