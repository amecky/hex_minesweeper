#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>

class GeoTestState : public ds::GameState {

public:
	GeoTestState();
	~GeoTestState();
	int update(float dt);
	void render();
private:
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Scene* _scene;
};

