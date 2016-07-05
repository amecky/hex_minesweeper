#pragma once
#include <Vector.h>
#include <math\hex.h>
#include <Common.h>

struct GridItem {

	Hex hex;
	v3 position;
	bool bomb;
	int adjacentBombs;
	int state; // 0 = closed / 1 = open / 2 = marked
	bool wiggle;
	float timer;
	v3 scale;
	ID id;
	ID numberID;
	ID marker_id;
	//bool rotating;
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
	void update(float dt);
	int getIndex(const Hex& h) const;
	int getIndex(ID id) const;
	v3 convert(int q, int r) const;
	int getWidth() const {
		return _qMax;
	}
	int getHeight() const {
		return _rMax;
	}
private:
	int _qMax;
	int _rMax;
	GridItem* _items;
	Layout _layout;
	int _hover;
};