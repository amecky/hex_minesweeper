#pragma once
#include <vector>
#include <sprites\Sprite.h>
#include <ui\IMGUI.h>

class SpriteGroup {

typedef std::vector<ds::Sprite> Sprites;

public:
	SpriteGroup(const char* name);
	~SpriteGroup();
	void load();
	void save();
	void render();
	void showDialog();
private:
	Sprites _sprites;
	int _state;
	const char* _name;
	gui::ComponentModel<ds::SID> _model;
	int _offset;
	v2 _position;
	ds::SID _currentSID;
};

