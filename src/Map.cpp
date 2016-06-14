#include "Map.h"
#include "Constants.h"
#include <Point.h>

namespace grid {
	
	v3 convert(int gx,int gy) {
		return v3(STARTX + gx * CELL_SIZE,STARTY + gy * CELL_SIZE,0.0f);	
	}

	v3 convert(const p2i& p) {
		return convert(p.x, p.y);
	}

	p2i convert(const v2& pos) {
		int my = ((int)pos.y - STARTY + HALF_CELL_SIZE) / CELL_SIZE;
		int mx = ((int)pos.x - STARTX + HALF_CELL_SIZE) / CELL_SIZE;
		return p2i(mx, my);
	}
}