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
	uint8_t grey_screen[3] = { 2,2,2 };
	uint8_t grey[3] = { 130,130,130 };

	Image funky_chars = Image("Images/funky_chars.bmp", white, red, 15, 8);
	Image chars_small = Image("Images/chars.bmp", white, white, 32, 3);
	Image arrow = Image("Images/Arrow.bmp", grey_screen, nullptr, 4, 2);

	Sound track_1 = Sound(L"./Sounds/Hiding Your Reality.wav");
	Sound hurt = Sound(L"./Sounds/523769__matrixxx__retro_hit.wav");
	Sound swing_sfx = Sound(L"./Sounds/Swing_Hit_SFX.wav");
	Sound click = Sound(L"./Sounds/Click.wav");
	Sound click_fail = Sound(L"./Sounds/Click_Fail.wav");

	Sprite play = Sprite(1, "Images/Play_Button.bmp", grey_screen, nullptr);
	Sprite player = Sprite(2, "Images/34x34sprites.bmp", grey_screen, nullptr, 9, 1);
	Sprite sword = Sprite(3, "Images/swordsprites.bmp", grey_screen, nullptr, 3, 8);
	Sprite shield = Sprite(4, "Images/shield1sprites.bmp", grey_screen, nullptr, 4, 2);
	Sprite shop = Sprite(5, "Images/Shop_Button.bmp", grey_screen, nullptr);
	Sprite back = Sprite(6, "Images/Back_Button.bmp", grey_screen, nullptr);
	Sprite upgrade_health = Sprite(7, "Images/Health_Button.bmp");
	Sprite reset = Sprite(8, "Images/Reset_Button.bmp", grey_screen);
	Sprite confirm = Sprite(9, "Images/Confirm_Button.bmp", grey_screen);
	Sprite speed_upgrade = Sprite(10, "Images/Speed_Upgrade.bmp", grey_screen);
	Sprite damage_upgrade = Sprite(11, "Images/Damage_Upgrade.bmp", grey_screen);
	Sprite item_upgrade = Sprite(12, "Images/Item_Upgrade.bmp", grey_screen);
	Sprite switch_item = Sprite(13, "Images/Switch_Item.bmp", grey_screen);
	Sprite sword_button = Sprite(14, "Images/Sword.bmp", grey_screen);
	Sprite bat_button = Sprite(15, "Images/Bat.bmp", grey_screen);
	Sprite claymore_button = Sprite(16, "Images/Claymore.bmp", grey_screen);
	Sprite shield_button = Sprite(17, "Images/Shield.bmp", grey_screen);
	Sprite big_shield_button = Sprite(18, "Images/Big_Shield.bmp", grey_screen);
	Sprite thorned_shield_button = Sprite(19, "Images/Thorned_Shield.bmp", grey_screen);

	std::string chars_map = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~ ";
	std::string weapon = "Sword";
	std::string tool = "Shield";
	std::string item_selected = "Medkit";

	Enemy enemy_arr[200];

	bool start = true;
	bool store_open = false;
	bool initial = true;
	bool debug = false;
	bool swing = false;
	bool damage_enemy = false;
	bool shielding = false;
	bool game_over_check = true;
	bool confirm_reset = false;
	bool item_spawn_pool[100];

	int WIDTH = 1024;
	int HEIGHT = 900;
	int fps = 60;
	int high_low_fps[2] = { 60,60 };
	int player_pos[2] = { 512, 450 };
	int player_image = 0;
	int arrow_image = 0;
	int attack_dir = 0;
	int player_hp[2] = { 10,10 }; // Stored as an array for Current Health and Max Health
	int frames = 0; // Keeps track of amount of frames in entire game
	int frame = 0; // Will probably need to change the name of this or something, this is what tracks animation frames
	int score = 0;
	int cash = 0;
	int spawn_amount = 1;
	int spawn_amount_increase = 50;
	int spawn_pool[100];
	int difficulty_increase = 30;
	int difficulty = 1;
	int i = 0;
	int check_x = 0;
	int check_y = 0;
	int player_speed = 7;
	int item_spawn_chance = 0;
	int upgrade_prices[4] = {
		12, // HP
		100, // Attack Multiplier
		50, // Speed
		75, // Medkit/Mine spawn chance
	};
	// If price is 0, that weapon/tool is unlocked
	int weapon_prices[3] = {
		0, // Sword
		50, // Bat
		100 // Claymore
	};
	int tool_prices[3] = {
		0, // Normal Shield
		50, // Large Shield
		100 // Thorned Shield
	};


	float attack_multiplier = 1.00;
	float damage = 1;
	float player_dir = 0;
	float game_time = 0;
	float enemy_spawn_cd = 3;
	float enemy_spawn_hp = 1;
	float last_enemy_spawn = 0;
	float attack_cooldown = -1;
	float difficulty_timer = 0;
	float track_time = 0;
	float frame_time = 0;
	float tick_60 = 0;
	float item_delay = 3;
	float delta = 0;
	float dir_x = 0;
	float dir_y = 0;
	float thorned_shield_cd = -1;
	float weapon_cooldowns[3] = {
		1,
		2,
		3
	};

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
