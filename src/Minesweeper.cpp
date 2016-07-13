#include "Minesweeper.h"
#include "utils\Log.h"
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\IntroState.h"
#include "gamestates\TestState.h"

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
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool Minesweeper::loadContent() {	

	_backgroundScene = game->create2DScene("Background");
	_material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);
	_backgroundScene->setActive(true);


	_context->hud = ds::res::getGUIDialog("HUD");
	addGameState(new MainGameState(_context, game));
	addGameState(new GameOverState(_context, game));
	addGameState(new MainMenuState(_context, game));
	addGameState(new HighscoreState(_context, game));
	addGameState(new TestState(_context, game));
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
	//scoring::load(&_context->highscore);

	ID id = _backgroundScene->add(v2(512, 384), math::buildTexture(0, 512, 512, 384), _material);
	_backgroundScene->scale(id, v2(2, 2));
	return true;
}

void Minesweeper::init() {
	// for testing
	_context->reset();
	activate("MainMenu");
}

void Minesweeper::render() {	
}