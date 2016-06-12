#pragma once
#include <Point.h>
#include <lib\collection_types.h>
// http://www.policyalmanac.org/games/aStarTutorial.htm

class AStar {

struct Node {
	p2i pos;
	int parent;
	float f;
	float g;
	float h;
	bool available;
};

public:
	AStar(int width,int height);
	~AStar();
	void set(const p2i& p, bool available);
	int find(const p2i& start, const p2i& goal, p2i* ret, int max);
private:
	p2i _start;
	p2i _goal;
	bool isValid(const p2i& p) const;
	int to_index(const p2i& p) const;
	bool isAvailable(const p2i& p) const;
	int find_adjacent(const p2i& p, p2i* ret);
	int _width;
	int _height;
	Node* _tiles;
	ds::Array<Node> _openList;
	ds::Array<Node> _closedList;
};

