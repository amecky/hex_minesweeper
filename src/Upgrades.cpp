#include "Upgrades.h"
#include <resources\ResourceContainer.h>
#include <math\math.h>
#include <gen\MeshGen.h>

// -------------------------------------------
// Upgrades
// -------------------------------------------
Upgrades::Upgrades() {
	_scene = ds::res::getScene("Objects");
	for (int i = 0; i < 7; ++i) {
		ds::Mesh* m = new ds::Mesh();
		ds::gen::MeshGen gen;
		gen.add_cube(v3(0, 0, 0), v3(1, 1, 1));
		for (int i = 0; i < 6; ++i) {
			gen.texture_face(i,math::buildTexture(0, 0, 128, 128));
		}
		gen.recalculate_normals();
		gen.build(m);
		_meshes.push_back(m);
	}

	for (int i = 0; i < 7; ++i) {
		v3 p = v3(0, -8 + 2 * i, 0);
		_ids[i] = _scene->add(_meshes[i], p, ds::DrawMode::TRANSFORM);
		ds::Entity& e = _scene->get(_ids[i]);
		e.type = 5;
		e.active = false;
	}
}

Upgrades::~Upgrades() {
	for (int i = 0; i < 7; ++i) {
		delete _meshes[i];
	}
}

// -------------------------------------------
// start
// -------------------------------------------
void Upgrades::start(int upgradeCount) {
	for (int i = 0; i < 7; ++i) {
		v3 p = v3(16, -6 + 2 * i, 0);
		ds::Entity& e = _scene->get(_ids[i]);
		e.position = p;
		e.active = true;
		if (i <= upgradeCount) {
			e.color = ds::Color(0, 0, 255, 255);
		}
		else {
			e.color = ds::Color(128,128,128, 255);
		}
	}
	_active = true;
}

// -------------------------------------------
// tick
// -------------------------------------------
bool Upgrades::tick(float dt) {
	if (_active) {
		v3 v(-5, 0, 0);
		int active = 0;
		for (int i = 0; i < 7; ++i) {
			ds::Entity& e = _scene->get(_ids[i]);
			if (e.active) {
				e.position += v * dt;
				if (e.position.x < -18.0f) {
					e.active = false;
				}
				else {
					++active;
				}
			}

		}
		if (active == 0) {
			_active = false;
		}
	}
	return _active;
}