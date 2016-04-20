#pragma comment(lib, "D11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
//#include "dxstdafx.h"
#include <renderer\render_types.h>
#include "GameContext.h"

class Minesweeper : public ds::BaseApp {

public:	
	Minesweeper();
	virtual ~Minesweeper();
	bool initialize() {
		return true;
	}
	bool loadContent();
	const char* getTitle() {
		return "HexMinesweeper";
	}
	void init();
	void update(float dt);
	void render();
	//void onGUIButton(ds::DialogID dlgID, int button);
	void OnChar(char ascii, unsigned int keyState);
private:
	GameContext* _context;
};