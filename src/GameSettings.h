#pragma once

struct GameSettings {//: public ds::DynamicGameSettings {

	float rebuildTime;
	float flashTTL;
	float droppingTTL;
	float grayScaleFactor;
	float gameOverTTL;

	GameSettings() {
		/*
		addFloat("flash_ttl", &flashTTL , 0.6f);
		addFloat("rebuild_time", &rebuildTime, 1.0f);
		addFloat("dropping_ttl", &droppingTTL, 0.3f);
		addFloat("gray_scale_factor", &grayScaleFactor, 2.0f);
		addFloat("game_over_ttl", &gameOverTTL, 2.0f);
		*/
		flashTTL = 1.6f;
		rebuildTime = 1.0f;
		droppingTTL = 1.3f;
		grayScaleFactor = 2.0f;
		gameOverTTL = 2.0f;
	}

	const char* getFileName() const {
		return "settings.json";
	}

};
