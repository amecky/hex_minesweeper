#pragma once
#include <Vector.h>
#include <lib\Grid.h>

namespace grid {

	v3 convert(int gx,int gy);

	v3 convert(const ds::Point& p);

	ds::Point convert(const v2& pos);
}

