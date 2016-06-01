#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <utils\MeshGen.h>
#include "..\commands\MeshCommand.h"

struct GUIState {

	v3 pos;
	v3 scale;
	bool showAddCube;

	void reset() {
		showAddCube = false;
		pos = v3(0, 0, 0);
		scale = v3(1, 1, 1);
	}
};

class GeoTestState : public ds::GameState {

public:
	GeoTestState();
	~GeoTestState();
	int update(float dt);
	void render();
	int onChar(int ascii);
private:
	void drawGUI();
	void createGriderBox(ds::gen::MeshGen* gen,float dim, float griderSize);
	ds::gen::MeshGen gen;
	ds::FPSCamera* _camera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Scene* _scene;
	bool _pressed;
	ds::OrthoCamera* _orthoCamera;
	int _selectedFace;
	GUIState _state;
	ds::Array<MeshCommand*> _commands;
};

