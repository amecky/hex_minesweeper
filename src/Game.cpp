#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <ds_imgui.h>
#include "..\resource.h"
#include "Dialog.h"
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
	SpriteBatchBufferInfo sbbInfo = { 2048, textureID, ds::TextureFilters::LINEAR };
	_spriteBuffer = new SpriteBatchBuffer(sbbInfo);

	_settings.wiggleScale = 0.2f;
	_settings.wiggleTTL = 0.2f;
	_settings.numberScaleAmplitude = 1.5f;
	_settings.numberScaleTTL = 0.4f;
	_settings.menuTTL = 1.2f;

	_board = new Board(_spriteBuffer, &_settings);
	_board->activate(2);

	Score score;

	_hud = new HUD(_spriteBuffer, &score, &_settings);
	_hud->reset();

	_mode = GM_MENU;
	_selectedMode = 2;

	dialog::init(_spriteBuffer);

	_running = true;

	_debugPanel = { 'D', false, false, 1 };

	_menuTimer = 0.0f;
	//float menuTTL = 1.6f;
}


Game::~Game() {
}

enum FloatInDirection {
	FID_LEFT,
	FID_RIGHT
};

int floatButton(float time, float ttl, FloatInDirection dir) {
	if (time <= ttl) {
		if (dir == FloatInDirection::FID_LEFT) {
			return tweening::interpolate(tweening::easeOutElastic, -200, 512, time, ttl);
		}
		else {
			return tweening::interpolate(tweening::easeOutElastic, 1020, 512, time, ttl);
		}
	}
	return 512;
}

// ---------------------------------------------------------------
// show game over menu
// ---------------------------------------------------------------
int showGameOverMenu(const Score& score, float time, float ttl) {
	int ret = 0;
	dialog::begin();
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	if (score.success) {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 420, 310, 60));
	}
	else {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 490, 300, 60));
	}
	dialog::Image(ds::vec2(512, 470), ds::vec4(610, 160, 400, 50));
	dialog::Image(ds::vec2(512, 390), ds::vec4(610, 160, 400, 50));
	dialog::FormattedText(ds::vec2(400, 470), "Bombs left: %d Rank %d", score.bombsLeft,score.rank);
	dialog::FormattedText(ds::vec2(400, 390), "Time: %02d:%02d", score.minutes, score.seconds);
	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 320), ds::vec4(0, 368, 300, 50), "Replay")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 230), ds::vec4(0, 300, 300, 50), "Main menu")) {
		ret = 2;
	}
	dialog::end();
	return ret;
}

// ---------------------------------------------------------------
// show main menu
// ---------------------------------------------------------------
int showMainMenu(float time, float ttl) {
	int ret = 0;
	dialog::begin();
	int dy = 600;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 600, 640, 70));

	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 450), ds::vec4(0, 368, 300, 50), "easy")) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 370), ds::vec4(0, 368, 300, 50), "medium")) {
		ret = 2;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 290), ds::vec4(0, 368, 300, 50), "hard")) {
		ret = 3;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 210), ds::vec4(0, 300, 300, 50), "Exit")) {
		ret = 4;
	}
	dialog::end();
	return ret;
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
	if (_score.success) {
		int offset = _selectedMode * 10;
		Highscore current = { _score.minutes,_score.seconds,_selectedMode };
		for (int i = 0; i < 10; ++i) {
			if (_highscores[offset + i].compare(current)) {
				// FIXME: insert score here
				return i;
			}
		}
	}
	return -1;
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
		gui::start(ds::vec2(0, 755));
		if (_debugPanel.active) {
			gui::begin("Debug", &_debugPanel.state);
			gui::Input("Menu TTL", &_settings.menuTTL);
			gui::Value("FPS", ds::getFramesPerSecond());
			gui::Input("Wiggle Scale", &_settings.wiggleScale);
			gui::Input("Wiggle TTL", &_settings.wiggleTTL);
			gui::Input("Number Scale", &_settings.numberScaleAmplitude);
			gui::Input("Number TTL", &_settings.numberScaleTTL);
			if (gui::Button("Small")) {
				_hud->reset();
				_selectedMode = 0;
				_board->activate(0);
			}
			if (gui::Button("Medium")) {
				_hud->reset();
				_selectedMode = 1;
				_board->activate(1);
			}
			if (gui::Button("Large")) {
				_hud->reset();
				_selectedMode = 2;
				_board->activate(2);
			}
			if (gui::Button("Show bombs")) {
				_board->toggleShowBombs();
			}
			if (gui::Button("Reset timer")) {
				_menuTimer = 0.0f;
			}
			if (gui::Button("Game over")) {
				_menuTimer = 0.0f;
				_mode = GM_GAMEOVER;
			}
		}
		gui::end();
	}
}
// ---------------------------------------------------------------
// render
// ---------------------------------------------------------------
void Game::render() {

	ds::begin();

	_spriteBuffer->begin();
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
		if (ret > 0 && ret < 4) {
			_selectedMode = ret - 1;
			_hud->reset();
			_board->activate(_selectedMode);
			_mode = GM_RUNNING;
		}
		else  if (ret == 4) {
			_running = false;
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

	_spriteBuffer->flush();
	
	renderDebugPanel();
	
	ds::end();
}
