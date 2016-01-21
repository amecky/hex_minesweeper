#include "Minesweeper.h"
#include "utils\Log.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\IntroState.h"

ds::BaseApp *app = new Minesweeper(); 

Minesweeper::Minesweeper() : ds::BaseApp() {
	_context = new GameContext;
	_context->mode = 0;
}

Minesweeper::~Minesweeper() {
	delete _context;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Minesweeper::loadContent() {	
	_context->hud = gui.get("HUD");
	stateMachine->add(new MainGameState(_context));
	stateMachine->add(new GameOverState(&gui,_context));
	stateMachine->add(new MainMenuState(&gui, _context));
	stateMachine->add(new HighscoreState(&gui, _context));
	stateMachine->add(new IntroState(_context));
	stateMachine->connect("Intro", 1, "MainMenu");
	stateMachine->connect("GameOver", 1, "MainGame");
	stateMachine->connect("GameOver", 2, "MainMenu");
	stateMachine->connect("MainGame", 1, "GameOver");
	stateMachine->connect("MainMenu", 1, "MainGame");
	stateMachine->connect("MainMenu", 5, "Highscores");
	stateMachine->connect("Highscores", 1, "MainMenu");
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
	//scoring::load(&_context->highscore);
	return true;
}

void Minesweeper::init() {
	// for testing
	_context->reset();
	stateMachine->activate("MainMenu");
}


// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Minesweeper::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Minesweeper::draw() {
	
}

// -------------------------------------------------------
// on GUI button
// -------------------------------------------------------
void Minesweeper::onGUIButton(ds::DialogID dlgID, int button) {
	if (dlgID == 2 && button == 4) {
		shutdown();
	}
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
void Minesweeper::OnChar(char ascii, unsigned int keyState) {
}