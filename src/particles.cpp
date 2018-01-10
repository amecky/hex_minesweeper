#include "particles.h"
#include <SpriteBatchBuffer.h>

Particles::Particles(StarParticleSettings* settings) : _settings(settings) {
	_numStars = 0;
}

Particles::~Particles() {

}

// ---------------------------------------------------------------
// clear - reset number of particles
// ---------------------------------------------------------------
void Particles::clear() {
	_numStars = 0;
}

// ---------------------------------------------------------------
// emitt particles
// ---------------------------------------------------------------
void Particles::emittStars(const ds::vec2& pos) {
	float rnd_max = 0.125f * ds::PI;
	for (int i = 0; i < _settings->num; ++i) {
		if (_numStars + 1 < 1024) {
			StarParticle& particle = _stars[_numStars++];
			particle.pos = pos;
			float radius = ds::random(_settings->radius - _settings->radiusVariance, _settings->radius + _settings->radiusVariance);
			float angle = static_cast<float>(i) / static_cast<float>(_settings->num) * (ds::TWO_PI + ds::random(0.0f, rnd_max));
			float vel = ds::random(_settings->velocity - _settings->velocityVariance, _settings->velocity + _settings->velocityVariance);
			float ca = cosf(angle);
			float sa = sinf(angle);
			particle.pos.x += ca * radius;
			particle.pos.y += sa * radius;
			particle.vel.x = ca * vel;
			particle.vel.y = sa * vel;
			particle.timer = 0.0f;
			particle.color = ds::Color(255, 255, 255, 255);
			particle.dying = false;
		}
	}
}

// ---------------------------------------------------------------
// tick
// ---------------------------------------------------------------
void Particles::tick(float dt) {
	int cnt = 0;
	while (cnt < _numStars) {
		StarParticle& p = _stars[cnt];
		if (p.dying) {
			p.timer += dt;
			if (p.timer >= _settings->ttl) {
				if (_numStars > 0) {
					_stars[cnt] = _stars[_numStars - 1];
				}
				--_numStars;
			}
		}
		p.vel += ds::vec2(0.0f, -_settings->acceleration) * dt;
		p.pos += p.vel * dt;
		if (p.pos.y < 100.0f) {
			p.pos.y = 102.0f;
			p.vel.y *= -1.0f;
			if (!p.dying) {
				p.dying = true;
			}
		}
		if (p.dying) {
			p.color.a = 1.0f - p.timer / _settings->ttl;
		}
		++cnt;
	}
}

// ---------------------------------------------------------------
// render
// ---------------------------------------------------------------
void Particles::render(SpriteBatchBuffer* buffer) {
	for (int i = 0; i < _numStars; ++i) {
		const StarParticle& p = _stars[i];
		buffer->add(p.pos, ds::vec4(200, 0, 6, 6), ds::vec2(1.0f), 0.0f, p.color);
	}
}