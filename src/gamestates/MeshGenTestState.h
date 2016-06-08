#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <gen\MeshGenGUI.h>

class MeshGenTestState : public ds::GameState {

public:
	MeshGenTestState(const char* meshName);
	~MeshGenTestState();
	void init();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	ds::gen::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::Mesh* _mesh;
	ds::Mesh* _grid;
	ds::Scene* _scene;
	ds::OrthoCamera* _orthoCamera;
	const char* _name;
	bool _pressed;
	ID _grid_id;
};

