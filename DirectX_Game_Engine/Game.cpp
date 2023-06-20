#include "MainWindow.h"
#include "Game.h"
#include <stdlib.h>
#include <chrono>
#include <thread>

Game::Game(MainWindow& _window) : window(_window), gui(_window) {}

void Game::game_loop() {
	if (initial) {
		gui.set_Target_SubGui(0, 0);
		gui.set_RGB_on_Gui(black);
		initial = false;
		track_1.Play();
		for (int i = 0; i < 200; i++) {
			enemy_arr[i].id = i;
		}
	}
	srand(rand());
	if (game_time - track_time >= 192) {
		track_time = game_time;
		track_1.Play();
	}
	if (start) {
		if (store_open) {
			store();
			return;
		}
		start_game();
		return;
	}
	check_time_and_fps(tick_60);
	reset_collisions();
	gui.set_RGB_on_Gui(black);

	if (game_time - last_enemy_spawn >= enemy_spawn_cd) {
		if (score >= spawn_amount_increase) {
			spawn_amount++;
			spawn_amount_increase += 50;
		}
		
		if (score >= difficulty_increase) {
			if (difficulty == 1) {
				difficulty++;
				for (i = 29; i < 45; i++) {
					spawn_pool[i] = 2;
				}
			}
			else if (difficulty == 2) {
				difficulty++;
				for (i = 14; i < 30; i++) {
					spawn_pool[i] = 3;
				}
			}
			else if (difficulty == 3) {
				difficulty++;
				for (i = 0; i < 15; i++) {
					spawn_pool[i] = 4;
				}
			}
			difficulty_increase += 30;
		}
		
		for (i = 0; i < spawn_amount; i++) {
			spawn_enemy();
		}
		last_enemy_spawn = game_time;
	}

	player_image = 0;
	key_press();
	gui.set_Image_at_Pixel(player_pos[0] - 8, player_pos[1] - 8, arrow[arrow_image]);
	gui.set_Sprite_at_Pixel(player_pos[0], player_pos[1], player[player_image]);

	if (swing) {
		if (frames - frame >= 3) {
			swing = false;
		}
		else {
			if (frames - frame == 1) {
				damage_enemy = true;
			}
			else {
				damage_enemy = false;
			}
			gui.set_Sprite_at_Pixel(player_pos[0]-50, player_pos[1]-50, sword[(frames - frame) + (3 * attack_dir)]);
		}
	}
	if (shielding) {
		gui.set_Sprite_at_Pixel(player_pos[0] - 16, player_pos[1] - 16, shield[arrow_image]);
	}

	for (i = 0; i < 200; i++) {
		if (enemy_arr[i].state != "") {
			bool hitting_shield = false;
			for (check_x = 0; check_x < 32; check_x++) {
				for (check_y = 0; check_y < 32; check_y++) {
					if (enemy_arr[i].pos[0] + check_x < 0 || enemy_arr[i].pos[1] + check_y < 0 || enemy_arr[i].pos[0] + check_x > 995 || enemy_arr[i].pos[1] + check_y > 870) {
						continue;
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + check_x, enemy_arr[i].pos[1] + check_y) == 4) {
						hitting_shield = true;
						break;
					}
				}
			}

			enemy_arr[i].move(player_pos, hitting_shield, game_time);
			gui.set_Sprite_at_Pixel(enemy_arr[i].pos[0], enemy_arr[i].pos[1], enemy_arr[i].entity[enemy_arr[i].sprite_image]);
			if (enemy_arr[i].old_hp != enemy_arr[i].hp) {
				enemy_arr[i].entity.set_Find_Replace_RGB(white, red);
				enemy_arr[i].old_hp = enemy_arr[i].hp;
			}
			for (check_x = 0; check_x < 32; check_x++) {
				for (check_y = 0; check_y < 32; check_y++) {
					if (enemy_arr[i].pos[0] + check_x < 0 || enemy_arr[i].pos[1] + check_y < 0 || enemy_arr[i].pos[0] + check_x > 995 || enemy_arr[i].pos[1] + check_y > 870) {
						continue;
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0]+ check_x, enemy_arr[i].pos[1]+ check_y) == 2) {
						if (game_time - enemy_arr[i].attack_cd >= .5) {
							hurt.Play();
							enemy_arr[i].attack_cd = game_time;
							if (enemy_arr[i].type == 3) {
								player_hp[0] -= 2;
								enemy_arr[i].die();
							}
							else if (enemy_arr[i].type == 4) {
								player_hp[0] -= 5;
							}
							else {
								player_hp[0]--;
							}
							if (player_hp[0] <= 0) {
								game_over();
							}
						}
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + check_x, enemy_arr[i].pos[1] + check_y) == 3 && damage_enemy) {
						if (game_time - enemy_arr[i].damage_cd >= .1) {
							hurt.Play();
							enemy_arr[i].damage_cd = game_time;
							enemy_arr[i].hp -= 1;
							enemy_arr[i].entity.set_Find_Replace_RGB(red, white);
							if (enemy_arr[i].hp <= 0) {
								enemy_arr[i].die();
								score++;
								if (enemy_spawn_cd > 1) {
									enemy_spawn_cd -= enemy_spawn_cd * .03;
								}
								else {
									if (enemy_spawn_hp < 3) {
										enemy_spawn_hp += 0.5;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	write("Time: " + std::to_string(game_time), (WIDTH / 2) - (15 * (3+(std::to_string(game_time).length()/2))), 0, "chars_small");
	write("Score: " + std::to_string(score), (WIDTH / 2) - (15 * (3 + (std::to_string(score).length() / 2))), 25, "chars_small");
	write("HP: " + std::to_string(player_hp[0]) + "/" + std::to_string(player_hp[1]), (WIDTH / 2) - (15 * (2 + ((std::to_string(player_hp[0]).length() + std::to_string(player_hp[1]).length()) / 2))), 50, "chars_small");

	gui.refresh_Gui();
}

void Game::start_game() {
	gui.set_Target_SubGui(0, 0);
	gui.set_RGB_on_Gui(black);

	gui.set_Sprite_at_Pixel((WIDTH / 2) - 150, (HEIGHT / 2) - 75, play);
	gui.set_Sprite_at_Pixel((WIDTH / 2) - 150, (HEIGHT / 2) + 75, shop);
	if (confirm_reset) {
		gui.set_Sprite_at_Pixel(0, HEIGHT - 55, confirm);
	}
	else {
		gui.set_Sprite_at_Pixel(0, HEIGHT - 55, reset);
	}

	write("SUPERACTION", (WIDTH / 2) - 425, 50);
	write("MANPOWERGUY", (WIDTH / 2) - 425, 150);
	
	gui.refresh_Gui();
	
	if (game_over_check) {
		std::this_thread::sleep_for(std::chrono::milliseconds{ 350 });
		game_over_check = false;
	}
	
	if (window.mouse.LeftIsPressed()) {
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 1) {
			click.Play();
			player_hp[0] = player_hp[1];
			player_pos[0] = 512; player_pos[1] = 450;
			enemy_spawn_cd = 3;
			attack_cooldown = 0;
			last_enemy_spawn = 0;
			frames = 0;
			frame = 0;
			score = 0;
			frame_time = 0;
			enemy_spawn_hp = 1;
			spawn_amount = 1;
			spawn_amount_increase = 50;
			difficulty_increase = 30;
			for (int i = 0; i < 100; i++) {
				spawn_pool[i] = 1;
			}
			swing = false;
			start = false;
			start_time = clock();
			check_time_and_fps(tick_60);
			gui.set_Target_SubGui(1, 0);
			gui.set_RGB_on_Gui(black);
		}
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 5) {
			click.Play();
			store_open = true;
		}
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 8) {
			click.Play();
			confirm_reset = true;
			std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
		}
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 9) {
			click.Play();
			confirm_reset = false;
			cash = 0;
			player_hp[1] = 10;
			std::this_thread::sleep_for(std::chrono::milliseconds{ 350 });
		}
	}
}

void Game::store() {
	gui.set_Target_SubGui(2, 0);
	gui.set_RGB_on_Gui(grey);

	write(std::to_string(window.mouse.GetPosX()) + "," + std::to_string(window.mouse.GetPosY()), 0, HEIGHT - 25, "chars_small");

	gui.set_Sprite_at_Pixel(20, 20, back);
	write("Cash: " + std::to_string(cash), (WIDTH / 2) - (15 * (3 + (std::to_string(cash).length() / 2))), 0, "chars_small");

	// HP
	upgrade_prices[0] = player_hp[1] + 2;
	write("Price: " + std::to_string(upgrade_prices[0]), 110, 259, "chars_small");
	write("Current: " + std::to_string(player_hp[1]), 110, 284, "chars_small");
	write("New: " + std::to_string(player_hp[1] + 2), 110, 309, "chars_small");
	gui.set_Sprite_at_Pixel(110, 180, upgrade_health);

	// Damage
	write("Price: " + std::to_string(upgrade_prices[1]), 410, 259, "chars_small");
	write("Current: " + std::to_string(attack_multiplier).erase(3), 410, 284, "chars_small");
	write("New: " + std::to_string(attack_multiplier * 1.3).erase(3), 410, 309, "chars_small");
	gui.set_Sprite_at_Pixel(410, 180, damage_upgrade);

	// Speed
	write("Price: " + std::to_string(upgrade_prices[2]), 710, 259, "chars_small");
	write("Current: " + std::to_string(player_speed), 710, 284, "chars_small");
	write("New: " + std::to_string(player_speed + 1), 710, 309, "chars_small");
	gui.set_Sprite_at_Pixel(710, 180, speed_upgrade);

	// Weapons
	write("Current Weapon: " + weapon, (WIDTH / 2) - (15 * (8 + (weapon.length() / 2))), 400, "chars_small");
	write("Current Utility: " + tool, (WIDTH / 2) - (15 * (9 + (tool.length() / 2))), 425, "chars_small");

	if (window.mouse.LeftIsPressed()) {
		uint8_t collision = gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY());
		if (collision == 6) {
			click.Play();
			store_open = false;
		}
		else if (collision == 7) {
			if (cash >= upgrade_prices[0]) {
				click.Play();
				cash -= upgrade_prices[0];
				player_hp[1] += 2;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
		else if (collision == 11) {
			if (cash >= upgrade_prices[1]) {
				click.Play();
				cash -= upgrade_prices[1];
				attack_cooldown = attack_cooldown * 1.3;
				upgrade_prices[1] += 100;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
		else if (collision == 10) {
			if (cash >= upgrade_prices[2]) {
				click.Play();
				cash -= upgrade_prices[2];
				player_speed++;
				upgrade_prices[2] += 50;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
	}

	gui.refresh_Gui();
}

void Game::write(std::string input, int start_x, int start_y, std::string font) {
	int x_offset = 0;

	for (i = 0; i < input.length(); i++) {
		if (font == "funky_chars") {
			gui.set_Image_at_Pixel(start_x + x_offset, start_y, funky_chars[chars_map.find_first_of(input[i])]);
			x_offset += 75;
		}
		else if (font == "chars_small") {
			gui.set_Image_at_Pixel(start_x + x_offset, start_y, chars_small[chars_map.find_first_of(input[i])]);
			x_offset += 15;
		}
	}
}

void Game::check_time_and_fps(float last_tick_60) {
	current_time = clock() - start_time;
	game_time = ((double)current_time) / CLOCKS_PER_SEC;
	tick_60 = game_time * 60;
	delta = tick_60 - last_tick_60;
	fps = round(60 / delta);
	if (fps > high_low_fps[0]) {
		high_low_fps[0] = fps;
	}
	else if (fps < high_low_fps[1] && fps >= 0) {
		high_low_fps[1] = fps;
	}
	if (game_time - frame_time >= .08) {
		frames++;
		frame_time = game_time;
	}
}

void Game::key_press() {
	if (gui.get_Gui_Elem() == 1) {
		// Player stuff
		dir_x = window.kbd.KeyIsPressed(0x44) - window.kbd.KeyIsPressed(0x41);
		dir_y = window.kbd.KeyIsPressed(0x53) - window.kbd.KeyIsPressed(0x57);

		if (dir_y == -1) {
			if (dir_x == 1) {
				player_image = 8;
			}
			else if (dir_x == -1) {
				player_image = 7;
			}
			else {
				player_image = 1;
			}
		}
		else if (dir_y == 1) {
			if (dir_x == 1) {
				player_image = 6;
			}
			else if (dir_x == -1) {
				player_image = 5;
			}
			else {
				player_image = 2;
			}
		}
		else if (dir_y == 0) {
			if (dir_x == 1) {
				player_image = 3;
			}
			else if (dir_x == -1) {
				player_image = 4;
			}
		}

		float hyp = sqrtf(dir_x * dir_x + dir_y * dir_y);
		if (hyp != 0) {
			dir_x /= hyp;
			dir_y /= hyp;
		}

		if (!shielding) {
			player_pos[0] += round(dir_x * player_speed);
			player_pos[1] += round(dir_y * player_speed);
		}
		else {
			player_pos[0] += round(dir_x * 6);
			player_pos[1] += round(dir_y * 6);
		}

		if (player_pos[0] >= 995) {
			player_pos[0] -= 7;
		}
		if (player_pos[0] <= 0) {
			player_pos[0] += 7;
		}
		if (player_pos[1] >= 870) {
			player_pos[1] -= 7;
		}
		if (player_pos[1] <= 0) {
			player_pos[1] += 7;
		}

		float x = window.mouse.GetPosX() - player_pos[0];
		float y = window.mouse.GetPosY() - player_pos[1];
		if (x != 0) {
			player_dir = atan2(y, x) * 180 / 3.14159;
			player_dir *= -1;
		}

		if (player_dir <= 22 && player_dir >= -22) {
			arrow_image = 0;
			attack_dir = 0;
		}
		else if (player_dir > 22 && player_dir < 68) {
			arrow_image = 4;
			if (!swing) {
				attack_dir = 4;
			}
		}
		else if (player_dir >= 68 && player_dir <= 112) {
			arrow_image = 3;
			if (!swing) {
				attack_dir = 3;
			}
		}
		else if (player_dir > 112 && player_dir < 158) {
			arrow_image = 7;
			if (!swing) {
				attack_dir = 7;
			}
		}
		else if (player_dir >= 158 && player_dir >= -158) {
			arrow_image = 2;
			if (!swing) {
				attack_dir = 2;
			}
		}
		else if (player_dir > -158 && player_dir < -112) {
			arrow_image = 6;
			if (!swing) {
				attack_dir = 6;
			}
		}
		else if (player_dir >= -112 && player_dir <= -68) {
			arrow_image = 1;
			if (!swing) {
				attack_dir = 1;
			}
		}
		else if (player_dir > -68 && player_dir <= -2) {
			arrow_image = 5;
			if (!swing) {
				attack_dir = 5;
			}
		}

		if (window.mouse.LeftIsPressed() && game_time - attack_cooldown >= 1 && !shielding) {
			swing_sfx.Play();
			swing = true;
			frame = frames;
			attack_cooldown = game_time;
		}
		if (window.mouse.RightIsPressed() && !swing) {
			shielding = true;
		}
		else if (!window.mouse.RightIsPressed()) {
			shielding = false;
		}
	}
}

void Game::spawn_enemy() {
	int spawn_x = rand() % WIDTH;
	int spawn_y = rand() % HEIGHT;
	bool valid = false;
	while (!valid) {
		spawn_x = rand() % WIDTH;
		spawn_y = rand() % HEIGHT;
		int dis_x = abs(player_pos[0] - spawn_x);
		int dis_y = abs(player_pos[1] - spawn_y);

		if (dis_x >= 200 && dis_y <= 200) {
			valid = true;
		}
	}

	for (i = 0; i < 200; i++) {
		if (enemy_arr[i].state == "") {
			int type = spawn_pool[rand() % 100];
			if (type == 1) {
				enemy_arr[i].spawn(1, enemy_spawn_hp, 0, spawn_x, spawn_y);
			}
			else if (type == 2) {
				enemy_arr[i].spawn(2, 5, 1, spawn_x, spawn_y);
			}
			else if (type == 3) {
				enemy_arr[i].spawn(3, 1, 2, spawn_x, spawn_y);
			}
			else if (type == 4) {
				enemy_arr[i].spawn(4, 5, 3, spawn_x, spawn_y);
			}
			break;
		}
	}
}

void Game::reset_collisions() {
	for (check_x = 0; check_x < WIDTH; check_x++) {
		for (check_y = 0; check_y < HEIGHT; check_y++) {
			if (gui.get_CollissionMap_Data_at_Pixel(check_x, check_y) != 0) {
				gui.set_CollissionMap_Data_at_Pixel(check_x, check_y, 0);
			}
		}
	}
}

void Game::game_over() {
	for (i = 0; i < 200; i++) {
		enemy_arr[i].die();
	}
	cash += score;
	start = true;
	game_over_check = true;
}