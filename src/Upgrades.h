#pragma once
#include <renderer\Scene.h>
#include <renderer\MeshBuffer.h>

class Upgrades {

public:
	Upgrades();
	~Upgrades();
	void start(int upgradeCount);
	bool tick(float dt);
private:
	ID _ids[7];
	ds::Scene* _scene;
	ds::Array<ds::Mesh*> _meshes;
	bool _active;
};

