#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <gen\MeshGenGUI.h>

class GeoTestState : public ds::GameState {

public:
	GeoTestState();
	~GeoTestState();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	void drawGUI();
	void createCoords();
	void createHandrail(float length, float griderSize, int segments, float height);
	void createGriderBox(ds::gen::MeshGen* gen,float dim, float griderSize);
	ds::gen::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Scene* _scene;
	ds::OrthoCamera* _orthoCamera;
	ds::gen::CommandContext _ctx;
	ds::gen::MeshGenGUI* _gui;
};

