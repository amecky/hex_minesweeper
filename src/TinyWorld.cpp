#include "TinyWorld.h"
#include <resources\ResourceContainer.h>

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

void TinyWorld::addHouse(uint16_t x, uint16_t y) {
	int idx = x + y * _size;
	_tiles[idx].type = WT_HOUSE;
}

void TinyWorld::addStreet(uint16_t x, uint16_t y) {
	int idx = x + y * _size;
	_tiles[idx].type = WT_STREET;
}

const Tile& TinyWorld::get(uint16_t x, uint16_t y) const {
	int idx = x + y * _size;
	return _tiles[idx];
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

int TinyWorld::connect(uint16_t fx, uint16_t fy, uint16_t sx, uint16_t sy, v2* ret, int max) {
	v2 start = v2(fx,fy);
	v2* came_from = new v2[_total];
	for (int i = 0; i < _total; ++i) {
		came_from[i] = v2(-1,-1);
	}
	
	v2 dirs[] = { v2(-1,0) , v2(0,-1), v2(1,0), v2(0,1) };

	ds::Stack<v2> frontier;
	frontier.push(v2(fx,fy));	
	v2 n[4];
	int cnt = 0;
	while (!frontier.empty()) {
		cnt = 0;
		v2 current = frontier.top();
		frontier.pop();
		// get neighbours
		for (int i = 0; i < 4; ++i) {
			v2 nep = current + dirs[i];
			if (nep.x >= 0 && nep.x < _size && nep.y >= 0 && nep.y < _size) {				
				if (get(nep.x,nep.y).type == WT_EMPTY) {
					n[cnt++] = nep;
				}
			}
		}
		//LOG << "current: " << DBG_V2(current) << " cnt: " << cnt;
		// for next in graph.neighbors(current):
		for (int k = 0; k < cnt; ++k) {
			bool found = false;
			int idx = n[k].x + n[k].y * _size;
			for (int j = 0; j < _total; ++j) {
				if (came_from[j].x == n[k].x && came_from[j].y == n[k].y) {
					found = true;
				}
			}
			if (!found) {
				frontier.push(n[k]);
				int idx = n[k].x + n[k].y * _size;
				came_from[idx] = current;
			}
		}
	}
	char buffer[32];
	LOG << "---------------------- Map ------------------------";
	for (int y = _size - 1; y >= 0; --y) {
		std::string line;
		for (int x = 0; x < _size; ++x) {
			sprintf(buffer, " (%g,%g) ", came_from[x + y * _size].x, came_from[x + y * _size].y);
			line += buffer;
		}
		LOG << line;
	}
	int r = 0;
	v2 current = v2(sx,sy);
	start = v2(fx, fy);
	//ds::Array<v2> path;
	ret[r++] = current;
	//path.push_back(current);
	bool running = true;
	while (running) {
		int idx = current.x + current.y * _size;		
		current = came_from[idx];
		//path.push_back(current);
		ret[r++] = current;
		if (current.x == start.x && current.y == start.y) {
			running = false;
		}
	}
	//LOG << "PATH: " << path.size();
	//for (int i = 0; i < path.size(); ++i) {
		//LOG << "current: " << DBG_V2(path[i]);
	//}
	delete[] came_from;
	return r;
}
