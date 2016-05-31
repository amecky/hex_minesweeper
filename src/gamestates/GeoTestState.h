#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <utils\MeshGen.h>

class GeoTestState : public ds::GameState {

public:
	GeoTestState();
	~GeoTestState();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	void createGriderBox(ds::gen::MeshGen* gen,float dim, float griderSize);
	ds::gen::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Scene* _scene;
	bool _pressed;
};
