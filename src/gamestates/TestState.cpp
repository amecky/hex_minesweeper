#include "TestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>
#include <utils\ObjLoader.h>

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	//_particles = ds::res::getParticleManager();
	_camera = new ds::FPSCamera(1024, 768);
	_camera->setPosition(v3(0, 8, -10), v3(0, 0, 1));
	_camera->resetPitch(DEGTORAD(45.0f));
	_camera->resetYAngle();
	graphics::setCamera(_camera);
	_orthoCamera = new ds::OrthoCamera(1024, 768);

	ds::Rect r(510, 340, 64, 64);
	for (int i = 0; i < 6; ++i) {
		_cubeTextures[i] = r;
		r.left += 64;
		r.right += 64;
	}
	
	_cubes = ds::res::getMesh(21);	
	ds::geometrics::createGrid(_cubes, 2.0f, 10, 10, ds::Rect(510, 0, 256, 256) , v3(-10, 0, -10));
	//ds::geometrics::createCube(_cubes, _cubeTextures, v3(0, 2, 0), v3(1, 1, 1));// , v3(0.0f, 0.0f, DEGTORAD(45.0f)));

	_player = ds::res::getMesh(26);
	ds::ObjLoader loader;
	loader.parse2("content\\objects\\player.obj",_player);
	_playerPos = v3(0.0f, 0.2f, 0.0f);
	_player->translate(_playerPos);
	_player->rotateX(DEGTORAD(-90.0f));
	_playerAngle = 0.0f;
	_timer = 0.0f;
	for (int i = 0; i < 10; ++i) {
		_states[i] = 0;
	}

	_bullets = ds::res::getMesh(27);
	_firing = false;
	_fireTimer = 0.0f;
}


TestState::~TestState() {
	delete _camera;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void TestState::init() {
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
int TestState::update(float dt) {
	//_particles->update(dt);
	v2 mp = ds::input::getMousePosition();
	//_camera->update(dt, mp);
	_timer += dt;
	if (_states[1] == 1) {
		_cubes->rotateX(_timer);
	}
	if (_states[2] == 1) {
		_cubes->rotateY(_timer);
	}
	if (_states[3] == 1) {
		_cubes->rotateZ(_timer);
	}
	if (_states[0] == 1) {
		v3 p;
		p.x = sin(_timer) * 2.0f;
		_cubes->translate(p);
	}
	if (_states[4] == 1) {
		float s = 0.8f + sin(_timer) * 0.2f;
		_cubes->scale(v3(s, s, s));
	}
	if (ds::input::getKeyState('D')) {
		_playerAngle += 0.75 * PI * dt;
		_player->rotateZ(_playerAngle);
	}
	if (ds::input::getKeyState('A')) {
		_playerAngle -= 0.75 * PI * dt;
		_player->rotateZ(_playerAngle);
	}
	if (ds::input::getKeyState('W')) {
		ds::mat3 R = ds::matrix::mat3RotationY(_playerAngle);
		v3 target = R * v3(0, 0, 1);
		_playerPos += target * 6.0f * dt;
		_player->translate(_playerPos);
	}
	if (ds::input::getKeyState('S')) {
		ds::mat3 R = ds::matrix::mat3RotationY(_playerAngle);
		v3 target = R * v3(0, 0, 1);
		_playerPos -= target * 6.0f * dt;
		_player->translate(_playerPos);
	}
	float cx = _playerPos.x + sin(_playerAngle + DEGTORAD(180.0f)) * 4.0f;
	float cz = _playerPos.z + cos(_playerAngle + DEGTORAD(180.0f)) * 4.0f;
	_camera->setPosition(v3(cx, 8.0f, cz), _playerPos);
	_camera->resetYaw(_playerAngle);


	Bullets::iterator it = _bulletList.begin();
	while (it != _bulletList.end()) {
		v3* p = &it->position;
		*p += it->velocity * dt;
		if (p->x > 10.0f) {
			it = _bulletList.erase(it);
		}
		else if (p->x < -10.0f) {
			it = _bulletList.erase(it);
		}
		else if (p->z < -10.0f) {
			it = _bulletList.erase(it);
		}
		else if (p->z > 10.0f) {
			it = _bulletList.erase(it);
		}
		else {
			++it;
		}
	}

	Cubes::iterator cit = _cubeList.begin();
	while (cit != _cubeList.end()) {
		Cube* c = &(*cit);
		if (c->state == 0) {
			v3* p = &cit->position;
			v3* v = &cit->velocity;
			*p += *v * dt;
			if (p->x > 10.0f) {
				v->x *= -1.0f;
			}
			else if (p->x < -10.0f) {
				v->x *= -1.0f;
			}
			else if (p->z < -10.0f) {
				v->z *= -1.0f;
			}
			else if (p->z > 10.0f) {
				v->z *= -1.0f;
			}
			c->angle = math::getAngle(v2(1, 0), v2(v->x, v->z));
		}
		else if (c->state == 1) {
			c->timer -= dt * 4.0f;
			c->position.x = -10.0f + cos(c->timer) * 1.3f;
			c->position.y = -0.3f + sin(c->timer) * 1.3f;
			c->roll = c->timer - DEGTORAD(60.0f);
			if (c->timer <= DEGTORAD(60.0f)) {
				c->state = 0;
				c->roll = 0.0f;
				float angle = DEGTORAD(315.0f) + math::random(0.0f, HALF_PI);
				v2 v = math::getRadialVelocity(angle, 4.0f);
				c->velocity.x = v.x;
				c->velocity.y = 0.0f;
				c->velocity.z = v.y;
				c->angle = angle;
			}
		}
		++cit;
	}

	if (ds::input::isMouseButtonPressed(0)) {
		_firing = true;
	}
	else {
		_firing = false;
	}
	if (_firing) {
		_fireTimer += dt;
		if (_fireTimer > 0.2f) {
			_fireTimer -= 0.2f;
			addBullet();
		}
	}

	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void TestState::render() {
	// scene
	graphics::setCamera(_camera);
	graphics::turnOnZBuffer();
	_cubes->draw();

	_bullets->reset();
	for (size_t i = 0; i < _bulletList.size(); ++i) {
		ds::geometrics::createPlane(_bullets,_bulletList[i].position,ds::Rect(0,210,10,20),v2(0.05f,0.3f),_bulletList[i].angle, ds::Color(192,0,0,255));
	}
	for (size_t i = 0; i < _cubeList.size(); ++i) {
		ds::geometrics::createCube(_bullets, _cubeTextures, _cubeList[i].position, v3(0.5f,0.5f,0.5f), v3(0.0f, _cubeList[i].angle, _cubeList[i].roll));
	}
	_bullets->draw();

	_player->draw();
	// GUI
	//drawGUI();	
}

void TestState::addCube() {
	Cube c;
	c.position = v3(-10.0f, -0.3f, 0.0f);
	//float angle = math::random(0.0f, TWO_PI);
	//v2 v = math::getRadialVelocity(angle, 4.0f);
	//c.velocity.x = v.y;
	//c.velocity.y = 0.0f;
	//c.velocity.z = v.x;
	//c.angle = angle;
	c.velocity = v3(0, 0, 0);
	c.angle = 0.0f;
	c.timer = DEGTORAD(225.0f);
	c.state = 1;
	_cubeList.push_back(c);
}

void TestState::addBullet() {
	Bullet b;
	b.position = _playerPos;
	b.position.y = 0.2f;
	b.velocity = v3(0, 0, 0);
	v2 v = math::getRadialVelocity(_playerAngle, 10.0f);
	b.velocity.x = v.y;
	b.velocity.z = v.x;
	b.angle = _playerAngle;
	_bulletList.push_back(b);
}

// -------------------------------------------------------
// draw GUI
// -------------------------------------------------------
void TestState::drawGUI() {
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	v2 pos(10, 760);
	int state = 1;
	gui::start(1, &pos, true);
	gui::begin("Camera", &state);
	char buffer[128];
	sprintf_s(buffer, 128, "%2.2f %2.2f %2.2f", _camera->getPosition().x, _camera->getPosition().y, _camera->getPosition().z);
	gui::Value("Pos", buffer);
	gui::Value("Yaw", RADTODEG(_camera->getAngle()));
	gui::Value("Pitch", RADTODEG(_camera->getPitch()));
	if (gui::Button("Reset")) {
		_camera->setPosition(v3(0, 2, -12), v3(0, 0, 1));
		_camera->resetYAngle();
	}
	v3* lp = _cubes->getLightPos();
	gui::InputVec3("LightPos", lp);
	if (gui::Button("Move")) {
		if (_states[0] == 0) {
			_states[0] = 1;
		}
		else {
			_states[0] = 0;
			_cubes->translate(v3(0, 0, 0));
		}
	}
	if (gui::Button("RotateX")) {
		if (_states[1] == 0) {
			_states[1] = 1;
		}
		else {
			_states[1] = 0;
			_cubes->rotateX(0.0f);
		}
	}
	if (gui::Button("RotateY")) {
		if (_states[2] == 0) {
			_states[2] = 1;
		}
		else {
			_states[2] = 0;
			_cubes->rotateY(0.0f);
		}
	}
	if (gui::Button("RotateZ")) {
		if (_states[3] == 0) {
			_states[3] = 1;
		}
		else {
			_states[3] = 0;
			_cubes->rotateZ(0.0f);
		}
	}
	if (gui::Button("Scale")) {
		if (_states[4] == 0) {
			_states[4] = 1;
		}
		else {
			_states[4] = 0;
			_cubes->scale(v3(1, 1, 1));
		}
	}
	gui::end();
}

int TestState::onKeyDown(WPARAM virtualKey) {
	return 0;
}

int TestState::onKeyUp(WPARAM virtualKey) {
	return 0;
}

int TestState::onChar(int ascii) {
	if (ascii == 'f') {
		addBullet();
	}
	if (ascii == 'g') {
		addCube();
	}
	return 0;
}
