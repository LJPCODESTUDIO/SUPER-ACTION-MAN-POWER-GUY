#pragma once
#include "MainWindow.h"
#include "Gui.h"
#include "Sound.h"
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
	uint8_t green_screen[3] = { 0,5,0 };

	Image funky_chars = Image("Images/funky_chars.bmp", white, red, 15, 8);
	Image chars_small = Image("Images/chars.bmp", white, red, 32, 3);

	Sound track_1 = Sound(L"./Sounds/Hiding Your Reality.wav");

	Sprite play = Sprite(1, "Images/Play_Button.bmp", green_screen, nullptr);

	std::string chars_map = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~ ";

	bool start = true;

	int WIDTH = 1024;
	int HEIGHT = 900;

	void start_game();

	void write(std::string input, int start_x, int start_y, std::string font = "funky_chars");

public:
	Game(class MainWindow& _app_window);
	void game_loop();
};
