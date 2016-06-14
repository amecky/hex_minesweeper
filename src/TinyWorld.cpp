#include "TinyWorld.h"
#include <resources\ResourceContainer.h>
#include "AStar.h"

TinyWorld::TinyWorld(uint16_t size) : _size(size) {
	_total = _size * _size;
	_tiles = new Tile[_total];
	clear();
}


TinyWorld::~TinyWorld() {
	delete[] _tiles;
}

void TinyWorld::clear() {
	for (int i = 0; i < _size; ++i) {
		_tiles[i].type = WT_EMPTY;
	}
}

void TinyWorld::addHouse(const p2i& p) {
	assert(isValid(p));
	_tiles[to_index(p)].type = WT_HOUSE;
}

void TinyWorld::add(const p2i& p, TileType type) {
	assert(isValid(p));
	_tiles[to_index(p)].type = type;
}

void TinyWorld::addPowerPlant(const p2i& p) {
	assert(isValid(p));
	_tiles[to_index(p)].type = WM_POWER_PLANT;
}

void TinyWorld::addStreet(uint16_t x, uint16_t y) {
	int idx = x + y * _size;
	_tiles[idx].type = WT_STREET;
}

// ---------------------------------------------------
// get
// ---------------------------------------------------
const Tile& TinyWorld::get(uint16_t x, uint16_t y) const {
	int idx = x + y * _size;
	return _tiles[idx];
}

// ---------------------------------------------------
// get
// ---------------------------------------------------
const Tile& TinyWorld::get(const p2i& p) const {
	assert(isValid(p));
	return _tiles[to_index(p)];
}

void TinyWorld::addForrest(uint16_t x, uint16_t y, uint16_t radius) {
	v2 c = v2(x, y);
	for (int sy = -radius; sy <= radius; ++sy) {
		for (int sx = -radius; sx <= radius; ++sx) {
			int px = x + sx;
			int py = y + sy;
			float l = length(c - v2(px, py));
			if (l <= radius) {
				LOG << "px: " << px << " py: " << py << " l: " << l;
				if (px >= 0 && px < _size && py >= 0 && py < _size) {
					int idx = px + py * _size;
					_tiles[idx].type = WT_TREE;
				}
			}
		}
	}
}

// ---------------------------------------------------
// rebuild streets
// ---------------------------------------------------
void TinyWorld::rebuildStreets() {
	for (int y = 0; y < _size; ++y) {
		for (int x = 0; x < _size; ++x) {
			Tile& t = _tiles[x + y * _size];
			p2i p(x, y);
			if (t.type == WT_STREET) {
				p = p2i(x - 1, y);
				if (isValid(p) && get(p).type == WT_STREET) {
					t.index |= 8;
				}
				p = p2i(x + 1, y);
				if (isValid(p) && get(p).type == WT_STREET) {
					t.index |= 2;
				}
				p = p2i(x, y + 1);
				if (isValid(p) && get(p).type == WT_STREET) {
					t.index |= 1;
				}
				p = p2i(x, y - 1);
				if (isValid(p) && get(p).type == WT_STREET) {
					t.index |= 4;
				}
			}
		}
	}
}

// ---------------------------------------------------
// is point valid
// ---------------------------------------------------
bool TinyWorld::isValid(const p2i& p) const {
	return p.x >= 0 && p.x < _size && p.y >= 0 && p.y < _size;
}

// ---------------------------------------------------
// find free neighbours
// ---------------------------------------------------
int TinyWorld::find_free_neighbours(const p2i& current, const p2i& start, const p2i& goal, p2i* p) {
	static Point2i directions[] = { Point2i(-1, 0), Point2i(0, 1), Point2i(1, 0), Point2i(0, -1) };
	int cnt = 0;
	for (int i = 0; i < 4; ++i) {
		Point2i nep = current + directions[i];
		if (isValid(nep)) {
			const Tile& t = get(nep);
			if (t.type == WT_EMPTY || t.type == WT_STREET || nep == start || nep == goal) {
				p[cnt++] = nep;
			}
		}
	}
	return cnt;
}

// ---------------------------------------------------
// p2i to index
// ---------------------------------------------------
int TinyWorld::to_index(const p2i& p) const {
	assert(isValid(p));
	return p.x + p.y * _size;
}

// ---------------------------------------------------
// Find path from Breadth First Search
// ---------------------------------------------------
void TinyWorld::connect(const p2i& start, const p2i& goal) {


	AStar as(_size,_size);
	for (int y = 0; y < _size; ++y) {
		for (int x = 0; x < _size; ++x) {
			const Tile& t = _tiles[x + y * _size];
			as.set(p2i(x, y), t.type == WT_EMPTY);
		}
	}
	p2i result[32];
	int nr = as.find(start, goal, result, 32);


	ds::Array<p2i> came_from(_total);
	for (int i = 0; i < _total; ++i) {
		came_from[i] = p2i(-1, -1);
	}
	ds::Stack<p2i> frontier;
	frontier.push(start);
	p2i n[4];
	while (!frontier.empty()) {
		p2i current = frontier.top();
		frontier.pop();
		int cnt = find_free_neighbours(current, start, goal, n);
		for (int k = 0; k < cnt; ++k) {
			const p2i& next = n[k];
			if (!came_from.contains(next)) {
				frontier.push(next);
				came_from[to_index(next)] = current;
			}
		}
	}
	p2i current = goal;
	while (current != start) {
		if (current != start && current != goal) {
			addStreet(current.x, current.y);
		}
		current = came_from[to_index(current)];		
	}
}
