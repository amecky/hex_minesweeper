#include "Board.h"
#include "Constants.h"
#include <math\GameMath.h>
#include <utils\Log.h>
#include <math\tweening.h>
#include <math\math.h>
#include <renderer\graphics.h>
#include "Map.h"
#include <imgui\IMGUI.h>
#include <resources\ResourceContainer.h>
#include <gen\MeshGen.h>
#include <base\InputSystem.h>
// -------------------------------------------------------
// Mouse over state
// -------------------------------------------------------
int MouseOverState::activate() {
	_selectedEntry = INVALID_POINT;
	return 0;
}

int MouseOverState::update(float dt) {
	/*
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	v2 mousePos = ds::renderer::getMousePosition();
	ds::Point gp = grid::convert(mousePos);
	if (gp.x >= 0 && gp.y >= 0) {
		if (gp != _selectedEntry) {
			_selectedEntry = gp;
			MyEntry& me = ctx->grid->get(gp.x, gp.y);
			if (me.state == TS_NORMAL) {
				me.timer = 0.0f;
				me.state = TS_WIGGLE;
			}
		}
	}
	*/
	return 0;
}

// -------------------------------------------------------
// Moving cells state
// -------------------------------------------------------
int MovingCellsState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		const MovingCell& m = ctx->movingCells[i];
		MyEntry& e = ctx->grid->get(m.x, m.y);
		e.hidden = false;
	}
	ctx->movingCells.clear();
	return 0;
}

int MovingCellsState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		m.current = tweening::interpolate(&tweening::easeInOutCubic, m.start, m.end, getTimer(), ctx->settings->droppingTTL);
	}
	return 0;
}

// -------------------------------------------------------
// Rebuilding cells state
// -------------------------------------------------------
int RebuildingCellsState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		const MovingCell& m = ctx->movingCells[i];
		MyEntry& e = ctx->grid->get(m.x, m.y);
		e.hidden = false;
	}
	ctx->movingCells.clear();
	return 0;
}

int RebuildingCellsState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		m.current = tweening::interpolate(&tweening::linear, m.start, m.end, getTimer(), ctx->settings->rebuildingTTL);
	}
	return 0;
}

// -------------------------------------------------------
// PrepareBoardState
// -------------------------------------------------------
int PrepareBoardState::activate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	ctx->grid->clear();
	ctx->movingCells.clear();
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			int cid = math::random(0.0f, 4.99f);
			int offset = offset = cid * CELL_SIZE;
			v3 p = grid::convert(x, y);
			MyEntry& e = ctx->grid->get(x, y);
			e.color = cid;
			e.hidden = true;
			e.scale = 1.0f;
			e.state = TS_NORMAL;
			e.timer = 0.0f;

			MovingCell m;
			m.x = x;
			m.y = y;
			m.color = e.color;
			m.start = grid::convert(x, y + 20 + y * 20 + math::random(0,6));
			m.end = grid::convert(x, y);
			m.current = m.start;
			ctx->movingCells.push_back(m);

			ctx->grid->set(x, y, e);
		}
	}
	return 0;
}

// -------------------------------------------------------
// SelectCellState
// -------------------------------------------------------
int SelectCellState::activate() {
	int ret = 0;
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	v2 mp = ds::input::getMousePosition();
	ds::mat4 projection = ctx->camera->getProjectionMatrix();
	float px = (((2.0f * mp.x) / 1024.0f) - 1.0f) / projection._11;

	float py = (((2.0f * mp.y) / 768.0f) - 1.0f) / projection._22;

	v3 origin = v3(0.0f, 0.0f, 0.0f);
	v3 direction = v3(px, py, 1.0f);

	ds::mat4 view = ctx->camera->getViewMatrix();
	view = ds::matrix::mat4Inverse(view);

	origin = ds::matrix::transformCoordinate(origin, view);
	direction = ds::matrix::transformNormal(direction, view);
	direction = normalize(direction);
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				v3 v = origin - grid::convert(x,y);

				float b = 2.0f * dot(direction, v);
				float cc = dot(v, v) - 0.5f * 0.5f;

				float discriminant = (b * b) - (4.0f * cc);

				if (discriminant >= 0.0f) {
					discriminant = std::sqrt(discriminant);
					float s0 = (-b + discriminant) / 2.0f;
					float s1 = (-b - discriminant) / 2.0f;
					if (s0 >= 0.0f || s1 >= 0.0f) {
						ctx->points.clear();
						ctx->grid->findMatchingNeighbours(x, y, ctx->points);
						if (ctx->points.size() > 1) {
							for (size_t i = 0; i < ctx->points.size(); ++i) {
								const p2i& gp = ctx->points[i];
								MyEntry& c = ctx->grid->get(gp.x, gp.y);
								c.state = TS_SHRINKING;
								c.timer = 0.0f;
							}
							LOG << "pieces: " << ctx->points.size();
							int score = ctx->points.size();
							sendEvent(BE_SCORE, &score, sizeof(int));
							return 1;
						}
						ctx->points.clear();
					}
				}
			}
		}
	}
	/*
	sendEvent(BE_INVALID_SELECTION);
	*/
	return 0;
}

// -------------------------------------------------------
// ShrinkState
// -------------------------------------------------------
int ShrinkState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				if (e.state == TS_SHRINKING) {
					float norm = getTimer() / ctx->settings->flashTTL;
					e.scale = 1.0f - norm * 0.9f;
				}
			}
		}
	}
	return 0;
}

int ShrinkState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	ctx->points.clear();
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!ctx->grid->isFree(x, y)) {
				MyEntry& e = ctx->grid->get(x, y);
				if (e.state == TS_SHRINKING) {
					ctx->points.push_back(p2i(x, y));
				}
			}
		}
	}
	return 0;
}

// -------------------------------------------------------
// DroppingCellsState
// -------------------------------------------------------
int DroppingCellsState::activate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);	
	ctx->grid->remove(ctx->points, true);
	ctx->droppedCells.clear();
	ctx->movingCells.clear();
	ctx->grid->dropCells(ctx->droppedCells);
	for (size_t i = 0; i < ctx->droppedCells.size(); ++i) {
		const ds::DroppedCell<MyEntry>& dc = ctx->droppedCells[i];
		MyEntry& e = ctx->grid->get(dc.to);
		e.hidden = true;
		MovingCell m;
		m.x = dc.to.x;
		m.y = dc.to.y;
		m.color = e.color;
		m.start = grid::convert(dc.from);
		m.end = grid::convert(dc.to);
		m.current = m.start;
		ctx->movingCells.push_back(m);
	}
	return 0;
}

int DroppingCellsState::update(float dt) {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		m.current = tweening::interpolate(&tweening::easeInOutCubic, m.start, m.end, getTimer(), ctx->settings->droppingTTL);
	}
	return 0;
}

int DroppingCellsState::deactivate() {
	BoardContext* ctx = static_cast<BoardContext*>(_ctx);
	for (size_t i = 0; i < ctx->movingCells.size(); ++i) {
		MovingCell& m = ctx->movingCells[i];
		MyEntry& e = ctx->grid->get(m.x, m.y);
		e.hidden = false;
	}
	ctx->movingCells.clear();
	return 0;
}


// -------------------------------------------------------
// Board
// -------------------------------------------------------
Board::Board(GameSettings* settings, ds::FPSCamera* camera) : _settings(settings) {
	for (int i = 0; i < 5; ++i) {
		_cellTextures[i] = math::buildTexture(680.0f, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
	}
	_context.grid = &m_Grid;
	_context.settings = settings;
	_context.camera = camera;
	_states = new ds::StateManager(&_context);
	_states->add<MouseOverState>();
	_states->add<MovingCellsState>();
	_states->add<PrepareBoardState>();
	_states->add<SelectCellState>();
	_states->add<ShrinkState>();
	_states->add<DroppingCellsState>();
	_states->add<RebuildingCellsState>();
	_states->addTransition(BM_FILLING,    0, BM_REBUILDING);
	_states->addTransition(BM_REBUILDING, 0, BM_RUNNING, _settings->rebuildingTTL);
	_states->addTransition(BM_MOVING,     0, BM_RUNNING, _settings->droppingTTL);
	_states->addTransition(BM_SELECTION,  1, BM_FLASHING);
	_states->addTransition(BM_SELECTION,  0, BM_RUNNING);
	_states->addTransition(BM_FLASHING,   0, BM_DROPPING, _settings->flashTTL);
	_states->addTransition(BM_DROPPING,   0, BM_RUNNING, _settings->droppingTTL);
	_dialogState = 1;
	_dialogPos = v2(10, 760);
	_showStates = false;
	_showBoard = false;

	_texturedBuffer = ds::res::getMeshBuffer(21);
	ds::Rect r(512, 342, 60, 60);
	for (int i = 0; i < 6; ++i) {
		_cubeTextures[i] = r;
		r.left += 64;
		r.right += 64;
	}
	ds::gen::MeshGen gen;
	gen.add_cube(v3(0, 0, 0), v3(1, 1, 1));	
	gen.recalculate_normals();
	for (int i = 0; i < 5; ++i) {
		ds::Mesh* box = new ds::Mesh();
		for (int j = 0; j < 6; ++j) {
			gen.texture_face(i, math::buildTexture(512, 342 + i * 64, 60, 60));
		}
		gen.build(box);
		_boxes.push_back(box);
	}
	
}

Board::~Board(void) {
	_boxes.destroy_all();
	delete _states;
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Board::render() {	
	_texturedBuffer->begin();	
	// pieces
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (!e.hidden) {
					float r = static_cast<float>(e.color) * DEGTORAD(90.0f);
					_texturedBuffer->add(_boxes[e.color], v3(grid::convert(x, y)), v3(e.scale,e.scale,e.scale),v3(0.0f,r,0.0f));
				}
			}
		}
	}
	// moving cells
	for (size_t i = 0; i < _context.movingCells.size(); ++i) {
		const MovingCell& cell = _context.movingCells[i];
		float r = static_cast<float>(cell.color) * DEGTORAD(90.0f);
		_texturedBuffer->add(_boxes[cell.color], cell.current,v3(1,1,1),v3(0.0f,r,0.0f));
	}
	_texturedBuffer->end();
	if (_showStates) {
		_states->showDialog();
	}
	if (_showBoard) {
		showDialog();
	}
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
int Board::update(float elapsed) {
	int ret = 0;
	_states->tick(elapsed);
	/*
	if (_states->hasEvents()) {
		const ds::EventStream& events = _states->getEventStream();
		for (int i = 0; i < events.num(); ++i) {
			if (events.getType(i) == BE_INVALID_SELECTION) {
				// play sound
				LOG << "INVALID SELECTION!!!";
			}
			else if (events.getType(i) == BE_SCORE) {
				// FIXME: get color and set headColor
				events.get(i, &ret);
				LOG << "ret: " << ret;
			}
		}
	}
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (e.state == TS_WIGGLE) {
					e.timer += elapsed;
					// FIXME: wiggleTTL
					if (e.timer >= 0.4f) {
						e.state = TS_NORMAL;
						e.scale = 1.0f;
					}
					else {
						float norm = e.timer / 0.4f;
						e.scale = 1.0f + sin(norm * TWO_PI * 2.0f) * 0.1f;
					}
				}
			}
		}
	}
	*/
	return ret;
}

// -------------------------------------------------------
// rebuild grid
// -------------------------------------------------------
void Board::rebuild() {
	_states->activate(BM_FILLING);
}

// -------------------------------------------------------
// Select
// -------------------------------------------------------
void Board::select() {
	if (_states->getCurrentMode() == BM_RUNNING) {
		_states->activate(BM_SELECTION);
	}
}

// -------------------------------------------------------
// debug
// -------------------------------------------------------
void Board::debug() {
	char buffer[32];
	LOG << "---------------------- GRID ------------------------";
	for (int y = MAX_Y - 1; y >= 0; --y) {
		std::string line;
		for (int x = 0; x < MAX_X; ++x) {
			if (m_Grid.isFree(x, y)) {
				line += " -- ";
			}
			else {
				MyEntry& c = m_Grid.get(x, y);
				sprintf(buffer, " %2d ", c.color);
				line += buffer;
			}
		}
		LOG << line;
	}
}

void Board::showDialog() {
	char buffer[32];
	gui::start(1, &_dialogPos);
	gui::begin("State", &_dialogState);

	for (int y = MAX_Y - 1; y >= 0; --y) {
		std::string line;
		for (int x = 0; x < MAX_X; ++x) {
			if (m_Grid.isFree(x, y)) {
				line += " -- ";
			}
			else {
				MyEntry& c = m_Grid.get(x, y);
				sprintf(buffer, " %2d ", c.color);
				line += buffer;
			}
		}
		gui::Label(line.c_str());
	}
	gui::end();
}
