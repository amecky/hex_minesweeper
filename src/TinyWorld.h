#pragma once
#include <lib\collection_types.h>
#include <stdint.h>
#include <renderer\Scene.h>
#include <gen\MeshGen.h>
#include <Point.h>

enum TileType {
	WT_EMPTY,
	WT_HOUSE,
	WT_TREE,
	WT_STREET,
	WM_POWER_PLANT,
	WM_WOOD_PLANT,
	WM_WAREHOUSE,
	WM_CITY_HALL
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
	void addHouse(const p2i& p);
	void addPowerPlant(const p2i& p);
	void add(const p2i& p,TileType type);
	void addStreet(uint16_t x, uint16_t y);
	void addForrest(uint16_t x, uint16_t y, uint16_t radius);
	const Tile& get(uint16_t x, uint16_t y) const;
	const Tile& get(const p2i& p) const;
	Tile& get(const p2i& p);
	void connect(const p2i& start, const p2i& goal);
	void rebuildStreets();
private:
	int to_index(const p2i& p) const;
	bool isValid(const p2i& p) const;
	int find_free_neighbours(const p2i& pos, const p2i& start, const p2i& goal, p2i* p);
	uint16_t _size;
	uint16_t _total;
	Tile* _tiles;
};

