#include "Waves.h"
#include <io\json.h>

Waves::Waves() {
}


Waves::~Waves() {
}

void Waves::load() {
	ds::JSONReader reader;
	if (reader.parse("content\\waves.json")) {
		int children[256];
		int num = reader.get_categories(children, 256);
		for (int i = 0; i < num; ++i) {
			WaveDescriptor desc;
			reader.get(children[i], "type", &desc.type);
			reader.get(children[i], "enemy", &desc.enemy);
			reader.get(children[i], "movement", &desc.movement);
			reader.get(children[i], "shoot_delay", &desc.shootDelay);
			reader.get(children[i], "animation", &desc.animation);
			_waves.push_back(desc);
		}
	}
}