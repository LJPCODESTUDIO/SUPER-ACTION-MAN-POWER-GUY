#pragma once
#include "MainWindow.h"
#include "Gui.h"
#include "Image.h"
#include "Sprite.h"


class Game {
private:
	MainWindow& window;
	Gui gui;

	//colors for images and sprites
	uint8_t white[3] = { 255,255,255 };
	uint8_t black[3] = { 0,0,0 };
	uint8_t yellow[3] = { 200,200,0 };
	uint8_t red[3] = { 210, 4, 45 };
	uint8_t orange[3] = { 255,67,36 };
	uint8_t blue[3] = { 0, 165, 255 };
	uint8_t pink[3] = { 255,0,231 };
	uint8_t green[3] = { 186, 254, 202 };

	Image funky_chars = Image("Images/funky_chars.bmp", white, red, 15, 8);

	bool start = true;

	void setup();

public:
	Game(class MainWindow& _app_window);
	void game_loop();

};
