#pragma once
#include <renderer\MeshBuffer.h>
#include "..\HexGrid.h"

struct GameObjects {

	ds::Mesh* hexagon;
	ds::Mesh* border;
	ds::Mesh* bomb;
	ds::Array<ds::Mesh*> numbers;

	GameObjects() : hexagon(0), border(0), bomb(0) {}

	~GameObjects() {
		if (hexagon != 0) {
			delete hexagon;
		}
		if (border != 0) {
			delete border;
		}
		if (bomb != 0) {
			delete bomb;
		}
		numbers.destroy_all();
	}

};

namespace objects {

	void build_objects(GameObjects* objects);

	void build_border(GameObjects* objects, const HexGrid& grid);
}

