#define DS_IMPLEMENTATION
#include <diesel.h>
#define DS_IMGUI_IMPLEMENTATION
#include <ds_imgui.h>
#define SPRITE_IMPLEMENTATION
#include <SpriteBatchBuffer.h>
#define DS_GAME_UI_IMPLEMENTATION
#include <ds_game_ui.h>
#define DS_ANIMATION_IMPLEMENTATION
#include "animation.h"
#include "Game.h"


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
	rs.supportDebug = true;
	ds::init(rs);
}


// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	initialize();

	Game game;

	while (ds::isRunning() && game.isRunning()) {

		game.tick(static_cast<float>(ds::getElapsedSeconds()));

		game.render();

	}
	ds::shutdown();
}