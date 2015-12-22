#pragma once



struct BaseScore {

	BaseScore() {}
	virtual int compare(const BaseScore& other) = 0;

};

template<class T,int SIZE = 10>
class HighscoreService {

public:
	HighscoreService() : _count(0) {
		_scores = new T[SIZE];
	}
	~HighscoreService() {
		delete[] _scores;
	}
	int add(const T& score) {
		int idx = -1;
		for (int i = 0; i < _count; ++i) {
			if (idx == -1) {
				int state = score.compare(_scores[i]);
				if (state >= 0) {
					idx = i;
				}
			}
		}
		if (idx != -1) {
			// FIXME: move the rest on further down
			_scores[idx] = score;
			++_count;
		}
		return idx;
	}
	bool load() {
		FILE* f = fopen("score.dat", "rb");
		if (f) {
			int count = 0;
			fread(&count, sizeof(int), 1, f);
			for (int i = 0; i < count; ++i) {
				fread(&_scores[i], sizeof(T), 1, f);
			}
			_count = count;
			fclose(f);
			return true;
		}
		return false;
	}
	bool save() {
		FILE* f = fopen("score.dat", "wb");
		if (f) {
			fwrite(&_count, sizeof(int), 1, f);
			for (int i = 0; i < _count; ++i) {
				fwrite(&_scores[i], sizeof(T), 1, f);
			}
			fclose(f);
			return true;
		}
		return false;
	}
	int get(T* list, int max) {
		int cnt = max;
		if (cnt > _count) {
			cnt = _count;
		}
		for (int i = 0; i < cnt; ++i) {
			list[i] = _scores[i];
		}
		return cnt;
	}
private:
	int _count;
	T* _scores;
};

struct PlayedTime {

	int minutes;
	int seconds;
	int mode;

	PlayedTime() : seconds(99), minutes(99) , mode(-1) {}

	int compare(const PlayedTime& other) const {
		if (mode != other.mode) {
			return -1;
		}
		int time = minutes * 60 + seconds;
		int other_time = other.minutes * 60 + other.seconds;
		if (time < other_time) {
			return -1;
		}
		if (time > other_time) {
			return 1;
		}
		return 0;
	}

};

struct Highscore {

	PlayedTime times[3];
};

namespace scoring {

	void load(Highscore* highscore);

	void save(const Highscore& highscore);

	bool hasScored(int seconds, int minutes, int mode, Highscore& score);

}