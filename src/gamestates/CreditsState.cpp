#include "CreditsState.h"
#include <utils\Log.h>
#include <base\EventStream.h>

CreditsState::CreditsState(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("CreditsState", "Credits", game), _context(context) {
	_dialog->registerCallback(this);
}


CreditsState::~CreditsState() {
}

int CreditsState::update(float dt) {
	_dialog->tick(dt);
	_timer += dt;
	if (_timer > 2.0f) {
		_timer -= 2.0f;
		int next = _current + 2;
		if (next > 4) {
			next = 2;
		}
		toggle(next);
	}
	return 0;
}
void CreditsState::activate() {
	_current = 2;
	toggle(2);
	_timer = 0.0f;
	/*
	_dialog->startTransition(1, v2(1200, 500), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(2, v2(-200, 380), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(3, v2(1200, 260), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(4, v2(1200, 120), SELECTION_MENU_TRANSITION);
	_dialog->setVisible(5, false);
	_dialog->setVisible(6, false);
	_dialog->setVisible(7, false);
	*/
}

void CreditsState::toggle(int id) {
	_current = id;
	for (int i = 0; i < 4; ++i) {
		_dialog->setVisible(2 + i, false);
	}
	_dialog->setVisible(id, true);
	_dialog->setVisible(id + 1, true);
	_dialog->fadeColor(id, ds::Color(255, 255, 255, 0), ds::Color(255, 255, 255, 255), 0.8f);
	_dialog->fadeColor(id + 1, ds::Color(255, 255, 255, 0), ds::Color(255, 255, 255, 255), 0.8f);
}
// --------------------------------------------
// click
// --------------------------------------------
int CreditsState::onGUIButton(int button) {
	LOG << "button: " << button;
	/*
	if (button == 1) {
		_context->mode = 0;
		return 1;
	}
	if (button == 2) {
		_context->mode = 1;
		return 1;
	}
	if (button == 3) {
		_context->mode = 2;
		return 1;
	}
	*/
	return button;
}
