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
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_scene = ds::res::getScene("World");
	_boardScene = ds::res::getScene("Board");
	_boardTexScene = ds::res::getScene("BoardTex");
	//  camera at : x: -8.12998 y: 13.2933 z: -1.76021 target: x: -0.00436331 y: 0 z: 0.99999 pitch: 75.4999 yaw: -0.25
	_camera->setPosition(v3(-8.0f, 13.0f, -1.7f), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(75.0f));

	//_colouredBuffer = ds::res::getMeshBuffer("ColouredBuffer");
	objects::build_objects(&_objects);

	
	
	_material = ds::res::find("MeshMaterial", ds::ResourceType::MATERIAL);
	_texMaterial = ds::res::find("TextureMeshMaterial", ds::ResourceType::MATERIAL);
	_hud = ds::res::getGUIDialog("HUD");
	
	
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

	_scene->clear();
	objects::build_border(&_objects, _grid);	
	_scene->add(_objects.border, v3(0,0,0), _material);

	_boardScene->clear();
	_boardTexScene->clear();
	for (int i = 0; i < _grid.size(); ++i) {
		GridItem& item = _grid.get(i);
		item.numberID = INVALID_ID;
		item.id = _boardScene->add(_objects.hexagon, v3(item.position.x, 0.0f, item.position.y), _material, ds::DrawMode::TRANSFORM);
		_boardScene->setColor(item.id, ds::Color(255,255,255));
		_boardScene->moveTo(item.id, v3(item.position.x, -20.0f - math::random(0.0f,25.0f), item.position.y), v3(item.position.x, 0.0f, item.position.y), 0.5f);
		if (!item.bomb && item.adjacentBombs > 0) {
			item.numberID = _boardTexScene->add(_objects.numbers[item.adjacentBombs - 1], v3(item.position.x, 0.15f, item.position.y), _texMaterial, ds::DrawMode::TRANSFORM);
			_boardTexScene->deactivate(item.numberID);
		}
	}
	delete[] temp;
}

// -------------------------------------------------------
// show bombs
// -------------------------------------------------------
void MainGameState::showBombs() {
	for (int i = 0; i < _grid.size(); ++i) {
		GridItem& item = _grid.get(i);
		if (item.bomb) {
			_boardTexScene->add(_objects.bomb, v3(item.position.x, 0.15f, item.position.y), _texMaterial, ds::DrawMode::TRANSFORM);
		}
	}
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
	LOG << "activate - mode: " << _context->mode << " width: " << _width << " height: " << _height << " bombs: " << _maxBombs;
	fillBombs();
	_context->marked = 0;
	_context->markedCorrectly = 0;	
	_hud->resetTimer(3);
	_hud->startTimer(3);
	char buffer[32];
	sprintf_s(buffer, 32, "%d / %d", _maxBombs, _maxBombs);
	_hud->updateText(2, buffer);
	_showBombs = false;
	_endTimer = 0.0f;
}

// -------------------------------------------------------
// deactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	_hud->deactivate();
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
			_boardScene->remove(item.id);
			//_boardScene->setColor(item.id, ds::Color(128, 128, 128));
			//item.timer = 0.0f;
			//item.rotating = true;
			openEmptyTiles(n[i]);
		}
		//else if (item.state == 0) {
			//item.state = 1;
		//}
	}
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	LOG << "button: " << button << " x: " << x << " y: " << y;
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
			LOG << "item: " << item.id << " adjacents: " << item.adjacentBombs << " bomb: " << item.bomb << " state: " << item.state << " hex q: " << item.hex.q << " r: " << item.hex.r;
			Hex h = item.hex;
			// right button -> mark cell or remove mark
			if (button == 1) {
				if (item.state == 0) {
					if (_context->marked < _maxBombs) {
						item.marker_id = _boardTexScene->add(_objects.bomb, v3(item.position.x, 0.15f, item.position.y), _texMaterial, ds::DrawMode::TRANSFORM);
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
					if (item.marker_id != INVALID_ID) {
						_boardTexScene->remove(item.marker_id);
						item.marker_id = INVALID_ID;
					}
					
					--_context->marked;
				}

				if (_context->markedCorrectly == _maxBombs) {
					return 1;
				}
				int left = _maxBombs - _context->marked;
				char buffer[32];
				sprintf_s(buffer, 32, "%d / %d", left, _maxBombs);
				_hud->updateText(2, buffer);
			}
			// left button
			else {
				if (item.state == 0) {
					if (item.bomb) {
						//return 1;
						_endTimer = 0.0f;
						_showBombs = true;
						showBombs();
						_hud->deactivate();

						// FIXME: game over!!!!

					}
					item.state = 1;
					//_boardScene->rotate(item.id, v3(0.0f, 0.0f, PI));					
					if (item.adjacentBombs == 0) {
						_boardScene->remove(item.id);
						openEmptyTiles(h);
					}
					else {
						_boardScene->setColor(item.id, ds::Color(128, 128, 128));
						item.timer = 0.0f;
						//item.rotating = true;
						_boardScene->rotateTo(item.id, v3(0, 0, 0), v3(0.0f, 0.0f, PI), 0.4f);
					}
				}
				else {
					LOG << "item already open";
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
	// update grid and HUD
	_grid.update(dt);
	_hud->tick(dt);

	// tick timer when showing bombs
	if (_showBombs) {
		_endTimer += dt;
		if (_endTimer > 2.0f) {
			return 1;
		}
	}

	_boardScene->tick(dt);
	if (_boardScene->hasEvents()) {
		for (int i = 0; i < _boardScene->numEvents(); ++i) {
			const ds::ActionEvent& evn = _boardScene->getEvent(i);
			if (evn.action == ds::AT_ROTATE_TO) {
				int idx = _grid.getIndex(evn.id);
				if (idx != -1) {
					const GridItem& item = _grid.get(idx);
					if (item.numberID != INVALID_ID) {
						_boardTexScene->activate(item.numberID);
					}
				}
			}
		}
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	graphics::setCamera(_camera);
	_scene->draw();
	_boardScene->draw();
	_boardTexScene->draw();
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	_hud->render();
	graphics::turnOnZBuffer();
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'f') {
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