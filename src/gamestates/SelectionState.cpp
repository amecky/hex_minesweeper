#include "SelectionState.h"
#include <utils\Log.h>
#include <base\EventStream.h>

SelectionState::SelectionState(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("SelectionState", "ModeSelection", game), _context(context) {
}


SelectionState::~SelectionState() {
}

int SelectionState::update(float dt) {
	_dialog->tick(dt);
	return 0;
}
void SelectionState::activate() {
	_dialog->startTransition(1, v2(1200, 500), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(2, v2(-200, 380), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(3, v2(1200, 260), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(4, v2(1200, 120), SELECTION_MENU_TRANSITION);
}
// --------------------------------------------
// click
// --------------------------------------------
int SelectionState::onGUIButton(int button) {
	LOG << "button: " << button;
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
	return button;
}

