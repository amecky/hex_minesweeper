#include "Highscores.h"
#include <stdio.h>
#include <assert.h>

namespace scoring {

	void load(Highscore* highscore) {
		FILE* f = fopen("score.dat", "rb");
		if (f) {
			fread(highscore, sizeof(Highscore), 1, f);
			fclose(f);
		}
	}

	void save(const Highscore& highscore) {
		FILE* f = fopen("score.dat", "wb");
		if (f) {
			fwrite(&highscore, sizeof(Highscore), 1, f);
			fclose(f);
		}
	}

	bool hasScored(int seconds, int minutes, int mode, Highscore& score) {
		assert(mode >= 0 && mode < 3);
		PlayedTime time = score.times[mode];
		int total = time.seconds + time.minutes * 60;
		int compare = seconds + minutes * 60;
		if (compare < total) {
			score.times[mode].seconds = seconds;
			score.times[mode].minutes = minutes;
			save(score);
			return true;
		}
		return false;
	}

}


