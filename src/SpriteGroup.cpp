#include "SpriteGroup.h"
#include <sprites\SpriteBatch.h>
#include <ui\IMGUI.h>

SpriteGroup::SpriteGroup(const char* name) : _name(name) {
	_position = v2(512, 384);
	_state = 1;
	_offset = 0;
	_currentSID = 0;
}


SpriteGroup::~SpriteGroup() {
}

void SpriteGroup::render() {
	for (size_t i = 0; i < _sprites.size(); ++i) {
		ds::sprites::draw(_sprites[i]);
	}
}

void SpriteGroup::load() {
	IdString str = ds::string::murmur_hash(_name);
	char buffer[128];
	sprintf_s(buffer, 128, "data\\%d", str);
	LOG << "loading binary file: " << buffer;
	FILE* f = fopen(buffer, "rb");
	if (f) {
		int nr = 0;
		fread(&nr, sizeof(int), 1, f);
		for (int i = 0; i < nr; ++i) {
			ds::Sprite sp;
			fread(&sp, sizeof(ds::Sprite), 1, f);
			_sprites.push_back(sp);
			char buffer[32];
			sprintf_s(buffer, 32, "Sprite %d", sp.id);
			_model.add(buffer, sp.id);
		}
		fclose(f);
	}
}

void SpriteGroup::save() {
	IdString str = ds::string::murmur_hash(_name);
	char buffer[128];
	sprintf_s(buffer, 128, "data\\%d", str);
	LOG << "saving binary file: " << buffer;
	FILE* f = fopen(buffer, "rb");
	if (f) {
		int nr = _sprites.size();
		fwrite(&nr, sizeof(int), 1, f);
		for (size_t i = 0; i < _sprites.size(); ++i) {
			fwrite(&_sprites[i], sizeof(ds::Sprite), 1, f);
		}
		fclose(f);
	}
}

void SpriteGroup::showDialog() {
	gui::start(100, &_position);
	if (gui::begin("SpriteGroup", &_state)) {
		gui::ComboBox(20, &_model, &_offset);
		gui::beginGroup();
		if (gui::Button(9, "Add")) {
			ds::Sprite sp;
			sp.id = _currentSID;
			sp.position = v2(100, 100);
			sp.texture = ds::math::buildTexture(0, 0, 50, 50);
			_sprites.push_back(sp);
			char buffer[32];
			sprintf_s(buffer, 32, "Sprite %d", _currentSID);
			_model.add(buffer, _currentSID);
			++_currentSID;
		}
		if (gui::Button(10, "Save")) {
			save();
		}
		gui::endGroup();
	}
	gui::end();

	if (gui::begin("Sprite", &_state)) {
		if (_model.hasSelection()) {
			int idx = _model.getSelection();
			ds::Sprite& sprite = _sprites[idx];
			ds::Rect r = sprite.texture.rect;
			gui::InputInt(2, "Texture_id", &sprite.texture.textureID);
			gui::InputVec2(3, "Position", &sprite.position);
			gui::InputRect(4, "TextureRect", &r);
			gui::InputVec2(5, "Scale", &sprite.scale);
			gui::InputFloat(6, "Rotation", &sprite.rotation);
			gui::InputColor(7, "Color", &sprite.color);
			gui::InputInt(8, "Type", &sprite.type);
			sprite.texture = ds::math::buildTexture(r);
			
		}
	}
	gui::end();
}