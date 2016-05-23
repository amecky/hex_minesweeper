#include "TestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>
#include <utils\ObjLoader.h>

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	_sprites = ds::res::getSpriteBuffer("BasicSpriteBuffer");
	_scene = ds::res::getScene("EnemiesScene");
	_camera = new ds::FPSCamera(1024, 768);
	_camera->setPosition(v3(0, 0, -26), v3(0, 0, 1));
	_camera->resetPitch(0.0f);
	_camera->resetYAngle();
	graphics::setCamera(_camera);
	_orthoCamera = new ds::OrthoCamera(1024, 768);
	
	_texturedBuffer = ds::res::getMeshBuffer(21);
	_colouredBuffer = ds::res::getMeshBuffer(26);

	_enemies.push_back(new Enemies(_scene, "RingMesh"));
	_enemies.push_back(new Enemies(_scene, "CubeMesh"));
	_movements.push_back(new FirstMovement(_scene));
	_movements.push_back(new SecondMovement(_scene));
	_activeEnemies = 0;
	_activeMovement = 0;
	_firing = false;
	_fireTimer = 0.0f;
	_pressed = false;
	
	_animation = rotate_enemy;
}


TestState::~TestState() {
	for (size_t i = 0; i < _enemies.size(); ++i) {
		delete _enemies[i];
	}
	for (size_t i = 0; i < _movements.size(); ++i) {
		delete _movements[i];
	}
	delete _orthoCamera;
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
	_camera->update(dt, mp);
	_timer += dt;
	if (_activeEnemies != -1) {
		_enemies[_activeEnemies]->update(dt);
	}
	/*
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
	bool move = false;
	if (ds::input::getKeyState('D')) {
		//_playerAngle += 0.75 * PI * dt;
		//_player->rotateZ(_playerAngle);
		_player->rotateY(DEGTORAD(20.0f));
		_playerPos.x += 4.0f * dt;
		move = true;
	}
	if (ds::input::getKeyState('A')) {
		//_playerAngle -= 0.75 * PI * dt;
		_player->rotateY(DEGTORAD(-20.0f));
		_playerPos.x -= 4.0f * dt;
		move = true;
	}
	if (!move) {
		_player->rotateY(0.0f);
	}
	_player->translate(_playerPos);
	*/
	/*
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
	*/
	/*
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
		else if (p->z > 7.0f) {
			it = _bulletList.erase(it);
		}
		else {
			++it;
		}
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

	checkCollisions();
	*/
	/*
	if (ds::input::isMouseButtonPressed(0) && !_pressed) {
		_pressed = true;
		v2 mp = ds::input::getMousePosition();
		ds::mat4 projection = _camera->getProjectionMatrix();
		float px = (((2.0f * mp.x) / 1024.0f) - 1.0f) / projection._11;

		float py = (((2.0f * mp.y) / 768.0f) - 1.0f) / projection._22;

		v3 origin = v3(0.0f, 0.0f, 0.0f);
		v3 direction = v3(px, py, 1.0f);

		ds::mat4 view = _camera->getViewMatrix();
		view = ds::matrix::mat4Inverse(view);

		origin = ds::matrix::transformCoordinate(origin,view);
		direction = ds::matrix::transformNormal(direction, view);
		direction = normalize(direction);
		//LOG << "direction: " << DBG_V3(direction) << " origin: " << DBG_V3(origin);
		Cubes::iterator cit = _cubeList.begin();
		while (cit != _cubeList.end()) {
			Cube* c = &(*cit);
			v3 v = origin - c->position;

			float b = 2.0f * dot(direction, v);
			float cc = dot(v, v) - 0.5f * 0.5f;

			float discriminant = (b * b) - (4.0f * cc);

			if (discriminant >= 0.0f) {
				discriminant = std::sqrt(discriminant);
				float s0 = (-b + discriminant) / 2.0f;
				float s1 = (-b - discriminant) / 2.0f;
				if (s0 >= 0.0f || s1 >= 0.0f) {
					//LOG << "HIT!!!!! AT " << DBG_V3(c->position);
					if (c->scale.x == 1.0f) {
						c->scale = v3(0.5f, 0.5f, 0.5f);
						c->rotation = v3(0.0f, 0.0f, DEGTORAD(45.0f));
					}
					else {
						c->scale = v3(1, 1, 1);
						c->rotation = v3(0, 0, 0);
					}
				}
			}
			++cit;
		}
	}
	*/
	if (!ds::input::isMouseButtonPressed(0) && _pressed) {
		_pressed = false;
	}
	return 0;
}

// -------------------------------------------------------
// simple collision check
// -------------------------------------------------------
void TestState::checkCollisions() {
	/*
	Bullets::iterator it = _bulletList.begin();
	while (it != _bulletList.end()) {
		Bullet* b = &(*it);
		bool hit = false;
		Cubes::iterator cit = _cubeList.begin();
		while (cit != _cubeList.end()) {
			Cube* c = &(*cit);
			v3 rel = b->position - c->position;
			float distSqr = rel.x * rel.x + rel.y * rel.y + rel.z * rel.z;
			// bullet : 0.3 cube : 0.5
			float minDist = 0.3f + 0.5f;
			if (distSqr <= minDist * minDist) {
				hit = true;
				cit = _cubeList.erase(cit);
			}
			else {
				++cit;				
			}
		}
		if (hit) {
			it = _bulletList.erase(it);
		}
		else {
			++it;
		}
	}
	*/
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void TestState::render() {
	// scene
	graphics::setCamera(_camera);
	graphics::turnOnZBuffer();
	if (_activeEnemies != -1) {
		_scene->draw();
	}
	graphics::setCamera(_orthoCamera);
	graphics::turnOffZBuffer();
	// GUI
	drawGUI();
	//_sprites->begin();
	//_sprites->draw(v2(512, 600), math::buildTexture(510, 0, 256, 256), DEGTORAD(0.0f), v2(1.0f, 1.0f), ds::Color(255, 255, 255, 255));
	//_sprites->end();
		
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
	gui::InputInt("Enemy", &_activeEnemies);
	gui::InputInt("Movement", &_activeMovement);
	if (gui::Button("Start")) {
		if (_activeEnemies != -1 && _activeMovement != -1) {
			_enemies[_activeEnemies]->start(_animation, _movements[_activeMovement]);
		}
	}
	if (gui::Button("Toggle")) {
		if (_activeEnemies != -1) {
			_enemies[_activeEnemies]->toggle();
		}
	}
	if (gui::Button("Rotate E")) {
		_animation = rotate_enemy;
	}
	if (gui::Button("Scale E")) {
		_animation = scale_enemy;
	}
	/*
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
	*/
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
	return 0;
}
