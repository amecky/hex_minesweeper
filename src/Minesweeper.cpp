#include "Minesweeper.h"
#include <core\log\Log.h>
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\IntroState.h"
#include "gamestates\SelectionState.h"

const char* SCORE_NAMES[] = { "easy.scr", "medium.scr", "hard.scr" };

ds::BaseApp *app = new Minesweeper(); 

Minesweeper::Minesweeper() : ds::BaseApp() {
	_context = new GameContext;
	_context->mode = 0;
}

Minesweeper::~Minesweeper() {
	delete _context;
}

// -------------------------------------------------------
// prepare
// -------------------------------------------------------
void Minesweeper::prepare(ds::Settings* settings) {
	settings->screenWidth = 1024;
	settings->screenHeight = 768;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
	settings->logTypes = ds::LogTypes::LT_CONSOLE|ds::LogTypes::LT_FILE;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Minesweeper::loadContent() {	
	
	addGameState(new MainGameState(_context));
	addGameState(new MainMenuState(_context));
	addGameState(new HighscoreState(_context));
	addGameState(new SelectionState(_context));
	connectGameStates("MainGame", 1, "MainGame");
	connectGameStates("MainGame", 2, "MainMenu");
	connectGameStates("MainMenu", 1, "SelectionState");
	connectGameStates("SelectionState", 4, "MainMenu");
	connectGameStates("SelectionState", 1, "MainGame");
	connectGameStates("MainMenu", 5, "Highscores");
	connectGameStates("Highscores", 1, "MainMenu");
	
	for (int i = 0; i < 3; ++i) {
		if (!_context->highscores[i].load(SCORE_NAMES[i])) {
			Highscore hs;
			hs.minutes = 30;
			hs.seconds = 30;
			hs.mode = i;
			_context->highscores[i].add(hs);
			_context->highscores[i].save(SCORE_NAMES[i]);
		}
	}
	for (int j = 0; j < 3; ++j) {
		LOG << "scores " << SCORE_NAMES[j];
		for (int i = 0; i < _context->highscores[j].size(); ++i) {
			const Highscore& s = _context->highscores[j].get(i);
			LOG << i << " : " << s.minutes << ":" << s.seconds;
		}
	}

	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void Minesweeper::init() {
	_context->reset();
	activate("MainMenu");
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void Minesweeper::render() {	
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->draw(v2(512, 384), math::buildTexture(0, 512, 512, 384), 0.0f, v2(2, 2));
}

// -------------------------------------------------------
// onShutdown
// -------------------------------------------------------
void Minesweeper::onShutdown() {
	LOG << "saving scores";
	for (int i = 0; i < 3; ++i) {
		_context->highscores[i].save(SCORE_NAMES[i]);
	}
}