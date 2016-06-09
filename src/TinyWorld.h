#pragma once
#include <lib\collection_types.h>
#include <stdint.h>
#include <renderer\Scene.h>
#include <gen\MeshGen.h>

enum TileType {
	WT_EMPTY,
	WT_HOUSE,
	WT_TREE,
	WT_STREET
};

struct Tile {
	TileType type;
	int index;

	Tile() : type(WT_EMPTY), index(0) {}
};

class TinyWorld {

public:
	TinyWorld(uint16_t size);
	~TinyWorld();
	void clear();
	void addHouse(uint16_t x, uint16_t y);
	void addForrest(uint16_t x, uint16_t y, uint16_t radius);
	const Tile& get(uint16_t x, uint16_t y) const;
private:
	uint16_t _size;
	Tile* _tiles;
};

