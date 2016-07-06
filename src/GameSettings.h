#pragma once
#include <data\DynamicSettings.h>

struct GameSettings : public ds::DynamicGameSettings {

	float rotateTTL;

	GameSettings() {
		add("rotate_ttl", &rotateTTL, 0.5f);
	}

	const char* getFileName() const {
		return "settings.json";
	}

};
