#pragma once

struct PlayedTime {

	int minutes;
	int seconds;
	
	PlayedTime() : seconds(99), minutes(99) {}

};

struct Highscore {

	PlayedTime times[3];
};

namespace scoring {

	void load(Highscore* highscore);

	void save(const Highscore& highscore);

	bool hasScored(int seconds, int minutes, int mode, Highscore& score);

}

