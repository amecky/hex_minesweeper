#include "MainGameState.h"
#include <Vector.h>
#include <resources\ResourceContainer.h>
#include <base\InputSystem.h>

MainGameState::MainGameState(GameContext* context, ds::Game* game) : ds::GameState("MainGame", game), _context(context) {
	_scene = game->create2DScene("Sprites");
	_material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);
	_scene->setActive(true);
	_selected = -1;
	_maxBombs = 60;
	_showBombs = false;
	_endTimer = 0.0f;
	_context->mode = 1;
	// closed
	_textures[0] = math::buildTexture(ds::Rect(0, 40, 40, 44));
	// marked
	_textures[1] = math::buildTexture(ds::Rect(0, 120, 40, 44));
	// closed
	_textures[2] = math::buildTexture(ds::Rect(0, 40, 40, 44));
	for (int i = 0; i < 6; ++i ) {
		_textures[i + 3] = math::buildTexture(ds::Rect(50, i * 40, 40, 44));
	}
	_textures[9] = math::buildTexture(ds::Rect(0, 80, 40, 44));
}

MainGameState::~MainGameState() {

}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void MainGameState::init() {
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
		int idx = math::random(0, total - 1);
		Hex t = temp[i];
		temp[i] = temp[idx];
		temp[idx] = t;
	}
	for (int i = 0; i < _maxBombs; ++i) {
		_grid.markAsBomb(temp[i]);
	}
	Hex n[6];
	_scene->clear();
	for (int r = 0; r < _height; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _width - q_offset; q++) {
			Hex h = Hex(q, r);
			if (_grid.isValid(h)) {
				int cnt = _grid.neighbors(h, n);
				GridItem& current = _grid.get(h);
				current.id = _scene->add(current.position, _textures[0], _material);
				for (int i = 0; i < cnt; ++i) {
					const GridItem& item = _grid.get(n[i]);
					if (item.bomb) {
						++current.adjacentBombs;
					}					
				}
			}
		}
	}
	for (int r = 0; r < _width + 1; ++r) {
		v2 p = _grid.convert(-_width / 2 + r + _gridOffset, _height);
		_scene->add(p, _textures[9], _material);		
		p = _grid.convert(r, -1);
		_scene->add(p, _textures[9], _material);
	}

	for (int r = 0; r < _height; ++r) {
		int q_offset = r >> 1;
		v2 p = _grid.convert(-r / 2 - 1, r);
		_scene->add(p, _textures[9], _material);
		p = _grid.convert(-r / 2 + _width, r);
		_scene->add(p, _textures[9], _material);
	}
	
	delete[] temp;
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	const GameMode& mode = GAME_MODES[_context->mode];
	_grid.resize(mode.width, mode.height);
	_grid.setOrigin(mode.origin);
	_width = mode.width;
	_height = mode.height;
	_maxBombs = mode.maxBombs;
	_gridOffset = 1;
	switch (_context->mode) {
		case 0: _gridOffset = 0; break;
		case 1: _gridOffset = 1; break;
		case 2: _gridOffset = 2; break;
	}
	fillBombs();
	_context->marked = 0;

	_context->markedCorrectly = 0;	
	_context->hud->resetTimer(3);
	_context->hud->startTimer(3);
	char buffer[32];
	sprintf_s(buffer, 32, "%d / %d", _maxBombs, _maxBombs);
	_context->hud->updateText(2, buffer);
	_showBombs = false;
	_endTimer = 0.0f;
	_hover = -1;
	_scene->setActive(true);
	_leftClick = false;
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	_scene->setActive(false);
	_context->hud->deactivate();
}
// -------------------------------------------------------
// open empty tiles
// -------------------------------------------------------
void MainGameState::openEmptyTiles(const Hex& h) {
	Hex n[6];
	int cnt = _grid.neighbors(h, n);
	GridItem& current = _grid.get(h);
	current.state = GIS_OPEN;
	_scene->setTexture(current.id, _textures[3]);
	for (int i = 0; i < cnt; ++i) {
		GridItem& item = _grid.get(n[i]);
		if (item.state == 0 && item.adjacentBombs == 0) {			
			openEmptyTiles(n[i]);
		}
		else if (item.state == 0) {
			_scene->setTexture(item.id, _textures[3]);
			item.state = GIS_OPEN;
		}
	}
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	Hex h = _grid.convertFromMousePos();

	if (ds::input::isMouseButtonPressed(0)) {
		if (!_leftClick) {
			_leftClick = true;
			LOG << "======> left clicked";
		}
	}
	else {
		if (_leftClick) {
			_leftClick = false;
			LOG << "======> left released";
		}
	}
	if (_grid.isValid(h)) {
		// right button -> mark cell or remove mark
		if (button == 1) {			
			GridItem& item = _grid.get(h);
			if (item.state == GIS_CLOSED) {
				if (_context->marked < _maxBombs) {
					item.state = GIS_MARKED;
					_scene->setTexture(item.id, _textures[1]);
					++_context->marked;
					if (item.bomb) {
						++_context->markedCorrectly;
					}
				}
			}
			else if (item.state == GIS_MARKED) {
				if (item.bomb) {
					--_context->markedCorrectly;
				}
				item.state = GIS_CLOSED;
				_scene->setTexture(item.id, _textures[0]);
				--_context->marked;
			}

			if (_context->markedCorrectly == _maxBombs) {
				return 1;
			}
			int left = _maxBombs - _context->marked;
			char buffer[32];
			sprintf_s(buffer, 32, "%d / %d", left, _maxBombs);
			_context->hud->updateText(2, buffer);
		}
		// left button
		else {
			GridItem& item = _grid.get(h);
			if (item.state == GIS_CLOSED) {
				if (item.bomb) {
					_endTimer = 0.0f;
					_showBombs = true;
					_context->hud->deactivate();
					for (int i = 0; i < _grid.size(); ++i) {
						const GridItem& current = _grid.get(i);
						if (current.bomb) {
							_scene->setTexture(current.id, _textures[1]);
						}
					}
				}
				else {
					item.state = GIS_OPEN;
					_scene->setTexture(item.id, _textures[item.adjacentBombs + 3]);
					if (item.adjacentBombs == 0) {
						openEmptyTiles(h);
					}
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
	Hex h = _grid.convertFromMousePos();
	// wiggle hover piece
	if (_grid.isValid(h)) {
		int current = _grid.getIndex(h);
		if (current != _hover) {
			_hover = current;
			GridItem& item = _grid.get(h);
			if (item.state == 0) {
				_scene->scaleTo(item.id, v2(1, 1), v2(0.8f, 0.8f), 0.2f, 0, tweening::easeSinus);
			}

		}
	}

	_context->hud->tick(dt);

	if (_showBombs) {
		_endTimer += dt;
		if (_endTimer > 2.0f) {
			return 1;
		}
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
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