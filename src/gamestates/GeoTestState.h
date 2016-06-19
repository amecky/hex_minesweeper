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
	void init();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	void buildHouse(const p2i& gridPos);
	void buildWall(ds::gen::MeshGen& gen,int face_idx);
	void buildStreet(const p2i& gridPos);
	void createTile(int index, int directions);
	void createWindow(const v3& center);
	void buildTerrain();
	void buildTestTerrain();
	void drawGUI();
	void createCoords();
	void createStreets();
	void createHandrail(float length, float griderSize, int segments, float height);
	void createGriderBox(ds::gen::MeshGen* gen,float dim, float griderSize);
	ds::gen::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Array<ds::Mesh*> _objects;
	ds::Scene* _scene;
	ds::OrthoCamera* _orthoCamera;
	ds::gen::CommandContext _ctx;
	ds::gen::MeshGenGUI* _gui;
	const char* _name;
	bool _pressed;
	ds::Array<ID> _ids;
};

