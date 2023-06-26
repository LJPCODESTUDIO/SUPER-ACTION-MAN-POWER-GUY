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
		for (i = 0; i < 200; i++) {
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
	if (game_time - item_spawn_cd >= 5) {
		spawn_item();
		item_spawn_cd = game_time;
	}

	player_image = 0;
	key_press();
	gui.set_Image_at_Pixel(player_pos[0] - 8, player_pos[1] - 9, arrow[arrow_image]);
	gui.set_Sprite_at_Pixel(player_pos[0], player_pos[1], player[player_image]);

	if (swing) {
		if (weapon == "Sword" || weapon == "Bat") {
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
				if (weapon == "Sword") {
					gui.set_Sprite_at_Pixel(player_pos[0] - 50, player_pos[1] - 50, sword[(frames - frame) + (3 * attack_dir)]);
				}
				else {
					gui.set_Sprite_at_Pixel(player_pos[0] - 50, player_pos[1] - 50, bat[(frames - frame) + (3 * attack_dir)]);
				}
			}
		}
		else if (weapon == "Claymore") {
			if (frames - frame < 3 && claymore_anim == 0) {
				claymore_frame = -1;
			}
			else if (claymore_anim == 2) {
				if (frames - frame < 3) {
					claymore_frame = -1;
				}
				else {
					swing = false;
					attack_cooldown = game_time;
				}
			}
			else {
				if (claymore_frame == -1) {
					claymore_frame = 0;
					damage_enemy = true;
					claymore_anim = 1;
					claymore_rotate = 0;
				}
				claymore_frame++;
				if (claymore_frame > 8) {
					claymore_frame = 0;
					claymore_rotate++;
				}
				if (claymore_rotate == 3) {
					damage_enemy = false;
					claymore_anim = 2;
					frame = frames;
				}
				if (claymore_frame > 8) {
					damage_enemy = false;
					claymore_anim = 2;
					frame = frames;
				}
				if (claymore_frame == 0) {
					attack_dir = 0;
				}
				else if (claymore_frame == 1) {
					attack_dir = 7;
				}
				else if (claymore_frame == 2) {
					attack_dir = 1;
				}
				else if (claymore_frame == 3) {
					attack_dir = 6;
				}
				else if (claymore_frame == 4) {
					attack_dir = 2;
				}
				else if (claymore_frame == 5) {
					attack_dir = 4;
				}
				else if (claymore_frame == 6) {
					attack_dir = 3;
				}
				else if (claymore_frame == 7) {
					attack_dir = 5;
				}
				else if (claymore_frame == 8) {
					attack_dir = 0;
				}
			}
			if (claymore_frame < 0) {
				gui.set_Sprite_at_Pixel(player_pos[0] - 100, player_pos[1] - 100, claymore[0]);
			}
			else if (swing) {
				gui.set_Sprite_at_Pixel(player_pos[0] - 100, player_pos[1] - 100, claymore_swing[attack_dir]);
			}
		}
	}
	if (shielding) {
		if (tool == "Shield") {
			gui.set_Sprite_at_Pixel(player_pos[0] - 16, player_pos[1] - 16, shield[arrow_image]);
		}
		else if (tool == "Big Shield") {
			gui.set_Sprite_at_Pixel(player_pos[0] - 16, player_pos[1] - 16, big_shield[arrow_image]);
		}
		else if (tool == "Thorned Shield") {
			gui.set_Sprite_at_Pixel(player_pos[0] - 16, player_pos[1] - 16, thorned_shield[attack_dir]);
		}
	}

	for (i = 0; i < 100; i++) {
		if (item_arr[i].type != 0) {
			if (item_arr[i].type == 1) {
				gui.set_Sprite_at_Pixel(item_arr[i].x, item_arr[i].y, item_arr[i].medkit);
			}
			else {
				gui.set_Sprite_at_Pixel(item_arr[i].x, item_arr[i].y, item_arr[i].mine);
			}
			for (check_x = 0; check_x < 32; check_x++) {
				for (check_y = 0; check_y < 32; check_y++) {
					if (item_arr[i].x + check_x < 0 || item_arr[i].y + check_y < 0 || item_arr[i].x + check_x > 995 || item_arr[i].y + check_y > 870) {
						continue;
					}
					if (gui.get_CollissionMap_Data_at_Pixel(item_arr[i].x + check_x, item_arr[i].y + check_y) == 2 && item_arr[i].type == 1) {
						player_hp[0] = player_hp[1];
						item_arr[i].remove();
					}
				}
			}
		}
	}
	for (i = 0; i < 200; i++) {
		if (enemy_arr[i].state != "") {
			bool hitting_shield = false;
			for (check_x = 0; check_x < 32; check_x++) {
				for (check_y = 0; check_y < 32; check_y++) {
					if (enemy_arr[i].pos[0] + check_x < 0 || enemy_arr[i].pos[1] + check_y < 0 || enemy_arr[i].pos[0] + check_x > 995 || enemy_arr[i].pos[1] + check_y > 870) {
						continue;
					}
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + check_x, enemy_arr[i].pos[1] + check_y) == tool_id) {
						if (tool == "Thorned Shield" && thorn_dash) {
							hurt.Play();
							enemy_arr[i].die();
						}
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
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0]+ check_x, enemy_arr[i].pos[1]+ check_y) == 2 && !hitting_shield) {
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
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + check_x, enemy_arr[i].pos[1] + check_y) == weapon_id && damage_enemy) {
						if (game_time - enemy_arr[i].damage_cd >= .1) {
							hurt.Play();
							enemy_arr[i].damage_cd = game_time;
							enemy_arr[i].hp -= damage * attack_multiplier;
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
					if (gui.get_CollissionMap_Data_at_Pixel(enemy_arr[i].pos[0] + check_x, enemy_arr[i].pos[1] + check_y) == 26) {
						explosion.Play();
						for (int j = 0; j < 100; j++) {
							if (item_arr[j].type == 2) {
								for (int _check_x = 0; _check_x < 32; _check_x++) {
									for (int _check_y = 0; _check_y < 32; _check_y++) {
										if (item_arr[j].x + _check_x < 0 || item_arr[j].y + _check_y < 0 || item_arr[j].x + _check_x > 995 || item_arr[j].y + _check_y > 870) {
											continue;
										}
										if (gui.get_CollissionMap_Data_at_Pixel(item_arr[j].x + _check_x, item_arr[j].y + _check_y) == 50) {
											item_arr[j].remove();
										}
									}
								}
							}
						}
						enemy_arr[i].die();
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
			difficulty = 1;
			frame_time = 0;
			enemy_spawn_hp = 1;
			spawn_amount = 1;
			spawn_amount_increase = 50;
			difficulty_increase = 30;
			thorned_shield_cd = -2;
			thorned_shield_dash = 0;
			item_spawn_cd = 0;
			control = true;
			dash_check = false;
			thorn_dash = false;
			for (i = 0; i < 100; i++) {
				spawn_pool[i] = 2;
				item_spawn_pool[i] = false;
			}
			for (i = 0; i < item_spawn_chance; i++) {
				item_spawn_pool[i] = true;
			}
			if (weapon == "Sword") {
				weapon_cooldown = 1;
				damage = 1;
				weapon_id = 3;
			}
			else if (weapon == "Bat") {
				weapon_cooldown = 3;
				damage = 5;
				weapon_id = 20;
			}
			else if (weapon == "Claymore") {
				weapon_cooldown = 5;
				damage = 20;
				weapon_id = 21;
			}
			if (tool == "Shield") {
				tool_id = 4;
			}
			else if (tool == "Big Shield") {
				tool_id = 23;
			}
			else if (tool == "Thorned Shield") {
				tool_id = 24;
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

	//write(std::to_string(window.mouse.GetPosX()) + "," + std::to_string(window.mouse.GetPosY()), 0, HEIGHT - 25, "chars_small");

	gui.set_Sprite_at_Pixel(20, 20, back);
	write("Cash: " + std::to_string(cash), (WIDTH / 2) - (15 * (3 + (std::to_string(cash).length() / 2))), 0, "chars_small");

	// HP
	upgrade_prices[0] = player_hp[1] + 2;
	write("Price: " + std::to_string(upgrade_prices[0]), 110, 259, "chars_small");
	write("Current: " + std::to_string(player_hp[1]), 110, 284, "chars_small");
	write("New: " + std::to_string(player_hp[1] + 2), 110, 309, "chars_small");
	gui.set_Sprite_at_Pixel(110, 180, upgrade_health);

	// Damage
	write("Price: " + std::to_string(upgrade_prices[1]), 330, 259, "chars_small");
	write("Current: " + std::to_string(attack_multiplier).erase(3), 330, 284, "chars_small");
	write("New: " + std::to_string(attack_multiplier * 1.3).erase(3), 330, 309, "chars_small");
	gui.set_Sprite_at_Pixel(330, 180, damage_upgrade);

	// Speed
	write("Price: " + std::to_string(upgrade_prices[2]), 550, 259, "chars_small");
	write("Current: " + std::to_string(player_speed), 550, 284, "chars_small");
	write("New: " + std::to_string(player_speed + 1), 550, 309, "chars_small");
	gui.set_Sprite_at_Pixel(550, 180, speed_upgrade);

	// Item
	if (item_spawn_chance < 100) {
		write("Price: " + std::to_string(upgrade_prices[3]), 770, 259, "chars_small");
		write("Current: " + std::to_string(item_spawn_chance) + "%", 770, 284, "chars_small");
		write("New: " + std::to_string(item_spawn_chance + 5) + "%", 770, 309, "chars_small");
		gui.set_Sprite_at_Pixel(770, 180, item_upgrade);
	}
	else {
		write("Price: MAX", 770, 259, "chars_small");
		write("Current: " + std::to_string(item_spawn_chance) + "%", 770, 284, "chars_small");
		write("New: MAX", 770, 309, "chars_small");
		gui.set_Sprite_at_Pixel(770, 180, item_upgrade);
	}

	gui.set_Sprite_at_Pixel((WIDTH / 2) - 69, 90, switch_item);
	write("Current: " + item_selected, (WIDTH / 2) - (15 * (4 + (item_selected.length() / 2))), 65, "chars_small");

	// Weapons
	write("Current Weapon: " + weapon, (WIDTH / 2) - (15 * (8 + (weapon.length() / 2))), 400, "chars_small");
	write("Current Utility: " + tool, (WIDTH / 2) - (15 * (8 + (tool.length() / 2))), 425, "chars_small");

	gui.set_Sprite_at_Pixel(110, 480, sword_button);
	write("Purchased", 250, 507, "chars_small");

	gui.set_Sprite_at_Pixel(110, 600, bat_button);
	if (weapon_prices[1] > 0) {
		write("Price: " + std::to_string(weapon_prices[1]), 250, 627, "chars_small");
	}
	else {
		write("Purchased", 250, 627, "chars_small");
	}

	gui.set_Sprite_at_Pixel(110, 720, claymore_button);
	if (weapon_prices[2] > 0) {
		write("Price: " + std::to_string(weapon_prices[2]), 250, 747, "chars_small");
	}
	else {
		write("Purchased", 250, 747, "chars_small");
	}

	gui.set_Sprite_at_Pixel(770, 480, shield_button);
	write("Purchased", 635, 507, "chars_small");

	gui.set_Sprite_at_Pixel(770, 600, big_shield_button);
	if (tool_prices[1] > 0) {
		write("Price: " + std::to_string(tool_prices[1]), 635, 627, "chars_small");
	}
	else {
		write("Purchased", 635, 627, "chars_small");
	}

	gui.set_Sprite_at_Pixel(770, 720, thorned_shield_button);
	if (tool_prices[2] > 0) {
		write("Price: " + std::to_string(tool_prices[2]), 620, 747, "chars_small");
	}
	else {
		write("Purchased", 635, 747, "chars_small");
	}

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
		else if (collision == 12) {
			if (cash >= upgrade_prices[3] && item_spawn_chance < 100) {
				click.Play();
				cash -= upgrade_prices[3];
				item_spawn_chance += 5;
				upgrade_prices[3] += 25;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
		else if (collision == 13) {
			click.Play();
			if (item_selected == "Medkit") {
				item_selected = "Mine";
			}
			else {
				item_selected = "Medkit";
			}
			std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
		}
		else if (collision == 14) {
			click.Play();
			weapon = "Sword";
			std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
		}
		else if (collision == 15) {
			if (weapon_prices[1] == 0) {
				click.Play();
				weapon = "Bat";
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else if (cash >= weapon_prices[1]) {
				click.Play();
				cash -= weapon_prices[1];
				weapon = "Bat";
				weapon_prices[1] = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
		else if (collision == 16) {
			if (weapon_prices[2] == 0) {
				click.Play();
				weapon = "Claymore";
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else if (cash >= weapon_prices[2]) {
				click.Play();
				cash -= weapon_prices[2];
				weapon = "Claymore";
				weapon_prices[2] = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
		else if (collision == 17) {
			click.Play();
			tool = "Shield";
			std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
		}
		else if (collision == 18) {
			if (tool_prices[1] == 0) {
				click.Play();
				tool = "Big Shield";
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else if (cash >= tool_prices[1]) {
				click.Play();
				cash -= tool_prices[1];
				tool = "Big Shield";
				tool_prices[1] = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
			else {
				click_fail.Play();
				std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
			}
		}
		else if (collision == 19) {
				if (tool_prices[2] == 0) {
					click.Play();
					tool = "Thorned Shield";
					std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
				}
				else if (cash >= tool_prices[2]) {
					click.Play();
					cash -= tool_prices[2];
					tool = "Thorned Shield";
					tool_prices[2] = 0;
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

		if (!shielding && control) {
			player_pos[0] += round(dir_x * player_speed);
			player_pos[1] += round(dir_y * player_speed);
		}
		else if (control) {
			player_pos[0] += round(dir_x * 6);
			player_pos[1] += round(dir_y * 6);
		}
		if (thorn_dash) {
			if (game_time - thorned_shield_dash < 0.5) {
				if (attack_dir == 0) {
					player_pos[0] += 15;
					player_pos[1] += 0;
				}
				else if (attack_dir == 4) {
					player_pos[0] += 12;
					player_pos[1] += -12;
				}
				else if (attack_dir == 3) {
					player_pos[0] += 0;
					player_pos[1] += -15;
				}
				else if (attack_dir == 7) {
					player_pos[0] += -12;
					player_pos[1] += -12;
				}
				else if (attack_dir == 2) {
					player_pos[0] += -15;
					player_pos[1] += 0;
				}
				else if (attack_dir == 6) {
					player_pos[0] += -12;
					player_pos[1] += 12;
				}
				else if (attack_dir == 1) {
					player_pos[0] += 0;
					player_pos[1] += 15;
				}
				else if (attack_dir == 5) {
					player_pos[0] += 12;
					player_pos[1] += 12;
				}
			}
			else {
				control = true;
				thorn_dash = false;
				thorned_shield_cd = game_time;
			}
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
			if (!swing && !thorn_dash) {
				attack_dir = 0;
			}
		}
		else if (player_dir > 22 && player_dir < 68) {
			arrow_image = 4;
			if (!swing && !thorn_dash) {
				attack_dir = 4;
			}
		}
		else if (player_dir >= 68 && player_dir <= 112) {
			arrow_image = 3;
			if (!swing && !thorn_dash) {
				attack_dir = 3;
			}
		}
		else if (player_dir > 112 && player_dir < 158) {
			arrow_image = 7;
			if (!swing && !thorn_dash) {
				attack_dir = 7;
			}
		}
		else if (player_dir >= 158 && player_dir >= -158) {
			arrow_image = 2;
			if (!swing && !thorn_dash) {
				attack_dir = 2;
			}
		}
		else if (player_dir > -158 && player_dir < -112) {
			arrow_image = 6;
			if (!swing && !thorn_dash) {
				attack_dir = 6;
			}
		}
		else if (player_dir >= -112 && player_dir <= -68) {
			arrow_image = 1;
			if (!swing && !thorn_dash) {
				attack_dir = 1;
			}
		}
		else if (player_dir > -68 && player_dir <= -2) {
			arrow_image = 5;
			if (!swing && !thorn_dash) {
				attack_dir = 5;
			}
		}

		if (window.mouse.LeftIsPressed() && game_time - attack_cooldown >= weapon_cooldown && !shielding) {
			if (weapon == "Sword") {
				swing_sfx.Play();
			}
			else if (weapon == "Bat") {
				bat_swing.Play();
			}
			else if (weapon == "Claymore") {
				claymore_sfx.Play();
			}
			swing = true;
			frame = frames;
			attack_cooldown = game_time;
			claymore_anim = 0;
		}
		if (window.mouse.RightIsPressed() && !swing) {
			shielding = true;
			if (tool == "Thorned Shield" && game_time - thorned_shield_cd >= 2 && !dash_check) {
				thorn_dash = true;
				thorned_shield_dash = game_time;
				control = false;
				dash_check = true;
			}
			else if (tool == "Thorned Shield") {
				dash_check = true;
			}
		}
		else if (!window.mouse.RightIsPressed()) {
			shielding = false;
			dash_check = false;
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
				enemy_arr[i].spawn(4, enemy_spawn_hp + 5, 3, spawn_x, spawn_y);
			}
			break;
		}
	}
}

void Game::spawn_item() {
	int spawn_x = rand() % WIDTH;
	int spawn_y = rand() % HEIGHT;

	for (i = 0; i < 100; i++) {
		if (item_arr[i].type == 0) {
			if (item_spawn_pool[rand() % 100]) {
				if (item_selected == "Medkit") {
					item_arr[i].spawn(spawn_x, spawn_y, 1);
				}
				else {
					item_arr[i].spawn(spawn_x, spawn_y, 2);
				}
				break;
			}
			else {
				break;
			}
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
	for (i = 0; i < 100; i++) {
		item_arr[i].remove();
	}
	cash += score;
	start = true;
	game_over_check = true;
}