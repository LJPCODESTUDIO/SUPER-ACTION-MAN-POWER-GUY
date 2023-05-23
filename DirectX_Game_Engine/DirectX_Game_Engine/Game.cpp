#include "MainWindow.h"
#include "Game.h"
#include <chrono>
#include <thread>

Game::Game(MainWindow& _window) : window(_window), gui(_window) {}

void Game::game_loop() {
	if (start) {
		start_game();
		return;
	}
	track_1.Play();
	gui.set_Target_SubGui(1, 0);
	gui.set_RGB_on_Gui(black);

	write("Score: ", 0, 0, "chars_small");

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