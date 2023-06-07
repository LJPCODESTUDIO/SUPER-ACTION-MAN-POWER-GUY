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
		for (int i = 0; i < 100; i++) {
			enemy_arr[i].id = i + 50;
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
		spawn_enemy();
		last_enemy_spawn = game_time;
	}
	if (game_time - difficulty_timer >= 1) {
		if (enemy_spawn_cd > 0.7) {
			enemy_spawn_cd -= enemy_spawn_cd * .01;
			difficulty_timer = game_time;
		}
		else {
			enemy_spawn_hp += enemy_spawn_hp * .01;
			difficulty_timer = game_time;
		}
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

	for (int i = 0; i < 100; i++) {
		if (enemy_arr[i].state != "") {
			bool hitting_shield = false;
			for (int x = 0; x < 32; x++) {
				for (int y = 0; y < 32; y++) {
					if (enemy_arr[i].pos[0] + x < 0 || enemy_arr[i].pos[1] + y < 0 || enemy_arr[i].pos[0] + x > 995 || enemy_arr[i].pos[1] + y > 870) {
						continue;
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + x, enemy_arr[i].pos[1] + y) == 4) {
						hitting_shield = true;
						break;
					}
				}
			}
			if (!hitting_shield) {
				enemy_arr[i].move(player_pos);
			}
			//gui.set_Sprite_Off(enemy_arr[i].entity);
			gui.set_Sprite_at_Pixel(enemy_arr[i].pos[0], enemy_arr[i].pos[1], enemy_arr[i].entity);
			for (int x = 0; x < 32; x++) {
				for (int y = 0; y < 32; y++) {
					if (enemy_arr[i].pos[0] + x < 0 || enemy_arr[i].pos[1] + y < 0 || enemy_arr[i].pos[0] + x > 995 || enemy_arr[i].pos[1] + y > 870) {
						continue;
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0]+x, enemy_arr[i].pos[1]+y) == 2) {
						if (game_time - damage_cooldown >= .5) {
							damage_cooldown = game_time;
							player_hp[0]--;
							if (player_hp[0] <= 0) {
								game_over();
							}
						}
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + x, enemy_arr[i].pos[1] + y) == 3 && damage_enemy) {
						enemy_arr[i].hp--;
						if (enemy_arr[i].hp <= 0) {
							enemy_arr[i].die();
							score++;
						}
					}
				}
			}
		}
	}

	write("Time: " + std::to_string(game_time), (WIDTH / 2) - (15 * (3+(std::to_string(game_time).length()/2))), 0, "chars_small");
	write("Score: " + std::to_string(score), (WIDTH / 2) - (15 * (3 + (std::to_string(score).length() / 2))), 25, "chars_small");
	write("HP: " + std::to_string(player_hp[0]) + "/" + std::to_string(player_hp[1]), (WIDTH / 2) - (15 * (2 + ((std::to_string(player_hp[0]).length() + std::to_string(player_hp[1]).length()) / 2))), 50, "chars_small");

	if (debug) {
		write("FPS: " + std::to_string(fps) + " High: " + std::to_string(high_low_fps[0]) + " Low: " + std::to_string(high_low_fps[1]), 0, 0, "chars_small");
		write("Pos: " + std::to_string(player_pos[0]) + "," + std::to_string(player_pos[1]), 0, 25, "chars_small");
		write("Direction: " + std::to_string(player_dir), 0, 50, "chars_small");
		write("Frames: " + std::to_string(frames), 0, 75, "chars_small");
		write("Spawn CD: " + std::to_string(enemy_spawn_cd), 0, 100, "chars_small");
		for (int i = 0; i < 100; i++) {
			if (enemy_arr[i].state != "") {
				write("Enemy" + std::to_string(i) + ": " + std::to_string(enemy_arr[i].pos[1]) + "," + std::to_string(enemy_arr[i].pos[1]), 0, 125 + (i * 25), "chars_small");
			}
		}
	}

	gui.refresh_Gui();
}

void Game::start_game() {
	gui.set_Target_SubGui(0, 0);
	gui.set_RGB_on_Gui(black);

	gui.set_Sprite_at_Pixel((WIDTH / 2) - 150, (HEIGHT / 2) - 75, play);
	gui.set_Sprite_at_Pixel((WIDTH / 2) - 150, (HEIGHT / 2) + 75, shop);

	write("SUPERACTION", (WIDTH / 2) - 425, 50);
	write("MANPOWERGUY", (WIDTH / 2) - 425, 150);
	
	gui.refresh_Gui();
	
	if (window.mouse.LeftIsPressed()) {
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 1) {
			player_hp[0] = player_hp[1];
			player_pos[0] = 512; player_pos[1] = 450;
			enemy_spawn_cd = 3;
			difficulty_timer = game_time;
			damage_cooldown = -1;
			attack_cooldown = 0;
			last_enemy_spawn = 0;
			frames = 0;
			frame = 0;
			score = 0;
			enemy_spawn_hp = 1;
			swing = false;
			damage_cooldown = false;
			start = false;
			start_time = clock();
			gui.set_Target_SubGui(1, 0);
			gui.set_RGB_on_Gui(black);
		}
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 5) {
			store_open = true;
		}
	}
}

void Game::store() {
	gui.set_Target_SubGui(2, 0);
	gui.set_RGB_on_Gui(grey);

	gui.set_Sprite_at_Pixel(20, 20, back);

	if (window.mouse.LeftIsPressed()) {
		if (gui.get_CollissionMap_Data_at_Pixel(window.mouse.GetPosX(), window.mouse.GetPosY()) == 6) {
			store_open = false;
		}
	}

	gui.refresh_Gui();
}

void Game::write(std::string input, int start_x, int start_y, std::string font) {
	int x_offset = 0;

	for (int i = 0; i < input.length(); i++) {
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
		float dir_x = window.kbd.KeyIsPressed(0x44) - window.kbd.KeyIsPressed(0x41);
		float dir_y = window.kbd.KeyIsPressed(0x53) - window.kbd.KeyIsPressed(0x57);

		if (dir_x > 0) {
			player_image = 2;
		}
		else if (dir_x < 0) {
			player_image = 1;
		}
		if (dir_x != 0 || dir_y != 0) {
			//gui.set_Sprite_Off(player);
		}

		float hyp = sqrtf(dir_x * dir_x + dir_y * dir_y);
		if (hyp != 0) {
			dir_x /= hyp;
			dir_y /= hyp;
		}

		if (!shielding) {
			player_pos[0] += round(dir_x * 7);
			player_pos[1] += round(dir_y * 7);
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

		if (window.kbd.KeyIsPressed(VK_F2)) {
			debug = !debug;
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
		int dis_x = player_pos[0] - spawn_x;
		int dis_y = player_pos[1] - spawn_y;
		float hyp = sqrtf(dis_x * dis_x + dis_y * dis_y);

		if (hyp >= 100) {
			valid = true;
		}
	}

	for (int i = 0; i < 100; i++) {
		if (enemy_arr[i].state == "") {
			enemy_arr[i].spawn(1, enemy_spawn_hp, 0, spawn_x, spawn_y);
			break;
		}
	}
}

void Game::reset_collisions() {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			if (gui.get_CollissionMap_Data_at_Pixel(x, y) != 0) {
				gui.set_CollissionMap_Data_at_Pixel(x, y, 0);
			}
		}
	}
}

void Game::game_over() {
	for (int i = 0; i < 100; i++) {
		enemy_arr[i].die();
	}
	cash += score;
	start = true;
}