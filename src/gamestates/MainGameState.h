#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\GameContext.h"
#include "..\HexGrid.h"
#include <vector>
#include <sprites\SpriteGroup.h>

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
	int _selected;
	int _width;
	int _height;
	ds::SpriteGroup* _easyGroup;
	ds::SpriteGroup* _mediumGroup;
	ds::SpriteGroup* _hardGroup;
	bool _showBombs;
	float _endTimer;
	
};

