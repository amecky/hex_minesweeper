#include "GeoTestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <utils\ObjLoader.h>
#include "..\objects.h"
#include <renderer\graphics.h>

GeoTestState::GeoTestState() : ds::GameState("GeoTestState") {
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	//_camera->setPosition(v3(0, -8, -21), v3(0, 0, 1));
	//_camera->resetPitch(DEGTORAD(355.0f));
	//_camera->resetYAngle();
	_mesh = new ds::Mesh();

	_ctx.gen = &gen;
	_ctx.mesh = _mesh;

	_gui = new ds::gen::MeshGenGUI("ortho",&_ctx);

	LOG << " ======================================================== ";
	_scene = ds::res::getScene("TestObjects");
	_buffer = ds::res::getMeshBuffer("TexturedBuffer");

	//createHandrail(6.0f, 0.1f, 7 , 0.6f);
	//createCoords();
	//v3 p[] = {v3(-6.0f,-0.6f,6.0f),v3(6.0f,-0.6f,6.0f),v3(6.0f,-0.6f,-6.0f),v3(-6.0f,-0.6f,-6.0f)};
	//v3 p[] = { v3(-1,1,0), v3(1,1,0),v3(1,-1,0),v3(-1,-1,0) };

	uint16_t f = gen.add_cube(v3(0, -3, 0), v3(1.0f, 0.1f, 1.0f));
	uint16_t slices[32];
	int num = gen.slice(4, 5, slices, 32);

	gen.move_edge(25, v3(-0.1f, 0.0f, 0.0f));
	gen.move_edge(65, v3(-0.1f, 0.0f, 0.0f));
	gen.move_edge(105, v3(-0.1f, 0.0f, 0.0f));

	gen.move_edge(35, v3(0.1f, 0.0f, 0.0f));
	gen.move_edge(75, v3(0.1f, 0.0f, 0.0f));
	gen.move_edge(115, v3(0.1f, 0.0f, 0.0f));

	gen.move_edge(42, v3(0.0f, 0.0f, 0.1f));
	gen.move_edge(50, v3(0.0f, 0.0f, 0.1f));
	gen.move_edge(58, v3(0.0f, 0.0f, 0.1f));

	gen.move_edge(62, v3(0.0f, 0.0f, -0.1f));
	gen.move_edge(70, v3(0.0f, 0.0f, -0.1f));
	gen.move_edge(78, v3(0.0f, 0.0f, -0.1f));
	
	int colors[] = {
		0, 1, 2, 1, 0,
		0, 1, 2, 1, 0,
		0, 1, 2, 1, 0,
		0, 1, 2, 1, 0,
		0, 1, 2, 1, 0
	};
	
	// crossing
	/*
	int colors[] = {
		0, 1, 2, 1, 0,
		1, 1, 2, 1, 1,
		2, 2, 2, 2, 2,
		1, 1, 2, 1, 1,
		0, 1, 2, 1, 0
	};
	*/
	for (int i = 0; i < num; ++i) {
		if (colors[i] == 0) {
			gen.set_color(slices[i], ds::Color(184, 203, 98, 255));
		}
		else if (colors[i] == 1) {
			gen.set_color(slices[i], ds::Color(223, 215, 204, 255));
		}
		if (colors[i] == 2) {
			gen.set_color(slices[i], ds::Color(151, 144, 138, 255));
		}
	}

	//for (int i = 0; i < 1024; ++i) {
		//gen.set_color(i, ds::Color(math::random(0, 255), math::random(0, 255), math::random(0, 255), 255));
	//}

	//gen.create_cylinder(1.0f, 1.0f, 12);
	
	/*
	gen.move_edge(2, v3(0.0f, 0.5f, 0.0f));	
	gen.move_edge(10, v3(0.0f,0.5f, 0.0f));
	gen.move_edge(24, v3(0.0f, -0.5f, 0.0f));
	gen.move_edge(32, v3(0.0f, -0.5f, 0.0f));
	gen.move_edge(11, v3(0.5f, 0.0f, 0.0f));
	gen.move_edge(35, v3(0.5f, 0.0f, 0.0f));
	gen.move_edge(1, v3(-0.5f, 0.0f, 0.0f));
	gen.move_edge(25, v3(-0.5f, 0.0f, 0.0f));
	*/
	//gen.scale_face(4, 1.5f);
	/*
	ds::Quaternion q = ds::quat::euler2quat(0.0f,0.0f,DEGTORAD(45.0f));
	ds::mat4 m = ds::quat::quat2matrix(q);
	v3 v(1, 0, 0);
	v3 n = m * v;
	LOG << "===> N: " << DBG_V3(n);
	*/
	/*
	gen.add_cube(v3(0, 0, 0), v3(2, 2, 2));
	ds::gen::IndexList il;
	for (int i = 0; i < 3; ++i) {
		il.clear();
		gen.find_adjacent_faces(0, il);
		for (int i = 0; i < il.indices.size(); ++i) {
			gen.subdivide(i);
		}
	}
	il.clear();
	gen.find_adjacent_faces(0, il);
	LOG << "faces: " << il.indices.size();
	gen.debug_face(242);
	gen.smooth(il,1.0f);
	gen.debug_face(242);
	*/
	// triangle
	//uint16_t faces[6];
	//uint16_t f = gen.add_cube(v3(-5, 0, 0), v3(1.0f, 2.0f, 0.5f));
	
	//gen.move_edge(16, v3(-0.4f, 0.0f, 0.0f));
	//gen.move_edge(18, v3( 0.4f, 0.0f, 0.0f));
	//gen.parse("triangle.def");
	//gen.parse("gem_stone.def");
	//gen.create_torus(5.0f, 0.5f, 32);
	/*
	gen.create_grid(v2(1, 1), 3, 3);
	ds::gen::IndexList il;
	gen.find_adjacent_faces(4, il);
	for (int i = 0; i < il.indices.size(); ++i) {
		LOG << i << " = " << il.indices[i];
	}
	*/
	/*
	gen.move_edge(28, v3(0.0f, 0.2f, 0.0f));
	gen.split_edge(104, 0.25f);
	gen.move_face(18, v3(0.0f, 0.0f, -0.2f));
	gen.scale_face(19, 0.8f);
	gen.extrude_edge(41, v3(0, 0, 1));

	uint16_t torus = gen.create_torus(v3(4,0,0),1.0f, 0.2f, 0.4f, 12);
	uint16_t torus_faces[128];
	int num_torus = gen.get_connected_faces(torus, torus_faces, 128);
	*/
	//
	//
	//uint16_t faces[20];
	//v3 p[] = { v3(-1,1,-1),v3(1,1,-1),v3(1,-1,-1),v3(-1,-1,-1) };
	//faces[0] = gen.add_face(p);
	//gen.hsplit_edge(0, 0.2f);
	//gen.hsplit_edge(4, 0.5f);
	//gen.move_face(1, v3(0.0f, 0.0f, -0.1f));
	//gen.vsplit_edge(3, 0.2f);
	//gen.subdivide(0);
	/*
	LOG << "creating grider";
	ds::gen::MeshGen g2;// = new ds::gen::MeshGen;
	createGriderBox(&g2, 0.75f, 0.08f);
	float a = 0.0f;
	for (int i = 0; i < 8; ++i) {
		float yp = static_cast<float>(i)* 0.75f;
		gen.add(g2, v3(-2.0f, -3.0f + yp, 0.0f),v3(1,1,1),v3(a,0.0f,0.0f));
		a += DEGTORAD(20.0f);
	}
	*/
	//delete g2;
	//gen.transform(v3(0, 0, 0), v3(0.5f,0.5f,0.5f), v3(DEGTORAD(45.0f), 0.0f, 0.0f));
	/*
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
	
	//for (int i = 0; i < 1024; ++i) {
		//gen.texture_face(i, math::buildTexture(682, 260, 32, 32));
		//gen.set_color(i, ds::Color(math::random(0, 255), math::random(0, 255), math::random(0, 255), 255));
	//}
	/*
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
	gen.recalculate_normals();
	//gen.debug();
	//gen.build(_mesh);
	//gen.save_mesh("street_1");
	//_mesh->load("street_1");
	//ID id = _scene->add(_mesh, v3(0, 0, 0), ds::DrawMode::IMMEDIATE);
	//_scene->add(_mesh, v3(0, 0, 2), ds::DrawMode::IMMEDIATE);
	//_scene->add(_mesh, v3(0, 0, -2), ds::DrawMode::IMMEDIATE);
	ds::Mesh* m1 = new ds::Mesh();
	m1->load("street_1");
	_scene->add(m1, v3(0, 0, 0), ds::DrawMode::IMMEDIATE);
	_objects.push_back(m1);
	ds::Mesh* m2 = new ds::Mesh();
	m2->load("street_2");
	_scene->add(m2, v3(0, 0, 1), ds::DrawMode::IMMEDIATE);
	_objects.push_back(m2);
}



GeoTestState::~GeoTestState() {
	_objects.destroy_all();
	delete _gui;
	delete _mesh;
}

void GeoTestState::createHandrail(float length, float griderSize, int segments, float height) {
	uint16_t faces[16];
	int cnt = 0;
	faces[cnt++] = gen.add_cube(v3(0, 0, 0), v3(length, griderSize, griderSize), v3(DEGTORAD(45.0f), 0.0f, 0.0f));
	int s = (segments - 1) / 2;
	float il = (length - griderSize * 4.0f);
	float step = il * 0.5f / s;	
	for (int i = -s; i < s + 1; ++i) {
		faces[cnt++] = gen.add_cube(v3(step * i, -height * 0.5f - griderSize * 0.5f, 0.0f), v3(griderSize, height, griderSize));
	}
	for (int i = 0; i < cnt; ++i) {
		gen.set_color(faces[i], ds::Color(128, 128, 128, 255));
	}
	//gen.save_bin("Test.data");
	//gen.save_text("Test.txt");
	gen.save_mesh("handrail");
}

void GeoTestState::createCoords() {
	// coordsystem
	uint16_t faces[6];
	gen.add_cube(v3(0.5f, 0.0f, 0.0f), v3(1.0f, 0.1f, 0.1f), faces);
	for (int i = 0; i < 6; ++i) {
		gen.set_color(faces[i], ds::Color(255, 0, 0, 255));
	}
	gen.add_cube(v3(0.05f, 0.5f, 0.0f), v3(0.1f, 0.9f, 0.1f), faces);
	for (int i = 0; i < 6; ++i) {
		gen.set_color(faces[i], ds::Color(0, 255, 0, 255));
	}
	gen.add_cube(v3(0.05f, 0.0f, 0.5f), v3(0.1f, 0.1f, 0.9f), faces);
	for (int i = 0; i < 6; ++i) {
		gen.set_color(faces[i], ds::Color(0, 0, 255, 255));
	}
}

void GeoTestState::createGriderBox(ds::gen::MeshGen* gen, float dim, float griderSize) {
	float size = dim;
	float hs = size * 0.5f;
	float w = griderSize;
	float hw = w * 0.5f;
	gen->add_cube(v3(-hs + hw, 0.0f,-hs + hw), v3(w, size, w));
	gen->add_cube(v3( hs - hw, 0.0f,-hs + hw), v3(w, size, w));
	gen->add_cube(v3(-hs + hw, 0.0f, hs - hw), v3(w, size, w));
	gen->add_cube(v3( hs - hw, 0.0f, hs - hw), v3(w, size, w));

	gen->add_cube(v3(0.0f, hs - hw, hs - hw), v3(size - 2.0f * w, w, w));
	gen->add_cube(v3(0.0f,-hs + hw,-hs + hw), v3(size - 2.0f * w, w, w));
	gen->add_cube(v3(0.0f, hs - hw,-hs + hw), v3(size - 2.0f * w, w, w));
	gen->add_cube(v3(0.0f,-hs + hw, hs - hw), v3(size - 2.0f * w, w, w));

	gen->add_cube(v3( hs - hw,-hs + hw, 0.0f), v3(w, w, size - 2.0f * w));
	gen->add_cube(v3( hs - hw, hs - hw, 0.0f), v3(w, w, size - 2.0f * w));
	gen->add_cube(v3(-hs + hw,-hs + hw, 0.0f), v3(w, w, size - 2.0f * w));
	gen->add_cube(v3(-hs + hw, hs - hw, 0.0f), v3(w, w, size - 2.0f * w));
}
// -------------------------------------------------------
// update
// -------------------------------------------------------
int GeoTestState::update(float dt) {
	_camera->update(dt);	
	_gui->handleClick(_camera);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void GeoTestState::render() {
	// scene
	_scene->draw();
	_gui->drawGUI();
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

void GeoTestState::drawGUI() {
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	v2 pos(10, 760);
	int state = 1;
	gui::start(1, &pos, true);
	gui::begin("Camera", &state);
	char buffer[128];
	sprintf_s(buffer, 128, "%2.2f %2.2f %2.2f", _camera->getPosition().x, _camera->getPosition().y, _camera->getPosition().z);
	gui::Value("Pos", buffer);
	gui::Value("Yaw", RADTODEG(_camera->getAngle()));
	gui::Value("Pitch", RADTODEG(_camera->getPitch()));
	if (gui::Button("Reset")) {
		_camera->setPosition(v3(0, 0, -10), v3(0, 0, 1));
		_camera->resetYAngle();
	}
	gui::end();
}