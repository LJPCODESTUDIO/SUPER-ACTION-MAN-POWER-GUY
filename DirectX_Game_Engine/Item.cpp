#include "Item.h"

Item::Item() {
	return;
}

Item::~Item() {
	return;
}

void Item::spawn(int _spawn_x, int _spawn_y, int _type) {
	type = _type;
	x = _spawn_x;
	y = _spawn_y;
}

void Item::remove() {
	type = 0;
}

void Item::check_collisions() {
	return;
}