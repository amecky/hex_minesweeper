#pragma once
#include <Vector.h>
#include <math\hex.h>

struct GridItem {

	Hex hex;
	v2 position;
	bool bomb;
	int adjacentBombs;
	int state; // 0 = closed / 1 = open / 2 = marked
};

class HexGrid {

public:
	HexGrid();
	~HexGrid();
	void resize(int qMax, int rMax);
	void fill();
	const GridItem& get(int index) const;
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
private:
	int _qMax;
	int _rMax;
	GridItem* _items;
	Layout _layout;
};