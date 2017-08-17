#pragma once
#include <diesel.h>
#include "tweening.h"
#include <string.h>

struct AnimationTimelineEntry {

	float start;
	tweening::TweeningType tweening;
	ds::vec2 value;

	AnimationTimelineEntry() {}

	AnimationTimelineEntry(float s, const float v) : start(s), tweening(tweening::linear), value(v) {}

	AnimationTimelineEntry(float s, const ds::vec2& v) : start(s), tweening(tweening::linear), value(v) {}

	AnimationTimelineEntry(float s, tweening::TweeningType t, const ds::vec2& v) : start(s), tweening(t), value(v) {}
};

struct AnimationTimeline {

	AnimationTimelineEntry* entries;
	uint16_t num;
	uint16_t capacity;

	AnimationTimeline() : entries(0), num(0), capacity(0) {}

	~AnimationTimeline() {
		if (entries != 0) {
			delete[] entries;
		}
	}

	float getStepAt(uint16_t idx) const {
		return entries[idx].start;
	}

	void add(float start, float value, tweening::TweeningType type = tweening::linear) {
		add(start, ds::vec2(value), type);
	}

	void add(float start, const ds::vec2& value, tweening::TweeningType type = tweening::linear) {
		if (start >= 0.0f && start <= 1.0f) {
			if ((num + 1) > capacity) {
				alloc(capacity * 2 + 8);
			}
			int idx = -1;
			for (uint16_t i = 0; i < num; ++i) {
				if (entries[i].start > start && idx == -1) {
					idx = i;
				}
			}
			if (idx == -1) {
				entries[num++] = { start, type, value };
			}
			else {
				for (int i = num - 1; i >= idx; --i) {
					entries[i + 1] = entries[i];
					entries[idx] = { start, type, value };
				}
				++num;
			}
		}
	}

	bool get(float normTime, float* ret) const {
		ds::vec2 r(0.0f);
		if (get(normTime, &r)) {
			*ret = r.x;
			return true;
		}
		return false;
	}

	bool get(float normTime, ds::vec2* ret) const {
		if (ret != 0) {
			if (num == 0) {
				return false;
			}
			if (normTime <= 0.0f) {
				*ret = entries[0].value;
				return true;
			}
			if (normTime >= 1.0f) {
				*ret = entries[num - 1].value;
				return true;
			}
			for (uint16_t i = 0; i < num - 1; ++i) {
				AnimationTimelineEntry& current = entries[i];
				AnimationTimelineEntry& next = entries[i + 1];
				if (normTime >= current.start && normTime <= next.start) {
					float nt = (normTime - current.start) / (next.start - current.start);
					*ret += tweening::interpolate(current.tweening, current.value, next.value, nt, 1.0f);
					return true;
				}
			}
		}
		return false;
	}

	void alloc(int newCapacity) {
		AnimationTimelineEntry* tmp = new AnimationTimelineEntry[newCapacity];
		if (entries != 0) {
			memcpy(tmp, entries, num * sizeof(AnimationTimelineEntry));
			delete[] entries;
		}
		capacity = newCapacity;
		entries = tmp;
	}
};

struct KeyFrameAnimation {

	AnimationTimeline scalingTimeline;
	AnimationTimeline rotationTimeline;
	AnimationTimeline translationTimeline;

	KeyFrameAnimation() {}

	void addScaling(float start, const ds::vec2& scale, tweening::TweeningType tweeningType = tweening::linear) {
		scalingTimeline.add(start, scale, tweeningType);
	}

	void addRotation(float start, float angle, tweening::TweeningType tweeningType = tweening::linear) {
		rotationTimeline.add(start, angle, tweeningType);
	}

	void addTranslation(float start, const ds::vec2& position, tweening::TweeningType tweeningType = tweening::linear) {
		translationTimeline.add(start, position, tweeningType);
	}

	void get(float t, ds::vec2* scale, float* rotation, ds::vec2* translation) const {
		if (scale != 0) {
			scalingTimeline.get(t, scale);
		}
		if (rotation != 0) {
			rotationTimeline.get(t, rotation);
		}
		if (translation != 0) {
			translationTimeline.get(t, translation);
		}
	}
};
