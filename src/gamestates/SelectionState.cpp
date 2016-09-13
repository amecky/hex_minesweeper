#include "SelectionState.h"
#include <core\log\Log.h>
#include <core\base\EventStream.h>

const float SELECTION_MENU_TRANSITION = 0.4f;

SelectionState::SelectionState(GameContext* context) : ds::BasicMenuGameState("SelectionState", "ModeSelection"), _context(context) {
	_dialog->registerCallback(this);
}


SelectionState::~SelectionState() {
}

void SelectionState::activate() {
	_dialog->activate();
	_dialog->startTransition(1, v2(1200, 500), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(2, v2(-200, 380), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(3, v2(1200, 260), SELECTION_MENU_TRANSITION);
	_dialog->startTransition(4, v2(1200, 120), SELECTION_MENU_TRANSITION);
	_dialog->setVisible(5, false);
	_dialog->setVisible(6, false);
	_dialog->setVisible(7, false);
}
void SelectionState::deactivate() {
	_dialog->deactivate();
}
// --------------------------------------------
// click
// --------------------------------------------
int SelectionState::onGUIButton(int button) {
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

void SelectionState::entering(ID id) {
	if (id == 1) {
		_dialog->setVisible(5,true);
	}
	if (id == 2) {
		_dialog->setVisible(6, true);
	}
	if (id == 3) {
		_dialog->setVisible(7, true);
	}
}

void SelectionState::leaving(ID id) {
	_dialog->setVisible(5, false);
	_dialog->setVisible(6, false);
	_dialog->setVisible(7, false);
}

