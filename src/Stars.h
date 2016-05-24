#pragma once
#include <renderer\Scene.h>
#include <renderer\MeshBuffer.h>

// ------------------------------------------------------------
// Stars
// 
// Creates a bunch of squares on three different layers
// and will move them to the left. 
// ------------------------------------------------------------
class Stars {

public:
	Stars();
	~Stars();
	void move(float dt);
	void draw();
private:
	v3 randomPos(int index);
	ds::Scene* _stars;
	ds::Mesh* _star;
};

