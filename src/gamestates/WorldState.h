#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <gen\MeshGenGUI.h>

class WorldState : public ds::GameState {

public:
	WorldState();
	~WorldState();
	void init();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	void buildTerrain();
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Array<ds::Mesh*> _objects;
	ds::Scene* _scene;
	bool _pressed;
};

