#include "HexGrid.h"
#include <assert.h>

HexGrid::HexGrid() : _qMax(0), _rMax(0), _items(0), _layout(layout_pointy, ds::vec2(24.0f, 24.0f), ds::vec2(100, 130)) {
}

HexGrid::~HexGrid() {
	if (_items != 0) {
		delete[] _items;
	}
}

// -------------------------------------------------------
// resize
// -------------------------------------------------------
void HexGrid::resize(int qMax, int rMax) {
	if (_items != 0) {
		delete[] _items;
	}
	_qMax = qMax;
	_rMax = rMax;
	_items = new GridItem[qMax * rMax];
	fill();
}

// -------------------------------------------------------
// fill
// -------------------------------------------------------
void HexGrid::fill() {
	assert(_items != 0);
	for (int r = 0; r < _rMax; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _qMax - q_offset; q++) {
			GridItem item;
			item.hex = Hex(q, r);
			item.position = hex_math::hex_to_pixel(_layout, item.hex);
			item.state = GIS_CLOSED;
			item.bomb = false;
			item.adjacentBombs = 0;
			item.wiggle = false;
			item.timer = 0.0f;
			item.scale = ds::vec2(1, 1);
			int idx = (q + q_offset) + r * _qMax;
			_items[idx] = item;
		}
	}
	_hover = -1;
}

const GridItem& HexGrid::get(const Hex& hex) const {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	return _items[idx];
}

GridItem& HexGrid::get(const Hex& hex) {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	return _items[idx];
}

int HexGrid::neighbors(const Hex& hex, Hex* ret) {
	int cnt = 0;
	for (int i = 0; i < 6; ++i) {
		Hex n = hex_math::neighbor(hex, i);
		if (isValid(n)) {
			ret[cnt++] = n;
		}
	}
	return cnt;
}

Hex HexGrid::convertFromMousePos() {
	ds::vec2 mousePos = ds::getMousePosition();
	return hex_math::hex_round(hex_math::pixel_to_hex(_layout, mousePos));
}

ds::vec2 HexGrid::convert(int q, int r) const {
	Hex hex = Hex(q, r);
	return hex_math::hex_to_pixel(_layout, hex);
}
// -------------------------------------------------------
// get
// -------------------------------------------------------
const GridItem& HexGrid::get(int index) const {
	return _items[index];
}

// -------------------------------------------------------
// get
// -------------------------------------------------------
GridItem& HexGrid::get(int index) {
	return _items[index];
}
// -------------------------------------------------------
// size
// -------------------------------------------------------
const int HexGrid::size() const {
	return _rMax * _qMax;
}

// -------------------------------------------------------
// select
// -------------------------------------------------------
int HexGrid::select(int x, int y) {
	ds::vec2 mousePos = ds::getMousePosition();
	Hex h = hex_math::hex_round(hex_math::pixel_to_hex(_layout, mousePos));
	int q_offset = h.r >> 1;
	int selected = -1;
	//LOG << "h: " << h.q << " " << h.r << " q_offset: " << q_offset;
	if (!isValid(h)) {
		return -1;
	}
	for (int i = 0; i < size(); ++i) {
		if (_items[i].hex == h) {
			selected = i;

		}
	}
	return selected;
}

// -------------------------------------------------------
// is valid
// -------------------------------------------------------
bool HexGrid::isValid(int q, int r) const {
	int q_offset = r >> 1;
	if (r < 0 || r >= _rMax) {
		return false;
	}
	if (q_offset + q < 0 || q_offset + q >= _qMax) {
		return false;
	}
	return true;
}

// -------------------------------------------------------
// is valid
// -------------------------------------------------------
bool HexGrid::isValid(const Hex& hex) const {
	return isValid(hex.q, hex.r);
}

void HexGrid::markAsBomb(const Hex& hex) {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	_items[idx].bomb = true;
}

void HexGrid::setOrigin(const ds::vec2& origin) {
	_layout.origin = origin;
}

// -------------------------------------------------------
// get index
// -------------------------------------------------------
int HexGrid::getIndex(const Hex& hex) const {
	int q_offset = hex.r >> 1;
	return (hex.q + q_offset) + hex.r * _qMax;
}

