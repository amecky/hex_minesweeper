#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include "GameContext.h"

struct Number {

	explicit Number(uint8_t sz) {
		int capacity = sz * (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(float));
		data = new char[capacity];
		items = (uint8_t*)data;
		scaleFlags = (uint8_t*)(items + sz);
		scalingTimers = (float*)(scaleFlags + sz);
		size = sz;
	}

	Number(uint8_t sz, int value) {
		int capacity = sz * (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(float));
		data = new char[capacity];
		items = (uint8_t*)data;
		scaleFlags = (uint8_t*)(items + sz);
		scalingTimers = (float*)(scaleFlags + sz);
		setValue(value, false);
		size = sz;
	}

	~Number() {
		delete[] data;
	}

	uint8_t* items;
	uint8_t* scaleFlags;
	float* scalingTimers;
	char* data;
	uint8_t size;
	int value;

	void reset() {
		value = 0;
		for (int i = 0; i < size; ++i) {
			items[i] = 0;
			scaleFlags[i] = 0;
			scalingTimers[i] = 0.0f;
		}
	}
	void setValue(int v, bool flash = true) {
		int tmp = v;
		int div = 1;
		for (int i = 0; i < size; ++i) {
			if (i > 0) {
				div *= 10;
			}
		}
		bool scale = false;
		for (int i = 0; i < size; ++i) {
			int r = tmp / div;
			if (r != items[i]) {
				scale = true;
			}
			items[i] = r;
			tmp = tmp - r * div;
			div /= 10;
		}
		if (scale && flash) {
			for (int i = 0; i < size; ++i) {
				scaleFlags[i] = 1;
				scalingTimers[i] = 0.0f;
			}
		}
		value = v;
	}

};

struct Timer {

	Number minutes;
	Number seconds;
	float timer;

	Timer() : minutes(2), seconds(2) {
		minutes.reset();
		seconds.reset();
		timer = 0.0f;
	}

	void reset() {
		minutes.setValue(0, false);
		seconds.setValue(0, false);
		timer = 0.0f;
	}

	void tick(float dt) {
		timer += dt;
		if (timer > 1.0f) {
			timer -= 1.0f;
			++seconds.value;
			if (seconds.value >= 60) {
				seconds.value = 0;
				++minutes.value;
				if (minutes.value > 99) {
					minutes.value = 0;
				}
				minutes.setValue(minutes.value, false);
			}
			seconds.setValue(seconds.value, false);
		}
	}
};

struct Score {	
	int seconds;
	int minutes;
	int bombsLeft;
	bool success;
};

class HUD {

public:
	HUD(SpriteBatchBuffer* buffer, Score* score, GameSettings* settings);
	~HUD();
	void render();
	void tick(float dt);
	void reset();
	int getMinutes() const {
		return _timer.minutes.value;
	}
	int getSeconds() const {
		return _timer.seconds.value;
	}
	void setBombs(int pc);
private:
	void scaleNumber(Number& nr, float dt);
	void renderNumber(const Number& nr, const ds::vec2& startPos);
	GameSettings* _settings;
	Number _bombs;
	Timer _timer;
	SpriteBatchBuffer* _buffer;
	Score* _score;
};

