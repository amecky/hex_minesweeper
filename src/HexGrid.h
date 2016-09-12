#pragma once
#include <Vector.h>
#include <core\math\hex.h>
#include <core\Common.h>

enum GridItemState {
	GIS_CLOSED,
	GIS_OPEN,
	GIS_MARKED
};

struct GridItem {

	ID id;
	Hex hex;
	v2 position;
	bool bomb;
	int adjacentBombs;
	GridItemState state; 
	bool wiggle;
	float timer;
	v2 scale;
};

class HexGrid {

public:
	HexGrid();
	~HexGrid();
	void resize(int qMax, int rMax);
	void fill();
	const GridItem& get(int index) const;
	GridItem& get(int index);
	const GridItem& get(const Hex& hex) const;
	GridItem& get(const Hex& hex);
	const int size() const;
	int select(int x, int y);
	bool isValid(int q, int r) const;
	bool isValid(const Hex& hex) const;
	void markAsBomb(const Hex& hex);
	int neighbors(const Hex& hex,Hex* ret);
	Hex convertFromMousePos();
	void setOrigin(const v2& origin);
	int getIndex(const Hex& h)const;
	v2 convert(int q, int r) const;
private:
	int _qMax;
	int _rMax;
	GridItem* _items;
	Layout _layout;
	int _hover;
};