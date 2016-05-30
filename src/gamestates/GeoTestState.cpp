#include "GeoTestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>
#include <utils\ObjLoader.h>
#include "..\objects.h"
#include <renderer\graphics.h>

GeoTestState::GeoTestState() : ds::GameState("GeoTestState") {
	_pressed = false;
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	//_camera->setPosition(v3(0, -8, -21), v3(0, 0, 1));
	//_camera->resetPitch(DEGTORAD(355.0f));
	//_camera->resetYAngle();
	LOG << " ======================================================== ";
	_scene = ds::res::getScene("TestObjects");
	_buffer = ds::res::getMeshBuffer("TexturedBuffer");

	_mesh = new ds::Mesh();
	

	// triangle
	//uint16_t faces[6];
	//uint16_t f = gen.add_cube(v3(0, 0, 0), v3(1.0f, 1.0f, 0.1f), faces);
	//gen.move_edge(16, v3(-0.4f, 0.0f, 0.0f));
	//gen.move_edge(18, v3( 0.4f, 0.0f, 0.0f));
	//gen.parse("triangle.def");
	gen.parse("gem_stone.def");
	//
	//
	/*
	uint16_t faces[20];
	v3 p[] = { v3(-1,1,-1),v3(1,1,-1),v3(1,-1,-1),v3(-1,-1,-1) };
	faces[0] = gen.add_face(p);
	v3 pn[] = { v3(1,1,1), v3(-1,1,1),v3(-1,-1,1),v3(1,-1,1) };
	faces[1] = gen.add_face(pn);
	// left
	faces[2] = gen.v_combine(1, 7);
	// right
	faces[3] = gen.v_combine(5,3);
	// top
	faces[4] = gen.h_combine(12,8);
	// bottom
	faces[5] = gen.h_combine(14, 10);
	//gen.scale_face(0, 1.5f);
	faces[6] = gen.extrude_face(faces[4], 2.0f);
	faces[6] = gen.extrude_face(faces[3], 2.0f);
	//gen.verify(10);
	//v3 tp[] = { v3(-1,0,0),v3(0,-1,0),v3(1,0,0),v3(0,-1,0) };
	//gen.verify(tp);
	*/
	/*
	gen.texture_face(faces[0], math::buildTexture(650, 260, 64, 64));
	faces[1] = gen.extrude_edge(1, v3(0, 0, 2));
	gen.texture_face(faces[1], math::buildTexture(714, 324, 64, 64));
	faces[2] = gen.combine(5, 3);
	gen.move_edge(5, v3(-0.5f, 0.0f, 0.0f));
	gen.texture_face(faces[2], math::buildTexture(714, 324, 64, 64));
	*/
	//gen.create_cube_ring(1.0f, 0.5f, 4);
	//gen.add_cube(v3(0, 0, 0), v3(2, 2, 2));
	/*
	for (int i = 0; i < 32; ++i) {
		gen.texture_face(i, math::buildTexture(650, 260, 64, 64));
	}
	v2 uv[] = { v2(286,650),v2(298,650),v2(324,714),v2(260,714) };
	gen.texture_face(0, math::buildTexture(uv));
	gen.texture_face(2, math::buildTexture(uv));
	*/
	//gen.split_edge(3, 0.2f);
	//gen.extrude_edge(7, v3(-0.8f, 0.0f, 0.0f));
	//gen.create_ring(1.0f, 0.2f, 4);
	//uint16_t fe[] = {7,3,13,9};
	//uint16_t fi = gen.make_face(fe);
	//gen.move_face(fi, v3(0.0f, 0.0f, -0.1f));
	/*	
	faces[1] = gen.extrude_edge(1, v3(0, 0, 2));
	faces[2] = gen.split_edge(0);
	gen.split_edge(4);
	gen.extrude_edge(13, v3(-2, 0, 0));
	gen.split_edge(16);
	gen.extrude_edge(21, v3(0, 0, -2));
	gen.split_edge(24);
	gen.move_edge(1, v3(0.0f, 0.0f, -0.5f));
	gen.move_edge(5, v3(0.5f, 0.0f, 0.0f));
	gen.move_edge(17, v3(0.0f, 0.0f, 0.5f));
	gen.move_edge(25, v3(-0.5f, 0.0f, 0.0f));
	*/
	//for (int i = 0; i < 16; ++i) {
		//gen.set_color(i, ds::Color(math::random(0,255), math::random(0, 255), math::random(0, 255), 255));
	//}
	//gen.recalculate_normals();
	gen.debug();
	gen.build(_mesh);
	ID id = _scene->add(_mesh, v3(0, 0, 0), ds::DrawMode::IMMEDIATE);
}


GeoTestState::~GeoTestState() {
	delete _mesh;
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int GeoTestState::update(float dt) {
	_camera->update(dt);	
	if (ds::input::isMouseButtonPressed(0) && !_pressed) {
		_pressed = true;
		ds::Ray r = graphics::getCameraRay(_camera);
		int selection = gen.intersects(r);
		if (selection != -1 ) {
			gen.debug_face(selection);
		}
	}
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void GeoTestState::render() {
	// scene
	_scene->draw();
}

int GeoTestState::onChar(int ascii) {
	if (ascii == 'r') {
		gen.parse("gem_stone.def");
		gen.debug();
		_mesh->clear();
		gen.build(_mesh);
	}
	return 0;
}
