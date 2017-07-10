#define DS_IMPLEMENTATION
#include <diesel.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define SPRITE_IMPLEMENTATION
#include <SpriteBatchBuffer.h>
#define DS_IMGUI_IMPLEMENTATION
#include <ds_imgui.h>
#include "..\resource.h"
#include "Board.h"
#include "HUD.h"
#include "GameContext.h"
#include "Dialog.h"
#include "tweening.h"
// ---------------------------------------------------------------
// Game modes
// ---------------------------------------------------------------
enum GameState {
	GM_MENU,
	GM_RUNNING,
	GM_GAMEOVER
};

// ---------------------------------------------------------------
// DebugPanel
// ---------------------------------------------------------------
struct DebugPanel {
	char key;
	bool pressed;
	bool active;
	int state;
};

struct DebugSettings {
	int value;
};

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

// ---------------------------------------------------------------
// initialize rendering system
// ---------------------------------------------------------------
void initialize() {
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Colors - match 3 game";
	rs.clearColor = ds::Color(0.125f, 0.125f, 0.125f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);
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
	int dy = 550;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	if (score.success) {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 420, 310, 60));
	}
	else {
		dialog::Image(ds::vec2(512, dy), ds::vec4(0, 490, 300, 60));
	}
	ds::vec2 mp = ds::getMousePosition();
	dialog::FormattedText(ds::vec2(400, 450), "Bombs left: %d", score.bombsLeft);
	dialog::FormattedText(ds::vec2(400, 400), "Time: %02d:%02d", score.minutes, score.seconds);
	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 320), ds::vec4(0, 368, 300, 50))) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 230), ds::vec4(0, 300, 300, 50))) {
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
	int dy = 550;
	if (time <= ttl) {
		dy = tweening::interpolate(tweening::easeOutElastic, 900, 600, time, ttl);
	}
	dialog::Image(ds::vec2(512, dy), ds::vec4(0, 600, 640, 70));
	
	int dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 450), ds::vec4(0, 368, 300, 50))) {
		ret = 1;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 370), ds::vec4(0, 368, 300, 50))) {
		ret = 2;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_LEFT);
	if (dialog::Button(ds::vec2(dx, 290), ds::vec4(0, 368, 300, 50))) {
		ret = 3;
	}
	dx = floatButton(time, ttl, FloatInDirection::FID_RIGHT);
	if (dialog::Button(ds::vec2(dx, 210), ds::vec4(0, 300, 300, 50))) {
		ret = 4;
	}
	dialog::end();
	return ret;
}
// ---------------------------------------------------------------
// handle input for debug panel
// ---------------------------------------------------------------
void handleDebugInput(DebugPanel* panel) {
	if (ds::isKeyPressed(panel->key)) {
		panel->pressed = true;
	}
	else if (panel->pressed) {
		panel->pressed = false;
		panel->active = !panel->active;
	}
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	initialize();

	gui::init();

	// load image using stb_image
	RID textureID = loadImage("content\\TextureArray.png");

	// create the sprite batch buffer
	SpriteBatchBufferInfo sbbInfo = { 2048, textureID, ds::TextureFilters::LINEAR };
	SpriteBatchBuffer spriteBuffer(sbbInfo);

	GameSettings settings;
	settings.wiggleScale = 0.2f;
	settings.wiggleTTL = 0.2f;

	Board board(&spriteBuffer, &settings);
	board.activate(2);

	Score score;
	
	HUD hud(&spriteBuffer, &score);
	hud.reset();

	GameState mode = GM_MENU;
	int selectedMode = 2;

	
	
	dialog::init(&spriteBuffer);
	
	bool running = true;

	DebugPanel debugPanel  = { 'D', false, false, 1 };

	float menuTimer = 0.0f;
	float menuTTL = 1.6f;

	while (ds::isRunning() && running) {

		handleDebugInput(&debugPanel);

		ds::begin();

		spriteBuffer.begin();		

		for (int i = 0; i < 6; ++i) {
			spriteBuffer.add(ds::vec2(-50 + i * 200, 384), ds::vec4(400, 0, 200, 600));
		}

		if (mode == GM_RUNNING) {
			int max = GAME_MODES[selectedMode].maxBombs;
			if (board.select()) {				
				int marked = board.getNumMarked();
				hud.setBombs(max - marked);
			}
			else {
				score.minutes = hud.getMinutes();
				score.seconds = hud.getSeconds();
				menuTimer = 0.0f;
				if (board.getMarkedCorrectly() == max) {
					score.success = true;
				}
				else {
					score.success = false;
				}
				mode = GM_GAMEOVER;
			}
			board.render();
		}

		if (mode == GM_MENU) {
			menuTimer += static_cast<float>(ds::getElapsedSeconds());
			int ret = showMainMenu(menuTimer, menuTTL);
			if (ret > 0 && ret < 4) {
				selectedMode = ret - 1;
				hud.reset();
				board.activate(selectedMode);
				mode = GM_RUNNING;
			}
			else  if (ret == 4) {
				running = false;
			}
		}
		else if (mode == GM_GAMEOVER) {
			menuTimer += static_cast<float>(ds::getElapsedSeconds());
			board.render();
			int ret = showGameOverMenu(score,menuTimer,menuTTL);
			if (ret == 1) {
				board.activate(selectedMode);
				hud.reset();
				mode = GM_RUNNING;
			}
			else if (ret == 2) {
				menuTimer = 0.0f;
				mode = GM_MENU;
			}
		}
		if (mode == GM_RUNNING || mode == GM_GAMEOVER) {
			board.tick(static_cast<float>(ds::getElapsedSeconds()));
			hud.tick(static_cast<float>(ds::getElapsedSeconds()));
		}

		if (mode == GM_RUNNING) {
			hud.render();
		}
		
		spriteBuffer.flush();

		if (debugPanel.active) {
			gui::start(ds::vec2(0, 755));
			if (debugPanel.active) {
				gui::begin("Debug", &debugPanel.state);
				gui::Value("FPS", ds::getFramesPerSecond());
				gui::Input("Wiggle Scale", &settings.wiggleScale);
				gui::Input("Wiggle TTL", &settings.wiggleTTL);
				if (gui::Button("Small")) {
					hud.reset();
					selectedMode = 0;
					board.activate(0);
				}
				if (gui::Button("Medium")) {
					hud.reset();
					selectedMode = 1;
					board.activate(1);
				}
				if (gui::Button("Large")) {
					hud.reset();
					selectedMode = 2;
					board.activate(2);
				}
				if (gui::Button("Show bombs")) {
					board.toggleShowBombs();
				}
				if (gui::Button("Reset timer")) {
					menuTimer = 0.0f;
				}
				if (gui::Button("Game over")) {
					menuTimer = 0.0f;
					mode = GM_GAMEOVER;
				}
			}
			gui::end();
		}
		ds::end();
	}
	ds::shutdown();
}