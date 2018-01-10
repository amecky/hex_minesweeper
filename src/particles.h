#pragma once
#include <diesel.h>

struct StarParticle {
	ds::vec2 pos;
	ds::vec2 vel;
	ds::Color color;
	float timer;
	bool dying;
};

struct StarParticleSettings {
	float velocity;
	float velocityVariance;
	float acceleration;
	float ttl;
	float radius;
	float radiusVariance;
	int num;
};

class SpriteBatchBuffer;

class Particles {

public:
	Particles(StarParticleSettings* settings);
	~Particles();
	void emittStars(const ds::vec2& pos);
	void tick(float dt);
	void render(SpriteBatchBuffer* buffer);
	void clear();
private:
	StarParticleSettings* _settings;
	StarParticle _stars[1024];
	int _numStars;
};