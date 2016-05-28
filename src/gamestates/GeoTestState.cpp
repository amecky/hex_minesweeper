#include "GeoTestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>
#include <utils\ObjLoader.h>
#include "..\objects.h"

GeoTestState::GeoTestState() : ds::GameState("GeoTestState") {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	//_camera->setPosition(v3(0, -8, -21), v3(0, 0, 1));
	//_camera->resetPitch(DEGTORAD(355.0f));
	//_camera->resetYAngle();
	LOG << " ======================================================== ";
	_scene = ds::res::getScene("TestObjects");
	_buffer = ds::res::getMeshBuffer("TexturedBuffer");

	_mesh = new ds::Mesh();
	ds::MeshGen gen;

	//gen.parse("test_object_def.opc");
	//
	//
	uint16_t faces[20];
	
	v3 p[] = { v3(-1,1,0),v3(1,1,0),v3(1,-1,0),v3(-1,-1,0) };
	faces[0] = gen.add_face(p);
	gen.texture_face(faces[0], math::buildTexture(650, 260, 64, 64));
	faces[1] = gen.extrude_edge(1, v3(0, 0, 2));
	gen.texture_face(faces[1], math::buildTexture(714, 324, 64, 64));
	
	//gen.add_cube(v3(0, 0, 0), v3(2, 2, 2));
	//for (int i = 0; i < 6; ++i) {
		//gen.texture_face(i, math::buildTexture(650 + i * 32, 260 + i * 32, 64, 64));
	//}
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
	gen.recalculate_normals();
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
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void GeoTestState::render() {
	// scene
	_scene->draw();
}
