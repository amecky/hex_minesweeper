#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include <renderer\render_types.h>
#include "GameContext.h"

class Minesweeper : public ds::BaseApp {

public:	
	Minesweeper();
	virtual ~Minesweeper();
	bool loadContent();
	const char* getTitle() {
		return "HexMinesweeper";
	}
	void init();
	void update(float dt);
	void draw();
	void onGUIButton(ds::DialogID dlgID, int button);
private:
	GameContext* _context;
};