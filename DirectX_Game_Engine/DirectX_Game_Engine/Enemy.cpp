#include "Enemy.h"

Enemy::Enemy() {
	init = true;
}

Enemy::~Enemy() {
	hp = NULL;
	type = NULL;
	pos[0] = NULL; pos[1] = NULL;
	state = "";
}

void Enemy::spawn(int _type, float _hp, int _image, int _start_x, int _start_y) {
	hp = _hp;
	type = _type;
	sprite_image = _image;
	pos[0] = _start_x; pos[1] = _start_y;
	state = "Active";
	init = false;
}

void Enemy::move(int player[]) {
	if (type == 1) {
		float dis_x = player[0] - pos[0];
		float dis_y = player[1] - pos[1];
		float hyp = sqrtf(dis_x * dis_x + dis_y * dis_y);
		if (hyp != 0) {
			dis_x /= hyp;
			dis_y /= hyp;
		}

		pos[0] += round(dis_x * 6); pos[1] += round(dis_y * 6);

		if (pos[0] >= 995) {
			pos[0] -= 7;
		}
		if (pos[0] <= 0) {
			pos[0] += 7;
		}
		if (pos[1] >= 870) {
			pos[1] -= 7;
		}
		if (pos[1] <= 0) {
			pos[1] += 7;
		}
	}
}

void Enemy::die() {
	hp = NULL;
	type = NULL;
	sprite_image = NULL;
	pos[0] = NULL; pos[1] = NULL;
	state = "";
}