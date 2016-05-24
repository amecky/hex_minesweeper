#pragma once
#include <stdint.h>
#include <lib\collection_types.h>

struct WaveDescriptor {
	uint16_t type;
	uint16_t enemy;
	uint16_t movement;
	float shootDelay;
	uint16_t animation;
};

class Waves {

public:
	Waves();
	~Waves();
	void load();
	const WaveDescriptor& getDescriptor(int index) const {
		return _waves[index];
	}
	int size() const {
		return _waves.size();
	}
private:
	ds::Array<WaveDescriptor> _waves;
};

