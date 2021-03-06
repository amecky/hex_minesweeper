#include "Minesweeper.h"
#include "utils\Log.h"
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\IntroState.h"
#include "GameSettings.h"

ds::BaseApp *app = new Minesweeper(); 

Minesweeper::Minesweeper() : ds::BaseApp() {
	//_CrtSetBreakAlloc(187);
	_context = new GameContext;
	_context->mode = 0;
	_context->settings = new GameSettings;
	_context->settings->load();
}

Minesweeper::~Minesweeper() {
	delete _context->settings;
	delete _context;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Minesweeper::loadContent() {	
	_context->mode = 2;
	addGameState(new MainGameState(_context));
	addGameState(new GameOverState(_context));
	addGameState(new MainMenuState(_context));
	//addGameState(new HighscoreState(_context));
	connectGameStates("GameOver", 1, "MainGame");
	connectGameStates("GameOver", 2, "MainMenu");
	connectGameStates("MainGame", 1, "GameOver");
	connectGameStates("MainMenu", 1, "MainGame");
	/*
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
	//activate("MeshGenTestState");
	activate("MainMenu");
	//activate("WorldState");
}

void Minesweeper::render() {
}

void Minesweeper::OnChar(uint8_t ascii) {
	
}