#include "TestGameState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

TestGameState::TestGameState(ds::DialogManager* gui, GameContext* context) : ds::GameState("TestState"), _context(context), _gui(gui) {
}


TestGameState::~TestGameState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void TestGameState::activate() {
	_gui->activate("Test");
}

// --------------------------------------------
// activate
// --------------------------------------------
void TestGameState::deactivate() {
	_gui->deactivate("Test");
}

// --------------------------------------------
// update
// --------------------------------------------
int TestGameState::update(float dt) {
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int TestGameState::onGUIButton(ds::DialogID dlgID, int button) {
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void TestGameState::render() {
	ds::sprites::draw(v2(512, 384), ds::math::buildTexture(ds::Rect(0, 512, 512, 384)), 0.0f, 2.0f, 2.0f);
	ds::sprites::drawTiledXY(v2(100, 384), v2(240, 120), ds::math::buildTexture(760.0f, 160.0f, 100.0f, 60.0f),10.0f);
	ds::sprites::drawTiledXY(v2(100, 584), v2(240.0f,140.f), ds::math::buildTexture(760.0f, 270.0f, 100.0f, 100.0f), 10.0f);
	int state = 1;
	v2 pos(512, 450);
	gui::start(1, &pos);
	if (gui::begin("Test", &state)) {
		//gui::BoxTest();
		gui::beginGroup();
		if (gui::Button(2, "OK")) {
			LOG << "OK pressed";
		}
		if (gui::Button(3, "Cancel")) {
			LOG << "Cancel clicked";
		}
		gui::endGroup();
	}
	gui::end();
}


