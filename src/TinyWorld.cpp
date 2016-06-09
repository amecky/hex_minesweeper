#include "TinyWorld.h"
#include <resources\ResourceContainer.h>

TinyWorld::TinyWorld(uint16_t size) : _size(size) {
	_tiles = new Tile[_size * _size];
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

const Tile& TinyWorld::get(uint16_t x, uint16_t y) const {
	int idx = x + y * _size;
	return _tiles[idx];
}

void TinyWorld::addForrest(uint16_t x, uint16_t y, uint16_t radius) {
	for (int sy = -radius; sy < radius; ++sy) {
		for (int sx = -radius; sx < radius; ++sx) {
			int px = x + sx;
			int py = y + sy;
			if (px >= 0 && px < _size && py >= 0 && py < _size) {
				int idx = px + py * _size;
				_tiles[idx].type = WT_TREE;
			}

		}
	}
}
