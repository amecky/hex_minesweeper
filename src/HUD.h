#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>


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

	void setValue(int v, bool flash = true) {
		int tmp = v;
		int div = 1;
		for (int i = 0; i < size; ++i) {
			if (i > 0) {
				div *= 10;
			}
		}
		for (int i = 0; i < size; ++i) {
			int r = tmp / div;
			if (r != items[i] && flash) {
				scaleFlags[i] = 1;
				scalingTimers[i] = 0.0f;
			}
			else {
				scaleFlags[i] = 0;
			}
			items[i] = r;
			tmp = tmp - r * div;
			div /= 10;
		}
		value = v;
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
	HUD(SpriteBatchBuffer* buffer, Score* score);
	~HUD();
	void render();
	void tick(float dt);
	void reset();
	//void rebuildScore(bool flash = true);
	int getMinutes() const {
		return _minutes.value;
	}
	int getSeconds() const {
		return _seconds.value;
	}
	void setBombs(int pc);
private:
	void scaleNumber(Number& nr, float dt);
	void renderNumber(const Number& nr, const ds::vec2& startPos);
	float _timer;
	Number _bombs;
	Number _minutes;
	Number _seconds;
	SpriteBatchBuffer* _buffer;
	Score* _score;
};

