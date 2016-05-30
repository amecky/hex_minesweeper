#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <utils\geometrics.h>

class GeoTestState : public ds::GameState {

public:
	GeoTestState();
	~GeoTestState();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	ds::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Scene* _scene;
	bool _pressed;
};

