#pragma once
#include <gamestates\GameState.h>
#include <renderer\render_types.h>
#include <imgui\IMGUI.h>
#include "..\GameContext.h"
#include "..\HexGrid.h"
#include <vector>
#include <renderer\sprites.h>
#include <postprocess\GrayFadePostProcess.h>

class MainGameState : public ds::GameState {

public:
	MainGameState(GameContext* context, ds::Game* game);
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
	void openEmptyTiles(const Hex& h, ds::Array<Hex>& opened);

	int _maxBombs;
	GameContext* _context;
	HexGrid _grid;
	int _selected;
	int _width;
	int _height;
	ds::Scene2D* _scene;
	RID _material;
	bool _showBombs;
	float _endTimer;
	Hex _current;
	int _hover;
	ds::Texture _textures[10];
	int _gridOffset;
	bool _leftClick;
	ID _ps;
	ID _testPS;
	ds::GrayFadePostProcess* _grayfade;
};

