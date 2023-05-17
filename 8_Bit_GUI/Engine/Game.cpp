#include "MainWindow.h"
#include "Game.h"
#include <chrono>
#include <thread>

Game::Game(MainWindow& _window) : window(_window), gui(_window) {}

void Game::game_loop() {
	if (start) {
		setup();
	}
	gui.refresh_Gui();
}

void Game::setup() {
	gui.set_Target_SubGui(0, 0);
	gui.set_RGB_on_Gui(black);

	gui.set_Image_at_LED(17, funky_chars[51]); //S
	gui.set_Image_at_LED(18, funky_chars[53]); //U
	gui.set_Image_at_LED(19, funky_chars[48]); //P
	gui.set_Image_at_LED(20, funky_chars[37]); //E
	gui.set_Image_at_LED(21, funky_chars[50]); //R
	gui.set_Image_at_LED(22, funky_chars[33]); //A
	gui.set_Image_at_LED(23, funky_chars[35]); //C
	gui.set_Image_at_LED(24, funky_chars[52]); //T
	gui.set_Image_at_LED(25, funky_chars[41]); //I
	gui.set_Image_at_LED(26, funky_chars[47]); //O
	gui.set_Image_at_LED(27, funky_chars[46]); //N
	gui.set_Image_at_LED(32, funky_chars[45]); //M
	gui.set_Image_at_LED(33, funky_chars[33]); //A
	gui.set_Image_at_LED(34, funky_chars[46]); //N
	gui.set_Image_at_LED(35, funky_chars[48]); //P
	gui.set_Image_at_LED(36, funky_chars[47]); //O
	gui.set_Image_at_LED(37, funky_chars[55]); //W
	gui.set_Image_at_LED(38, funky_chars[37]); //E
	gui.set_Image_at_LED(39, funky_chars[50]); //R
	gui.set_Image_at_LED(40, funky_chars[39]); //G
	gui.set_Image_at_LED(41, funky_chars[53]); //U
	gui.set_Image_at_LED(42, funky_chars[57]); //Y
}