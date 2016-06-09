#pragma once
#include <Vector.h>
#include <lib\collection_types.h>
// http://www.policyalmanac.org/games/aStarTutorial.htm

class AStar {

struct Node {
	int x;
	int y;
	int parent;
	float f;
	float g;
	float h;

};

public:
	AStar(int width,int height);
	~AStar();
	void set(int x, int y, bool available);
	int find(int sx, int sy, int ex, int ey, v2* ret, int max);
private:
	bool isAvailable(int x, int y);
	int find_adjacent(int x, int y, int* ret);
	int _width;
	int _height;
	int* _tiles;
	ds::Array<Node> _openList;
	ds::Array<Node> _closedList;
};

