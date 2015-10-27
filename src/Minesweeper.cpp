#include "Minesweeper.h"
#include "utils\Log.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"

ds::BaseApp *app = new Minesweeper(); 

Minesweeper::Minesweeper() : ds::BaseApp() {
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	_settings.clearColor = ds::Color(0,0,0,255);	
	_settings.showEditor = true;
	_context = new GameContext;
	_context->hud = &hud;
	_context->mode = 0;
}

Minesweeper::~Minesweeper() {
	delete _context;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Minesweeper::loadContent() {	
	int texture = ds::renderer::loadTexture("TextureArray");
	assert(texture != -1);
	ds::sprites::initializeTextSystem(texture, "xscale");
	gui::initialize(_settings.showEditor);
	initializeGUI();
	initializeHUD(0, "xscale");
	stateMachine->add(new MainGameState(_context));
	stateMachine->add(new GameOverState(&gui,_context));
	stateMachine->add(new MainMenuState(&gui, _context));
	stateMachine->add(new HighscoreState(&gui, _context));
	stateMachine->connect("GameOver", 1, "MainGame");
	stateMachine->connect("GameOver", 2, "MainMenu");
	stateMachine->connect("MainGame", 1, "GameOver");
	stateMachine->connect("MainMenu", 1, "MainGame");
	stateMachine->connect("MainMenu", 5, "Highscores");
	stateMachine->connect("Highscores", 1, "MainMenu");

	scoring::load(&_context->highscore);
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

void Minesweeper::onGUIButton(ds::DialogID dlgID, int button) {
	if (dlgID == 1 && button == 4) {
		shutdown();
	}
}
