#include "Enemy.h"

Enemy::Enemy() {
	init = true;
}

Enemy::~Enemy() {
	hp = NULL;
	type = NULL;
	pos[0] = NULL; pos[1] = NULL;
	entity = Sprite(rand(), "/Images/Enemy.bmp", green_screen);
}

void Enemy::_init_(int _type, int _hp, int _id, int _image, int _start_x, int _start_y) {
	hp = _hp;
	type = _type;
	sprite_image = _image;
	pos[0] = _start_x; pos[1] = _start_y;
	id = _id;
	init = false;
}