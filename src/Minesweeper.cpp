#include "Minesweeper.h"
#include "utils\Log.h"
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\IntroState.h"
#include "gamestates\TestState.h"
#include "gamestates\RunState.h"
#include "GameSettings.h"
#include "gamestates\GeoTestState.h"
#include "gamestates\MeshGenTestState.h"
#include "gamestates\WorldState.h"

ds::BaseApp *app = new Minesweeper(); 

Minesweeper::Minesweeper() : ds::BaseApp() {
	//_CrtSetBreakAlloc(187);
	_context = new GameContext;
	_context->mode = 0;
	_context->settings = new GameSettings;
}

Minesweeper::~Minesweeper() {
	delete _context->settings;
	delete _context;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Minesweeper::loadContent() {	
	_context->hud = ds::res::getGUIDialog(18);
	_sprites = ds::res::getSpriteBuffer("BasicSpriteBuffer");
	//addGameState(new MainGameState(_context));
	//addGameState(new GameOverState(_context));
	//addGameState(new MainMenuState(_context));
	//addGameState(new HighscoreState(_context));
	addGameState(new TestState(_context));
	addGameState(new GeoTestState());
	addGameState(new MeshGenTestState("hexagon"));
	addGameState(new WorldState());
	/*
	addGameState(new RunState(_context));
	connectGameStates("GameOver", 1, "MainGame");
	connectGameStates("GameOver", 2, "MainMenu");
	connectGameStates("MainGame", 1, "GameOver");
	connectGameStates("MainMenu", 1, "MainGame");
	connectGameStates("MainMenu", 5, "Highscores");
	connectGameStates("Highscores", 1, "MainMenu");
	if (!_context->highscore_service.load()) {
		// add default scores
		for (int i = 0; i < 3; ++i) {
			PlayedTime t;
			t.minutes = 99;
			t.seconds = 99;
			t.mode = i;
			_context->highscore_service.add(t);
			_context->highscore_service.save();
		}
	}
	*/
	//scoring::load(&_context->highscore);
	return true;
}

void Minesweeper::init() {
	// for testing
	//_context->reset();
	activate("MeshGenTestState");
	//activate("GeoTestState");
	//activate("WorldState");
}

void Minesweeper::render() {
	/*
	_sprites->begin();
	for (int y = 0; y < 3; ++y) {
		for (int i = 0; i < 4; ++i) {
			_sprites->draw(v2(150 + i * 300, 150 + y * 300), math::buildTexture(510, 700, 300, 300));
		}
	}
	_sprites->end();
	*/
}