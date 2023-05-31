#pragma once
#include "MainWindow.h"
#include "Gui.h"
#include "Sound.h"
#include "Image.h"
#include "Sprite.h"
#include <time.h>
#include <cmath>

class Enemy {
private:
	//colors for images and sprites
	uint8_t white[3] = { 255,255,255 };
	uint8_t black[3] = { 0,0,0 };
	uint8_t yellow[3] = { 200,200,0 };
	uint8_t red[3] = { 210, 4, 45 };
	uint8_t orange[3] = { 255,67,36 };
	uint8_t blue[3] = { 0, 165, 255 };
	uint8_t pink[3] = { 255,0,231 };
	uint8_t green[3] = { 186, 254, 202 };
	uint8_t green_screen[3] = { 10,10,10 };

public:
	int hp = NULL;
	int type = NULL;
	int pos[2] = { NULL,NULL };
	int id = NULL;
	int sprite_image = 0;

	bool init = true;

	std::string state = "";

	Sprite entity = Sprite(rand()+4, "Images/Enemy.bmp", green_screen);

	Enemy();
	~Enemy();

	void spawn(int _type, int _hp, int _image, int _start_x, int _start_y);

	void move(int player[]);
};