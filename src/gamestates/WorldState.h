#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <gen\MeshGenGUI.h>
#include "..\TinyWorld.h"

const int WORLD_SIZE = 16;

class WorldState : public ds::GameState {

	struct WorldTile {
		ID id;
		p2i coord;
	};

public:
	WorldState();
	~WorldState();
	void init();
	int update(float dt);
	void render();
	int onChar(int ascii);
	void activate();
private:
	void loadObjects();
	void loadObject(const char* name);
	void buildTerrain();
	void drawGUI();
	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Array<ds::Mesh*> _objects;
	ds::Scene* _scene;
	TinyWorld* _world;
	bool _pressed;
	WorldTile _tiles[WORLD_SIZE * WORLD_SIZE];
	bool _switch;
	p2i _selectedCell;
};

