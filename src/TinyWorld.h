#pragma once
#include <lib\collection_types.h>
#include <stdint.h>
#include <renderer\Scene.h>
#include <gen\MeshGen.h>
#include <Point.h>

struct Tile {

	float height;

	Tile() : height(0.0f) {}
};

class TinyWorld {

public:
	TinyWorld(uint16_t size);
	~TinyWorld();
	void clear();
	void add(const p2i& p,ID id);
	const Tile& get(uint16_t x, uint16_t y) const;
	const Tile& get(const p2i& p) const;
	Tile& get(const p2i& p);
private:
	int to_index(const p2i& p) const;
	bool isValid(const p2i& p) const;
	//int find_free_neighbours(const p2i& pos, const p2i& start, const p2i& goal, p2i* p);
	uint16_t _size;
	uint16_t _total;
	Tile* _tiles;
};

