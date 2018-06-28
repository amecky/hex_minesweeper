#pragma once
#include "tweening.h"

struct Animation {

	float steps[32];
	float values[32];
	int count;
	float lastTime;
	tweening::TweeningType tweening;

	Animation() : count(0), lastTime(0.0f), tweening(tweening::linear) {}

};

namespace anim {

	void build_simple_animation(Animation* anim, float start, float end, float ttl);

	void add_step(Animation* anim, float time, float value);

	float interpolate(Animation* anim, float time);
}

#ifdef DS_ANIMATION_IMPLEMENTATION

namespace anim {

	void build_simple_animation(Animation* anim, float start, float end, float ttl) {
		anim->count = 0;
		add_step(anim, 0.0f, start);
		add_step(anim, ttl, end);
	}

	void add_step(Animation* anim, float time, float value) {
		if ((anim->count + 1) < 32) {
			anim->steps[anim->count] = time;
			anim->values[anim->count] = value;
			++anim->count;
			if (time > anim->lastTime) {
				anim->lastTime = time;
			}
		}
	}

	static float anim__normalize(Animation* anim, float time) {
		float normTime = time / anim->lastTime;
		if (normTime > 1.0f) {
			normTime = 1.0f;
		}
		return normTime;
	}

	float interpolate(Animation* anim, float time) {
		if (anim->count > 0) {
			if (anim->count == 1) {
				return anim->values[0];
			}
			else if (anim->count == 2) {
				float normTime = anim__normalize(anim, time);
				return tweening::interpolate(anim->tweening, anim->values[0], anim->values[1], normTime, 1.0f);
			}
			else {
				for (int i = 0; i < anim->count - 1; ++i) {
					float currentTime = anim->steps[i];
					float nextTime = anim->steps[i + 1];
					if (time >= currentTime && time <= nextTime) {
						float t = (time - currentTime) / (nextTime - currentTime);
						return tweening::interpolate(anim->tweening, anim->values[i], anim->values[i + 1], t, anim->lastTime);
					}
				}
				return anim->values[anim->count -1];
			}
		}
		return 0.0f;
	}

}
#endif
