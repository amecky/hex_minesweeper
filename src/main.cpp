#define DS_IMPLEMENTATION
#include <diesel.h>
#define DS_IMGUI_IMPLEMENTATION
#include <ds_imgui.h>
#include "Game.h"
#define DS_LOG_PANEL
#include <ds_logpanel.h>
// ---------------------------------------------------------------
// initialize rendering system
// ---------------------------------------------------------------
void initialize() {
	ds::RenderSettings rs;
#ifdef DEBUG
	rs.width = 1500;
#elif
	rs.width = 1024;
#endif
	rs.height = 768;
	rs.title = "Colors - match 3 game";
	rs.clearColor = ds::Color(0,225,121,255);
	rs.multisampling = 4;
	ds::init(rs);
}

void myLogging(const logging::LogLevel&, const char* message) {
	OutputDebugString(message);
	OutputDebugString("\n");
	logpanel::add_line(message);
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	logging::logHandler = myLogging;
	//logging::currentLevel = logging::LL_INFO;
	logpanel::init(32);
	initialize();	
	LOG_DEBUG("Game initialized");
	Game game;

	while (ds::isRunning() && game.isRunning()) {

		game.tick(static_cast<float>(ds::getElapsedSeconds()));
		game.render();

	}
	ds::shutdown();
}