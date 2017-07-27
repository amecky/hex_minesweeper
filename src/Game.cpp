#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "..\resource.h"
#include "tweening.h"
#include <Windows.h>
// ---------------------------------------------------------------
// load image from the resources
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	HRSRC resourceHandle = ::FindResource(NULL, MAKEINTRESOURCE(IDB_PNG1), "PNG");
	if (resourceHandle == 0) {
		return NO_RID;
	}
	DWORD imageSize = ::SizeofResource(NULL, resourceHandle);
	if (imageSize == 0) {
		return NO_RID;
	}
	HGLOBAL myResourceData = ::LoadResource(NULL, resourceHandle);
	void* pMyBinaryData = ::LockResource(myResourceData);
	unsigned char *data = stbi_load_from_memory((const unsigned char*)pMyBinaryData, imageSize, &x, &y, &n, 4);
	ds::TextureInfo info = { x,y,n,data,ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(info, name);
	stbi_image_free(data);
	UnlockResource(myResourceData);
	FreeResource(myResourceData);
	return textureID;
}

Game::Game() {

	gui::init();

	// load image using stb_image
	RID textureID = loadImage("content\\TextureArray.png");

	// create the sprite batch buffer
	SpriteBatchBufferInfo sbbInfo = { 2048, textureID, ds::TextureFilters::POINT };
	_spriteBuffer = new SpriteBatchBuffer(sbbInfo);

	_settings.wiggleScale = 0.2f;
	_settings.wiggleTTL = 0.2f;
	_settings.numberScaleAmplitude = 1.5f;
	_settings.numberScaleTTL = 0.4f;
	_settings.menuTTL = 1.2f;
	_settings.highscorePagingTTL = 2.0f;

	_board = new Board(_spriteBuffer, &_settings);
	_board->activate(2);

	_score.rank = -1;
	_score.bombsLeft = 0;
	_score.minutes = 0;
	_score.seconds = 0;
	_score.success = false;

	_hud = new HUD(_spriteBuffer, &_score, &_settings);
	_hud->reset();

	_mode = GM_MENU;
	_selectedMode = 2;

	dialog::init(_spriteBuffer);

	_running = true;

	_debugPanel = { 'D', false, false, 1 };

	_menuTimer = 0.0f;

	// load highscores
	FILE* fp = fopen("scores.scr", "rb");
	if (fp) {
		for (int i = 0; i < 30; ++i) {
			fread(&_highscores[i], sizeof(Highscore), 1, fp);
		}
		fclose(fp);
	}

	_debugScore = ds::vec2(0, 0);

	_page = 0;
	_pageTimer = 0.0f;

	sprintf(_playerName, "%s", "Name");

	_inputActive = false;

	_dummy = 0;
	_dummyColor = ds::Color(128, 64, 255, 255);
	_dialogPos = p2i(20, 720);
}

Game::~Game() {
	// save highscores
	FILE* fp = fopen("scores.scr", "wb");
	if (fp) {
		for (int i = 0; i < 30; ++i) {
			fwrite(&_highscores[i], sizeof(Highscore), 1, fp);
		}
		fclose(fp);
	}
	delete _hud;
	delete _board;
	delete _spriteBuffer;
}

// ---------------------------------------------------------------
// handle input for debug panel
// ---------------------------------------------------------------
void Game::handleDebugInput(DebugPanel* panel) {
	if (ds::isKeyPressed(panel->key)) {
		panel->pressed = true;
	}
	else if (panel->pressed) {
		panel->pressed = false;
		panel->active = !panel->active;
	}
}

// ---------------------------------------------------------------
// handle highscore
// ---------------------------------------------------------------
int Game::handleScore() {
	int idx = -1;
	if (_score.success) {
		int offset = _selectedMode * 10;
		Highscore current = { _score.minutes,_score.seconds,_selectedMode };
		for (int i = 0; i < 10; ++i) {
			if (_highscores[offset + i].compare(current) != -1 && idx == -1) {
				idx = i;
			}
		}
		if (idx != -1) {
			int pos = _selectedMode * 10 + idx;
			// FIXME: move existing down the line
			int last = _selectedMode * 10 + 8;
			for (int p = last; p >= pos; --p) {
				_highscores[p + 1] = _highscores[p];
			}
			_highscores[pos] = current;
		}
	}
	return idx;
}

// ---------------------------------------------------------------
// tick
// ---------------------------------------------------------------
void Game::tick(float dt) {
	handleDebugInput(&_debugPanel);
	if (_mode == GM_RUNNING) {
		int max = GAME_MODES[_selectedMode].maxBombs;
		if (_board->select()) {
			int marked = _board->getNumMarked();
			int diff = max - _board->getNumMarked();
			if (diff != _score.bombsLeft) {
				_score.bombsLeft = diff;
				_hud->setBombs(diff);
			}
		}
		else {
			_score.minutes = _hud->getMinutes();
			_score.seconds = _hud->getSeconds();
			_menuTimer = 0.0f;
			if (_board->getMarkedCorrectly() == max) {
				_score.success = true;
			}
			else {
				_score.success = false;
			}
			_score.bombsLeft = max - _board->getMarkedCorrectly();
			_score.rank = handleScore();
			// FIXME: find highscore/ranking
			if (_score.rank != -1) {
				_inputActive = true;
			}
			else {
				_inputActive = false;
			}
			_mode = GM_GAMEOVER;
		}
		_board->tick(dt);
		_hud->tick(dt);
	}
}

// ---------------------------------------------------------------
// render debug panel if active
// ---------------------------------------------------------------
void Game::renderDebugPanel() {
	if (_debugPanel.active) {
		gui::start();		
		gui::begin("Debug", &_debugPanel.state,&_dialogPos,440);
		if (gui::Button("Dummy")) {
			++_dummy;
		}
		gui::Value("Dummy#1", _dummy);
		gui::Input("Menu TTL", &_settings.menuTTL);
		gui::Value("FPS", ds::getFramesPerSecond());
		gui::Input("Wiggle Scale", &_settings.wiggleScale);
		gui::Input("Wiggle TTL", &_settings.wiggleTTL);
		gui::Input("Number Scale", &_settings.numberScaleAmplitude);
		gui::Input("Number TTL", &_settings.numberScaleTTL);	
		gui::Input("HS TTL", &_settings.highscorePagingTTL);
		gui::Input("Mode", &_selectedMode);
		gui::Input("Color", &_dummyColor);
		if (gui::Button("Start Game")) {
			_hud->reset();
			_board->activate(_selectedMode);
		}
		if (gui::Button("Show bombs")) {
			_board->toggleShowBombs();
		}
		if (gui::Button("Stop game")) {
			_menuTimer = 0.0f;
			_mode = GM_GAMEOVER;
		}
		gui::Input("Score", &_debugScore);
		if (gui::Button("Reset timer")) {
			_menuTimer = 0.0f;
		}
		if (gui::Button("Game over")) {
			_menuTimer = 0.0f;
			_score.success = true;
			_score.seconds = _debugScore.y;
			_score.minutes = _debugScore.x;
			_score.rank = handleScore();
			_score.bombsLeft = 42;
			_mode = GM_GAMEOVER;
		}
		if (gui::Button("Highscores")) {
			_menuTimer = 0.0f;
			_mode = GM_HIGHSCORES;
		}
		if (gui::Button("Input Name")) {
			_inputActive = true;
			_inputDialog.reset(_playerName);
		}
		gui::debug();
		gui::end();
	}
}
// ---------------------------------------------------------------
// render
// ---------------------------------------------------------------
void Game::render() {

	ds::begin();

	_spriteBuffer->begin();
	dialog::begin();
	//
	// background
	//
	for (int i = 0; i < 6; ++i) {
		_spriteBuffer->add(ds::vec2(-50 + i * 200, 384), ds::vec4(400, 0, 200, 600));
	}

	if (_mode == GM_RUNNING) {
		//
		// render board and HUD
		//
		_board->render();
		_hud->render();
	}
	else if (_mode == GM_MENU) {
		_menuTimer += static_cast<float>(ds::getElapsedSeconds());
		//
		// render main menu immediate mode
		//
		int ret = showMainMenu(_menuTimer, _settings.menuTTL);
		if (!_inputActive) {
			if (ret > 0 && ret < 4) {
				_selectedMode = ret - 1;
				_hud->reset();
				_board->activate(_selectedMode);
				_mode = GM_RUNNING;
			}
			if (ret == 5) {
				_menuTimer = 0.0f;
				_pageTimer = 0.0f;
				_mode = GM_HIGHSCORES;
			}
			else  if (ret == 4) {
				_running = false;
			}
		}
	}
	else if (_mode == GM_GAMEOVER) {
		_menuTimer += static_cast<float>(ds::getElapsedSeconds());
		//
		// render game over menu immediate mode and board
		//
		_board->render();
		int ret = showGameOverMenu(_score, _menuTimer, _settings.menuTTL);
		if (ret == 1) {
			_board->activate(_selectedMode);
			_hud->reset();
			_mode = GM_RUNNING;
		}
		else if (ret == 2) {
			_menuTimer = 0.0f;
			_mode = GM_MENU;
		}
	}
	else if (_mode == GM_HIGHSCORES) {
		_menuTimer += static_cast<float>(ds::getElapsedSeconds());
		_pageTimer += static_cast<float>(ds::getElapsedSeconds());
		if (_pageTimer >= _settings.highscorePagingTTL) {
			_pageTimer -= _settings.highscorePagingTTL;
			_page = (_page + 1) & 1;
		}
		//
		// render game over menu immediate mode and board
		//
		int ret = showHighscores(_menuTimer, _settings.menuTTL, _selectedMode,_highscores, _page);
		if (ret == 1) {
			_menuTimer = 0.0f;
			_mode = GM_MENU;
		}
	}

	if (_inputActive) {
		_inputDialog.tick(static_cast<float>(ds::getElapsedSeconds()));
		int ret = _inputDialog.render();
		if (ret == 1 ) {
			sprintf(_playerName, "%s", _inputDialog.getName());
			if (_score.rank != -1 && _score.success) {
				sprintf(_highscores[_score.rank + _selectedMode * 10].name, "%s", _playerName);
			}
			_inputActive = false;
		}
		if (ret == 2) {
			_inputActive = false;
		}
	}
	dialog::end();
	_spriteBuffer->flush();
	
	renderDebugPanel();
	
	ds::end();
}
