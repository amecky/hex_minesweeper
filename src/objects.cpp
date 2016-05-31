#include "objects.h"
#include <utils\MeshGen.h>

namespace objects {

	void createCubeEnemy(ds::Mesh* mesh) {
		ds::MeshGen gen;
		uint16_t faces[20];
		float dim = 1.0f;
		float size = 0.5f;
		float half_size = size * 0.5f;
		float delta = dim * 0.5f;
		float innerSize = dim - size;
		faces[0] = gen.add_cube(v3(-delta, -delta, -delta), v3(size));
		faces[1] = gen.add_cube(v3(delta, -delta, -delta), v3(size));
		faces[2] = gen.add_cube(v3(-delta, delta, -delta), v3(size));
		faces[3] = gen.add_cube(v3(delta, delta, -delta), v3(size));
		faces[4] = gen.add_cube(v3(-delta, -delta, delta), v3(size));
		faces[5] = gen.add_cube(v3(delta, -delta, delta), v3(size));
		faces[6] = gen.add_cube(v3(-delta, delta, delta), v3(size));
		faces[7] = gen.add_cube(v3(delta, delta, delta), v3(size));

		faces[8] = gen.add_cube(v3(0.0f, delta, delta), v3(innerSize, half_size, half_size));
		faces[9] = gen.add_cube(v3(0.0f, -delta, delta), v3(innerSize, half_size, half_size));
		faces[10] = gen.add_cube(v3(0.0f, delta, -delta), v3(innerSize, half_size, half_size));
		faces[11] = gen.add_cube(v3(0.0f, -delta, -delta), v3(innerSize, half_size, half_size));

		faces[12] = gen.add_cube(v3(delta, 0.0f, -delta), v3(half_size, innerSize, half_size));
		faces[13] = gen.add_cube(v3(delta, 0.0f, delta), v3(half_size, innerSize, half_size));
		faces[14] = gen.add_cube(v3(-delta, 0.0f, -delta), v3(half_size, innerSize, half_size));
		faces[15] = gen.add_cube(v3(-delta, 0.0f, delta), v3(half_size, innerSize, half_size));

		faces[16] = gen.add_cube(v3(delta, delta, 0.0f), v3(half_size, half_size, innerSize));
		faces[17] = gen.add_cube(v3(delta, -delta, 0.0f), v3(half_size, half_size, innerSize));
		faces[18] = gen.add_cube(v3(-delta, delta, 0.0f), v3(half_size, half_size, innerSize));
		faces[19] = gen.add_cube(v3(-delta, -delta, 0.0f), v3(half_size, half_size, innerSize));
		for (int i = 8; i < 20; ++i) {
			gen.set_color(faces[i], ds::Color(192, 192, 192, 255));
		}
		gen.build(mesh);
	}

	void createWingEnemy(ds::Mesh* mesh) {
		ds::MeshGen gen;
		uint16_t faces[20];
		float dim = 1.0f;
		float size = 0.5f;
		float half_size = size * 0.5f;
		float delta = dim * 0.5f;
		float innerSize = dim - size;
		faces[0] = gen.add_cube(v3(-delta, -delta, -delta), v3(size));
		faces[1] = gen.add_cube(v3(delta, -delta, -delta), v3(size));
		faces[2] = gen.add_cube(v3(-delta, delta, -delta), v3(size));
		faces[3] = gen.add_cube(v3(delta, delta, -delta), v3(size));
		faces[4] = gen.add_cube(v3(-delta, -delta, delta), v3(size));
		faces[5] = gen.add_cube(v3(delta, -delta, delta), v3(size));
		faces[6] = gen.add_cube(v3(-delta, delta, delta), v3(size));
		faces[7] = gen.add_cube(v3(delta, delta, delta), v3(size));

		faces[8] = gen.add_cube(v3(0.0f, delta, delta), v3(innerSize, half_size, half_size));
		faces[9] = gen.add_cube(v3(0.0f, -delta, delta), v3(innerSize, half_size, half_size));
		faces[10] = gen.add_cube(v3(0.0f, delta, -delta), v3(innerSize, half_size, half_size));
		faces[11] = gen.add_cube(v3(0.0f, -delta, -delta), v3(innerSize, half_size, half_size));

		faces[12] = gen.add_cube(v3(delta, 0.0f, -delta), v3(half_size, innerSize, half_size));
		faces[13] = gen.add_cube(v3(delta, 0.0f, delta), v3(half_size, innerSize, half_size));
		faces[14] = gen.add_cube(v3(-delta, 0.0f, -delta), v3(half_size, innerSize, half_size));
		faces[15] = gen.add_cube(v3(-delta, 0.0f, delta), v3(half_size, innerSize, half_size));

		faces[16] = gen.add_cube(v3(delta, delta, 0.0f), v3(half_size, half_size, innerSize));
		faces[17] = gen.add_cube(v3(delta, -delta, 0.0f), v3(half_size, half_size, innerSize));
		faces[18] = gen.add_cube(v3(-delta, delta, 0.0f), v3(half_size, half_size, innerSize));
		faces[19] = gen.add_cube(v3(-delta, -delta, 0.0f), v3(half_size, half_size, innerSize));
		for (int i = 8; i < 20; ++i) {
			gen.set_color(faces[i], ds::Color(192, 192, 192, 255));
		}
		gen.build(mesh);
	}
}
