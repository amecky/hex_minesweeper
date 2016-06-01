#pragma once
#include <renderer\MeshBuffer.h>
#include <utils\MeshGen.h>

struct CommandContext {

	ds::gen::MeshGen* gen;
	ds::Mesh* mesh;
	int selectedFace;

};

class MeshCommand {

public:
	MeshCommand(ds::gen::MeshGen* gen, ds::Mesh* mesh) : _gen(gen), _mesh(mesh) , _active(false) {}
	virtual ~MeshCommand() {}
	virtual void draw() = 0;
	virtual const char* getButtonName() const = 0;
	const bool isActive() const {
		return _active;
	}
	void setActive(bool active) {
		_active = active;
	}
protected:
	ds::gen::MeshGen* _gen;
	ds::Mesh* _mesh;
private:
	bool _active;
};

class AddCubeCommand : public MeshCommand {

public:
	AddCubeCommand(ds::gen::MeshGen* gen, ds::Mesh* mesh) : MeshCommand(gen, mesh) , _pos(0,0,0), _scale(1,1,1) {}
	virtual ~AddCubeCommand() {}
	void draw();
	const char* getButtonName() const {
		return "Add Cube";
	}
private:
	v3 _pos;
	v3 _scale;
};
