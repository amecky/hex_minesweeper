#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <gen\MeshGenGUI.h>
#include <imgui\IMGUI.h>

class MeshGenTestState : public ds::GameState {

public:
	MeshGenTestState(const char* meshName);
	~MeshGenTestState();
	void init();
	int update(float dt);
	void render();
	int onChar(int ascii);
	void activate();
private:
	void showSelectionCubes(bool active);
	void drawGUI();
	ds::gen::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::Mesh* _mesh;
	ds::Mesh* _grid;
	ds::Mesh* _squares[4];
	ID _square_ids[4];
	ds::Scene* _scene;
	ds::OrthoCamera* _orthoCamera;
	const char* _name;
	bool _pressed;
	ID _grid_id;
	gui::ComponentModel<int> _model;
	int _offset;
	bool _switch;
};

