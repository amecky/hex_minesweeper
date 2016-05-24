#include "Stars.h"
#include <resources\ResourceContainer.h>
#include <math\math.h>
#include <utils\geometrics.h>

// ----------------------------------------
// Stars
// ----------------------------------------
Stars::Stars() {
	_stars = ds::res::getScene("Stars");
	_star = new ds::Mesh();
	// create star
	ds::geometrics::createXYPlane(_star, v3(0, 0, 0), ds::Rect(0, 0, 128, 128), v2(0.1f, 0.1f));
	// fill scene with stars
	float z = 2.0f;
	for (int i = 0; i < 60; ++i) {
		v3 p = randomPos(i);
		ID id = _stars->add(_star, p, ds::DrawMode::TRANSFORM);
		ds::Entity& e = _stars->get(id);
		e.type = 1;
	}
}

// ----------------------------------------
// destructor
// ----------------------------------------
Stars::~Stars() {
	delete _star;
}

// ----------------------------------------
// get random position
// ----------------------------------------
v3 Stars::randomPos(int index) {
	float sx = math::random(-17.0f, 17.0f);
	float sy = math::random(-10.0f, 10.0f);
	float sz = 0.0f;
	if (index < 20) {
		sz = 5.0f;
	}
	else if (index < 40) {
		sz = 9.0f;
	}
	else {
		sz = 13.0f;
	}
	return v3(sx, sy, sz);
}

// ----------------------------------------
// move
// ----------------------------------------
void Stars::move(float dt) {
	ID ids[256];
	int num = _stars->find(1, ids, 256);
	for (int i = 0; i < num; ++i) {
		ds::Entity& star = _stars->get(ids[i]);
		star.position += v3(-5, 0, 0) * dt;
		if (star.position.x < -18.0f) {
			star.position = randomPos(i);
			star.position.x = 18.0f;
		}
	}
}

// ----------------------------------------
// draw
// ----------------------------------------
void Stars::draw() {
	_stars->transform();
	_stars->draw();
}