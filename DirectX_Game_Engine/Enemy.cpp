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
	old_hp = _hp;
	type = _type;
	sprite_image = _image;
	pos[0] = _start_x; pos[1] = _start_y;
	state = "Active";
	damage_cd = -1;
	init = true;
}

void Enemy::move(int player[], bool hitting_shield) {
	if (type == 1) {
		init = false;
		float dis_x = player[0] - pos[0];
		float dis_y = player[1] - pos[1];
		float hyp = sqrtf(dis_x * dis_x + dis_y * dis_y);
		if (hyp != 0) {
			dis_x /= hyp;
			dis_y /= hyp;
		}

		if (!hitting_shield) {
			pos[0] += round(dis_x * 5); pos[1] += round(dis_y * 5);
		}
		else {
			pos[0] -= round(dis_x * 6); pos[1] -= round(dis_y * 6);
		}

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

	else if (type == 2) {
		if (init) {
			move_perm[0] = player[0] - pos[0];
			move_perm[1] = player[1] - pos[1];
			move_perm[2] = sqrtf(move_perm[0] * move_perm[0] + move_perm[1] * move_perm[1]);
			if (move_perm[2] != 0) {
				move_perm[0] /= move_perm[2];
				move_perm[1] /= move_perm[2];
			}
			init = false;
		}
		else {
			if (!hitting_shield) {
				pos[0] += round(move_perm[0] * 10); pos[1] += round(move_perm[1] * 10);
			}
			else {
				pos[0] -= round(move_perm[0] * 11); pos[1] -= round(move_perm[1] * 11);
			}
			if (pos[0] >= 995) {
				die();
			}
			if (pos[0] <= 0) {
				die();
			}
			if (pos[1] >= 870) {
				die();
			}
			if (pos[1] <= 0) {
				die();
			}
		}


	}

	else if (type == 3) {
		if (init) {
			move_perm[0] = player[0] - pos[0];
			move_perm[1] = player[1] - pos[1];
			move_perm[2] = sqrtf(move_perm[0] * move_perm[0] + move_perm[1] * move_perm[1]);
			if (move_perm[2] != 0) {
				move_perm[0] /= move_perm[2];
				move_perm[1] /= move_perm[2];
			}
			init = false;
		}
		else {
			if (!hitting_shield) {
				pos[0] += round(move_perm[0] * 10); pos[1] += round(move_perm[1] * 10);
			}
			else {
				pos[0] -= round(move_perm[0] * 11); pos[1] -= round(move_perm[1] * 11);
			}
			if (pos[0] >= 995) {
				init = true;
			}
			if (pos[0] <= 0) {
				init = true;
			}
			if (pos[1] >= 870) {
				init = true;
			}
			if (pos[1] <= 0) {
				init = true;
			}
		}
	}
}

void Enemy::die() {
	hp = NULL;
	type = NULL;
	sprite_image = NULL;
	pos[0] = NULL; pos[1] = NULL;
	state = "";
	entity.set_Find_Replace_RGB(white, red);
}