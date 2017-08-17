#include "Animation.h"
#include <fstream>

namespace animation {

	bool load_text(const char* name, KeyFrameAnimation* animation) {
		animation->clear();
		std::ifstream file(name);
		if (!file) {
			return false;
		}
		char cmd[256] = { 0 };
		for (;; ) {
			file >> cmd;
			if (!file) {
				break;
			}
			if (*cmd == '#') {

			}
			else if (strcmp(cmd, "s:") == 0) {
				float t, sx, sy;
				file >> t;
				file.ignore();
				file >> sx;
				file.ignore();
				file >> sy;
				animation->addScaling(t, ds::vec2(sx, sy));
			}
			else if (strcmp(cmd, "r:") == 0) {
				float t, r;
				file >> t;
				file.ignore();
				file >> r;
				animation->addRotation(t, r);
			}
			else if (strcmp(cmd, "t:") == 0) {
				float t, tx, ty;
				file >> t;
				file.ignore();
				file >> tx;
				file.ignore();
				file >> ty;
				animation->addTranslation(t, ds::vec2(tx, ty));
			}
		}
		return true;
	}

	bool save_text(const char* name, KeyFrameAnimation* animation) {
		std::ofstream file(name);
		if (!file) {
			return false;
		}
		file << "# scale\n";
		const AnimationTimeline& st = animation->scalingTimeline;
		for (int i = 0; i < st.num; ++i) {
			file << "s: " << st.entries[i].start;
			file << " , " << st.entries[i].value.x << " , " << st.entries[i].value.y;
			file << "\n";
		}
		file << "# rotation\n";
		const AnimationTimeline& rt = animation->rotationTimeline;
		for (int i = 0; i < rt.num; ++i) {
			file << "r: " << rt.entries[i].start;
			file << " , " << rt.entries[i].value.x;
			file << "\n";
		}
		file << "# translation\n";
		const AnimationTimeline& tt = animation->translationTimeline;
		for (int i = 0; i < tt.num; ++i) {
			file << "t: " << tt.entries[i].start;
			file << " , " << tt.entries[i].value.x << " , " << tt.entries[i].value.y;
			file << "\n";
		}
		return true;
	}
}