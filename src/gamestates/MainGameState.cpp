#include "MainGameState.h"
#include <Vector.h>
#include <resources\ResourceContainer.h>
#include <base\InputSystem.h>
#include <gen\MeshGen.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	//_selected = -1;
	_maxBombs = 60;
	_showBombs = false;
	_endTimer = 0.0f;
	_context->mode = 1;

	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_scene = ds::res::getScene("World");
	_boardScene = ds::res::getScene("Board");
	_boardTexScene = ds::res::getScene("BoardTex");
	//  camera at : x: -8.12998 y: 13.2933 z: -1.76021 target: x: -0.00436331 y: 0 z: 0.99999 pitch: 75.4999 yaw: -0.25
	_camera->setPosition(v3(-8.0f, 13.0f, -1.7f), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(75.0f));
	//_camera->resetYAngle();
	//graphics::setCamera(_camera);
	_colouredBuffer = ds::res::getMeshBuffer("ColouredBuffer");
	ds::gen::MeshGen gen;
	gen.set_color_selection(ds::Color(235,123,89));
	gen.create_cylinder(v3(0, 0, 0), 0.45f, 0.45f, 0.2f, 6,DEGTORAD(90.0f));
	gen.set_color(9, ds::Color(128, 128, 128));
	gen.set_color(10, ds::Color(128, 128, 128));
	gen.set_color(11, ds::Color(128, 128, 128));
	gen.set_color(12, ds::Color(128, 128, 128));
	//gen.debug_colors();
	_hexagon = new ds::Mesh();
	gen.build(_hexagon);
	
	gen.clear();
	_bomb = new ds::Mesh();	
	float pp = 0.3f;
	v3 p[] = { v3(-pp, 0.0f, pp), v3(pp, 0.0f, pp), v3(pp, 0.0f, -pp), v3(-pp, 0.0f, -pp) };
	gen.add_face(p);
	gen.texture_face(0, math::buildTexture(650, 520, 70, 70));
	gen.build(_bomb);

	// numbers
	gen.clear();	
	gen.add_face(p);
	for (int i = 0; i < 6; ++i) {
		ds::Mesh* m = new ds::Mesh();		
		gen.texture_face(0, math::buildTexture(580, 520 + i * 70, 70, 70));
		gen.build(m);
		_numbers.push_back(m);
	}
	
	_material = ds::res::find("MeshMaterial", ds::ResourceType::MATERIAL);
	_texMaterial = ds::res::find("TextureMeshMaterial", ds::ResourceType::MATERIAL);
	//_hud = ds::res::getGUIDialog("HUD");
	_border = new ds::Mesh;
	
}

MainGameState::~MainGameState() {
	delete _border;
	delete _bomb;
	delete _hexagon;
	_numbers.destroy_all();
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
	float sz = 0.45f;
	gen.set_color_selection(ds::Color(192, 192, 192));
	for (int r = 0; r < _width + 1; ++r) {
		v3 p = _grid.convert(-_width / 2 + r + 1, _height);
		gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
		p = _grid.convert(r, -1);
		gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
	}
	for (int r = 0; r < _height + 1; ++r) {
		int q_offset = r >> 1;
		v3 p = _grid.convert(-r / 2 - 1,r);
		gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
		p = _grid.convert(-r /2 + _width, r);
		gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
	}
	gen.build(_border);

	_scene->add(_border, v3(0,0,0), _material);

	// FIXME: _scene->clear();
	for (int i = 0; i < _grid.size(); ++i) {
		GridItem& item = _grid.get(i);
		item.numberID = INVALID_ID;
		item.id = _boardScene->add(_hexagon, v3(item.position.x, 0.0f, item.position.y), _material, ds::DrawMode::TRANSFORM);
		if (item.bomb) {
			_boardTexScene->add(_bomb, v3(item.position.x, 0.15f, item.position.y), _texMaterial,ds::DrawMode::TRANSFORM);
		}
		else if (item.adjacentBombs > 0) {
			item.numberID = _boardTexScene->add(_numbers[item.adjacentBombs], v3(item.position.x, 0.15f, item.position.y), _texMaterial, ds::DrawMode::TRANSFORM);
			_boardTexScene->deactivate(item.numberID);
		}
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
			_boardScene->rotate(item.id,v3(0.0f, 0.0f, PI));
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
	ID id = _boardScene->intersects(r);
	if (id != INVALID_ID) {
		int idx = -1;
		for (int i = 0; i < _grid.size(); ++i) {
			const GridItem& item = _grid.get(i);
			if (item.id == id) {
				idx = i;
			}
		}
		if (idx != -1) {
			GridItem& item = _grid.get(idx);
			LOG << "item: " << item.id << " adjacents: " << item.adjacentBombs << " bomb: " << item.bomb;
			Hex h = item.hex;
			// right button -> mark cell or remove mark
			if (button == 1) {
				if (item.state == 0) {
					if (_context->marked < _maxBombs) {
						
						// FIXME: place marker
						
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
					_boardScene->rotate(item.id, v3(0.0f, 0.0f, PI));
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
				if (item.state == 0) {
					if (item.bomb) {
						//return 1;
						_endTimer = 0.0f;
						_showBombs = true;
						//_context->hud->deactivate();

						// FIXME: game over!!!!

					}
					item.state = 1;
					_boardScene->rotate(item.id, v3(0.0f, 0.0f, PI));
					if (item.numberID != INVALID_ID) {
						_boardTexScene->activate(item.numberID);
					}
					LOG << "adjacents: " << item.adjacentBombs;
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
	_scene->draw();
	_boardScene->draw();
	_boardTexScene->draw();
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
	if (ascii == 't') {
		LOG << "camera at : " << DBG_V3(_camera->getPosition()) << " target: " << DBG_V3(_camera->getTarget()) << " pitch: " << RADTODEG(_camera->getPitch()) << " yaw: " << RADTODEG(_camera->getYaw());
	}
	return 0;
}