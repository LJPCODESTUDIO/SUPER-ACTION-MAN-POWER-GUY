#pragma once
#include "MainWindow.h"
#include "Gui.h"
#include "Sound.h"
#include "Image.h"
#include "Sprite.h"
#include <time.h>
#include <cmath>

class Item {
private:
	//colors for images and sprites
	uint8_t white[3] = { 255,255,255 };
	uint8_t black[3] = { 0,0,0 };
	uint8_t yellow[3] = { 200,200,0 };
	uint8_t red[3] = { 233,67,67 };
	uint8_t orange[3] = { 255,67,36 };
	uint8_t blue[3] = { 0, 165, 255 };
	uint8_t pink[3] = { 255,0,231 };
	uint8_t green[3] = { 186, 254, 202 };
	uint8_t grey_screen[3] = { 2,2,2 };

public:
	Sprite medkit = Sprite(25, "Images/medkit.bmp", grey_screen, nullptr, 1, 1);
	Sprite mine = Sprite(26, "Images/mine.bmp", grey_screen, nullptr, 1, 1);
	
	int type = 0; // 1 for medkit, 2 for mine
	int x = 0;
	int y = 0;

	Item();
	~Item();

	void spawn(int _spawn_x, int _spawn_y, int _type);

	void remove();

	void check_collisions();

};