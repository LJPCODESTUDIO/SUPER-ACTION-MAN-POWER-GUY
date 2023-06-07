#pragma once
#include "MainWindow.h"
#include "Gui.h"
#include "Sound.h"
#include "Image.h"
#include "Sprite.h"
#include <time.h>
#include <cmath>
#include "Enemy.h"

class Game {
private:
	MainWindow& window;
	Gui gui;

	clock_t start_time, current_time;

	//colors for images and sprites
	uint8_t white[3] = { 255,255,255 };
	uint8_t black[3] = { 0,0,0 };
	uint8_t yellow[3] = { 200,200,0 };
	uint8_t red[3] = { 255, 0, 0 };
	uint8_t orange[3] = { 255,67,36 };
	uint8_t blue[3] = { 0, 165, 255 };
	uint8_t pink[3] = { 255,0,231 };
	uint8_t green[3] = { 186, 254, 202 };
	uint8_t green_screen[3] = { 10,10,10 };
	uint8_t grey[3] = { 130,130,130 };

	Image funky_chars = Image("Images/funky_chars.bmp", white, red, 15, 8);
	Image chars_small = Image("Images/chars.bmp", white, white, 32, 3);
	Image arrow = Image("Images/Arrow.bmp", green_screen, nullptr, 4, 2);

	Sound track_1 = Sound(L"./Sounds/Hiding Your Reality.wav");

	Sprite play = Sprite(1, "Images/Play_Button.bmp", green_screen, nullptr);
	Sprite player = Sprite(2, "Images/Player.bmp", green_screen, nullptr, 3, 1);
	Sprite sword = Sprite(3, "Images/Sword_Attack.bmp", green_screen, nullptr, 3, 8);
	Sprite shield = Sprite(4, "Images/default_shield.bmp", green_screen, nullptr, 4, 2);
	Sprite shop = Sprite(5, "Images/Shop_Button.bmp", green_screen, nullptr);
	Sprite back = Sprite(6, "Images/Back_Button.bmp", green_screen, nullptr);
	Sprite upgrade_health = Sprite(7, "Images/Health_Button.bmp");

	std::string chars_map = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~ ";

	Enemy enemy_arr[100];

	bool start = true;
	bool store_open = false;
	bool initial = true;
	bool debug = false;
	bool swing = false;
	bool damage_enemy = false;
	bool shielding = false;
	bool game_over_check = true;

	int WIDTH = 1024;
	int HEIGHT = 900;
	int fps = 60;
	int high_low_fps[2] = { 60,60 };
	int player_pos[2] = { 512, 450 };
	int player_image = 0;
	int arrow_image = 0;
	int attack_dir = 0;
	int player_hp[2] = { 10,10 }; // Stored as an array for Current Health and Max Health
	int enemys = 0; // Mostly just for testing, who knows if I'll keep this
	int frames = 0; // Keeps track of amount of frames in entire game
	int frame = 0; // Will probably need to change the name of this or something, this is what tracks animation frames
	int score = 0;
	int cash = 0;

	float player_dir = 0;
	float game_time = 0;
	float enemy_spawn_cd = 3;
	float enemy_spawn_hp = 1;
	float last_enemy_spawn = 0;
	float damage_cooldown = -1;
	float attack_cooldown = -1;
	float difficulty_timer = 0;
	float track_time = 0;
	float frame_time = 0;
	float tick_60 = 0;
	float delta = 0;

	void start_game();

	void write(std::string input, int start_x, int start_y, std::string font = "funky_chars");

	void check_time_and_fps(float last_tick_60);

	void key_press();

	void spawn_enemy();

	void reset_collisions();

	void game_over();

	void store();

public:
	Game(class MainWindow& _app_window);
	void game_loop();
};
