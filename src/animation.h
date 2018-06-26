#pragma once
#include "tweening.h"

struct PathItem {
	float time;
	float value;
};

class Animation {

public:
	Animation() : _count(0), _tweening(tweening::linear), _lastTime(1.0f) {}
	~Animation() {}

	void create(float start, float end, float ttl) {
		add(0.0f, start);
		add(ttl, end);
	}

	void add(float timeStep, float value) {
		if (_count < 32) {
			PathItem& item = _array[_count++];
			item.time = timeStep;
			item.value = value;
			if (timeStep > _lastTime) {
				_lastTime = timeStep;
			}
		}
	}

	float normalize(float time) const {
		float normTime = time / _lastTime;
		if (normTime > 1.0f) {
			normTime = 1.0f;
		}
		return normTime;
	}

	float get(float time) const {
		if (_count > 0) {
			if (_count == 1) {
				return _array[0].value;
			}
			else {
				float normTime = normalize(time);
				for (int i = 0; i < _count - 1; ++i) {
					const PathItem& current = _array[i];
					const PathItem& next = _array[i + 1];
					if (normTime >= current.time && normTime <= next.time) {
						float t = (normTime - current.time) / (next.time - current.time);
						return tweening::interpolate(_tweening, current.value, next.value, t, 1.0f);
					}
				}
			}
		}
		return 0.0f;
	}

	void reset() {
		_count = 0;
	}

	const int size() const {
		return _count;
	}

	const float key(int index) const {
		return _array[index].time;
	}

	float value(int index) const {
		return _array[index].value;
	}

	void setInterpolationMode(const tweening::TweeningType& tweening) {
		_tweening = tweening;
	}
private:
	PathItem _array[32];
	int _count;
	float _lastTime;
	tweening::TweeningType _tweening;
};