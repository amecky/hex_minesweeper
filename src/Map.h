#pragma once
#include <Vector.h>
#include <lib\Grid.h>

namespace grid {

	v3 convert(int gx,int gy);

	v3 convert(const p2i& p);

	p2i convert(const v2& pos);
}

