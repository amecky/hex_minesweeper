#include "MainGameState.h"
#include <Vector.h>
#include <resources\ResourceContainer.h>
#include <base\InputSystem.h>
#include <gen\MeshGen.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_selected = -1;
	_maxBombs = 60;
	_showBombs = false;
	_endTimer = 0.0f;
	_context->mode = 1;

	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_scene = ds::res::getScene("World");
	//_camera->resetPitch(DEGTORAD(45.0f));
	//_camera->resetYAngle();
	//graphics::setCamera(_camera);
	_colouredBuffer = ds::res::getMeshBuffer("ColouredBuffer");
	ds::gen::MeshGen gen;
	gen.set_color_selection(ds::Color(192, 0, 0));
	gen.create_cylinder(v3(0, 0, 0), 0.475f, 0.475f, 0.2f, 6);
	//gen.debug_colors();
	_hexagon = new ds::Mesh();
	gen.build(_hexagon);
	_material = ds::res::find("MeshMaterial", ds::ResourceType::MATERIAL);
	//_hud = ds::res::getGUIDialog("HUD");
	_border = new ds::Mesh;
	
}

MainGameState::~MainGameState() {
	delete _border;
	delete _hexagon;
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
	_border->clear();
	ds::gen::MeshGen gen;
	gen.set_color_selection(ds::Color(192, 192, 192));
	for (int r = 0; r < _width + 1; ++r) {
		v3 p = _grid.convert(-_width / 2 + r + 1, _height);
		gen.create_cylinder(p, 0.475f, 0.475f, 0.2f, 6);
		p = _grid.convert(r, -1);
		gen.create_cylinder(p, 0.475f, 0.475f, 0.2f, 6);
	}
	for (int r = 0; r < _height + 1; ++r) {
		int q_offset = r >> 1;
		v3 p = _grid.convert(-r / 2 - 1,r);
		gen.create_cylinder(p, 0.475f, 0.475f, 0.2f, 6);
		p = _grid.convert(-r /2 + _width, r);
		gen.create_cylinder(p, 0.475f, 0.475f, 0.2f, 6);
	}
	gen.build(_border);

	_scene->add(_border, v3(0,0,0), _material);// , ds::Color(128, 0, 0, 255));// , v

	// FIXME: _scene->clear();
	for (int i = 0; i < _grid.size(); ++i) {
		const GridItem& item = _grid.get(i);
		_scene->add(_hexagon, v3(item.position.x, 0.0f, item.position.y),_material);// , ds::Color(128, 0, 0, 255));// , v
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
	fillBombs();
	_context->marked = 0;
	_context->markedCorrectly = 0;	
	//_context->hud->resetTimer(3);
	//_context->hud->startTimer(3);
	//_context->hud->setNumber(2, _maxBombs);
	char buffer[32];
	sprintf_s(buffer, 32, "%d / %d", _maxBombs, _maxBombs);
	//_context->hud->updateText(2, buffer);
	_showBombs = false;
	_endTimer = 0.0f;
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	//_context->hud->deactivate();
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
	
	ds::Ray r = graphics::getCameraRay(_camera);
	ID id = _scene->intersects(r);
	if (id != INVALID_ID) {
		LOG << "selected: " << id;
	}


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
			//_context->hud->setNumber(2, left);
			char buffer[32];
			sprintf_s(buffer, 32, "%d / %d", left, _maxBombs);
			//_context->hud->updateText(2, buffer);
		}
		// left button
		else {
			GridItem& item = _grid.get(h);
			if (item.state == 0) {
				if (item.bomb) {
					//return 1;
					_endTimer = 0.0f;
					_showBombs = true;
					//_context->hud->deactivate();

					// FIXME: game over!!!!

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
	Hex h = _grid.convertFromMousePos();
	_grid.update(dt);
	//_context->hud->tick(dt);
	if (_showBombs) {
		_endTimer += dt;
		if (_endTimer > 2.0f) {
			return 1;
		}
	}
	_camera->update(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	//_sprites->begin();
	//ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
	_scene->draw();
	/*
	graphics::setCamera(_camera);
	graphics::turnOnZBuffer();
	_colouredBuffer->begin();

	_colouredBuffer->add(_border);

	if (_context->mode == 0) {
		//_easyGroup->render();
	}
	else if (_context->mode == 1) {
		//_mediumGroup->render();
	}
	if (_context->mode == 2) {
		//_hardGroup->render();
	}
	for (int i = 0; i < _grid.size(); ++i) {
		const GridItem& item = _grid.get(i);
		if (_showBombs && item.bomb) {
			//_sprites->draw(item.position, math::buildTexture(ds::Rect(0, 120, 40, 44)));
		}
		else {
			// marked
			if (item.state == 2) {
				//_sprites->draw(item.position, math::buildTexture(ds::Rect(0, 120, 40, 44)));
			}
			// opened
			else if (item.state == 1) {
				int offset = item.adjacentBombs * 40;
				//_sprites->draw(item.position, math::buildTexture(ds::Rect(50, offset, 40, 44)));
			}
			// closed
			else {
				//_sprites->draw(item.position, math::buildTexture(ds::Rect(0, 40, 40, 44)),0.0f,item.scale);
				_colouredBuffer->add(_hexagon, v3(item.position.x, 0.0f, item.position.y), ds::Color(128,0,0,255));// , v3(1, 1, 1), v3(DEGTORAD(90.0f), 0.0f, 0.0f));// , c->scale, c->rotation);
			}
		}
		
	}
	_colouredBuffer->end();
	*/
	//_sprites->end();
	//_context->hud->render();
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