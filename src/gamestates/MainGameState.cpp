#include "MainGameState.h"
#include <Vector.h>
#include <resources\ResourceContainer.h>
#include <base\InputSystem.h>


MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_gameOverDialog = ds::res::getGUIDialog("GameOver");
	_hud = ds::res::getGUIDialog("HUD");
	_mode = GM_RUNNING;
	ds::SceneDescriptor sdef;
	_scene = new ds::Scene2D(sdef);
	_material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);
	_scene->setActive(true);
	_scene->useRenderTarget("RT1");
	
	// grayfade post process
	ds::GrayFadePostProcessDescriptor gfDesc;
	gfDesc.ttl = 2.0f;
	gfDesc.source = ds::res::find("RT1", ds::ResourceType::RENDERTARGET);
	//gfDesc.target = ds::res::find("RT2", ds::ResourceType::RENDERTARGET);
	_grayfade = new ds::GrayFadePostProcess(gfDesc);
	_grayfade->deactivate();
	_scene->addPostProcess(_grayfade);


	// screen shake
	//ds::ScreenShakePostProcessDescriptor desc;
	//desc.ttl = 0.7f;
	//desc.frequency = 5.0f;
	//desc.shakeAmount = 6.0f;
	//desc.source = ds::res::find("RT2", ds::ResourceType::RENDERTARGET);
	//_screenShake = new ds::ScreenShakePostProcess(desc);
	//_screenShake->deactivate();
	//_scene->addPostProcess(_screenShake);

	_scene->activateRenderTarget();

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
	delete _grayfade;
	//delete _screenShake;
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
	_hud->activate();
	_hud->resetTimer(3);
	_hud->startTimer(3);
	_hud->setNumber(2, _maxBombs);
	_showBombs = false;
	_endTimer = 0.0f;
	_hover = -1;
	_scene->setActive(true);
	_leftClick = false;
	_grayfade->deactivate();
	//_screenShake->deactivate();
	_mode = GM_RUNNING;
	_gameOverDialog->deactivate();
	_scene->deactivateRenderTarget();
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	_scene->setActive(false);
	_hud->deactivate();
}
// -------------------------------------------------------
// open empty tiles
// -------------------------------------------------------
void MainGameState::openEmptyTiles(const Hex& h, ds::Array<Hex>& opened) {
	Hex n[6];
	int cnt = _grid.neighbors(h, n);
	GridItem& current = _grid.get(h);
	current.state = GIS_OPEN;
	opened.push_back(h);
	_scene->setTexture(current.id, _textures[3]);
	for (int i = 0; i < cnt; ++i) {
		GridItem& item = _grid.get(n[i]);
		if (item.state == 0 && item.adjacentBombs == 0 && !item.bomb) {			
			openEmptyTiles(n[i],opened);
		}
	}
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	if (_mode == GM_OVER) {
		// FIXME: include some delay if someone has clicked accidentially
		int ret = _gameOverDialog->onButton(button, x, y, true);
		if (ret != -1) {
			return ret;
		}
	}
	else {
		Hex h = _grid.convertFromMousePos();
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
					stopGame();
				}
				int left = _maxBombs - _context->marked;
				_hud->setNumber(2, left);
			}
			// left button
			else {
				GridItem& item = _grid.get(h);
				if (item.state == GIS_CLOSED) {
					// game over
					if (item.bomb) {

						stopGame();

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
							ds::Array<Hex> opened;
							openEmptyTiles(h, opened);
							Hex n[6];
							for (uint32_t i = 0; i < opened.size(); ++i) {
								const Hex& h = opened[i];
								int cnt = _grid.neighbors(h, n);
								for (int j = 0; j < cnt; ++j) {
									GridItem& gi = _grid.get(n[j]);
									if (!gi.bomb) {
										_scene->setTexture(gi.id, _textures[gi.adjacentBombs + 3]);
										gi.state = GIS_OPEN;
									}
								}
							}
						}
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
	if (_mode == GM_RUNNING) {
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

		_hud->tick(dt);

	}	
	_scene->tick(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	_scene->draw();
	if (_mode == GM_RUNNING) {
		_hud->render();
	}
	_gameOverDialog->render();
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'e') {
		stopGame();
	}
	if (ascii == 'r') {
		fillBombs();
	}
	if (ascii == 'z') {
		_grayfade->activate();
	}
	if (ascii == 'u') {
		_grayfade->deactivate();
	}
	return 0;
}

void MainGameState::stopGame() {
	_grayfade->activate();
	_endTimer = 0.0f;
	_showBombs = true;
	_mode = GM_OVER;
	_gameOverDialog->activate();
	_hud->deactivate();
	_gameOverDialog->updateTextFormatted(12, "%d / %d", _context->markedCorrectly, GAME_MODES[_context->mode].maxBombs);
	ds::GameTimer* timer = _hud->getTimer(3);
	_gameOverDialog->updateTextFormatted(14, "%02d:%02d", timer->getMinutes(), timer->getSeconds());
	int state = 1;
	if (_context->markedCorrectly == GAME_MODES[_context->mode].maxBombs) {
		state = 2;
		_gameOverDialog->updateImage(11, 140, 650, ds::Rect(450, 0, 465, 85));
		Highscore hs;
		int ss = _context->highscores[_context->mode].add(hs);
		LOG << "add score at: " << ss;
		if (ss == 0) {
			state = 3;
		}
	}
	else {
		_gameOverDialog->updateImage(11, 140, 660, ds::Rect(450, 470, 440, 85));
	}
	if (state == 3) {
		_gameOverDialog->updateText(19, "Congratulations - New highscore");
	}
	else if (state == 2) {
		_gameOverDialog->updateText(19, "Try to be faster next time");
	}
	else {
		_gameOverDialog->updateText(19, "You have unveiled a bomb");
	}
	_scene->activateRenderTarget();
}