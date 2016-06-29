#pragma once
#include <gamestates\GameState.h>
#include <renderer\render_types.h>
#include <imgui\IMGUI.h>
#include "..\GameContext.h"
#include "..\HexGrid.h"
#include <vector>
#include <renderer\sprites.h>

class MainGameState : public ds::GameState {

public:
	MainGameState(GameContext* context);
	virtual ~MainGameState();
	void init();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
	int onChar(int ascii);
	int onButtonUp(int button, int x, int y);
private:
	void fillBombs();
	void openEmptyTiles(const Hex& h);

	int _maxBombs;
	GameContext* _context;
	HexGrid _grid;
	//int _selected;
	int _width;
	int _height;
	bool _showBombs;
	float _endTimer;
	Hex _current;
	//ID _selected;
	ds::Mesh* _hexagon;
	ds::Mesh* _border;
	ds::Scene* _scene;
	ds::Scene* _boardScene;
	ds::Scene* _boardTexScene;
	ds::MeshBuffer* _colouredBuffer;
	ds::FPSCamera* _camera;
	ds::Mesh* _bomb;
	ds::Array<ds::Mesh*> _numbers;
	RID _material;
	RID _texMaterial;
};

