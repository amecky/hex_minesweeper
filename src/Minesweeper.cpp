#include "Minesweeper.h"
#include "utils\Log.h"
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\IntroState.h"
#include "gamestates\TestState.h"
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
	addGameState(new SelectionState(_context, game));
	addGameState(new TestState(_context, game));
	connectGameStates("GameOver", 1, "MainGame");
	connectGameStates("GameOver", 2, "MainMenu");
	connectGameStates("MainGame", 1, "GameOver");
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
	ID id = _backgroundScene->add(v2(512, 384), math::buildTexture(0, 512, 512, 384), _material);
	_backgroundScene->scale(id, v2(2, 2));
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