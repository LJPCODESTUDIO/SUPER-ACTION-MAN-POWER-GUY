#include "MainWindow.h"
#include "Gui.h"
#include <fstream>


Gui::Gui(class MainWindow& _app_window) : app_window(_app_window), gfx_ops(_app_window) {

	//SETUP SUB-GUIS HERE************************************************************************************************************************//
	total_gui_count = 3;

	subgui_count = new uint8_t * [total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) { subgui_count[g] = new uint8_t[1]; }

	subgui_count[0][0] = 1; // first gui total sub-guis
	subgui_count[1][0] = 2; // second gui total sub-guis
	subgui_count[2][0] = 1; // second gui total sub-guis

    //                                           0    1  2     3     4    5     6   7   8 
	uint16_t first_gui__first_subgui[9] = {      0,   0, 1200, 900,  15,  11,   0,  0,  0};

	uint16_t second_gui__first_subgui[9] = {     0,   0, 900,  900,  8,   8,    4,  4,  3};
	uint16_t second_gui__second_subgui[9] = {    900, 0, 1200, 900,  12,  37,   5,  5,  1};

	uint16_t third_gui__first_subgui[9] = {      0,   0, 1200, 900,  63,  47,   1,  4,  1};

	subgui_data = new uint16_t** [total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) {
		subgui_data[g] = new uint16_t* [subgui_count[g][0]];
		for (uint8_t i = 0; i < subgui_count[g][0]; i++) { 
			subgui_data[g][i] = new uint16_t[10];

			if (g == 0 && i == 0) for (uint8_t a = 0; a < 9; a++) { subgui_data[g][i][a] = first_gui__first_subgui[a]; }
			if (g == 1 && i == 0) for (uint8_t a = 0; a < 9; a++) { subgui_data[g][i][a] = second_gui__first_subgui[a]; }
			if (g == 1 && i == 1) for (uint8_t a = 0; a < 9; a++) { subgui_data[g][i][a] = second_gui__second_subgui[a]; }
			if (g == 2 && i == 0) for (uint8_t a = 0; a < 9; a++) { subgui_data[g][i][a] = third_gui__first_subgui[a]; }
		}
	}

	//*********************************************************************************************************************************************//

	subgui_led_data = new uint16_t*** [total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) {
		subgui_led_data[g] = new uint16_t** [subgui_count[g][0]];
		for (uint8_t i = 0; i < subgui_count[g][0]; i++) {
			subgui_led_data[g][i] = new uint16_t*[subgui_data[g][i][4] * subgui_data[g][i][5]];
			for (uint32_t j = 0; j < (subgui_data[g][i][4] * subgui_data[g][i][5]); j++) {
				subgui_led_data[g][i][j] = new uint16_t[4];
				for (uint8_t a = 0; a < 4; a++) { subgui_led_data[g][i][j][a] = 0; }
			}
		}
	}

	GS = new uint8_t**[total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) {
		GS[g] = new uint8_t* [gfx_ops.ScreenWidth * gfx_ops.ScreenHeight];
		for (uint32_t i = 0; i < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); i++) { 
			GS[g][i] = new uint8_t[10];
			for (uint8_t a = 0; a < 10; a++) { GS[g][i][a] = 0; }                 
		}
	}

	float temp_led_size_wrt_x = 0, temp_led_size_wrt_y = 0;

	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint8_t i = 0; i < subgui_count[g][0]; i++) {
			temp_led_size_wrt_y = ((float)((subgui_data[g][i][3] - subgui_data[g][i][1]) - subgui_data[g][i][7] - subgui_data[g][i][8] - (subgui_data[g][i][5] * subgui_data[g][i][8])) / subgui_data[g][i][5]);
			temp_led_size_wrt_x = ((float)((subgui_data[g][i][2] - subgui_data[g][i][0]) - subgui_data[g][i][6] - subgui_data[g][i][8] - (subgui_data[g][i][4] * subgui_data[g][i][8])) / subgui_data[g][i][4]);

			if (temp_led_size_wrt_y > 0 && temp_led_size_wrt_y == temp_led_size_wrt_x) subgui_data[g][i][9] = temp_led_size_wrt_x;
			else if (temp_led_size_wrt_y > 0 && temp_led_size_wrt_y < temp_led_size_wrt_x) subgui_data[g][i][9] = temp_led_size_wrt_y;
			else if (temp_led_size_wrt_x > 0 && temp_led_size_wrt_y > temp_led_size_wrt_x) subgui_data[g][i][9] = temp_led_size_wrt_x;

			for (int y = 0; y < subgui_data[g][i][5]; y++) {
				for (int x = 0; x < subgui_data[g][i][4]; x++) {
					subgui_led_data[g][i][y * subgui_data[g][i][4] + x][0] = (subgui_data[g][i][0] + subgui_data[g][i][6] + subgui_data[g][i][8] + (x * (subgui_data[g][i][9] + subgui_data[g][i][8])));
					subgui_led_data[g][i][y * subgui_data[g][i][4] + x][1] = subgui_led_data[g][i][y * subgui_data[g][i][4] + x][0] + subgui_data[g][i][9] - 1;

					subgui_led_data[g][i][y * subgui_data[g][i][4] + x][2] = (subgui_data[g][i][1] + subgui_data[g][i][7] + subgui_data[g][i][8] + (y * (subgui_data[g][i][9] + subgui_data[g][i][8])));
					subgui_led_data[g][i][y * subgui_data[g][i][4] + x][3] = subgui_led_data[g][i][y * subgui_data[g][i][4] + x][2] + subgui_data[g][i][9] - 1;
				}
			}
		}
	}
}

Gui::~Gui() {
	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint8_t i = 0; i < subgui_count[g][0]; i++) {
			for (uint32_t k = 0; k < (subgui_data[g][i][4] * subgui_data[g][i][5]); k++) { 
				delete[] subgui_led_data[g][i][k]; subgui_led_data[g][i][k] = nullptr; 
			} delete[] subgui_led_data[g][i]; subgui_led_data[g][i] = nullptr;
		} delete[] subgui_led_data[g]; subgui_led_data[g] = nullptr;
	} delete[] subgui_led_data; subgui_led_data = nullptr;

	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint8_t i = 0; i < subgui_count[g][0]; i++) { 
			delete[] subgui_data[g][i]; 
			subgui_data[g][i] = nullptr;
		} delete[] subgui_data[g]; subgui_data[g] = nullptr;
	} delete[] subgui_data; subgui_data = nullptr;

	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint32_t j = 0; j < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); j++) {
			delete[] GS[g][j]; GS[g][j] = nullptr;
		} delete[] GS[g]; GS[g] = nullptr;
	} delete[] GS; GS = nullptr;

	for (uint8_t g = 0; g < total_gui_count; g++) { delete[] subgui_count[g]; subgui_count[g] = nullptr; }
	delete[] subgui_count; subgui_count = nullptr;
}

void Gui::refresh_Gui() {
	gfx_ops.BeginFrame();
	for (uint16_t y = 0; y < gfx_ops.ScreenHeight; y++) {
		for (uint16_t x = 0; x < gfx_ops.ScreenWidth; x++) {
			gfx_ops.PutPixel(x, y, GS[gui_elem][y * gfx_ops.ScreenWidth + x][0], GS[gui_elem][y * gfx_ops.ScreenWidth + x][1], GS[gui_elem][y * gfx_ops.ScreenWidth + x][2]);
		}
	}
	gfx_ops.EndFrame();
}


uint8_t Gui::get_Gui_Elem() { return gui_elem; }

uint8_t Gui::get_SubGui_Elem() { return subgui_elem; }

void Gui::set_Target_SubGui(uint8_t _gui_elem, uint8_t _subgui_elem) { gui_elem = _gui_elem; subgui_elem = _subgui_elem; }


void Gui::set_RGB_on_Gui(uint8_t* _rgb) {
	for (uint32_t k = 0; k < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); k++) {
		for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][k][a] = _rgb[a]; }
	}
}

void Gui::set_RGB_on_SubGui(uint8_t* _rgb) {
	for (uint16_t y = subgui_data[gui_elem][subgui_elem][1]; y < subgui_data[gui_elem][subgui_elem][3]; y++) {
		for (uint16_t x = subgui_data[gui_elem][subgui_elem][0]; x < subgui_data[gui_elem][subgui_elem][2]; x++) {
			for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _rgb[a]; }
		}
	}
}

void Gui::set_RGB_on_LED(uint16_t _led_elem, uint8_t* _rgb) {
	for (uint16_t y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2]; y <= subgui_led_data[gui_elem][subgui_elem][_led_elem][3]; y++) {
		for (uint16_t x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0]; x <= subgui_led_data[gui_elem][subgui_elem][_led_elem][1]; x++) {
			for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _rgb[a]; }
		}
	}
}

void Gui::set_RGB_on_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t* _rgb) { for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _rgb[a]; } }

void Gui::set_RGB_on_Sprite_Region_ID(uint8_t _sprite_region_id, uint8_t* _rgb) {
	for (uint32_t i = 0; i < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); i++) {
		if (GS[gui_elem][i][6] == _sprite_region_id) { for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][i][a] = _rgb[a]; } }
	}
}

void Gui::set_RGB_on_LED_Border(uint16_t _led_elem, uint8_t* _led_border_rgb, bool _n, bool _e, bool _s, bool _w) {
	for (uint16_t y = (subgui_led_data[gui_elem][subgui_elem][_led_elem][2] - subgui_data[gui_elem][subgui_elem][8]); y <= (subgui_led_data[gui_elem][subgui_elem][_led_elem][3] + subgui_data[gui_elem][subgui_elem][8]); y++) {
		for (uint16_t x = (subgui_led_data[gui_elem][subgui_elem][_led_elem][0] - subgui_data[gui_elem][subgui_elem][8]); x <= (subgui_led_data[gui_elem][subgui_elem][_led_elem][1] + subgui_data[gui_elem][subgui_elem][8]); x++) {
			if (_n && y < subgui_led_data[gui_elem][subgui_elem][_led_elem][2]) {
				for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _led_border_rgb[a]; }
			}
			if (_e && x > subgui_led_data[gui_elem][subgui_elem][_led_elem][1]) {
				for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _led_border_rgb[a]; }
			}
			if (_s && y > subgui_led_data[gui_elem][subgui_elem][_led_elem][3]) {
				for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _led_border_rgb[a]; }
			}
			if (_w && x < subgui_led_data[gui_elem][subgui_elem][_led_elem][0]) {
				for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _led_border_rgb[a]; }
			}
		}
	}
}


void Gui::set_Image_on_Gui(Image& _image) {

	if ((gfx_ops.ScreenHeight >= _image.height) && (gfx_ops.ScreenWidth >= _image.width)) {

		uint16_t surface_y = (gfx_ops.ScreenHeight - _image.height) / 2;
		uint16_t surface_x = (gfx_ops.ScreenWidth - _image.width) / 2;

		for (uint16_t y = 0; y < _image.height; y++) {
			for (uint16_t x = 0; x < _image.width; x++) {
				if (_image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				surface_x++;
			}
			surface_y++;
			surface_x = ((gfx_ops.ScreenWidth - _image.width) / 2);
		}
	}
}

void Gui::set_Image_on_SubGui(Image& _image) {

	if (((subgui_data[gui_elem][subgui_elem][3] - subgui_data[gui_elem][subgui_elem][1]) >= _image.height) &&
		((subgui_data[gui_elem][subgui_elem][2] - subgui_data[gui_elem][subgui_elem][0]) >= _image.width)) {

		uint16_t surface_y = subgui_data[gui_elem][subgui_elem][1] + ((subgui_data[gui_elem][subgui_elem][3] - subgui_data[gui_elem][subgui_elem][1]) - _image.height) / 2;
		uint16_t surface_x = subgui_data[gui_elem][subgui_elem][0] + ((subgui_data[gui_elem][subgui_elem][2] - subgui_data[gui_elem][subgui_elem][0]) - _image.width) / 2;

		for (uint16_t y = 0; y < _image.height; y++) {
			for (uint16_t x = 0; x < _image.width; x++) {
				if (_image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				surface_x++;
			}
			surface_y++;
			surface_x = subgui_data[gui_elem][subgui_elem][0] + ((subgui_data[gui_elem][subgui_elem][2] - subgui_data[gui_elem][subgui_elem][0]) - _image.width) / 2;
		}
	}
}

void Gui::set_Image_on_LED(uint16_t _led_elem, Image& _image) {

	if ((subgui_data[gui_elem][subgui_elem][9] >= _image.width) &&
		(subgui_data[gui_elem][subgui_elem][9] >= _image.height)) {

		uint16_t surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0] + ((subgui_data[gui_elem][subgui_elem][9] - _image.width) / 2);
		uint16_t surface_y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2] + ((subgui_data[gui_elem][subgui_elem][9] - _image.height) / 2);

		for (uint16_t y = 0; y < _image.height; y++) {
			for (uint16_t x = 0; x < _image.width; x++) {
				if (_image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { 
						GS[gui_elem][surface_y * gfx_ops.ScreenWidth + surface_x][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; 
					}
				}
				surface_x++;
			}
			surface_y++;
			surface_x -= _image.width;
		}
	}
}

void Gui::set_Image_at_LED(uint16_t _led_elem, Image& _image) {

	if (((subgui_data[gui_elem][subgui_elem][3] - subgui_led_data[gui_elem][subgui_elem][_led_elem][2]) >= _image.height) &&
		((subgui_data[gui_elem][subgui_elem][2] - subgui_led_data[gui_elem][subgui_elem][_led_elem][0]) >= _image.width)) {

		uint16_t surface_y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2];
		uint16_t surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0];

		for (uint16_t y = 0; y < _image.height; y++) {
			for (uint16_t x = 0; x < _image.width; x++) {
				for (uint8_t a = 0; a < 3; a++) {
					if (_image.rgb[_image.elem][y * _image.width + x][0]) {
						GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1];
					}
				}
				surface_x++;
			}
			surface_y++;
			surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0];
		}
	}
}

void Gui::set_Image_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Image& _image) {
	if ((gfx_ops.ScreenHeight >= (_surface_x + _image.height)) && (gfx_ops.ScreenWidth >= (_surface_x + _image.width))) {
		for (uint16_t y = 0; y < _image.height; y++) {
			for (uint16_t x = 0; x < _image.width; x++) {
				if (_image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _image.width;
			_surface_y++;
		}
	}
}


void Gui::set_Sprite_on_Gui(Sprite& _sprite) {

	if ((gfx_ops.ScreenHeight >= _sprite.height) && (gfx_ops.ScreenWidth >= _sprite.width)) {

		uint16_t surface_y = (gfx_ops.ScreenHeight - _sprite.height) / 2;
		uint16_t surface_x = (gfx_ops.ScreenWidth - _sprite.width) / 2;

		for (uint16_t y = 0; y < _sprite.height; y++) {
			for (uint16_t x = 0; x < _sprite.width; x++) {
				if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a + 7] = GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
				}
				surface_x++;
			}
			surface_y++;
			surface_x = ((gfx_ops.ScreenWidth - _sprite.width) / 2);
		}
	}
}

void Gui::set_Sprite_on_SubGui(Sprite& _sprite) {

	if (((subgui_data[gui_elem][subgui_elem][3] - subgui_data[gui_elem][subgui_elem][1]) >= _sprite.height) &&
		((subgui_data[gui_elem][subgui_elem][2] - subgui_data[gui_elem][subgui_elem][0]) >= _sprite.width)) {

		uint16_t surface_y = subgui_data[gui_elem][subgui_elem][1] + ((subgui_data[gui_elem][subgui_elem][3] - subgui_data[gui_elem][subgui_elem][1]) - _sprite.height) / 2;
		uint16_t surface_x = subgui_data[gui_elem][subgui_elem][0] + ((subgui_data[gui_elem][subgui_elem][2] - subgui_data[gui_elem][subgui_elem][0]) - _sprite.width) / 2;

		for (uint16_t y = 0; y < _sprite.height; y++) {
			for (uint16_t x = 0; x < _sprite.width; x++) {
				if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a + 7] = GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
				}
				surface_x++;
			}
			surface_y++;
			surface_x = subgui_data[gui_elem][subgui_elem][0] + ((subgui_data[gui_elem][subgui_elem][2] - subgui_data[gui_elem][subgui_elem][0]) - _sprite.width) / 2;
		}
	}
}

void Gui::set_Sprite_at_LED(uint16_t _led_elem, Sprite& _sprite) {

	if (((gfx_ops.ScreenHeight - subgui_led_data[gui_elem][subgui_elem][_led_elem][2]) >= _sprite.height) &&
		((gfx_ops.ScreenWidth - subgui_led_data[gui_elem][subgui_elem][_led_elem][0]) >= _sprite.width)) {

		uint16_t surface_y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2];
		uint16_t surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0];

		for (uint16_t y = 0; y < _sprite.height; y++) {
			for (uint16_t x = 0; x < _sprite.width; x++) {
				if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a + 7] = GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(surface_y * gfx_ops.ScreenWidth + surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
				}
				surface_x++;
			}
			surface_y++;
			surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0];
		}
	}
}

void Gui::set_Sprite_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, std::string _origin) {
	if (_origin == "nw") {
		for (uint16_t y = 0; y < _sprite.height; y++) {
			for (uint16_t x = 0; x < _sprite.width; x++) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth) {
					if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
						GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
					}
				}
				_surface_x++;
			}
			_surface_x -= _sprite.width;
			_surface_y++;
		}
	}
	else if (_origin == "ne") {
		for (uint16_t y = 0; y < _sprite.height; y++) {
			for (int16_t x = _sprite.width - 1; x > -1; x--) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth) {
					if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
						GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
					}
				}
				_surface_x--;
			}
			_surface_x += _sprite.width;
			_surface_y++;
		}
	}
	else if (_origin == "sw") {
		for (int16_t y = _sprite.height - 1; y > -1; y--) {
			for (uint16_t x = 0; x < _sprite.width; x++) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth) {
					if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
						GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
					}
				}
				_surface_x++;
			}
			_surface_x -= _sprite.width;
			_surface_y--;
		}
	}
	else if (_origin == "se") {
		for (int16_t y = _sprite.height - 1; y > -1; y--) {
			for (int16_t x = _sprite.width - 1; x > -1; x--) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth) {
					if (_sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
						GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
						for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
					}
				}
				_surface_x--;
			}
			_surface_x += _sprite.width;
			_surface_y--;
		}
	}
}

void Gui::set_Sprite_Off(Sprite& _sprite) {
	for (uint32_t k = 0; k < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); k++) {
		if (GS[gui_elem][k][6] >= _sprite.id && GS[gui_elem][k][6] <= (_sprite.id + _sprite.total_regions - 1)) {
			GS[gui_elem][k][6] = 0;
			for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][k][a] = GS[gui_elem][k][a + 7];}
		}
	}
}


void Gui::set_Collission_Map_at_LED(uint16_t _led_elem, uint8_t _region_id) {
	for (uint16_t _surface_y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2]; _surface_y <= subgui_led_data[gui_elem][subgui_elem][_led_elem][3]; _surface_y++) {
		for (uint16_t _surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0]; _surface_x <= subgui_led_data[gui_elem][subgui_elem][_led_elem][1]; _surface_x++) {
			GS[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6] = _region_id;
		}
	}
}

void Gui::set_Collission_Map_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t _region_id) { GS[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6] = _region_id; }

void Gui::write_to_Text_File(std::string _text) {
	std::ofstream GuiFile;
	GuiFile.open("Text_File/Text_File.txt", std::ios_base::app);
	GuiFile << _text << "\n";
	GuiFile.close();
}


uint8_t Gui::get_Collission_Map_at_LED(uint16_t _led_elem) {
	for (uint16_t  _surface_y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2]; _surface_y <= subgui_led_data[gui_elem][subgui_elem][_led_elem][3]; _surface_y++) {
		for (uint16_t _surface_x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0]; _surface_x <= subgui_led_data[gui_elem][subgui_elem][_led_elem][1]; _surface_x++) {
			if (GS[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6] > 0) return GS[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6];
		}
	}
}

uint8_t Gui::get_Collission_Map_at_Pixel(uint16_t _surface_x, uint16_t _surface_y) { return GS[gui_elem][_surface_y*gfx_ops.ScreenWidth+ _surface_x][6]; }


void Gui::save_LED(uint16_t _led_elem) {
	for (uint16_t y = subgui_led_data[gui_elem][subgui_elem][_led_elem][2]; y <= subgui_led_data[gui_elem][subgui_elem][_led_elem][3]; y++) {
		for (uint16_t x = subgui_led_data[gui_elem][subgui_elem][_led_elem][0]; x <= subgui_led_data[gui_elem][subgui_elem][_led_elem][1]; x++) {
			for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(y * gfx_ops.ScreenWidth + x)][a+3] = GS[gui_elem][(y * gfx_ops.ScreenWidth + x)][a]; }
		}
	}
}

void Gui::save_Pixel(uint16_t _surface_x, uint16_t _surface_y){
	for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 3] = GS[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
}

void Gui::load_LED(uint16_t _source_led_elem, int16_t _target_led_elem) {
	if (_target_led_elem < 0) _target_led_elem = _source_led_elem;

	uint16_t target_y = subgui_led_data[gui_elem][subgui_elem][_target_led_elem][2];
	uint16_t target_x = subgui_led_data[gui_elem][subgui_elem][_target_led_elem][0];

	for (uint16_t source_y = subgui_led_data[gui_elem][subgui_elem][_source_led_elem][2]; source_y <= subgui_led_data[gui_elem][subgui_elem][_source_led_elem][3]; source_y++) {
		for (uint16_t source_x = subgui_led_data[gui_elem][subgui_elem][_source_led_elem][0]; source_x <= subgui_led_data[gui_elem][subgui_elem][_source_led_elem][1]; source_x++) {
			for (uint8_t a = 0; a < 3; a++) { GS[gui_elem][(target_y * gfx_ops.ScreenWidth + target_x)][a] = GS[gui_elem][(source_y * gfx_ops.ScreenWidth + source_x)][a+3];}
			target_x++;
		}
		target_x = subgui_led_data[gui_elem][subgui_elem][_target_led_elem][0];
		target_y++;
	}
}

void Gui::load_Pixel(uint16_t _source_surface_x, uint16_t _source_surface_y, int16_t _target_surface_x, int16_t _target_surface_y) {
	if (_target_surface_x < 0) _target_surface_x = _source_surface_x;
	if (_target_surface_y < 0) _target_surface_y = _source_surface_y;

	for (uint8_t a = 0; a < 3; a++) { 
		GS[gui_elem][(_target_surface_y * gfx_ops.ScreenWidth + _target_surface_x)][a] = GS[gui_elem][(_source_surface_y * gfx_ops.ScreenWidth + _source_surface_x)][a + 3]; }
}

int16_t Gui::get_Mouse_Pos_at_LED() {
	uint16_t mouse_pos_x = app_window.mouse.GetPosX(), mouse_pos_y = app_window.mouse.GetPosY();

	for (uint16_t k = 0; k < (subgui_data[gui_elem][subgui_elem][4] * subgui_data[gui_elem][subgui_elem][5]); k++) {
		if (mouse_pos_x >= subgui_led_data[gui_elem][subgui_elem][k][0] && mouse_pos_x <= subgui_led_data[gui_elem][subgui_elem][k][1] &&
			mouse_pos_y >= subgui_led_data[gui_elem][subgui_elem][k][2] && mouse_pos_y <= subgui_led_data[gui_elem][subgui_elem][k][3]) {
			return k;
		}
	}
	return (- 1);
}

uint16_t Gui::get_LED_Size() { return subgui_data[gui_elem][subgui_elem][9]; }