#include "AStar.h"

/*
1) Add the starting square (or node) to the open list.

2) Repeat the following:

a) Look for the lowest F cost square on the open list. We refer to this as the current square.

b) Switch it to the closed list.

c) For each of the 8 squares adjacent to this current square …

If it is not walkable or if it is on the closed list, ignore it. Otherwise do the following.

If it isn’t on the open list, add it to the open list. Make the current square the parent of this square. Record the F, G, and H costs of the square.

If it is on the open list already, check to see if this path to that square is better, using G cost as the measure. A lower G cost means that this is a better path. If so, change the parent of the square to the current square, and recalculate the G and F scores of the square. If you are keeping your open list sorted by F score, you may need to resort the list to account for the change.

d) Stop when you:

Add the target square to the closed list, in which case the path has been found (see note below), or
Fail to find the target square, and the open list is empty. In this case, there is no path.
3) Save the path. Working backwards from the target square, go from each square to its parent square until you reach the starting square. That is your path.
*/

AStar::AStar(int width, int height) : _width(width) , _height(height) {
	_tiles = new Node[_width * _height];
}


AStar::~AStar() {
	delete[] _tiles;
}

bool AStar::isValid(const p2i& p) const {
	return (p.x >= 0 && p.x < _width && p.y >= 0 && p.y < _height);
}

int AStar::to_index(const p2i& p) const {
	assert(isValid(p));
	return p.x + p.y * _width;
}

void AStar::set(const p2i& p, bool available) {
	_tiles[to_index(p)].available = available;
}

bool AStar::isAvailable(const p2i& p) const {
	if (!isValid(p)) {
		return false;
	}
	return _tiles[to_index(p)].available;	
}

int AStar::find_adjacent(const p2i& p, p2i* ret) {
	int cnt = 0;
	static p2i directions[] = { p2i(0, 1), p2i(1, 0), p2i(0, -1), p2i(-1, 0) };
	for (int i = 0; i < 4; ++i) {
		p2i current = p + directions[i];
		if (isAvailable(current)) {
			ret[cnt++] = current;
		}
	}	
	return cnt;
}

int AStar::find(const p2i& start, const p2i& goal, p2i* ret, int max) {
	_tiles[to_index(start)].available = true;
	_tiles[to_index(goal)].available = true;
	//Node n;
	//n.x = sx;
	//n.y = sy;
	//n.parent = -1;
	//_openList.push_back(n);
	p2i n[4];
	int num = find_adjacent(start, n);
	for (int i = 0; i < num; ++i) {
		//Node n;
		//n.x = sx;
		//n.y = sy;
		//n.parent = 0;
		// calculate costs
		//_openList.push_back(n);
	}
	// remove start from open list and move it to closed list

	// find node with smallest F -> drop it from open list -> add it to closed list

	// find adjacent
/*
	5) Check all of the adjacent squares.Ignoring those that are on the closed list or unwalkable(terrain with walls, water, or other illegal terrain), add squares to the open list if they are not on the open list already.Make the selected square the “parent” of the new squares.

		6) If an adjacent square is already on the open list, check to see if this path to that square is a better one.In other words, check to see if the G score for that square is lower if we use the current square to get there.If not, don’t do anything.
		On the other hand, if the G cost of the new path is lower, change the parent of the adjacent square to the selected square(in the diagram above, change the direction of the pointer to point at the selected square).Finally, recalculate both the F and G scores of that square.If this seems confusing, you will see it illustrated below.
		*/


	// We repeat this process until we add the target square to the closed list, at which point it looks something like the illustration below.
	return 0;
}