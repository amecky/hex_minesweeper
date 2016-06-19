#pragma once
#include <gamestates\GameState.h>
#include <renderer\QuadBuffer.h>
#include <vector>
#include <gen\MeshGen.h>
#include <gen\MeshGenCommand.h>
#include <gen\MeshGenGUI.h>
#include "..\TinyWorld.h"

const int WORLD_SIZE = 6;

class WorldState : public ds::GameState {

	struct TileCommand {

		p2i coord;
		ID id;

	};

	struct WorldTile {

		ID id;
		p2i coord;
		float height;

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
	void save();
	void load();
	void process(const TileCommand& command);
	void parseFile();
	void buildTerrain();
	void drawGUI();
	ds::FPSCamera* _camera;
	ds::OrthoCamera* _orthoCamera;
	ds::MeshBuffer* _buffer;
	ds::Mesh* _mesh;
	ds::Mesh* _selectionMesh;
	ID _selectionID;
	ds::Array<ds::Mesh*> _objects;
	ds::Scene* _scene;
	TinyWorld* _world;
	bool _pressed;
	WorldTile _tiles[WORLD_SIZE * WORLD_SIZE];
	bool _switch;
	p2i _selectedCell;
	int _meshID;
	p2i _gridPos;
	ds::Array<TileCommand> _commands;
};

