#include "MainWindow.h"
#include "Game.h"
#include <chrono>
#include <thread>

Game::Game(MainWindow& _window) : window(_window), gui(_window) {}

void Game::game_loop() {
	if (initial) {
		start_time = clock();
		initial = false;
		track_1.Play();
	}
	check_time_and_fps(tick_60);
	if (game_time - track_time >= 192) {
		track_time = game_time;
		track_1.Play();
	}
	if (start) {
		start_game();
		return;
	}

	gui.set_Target_SubGui(1, 0);
	gui.set_RGB_on_Gui(black);

	player_image = 0;
	keyboard_key_press();

	gui.set_Image_at_Pixel(player_pos[0], player_pos[1], player[player_image]);

	write("Score: 0", 0, 0, "chars_small");
	write("Time: " + std::to_string(game_time), 0, 25, "chars_small");
	write("FPS: " + std::to_string(fps) + " High: " + std::to_string(high_low_fps[0]) + " Low: " + std::to_string(high_low_fps[1]), 0, 50, "chars_small");
	write("Pos: " + std::to_string(player_pos[0]) + "," + std::to_string(player_pos[1]), 0, 75, "chars_small");

	gui.refresh_Gui();

}

void Game::start_game() {

	gui.set_Target_SubGui(0, 0);
	gui.set_RGB_on_Gui(black);

	gui.set_Sprite_at_Pixel((WIDTH / 2) - 150, (HEIGHT / 2) - 75, play);

	write("SUPERACTION", (WIDTH / 2) - 425, 50);
	write("MANPOWERGUY", (WIDTH / 2) - 425, 150);
	
	gui.refresh_Gui();
	
	if (window.mouse.LeftIsPressed()) {
		if (gui.get_CollissionMap_Data_at_GuiRegion(gui.get_Mouse_Pos_at_GuiRegion()) == 1) {
			start = false;
		}
	}
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
}

void Game::keyboard_key_press() {
	if (gui.get_Gui_Elem() == 1) {
		int dir_x = window.kbd.KeyIsPressed(0x44) - window.kbd.KeyIsPressed(0x41);
		int dir_y = window.kbd.KeyIsPressed(0x53) - window.kbd.KeyIsPressed(0x57);
		int dash = window.kbd.KeyIsPressed(VK_SPACE) * 10;
		player_pos[0] += dir_x * (7 + dash);
		player_pos[1] += dir_y * (7 + dash);
		if (player_pos[0] >= 995) {
			player_pos[0] -= 7 + dash;
		}
		if (player_pos[0] <= 0) {
			player_pos[0] += 7 + dash;
		}
		if (player_pos[1] >= 870) {
			player_pos[1] -= 7 + dash;
		}
		if (player_pos[1] <= 0) {
			player_pos[1] += 7 + dash;
		}
	}
}