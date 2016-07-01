#include "GameObjects.h"
#include <gen\MeshGen.h>
#include <math\math.h>
#include <Common.h>

namespace objects {

	void build_objects(GameObjects* objects) {
		ds::gen::MeshGen gen;
		gen.set_color_selection(ds::Color(235, 123, 89));
		gen.create_cylinder(v3(0, 0, 0), 0.45f, 0.45f, 0.2f, 6, HALF_PI);

		objects->hexagon = new ds::Mesh();
		gen.build(objects->hexagon);

		gen.clear();
		objects->bomb = new ds::Mesh();
		float pp = 0.3f;
		v3 p[] = { v3(-pp, 0.0f, pp), v3(pp, 0.0f, pp), v3(pp, 0.0f, -pp), v3(-pp, 0.0f, -pp) };
		gen.add_face(p);
		gen.texture_face(0, math::buildTexture(650, 520, 70, 70));
		gen.build(objects->bomb);

		// numbers
		gen.clear();
		gen.add_face(p);
		for (int i = 0; i < 6; ++i) {
			ds::Mesh* m = new ds::Mesh();
			gen.texture_face(0, math::buildTexture(580, 520 + i * 70, 70, 70));
			gen.build(m);
			objects->numbers.push_back(m);
		}

		objects->border = new ds::Mesh;
	}

	void build_border(GameObjects* objects, const HexGrid& grid) {
		objects->border->clear();
		ds::gen::MeshGen gen;
		float sz = 0.45f;
		int width = grid.getWidth();
		int height = grid.getHeight();
		gen.set_color_selection(ds::Color(192, 192, 192));
		for (int r = 0; r < width + 1; ++r) {
			v3 p = grid.convert(-width / 2 + r + 1, height);
			gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
			p = grid.convert(r, -1);
			gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
		}
		for (int r = 0; r < height + 1; ++r) {
			int q_offset = r >> 1;
			v3 p = grid.convert(-r / 2 - 1, r);
			gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
			p = grid.convert(-r / 2 + width, r);
			gen.create_cylinder(p, sz, sz, 0.4f, 6, HALF_PI);
		}
		gen.build(objects->border);
	}

}