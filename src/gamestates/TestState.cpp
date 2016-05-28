#include "TestState.h"
#include <utils\Log.h>
#include <base\EventStream.h>
#include <base\InputSystem.h>
#include <utils\geometrics.h>
#include <utils\ObjLoader.h>
#include <io\json.h>
#include "..\objects.h"

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	_sprites = ds::res::getSpriteBuffer("BasicSpriteBuffer");
	_scene = ds::res::getScene("EnemiesScene");
	_camera = (ds::FPSCamera*)ds::res::getCamera("fps");
	graphics::setCamera(_camera);
	_orthoCamera = (ds::OrthoCamera*)ds::res::getCamera("ortho");
	
	_objects = ds::res::getScene("Objects");
	ds::Mesh* m = ds::res::getMesh("PlayerMesh");
	//_player = _objects->add(m, v3(0, 0, 0), ds::DrawMode::IMMEDIATE);

	_bulletMesh = new ds::Mesh();
	ds::geometrics::createXYPlane(_bulletMesh, v3(0, 0, 0), ds::Rect(0, 0, 128, 128), v2(0.2f, 0.2f));

	_waves.load();
	_wavesIndex = 0;

	readPathInformations();

	_cubeEnemy = new ds::Mesh();
	objects::createCubeEnemy(_cubeEnemy);

	_enemies.push_back(new Enemies(_scene, "RingMesh"));
	_enemies.push_back(new Enemies(_scene, _cubeEnemy));
	_enemies.push_back(new Enemies(_scene, "IcoSphereMesh"));

	_movements.push_back(new PathMovement(_scene,_paths[0]));
	_movements.push_back(new PathMovement(_scene, _paths[1]));
	_movements.push_back(new PathMovement(_scene, _paths[2]));
	_movements.push_back(new PathMovement(_scene, _paths[3]));
	_movements.push_back(new PathMovement(_scene, _paths[4]));

	_animations.push_back(new RotateAnimation(_scene));
	_animations.push_back(new ScaleAnimation(_scene));
	_activeEnemies = 0;
	_activeMovement = 0;
	_activeAnimation = 0;
	_firing = false;
	_fireTimer = 0.0f;
	_pressed = false;
	
}


TestState::~TestState() {
	for (size_t i = 0; i < _enemies.size(); ++i) {
		delete _enemies[i];
	}
	for (size_t i = 0; i < _movements.size(); ++i) {
		delete _movements[i];
	}
	for (size_t i = 0; i < _paths.size(); ++i) {
		delete _paths[i];
	}
	//delete _orthoCamera;
	//delete _camera;
	delete _cubeEnemy;
	delete _bulletMesh;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void TestState::init() {
}

void TestState::startWave() {
	if (_wavesIndex < _waves.size()) {
		const WaveDescriptor& desc = _waves.getDescriptor(_wavesIndex++);
		if (desc.type == 0) {
			_activeEnemies = desc.enemy;
			_activeAnimation = desc.animation;
			_enemies[_activeEnemies]->start(_animations[_activeAnimation], _movements[desc.movement]);
			_currentShootDelay = desc.shootDelay;
			_enemyShootTimer = 0.0f;
		}
		else {
			// start upgrades
		}
	}
	else {
		// GAME OVER - player has won!!!
		_activeEnemies = -1;
		_wavesIndex = 0;
		_currentShootDelay = -1.0f;
	}
}

// -------------------------------------------------------
// move enemies
// -------------------------------------------------------
void TestState::moveEnemies(float dt) {
	if (_activeEnemies != -1 && _enemies[_activeEnemies]->isActive()) {
		if (!_enemies[_activeEnemies]->update(dt)) {
			_enemies[_activeEnemies]->setActive(false);
			startWave();
		}
		if (_currentShootDelay > 0.0f) {
			_enemyShootTimer += dt;
			if (_enemyShootTimer >= _currentShootDelay) {
				_enemyShootTimer -= _currentShootDelay;
				// pick random pos from enemies
				v3 bp;
				if (_enemies[_activeEnemies]->pickRandomPos(&bp)) {
					// start bullet
					ID id = _objects->add(_bulletMesh, bp);
					ds::Entity& bullet = _objects->get(id);
					bullet.type = 4;
				}
			}
		}
	}
}
// -------------------------------------------------------
// update
// -------------------------------------------------------
int TestState::update(float dt) {
	//_particles->update(dt);
	_camera->update(dt);
	_timer += dt;

	_stars.move(dt);
	_upgrades.tick(dt);
	moveEnemies(dt);

	ID bullets[128];
	ID toKill[128];
	int numKills = 0;
	int numBullets = _objects->find(4, bullets, 128);
	for (int i = 0; i < numBullets; ++i) {
		ds::Entity& e = _objects->get(bullets[i]);
		e.position += v3(-2, 0, 0) * dt;
		if (e.position.x < -14.0f) {
			toKill[numKills++] = bullets[i];
		}
	}
	if (numKills > 0) {
		for (int i = 0; i < numKills; ++i) {
			_objects->remove(toKill[i]);
		}
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
	*/
	/*
	ds::Entity& player = _objects->get(_player);
	bool move = false;
	if (ds::input::getKeyState('D')) {
		//_playerAngle += 0.75 * PI * dt;
		//_player->rotateZ(_playerAngle);
		//_player->rotateY(DEGTORAD(20.0f));
		player.position.x += 4.0f * dt;
	}
	if (ds::input::getKeyState('A')) {
		//_playerAngle -= 0.75 * PI * dt;
		//_player->rotateY(DEGTORAD(-20.0f));
		player.position.x -= 4.0f * dt;
	}
	*/
	//if (!move) {
		//_player->rotateY(0.0f);
	//}
	//_player->translate(_playerPos);
	
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
// read path informations
// -------------------------------------------------------
void TestState::readPathInformations() {
	ds::JSONReader reader;
	char tmp[128];
	v2 buffer[32];
	if (reader.parse("content\\path.json")) {
		int children[256];
		int num = reader.get_categories(children, 256);
		for (int i = 0; i < num; ++i) {
			uint32_t size = 0;
			reader.get(children[i], "size", &size);
			const char* type = reader.get_string(children[i], "type");
			int cnt = 0;
			for (int j = 0; j < size; ++j) {
				sprintf_s(tmp, 128, "p%d", j);
				v2 p;
				reader.get(children[i], tmp, &p);
				buffer[cnt++] = p;
			}
			if (strcmp(type, "straight") == 0 ) {				
				ds::StraightPath* path = new ds::StraightPath;
				path->create(buffer[0], buffer[1]);
				for (int j = 2; j < cnt; ++j) {
					path->add(buffer[j]);
				}
				path->build();
				_paths.push_back(path);
			}
			else {
				ds::CubicBezierPath* path = new ds::CubicBezierPath;
				path->create(buffer[0], buffer[1], buffer[2], buffer[3]);
				int n = (cnt - 4) / 3;
				for (int j = 0; j < n; ++j) {
					path->add(buffer[j * 3 + 4], buffer[j * 3 + 5], buffer[j * 3 + 6]);
				}
				path->build();
				_paths.push_back(path);
			}
		}
	}
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void TestState::render() {
	// scene
	_stars.draw();
	if (_activeEnemies != -1) {
		_scene->transform();
		_scene->draw();
	}
	_objects->transform();
	_objects->draw();

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
	//_bulletList.push_back(b);
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
	gui::InputInt("Anim", &_activeAnimation);
	if (gui::Button("Start")) {
		if (_activeEnemies != -1 && _activeMovement != -1) {
			_enemies[_activeEnemies]->start(_animations[_activeAnimation], _movements[_activeMovement]);
		}
	}
	if (gui::Button("Toggle")) {
		if (_activeEnemies != -1) {
			_enemies[_activeEnemies]->toggle();
		}
	}
	gui::InputInt("WaveIndex", &_wavesIndex);
	if (gui::Button("Start waves")) {
		startWave();
	}
	if (gui::Button("Upgrades")) {
		_upgrades.start(2);
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
