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
	int _selected;
	int _width;
	int _height;
	//ds::SpriteGroup* _easyGroup;
	//ds::SpriteGroup* _mediumGroup;
	//ds::SpriteGroup* _hardGroup;
	ds::SpriteBuffer* _sprites;
	bool _showBombs;
	float _endTimer;
	Hex _current;
};

