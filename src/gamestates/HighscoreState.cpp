#include "HighscoreState.h"
#include <utils\Log.h>
/*
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

HighscoreState::HighscoreState(ds::DialogManager* gui, GameContext* context) : ds::GameState("Highscores"), _context(context), _gui(gui) {
}


HighscoreState::~HighscoreState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::activate() {
	_gui->activate("Highscores");	
	
	ds::GUIDialog* dlg = _gui->get("Highscores");
	PlayedTime entries[3];
	int cnt = _context->highscore_service.get(entries, 3);
	for (int i = 0; i < cnt; ++i ) {
	//for (int i = 0; i < 3; ++i) {
		//PlayedTime t1 = _context->highscore.times[i];
		std::string str;
		ds::string::formatTime(entries[i].minutes, entries[i].seconds, str);
		dlg->updateText(20 + i, str);
	}
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::deactivate() {
	_gui->deactivate("Highscores");
}

// --------------------------------------------
// update
// --------------------------------------------
int HighscoreState::update(float dt) {
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int HighscoreState::onGUIButton(ds::DialogID dlgID, int button) {
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void HighscoreState::render() {
	//ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
}


*/