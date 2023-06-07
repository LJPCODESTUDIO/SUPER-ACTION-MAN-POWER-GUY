#include "MainWindow.h"
#include "Gui.h"
#include <fstream>


Gui::Gui(class MainWindow& _app_window) : app_window(_app_window), gfx_ops(_app_window) {

	//SETUP SUB-GUIS HERE************************************************************************************************************************//
	total_gui_count = 3;

	SubGui_count = new uint8_t * [total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) { SubGui_count[g] = new uint8_t[1]; }

	SubGui_count[0][0] = 1; // first gui total sub-guis
	SubGui_count[1][0] = 2; // second gui total sub-guis
	SubGui_count[2][0] = 1; // second gui total sub-guis

	//                                           0    1  2     3     4    5     6   7   8 
	uint16_t first_gui__first_SubGui[9] = { 0,   0, 1200, 900,  15,  11,   0,  0,  0 };

	uint16_t second_gui__first_SubGui[9] = { 0,   0, 900,  900,  8,   8,    4,  4,  3 };
	uint16_t second_gui__second_SubGui[9] = { 900, 0, 1200, 900,  12,  37,   5,  5,  1 };

	uint16_t third_gui__first_SubGui[9] = { 0,   0, 1200, 900,  63,  47,   1,  4,  1 };

	SubGui_data = new uint16_t * *[total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) {
		SubGui_data[g] = new uint16_t * [SubGui_count[g][0]];
		for (uint8_t i = 0; i < SubGui_count[g][0]; i++) {
			SubGui_data[g][i] = new uint16_t[10];

			if (g == 0 && i == 0) for (uint8_t a = 0; a < 9; a++) { SubGui_data[g][i][a] = first_gui__first_SubGui[a]; }
			if (g == 1 && i == 0) for (uint8_t a = 0; a < 9; a++) { SubGui_data[g][i][a] = second_gui__first_SubGui[a]; }
			if (g == 1 && i == 1) for (uint8_t a = 0; a < 9; a++) { SubGui_data[g][i][a] = second_gui__second_SubGui[a]; }
			if (g == 2 && i == 0) for (uint8_t a = 0; a < 9; a++) { SubGui_data[g][i][a] = third_gui__first_SubGui[a]; }
		}
	}

	//*********************************************************************************************************************************************//

	GuiRegion_data = new uint16_t * **[total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) {
		GuiRegion_data[g] = new uint16_t * *[SubGui_count[g][0]];
		for (uint8_t i = 0; i < SubGui_count[g][0]; i++) {
			GuiRegion_data[g][i] = new uint16_t * [SubGui_data[g][i][4] * SubGui_data[g][i][5]];
			for (uint32_t j = 0; j < (SubGui_data[g][i][4] * SubGui_data[g][i][5]); j++) {
				GuiRegion_data[g][i][j] = new uint16_t[4];
				for (uint8_t a = 0; a < 4; a++) { GuiRegion_data[g][i][j][a] = 0; }
			}
		}
	}

	GuiSurface = new uint8_t * *[total_gui_count];
	for (uint8_t g = 0; g < total_gui_count; g++) {
		GuiSurface[g] = new uint8_t * [gfx_ops.ScreenWidth * gfx_ops.ScreenHeight];
		for (uint32_t i = 0; i < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); i++) {
			GuiSurface[g][i] = new uint8_t[10];
			for (uint8_t a = 0; a < 10; a++) { GuiSurface[g][i][a] = 0; }
		}
	}

	float temp_GuiRegion_size_wrt_x = 0, temp_GuiRegion_size_wrt_y = 0;

	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint8_t i = 0; i < SubGui_count[g][0]; i++) {
			temp_GuiRegion_size_wrt_y = ((float)((SubGui_data[g][i][3] - SubGui_data[g][i][1]) - SubGui_data[g][i][7] - SubGui_data[g][i][8] - (SubGui_data[g][i][5] * SubGui_data[g][i][8])) / SubGui_data[g][i][5]);
			temp_GuiRegion_size_wrt_x = ((float)((SubGui_data[g][i][2] - SubGui_data[g][i][0]) - SubGui_data[g][i][6] - SubGui_data[g][i][8] - (SubGui_data[g][i][4] * SubGui_data[g][i][8])) / SubGui_data[g][i][4]);

			if (temp_GuiRegion_size_wrt_y > 0 && temp_GuiRegion_size_wrt_y == temp_GuiRegion_size_wrt_x) SubGui_data[g][i][9] = temp_GuiRegion_size_wrt_x;
			else if (temp_GuiRegion_size_wrt_y > 0 && temp_GuiRegion_size_wrt_y < temp_GuiRegion_size_wrt_x) SubGui_data[g][i][9] = temp_GuiRegion_size_wrt_y;
			else if (temp_GuiRegion_size_wrt_x > 0 && temp_GuiRegion_size_wrt_y > temp_GuiRegion_size_wrt_x) SubGui_data[g][i][9] = temp_GuiRegion_size_wrt_x;

			for (int y = 0; y < SubGui_data[g][i][5]; y++) {
				for (int x = 0; x < SubGui_data[g][i][4]; x++) {
					GuiRegion_data[g][i][y * SubGui_data[g][i][4] + x][0] = (SubGui_data[g][i][0] + SubGui_data[g][i][6] + SubGui_data[g][i][8] + (x * (SubGui_data[g][i][9] + SubGui_data[g][i][8])));
					GuiRegion_data[g][i][y * SubGui_data[g][i][4] + x][1] = GuiRegion_data[g][i][y * SubGui_data[g][i][4] + x][0] + SubGui_data[g][i][9] - 1;

					GuiRegion_data[g][i][y * SubGui_data[g][i][4] + x][2] = (SubGui_data[g][i][1] + SubGui_data[g][i][7] + SubGui_data[g][i][8] + (y * (SubGui_data[g][i][9] + SubGui_data[g][i][8])));
					GuiRegion_data[g][i][y * SubGui_data[g][i][4] + x][3] = GuiRegion_data[g][i][y * SubGui_data[g][i][4] + x][2] + SubGui_data[g][i][9] - 1;
				}
			}
		}
	}
}

Gui::~Gui() {
	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint8_t i = 0; i < SubGui_count[g][0]; i++) {
			for (uint32_t k = 0; k < (SubGui_data[g][i][4] * SubGui_data[g][i][5]); k++) {
				delete[] GuiRegion_data[g][i][k]; GuiRegion_data[g][i][k] = nullptr;
			} delete[] GuiRegion_data[g][i]; GuiRegion_data[g][i] = nullptr;
		} delete[] GuiRegion_data[g]; GuiRegion_data[g] = nullptr;
	} delete[] GuiRegion_data; GuiRegion_data = nullptr;

	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint8_t i = 0; i < SubGui_count[g][0]; i++) {
			delete[] SubGui_data[g][i];
			SubGui_data[g][i] = nullptr;
		} delete[] SubGui_data[g]; SubGui_data[g] = nullptr;
	} delete[] SubGui_data; SubGui_data = nullptr;

	for (uint8_t g = 0; g < total_gui_count; g++) {
		for (uint32_t j = 0; j < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); j++) {
			delete[] GuiSurface[g][j]; GuiSurface[g][j] = nullptr;
		} delete[] GuiSurface[g]; GuiSurface[g] = nullptr;
	} delete[] GuiSurface; GuiSurface = nullptr;

	for (uint8_t g = 0; g < total_gui_count; g++) { delete[] SubGui_count[g]; SubGui_count[g] = nullptr; }
	delete[] SubGui_count; SubGui_count = nullptr;
}


uint16_t Gui::get_GuiRegion_Size() { return SubGui_data[gui_elem][SubGui_elem][9]; }


void Gui::refresh_Gui() {
	gfx_ops.BeginFrame();
	for (uint16_t y = 0; y < gfx_ops.ScreenHeight; y++) {
		for (uint16_t x = 0; x < gfx_ops.ScreenWidth; x++) {
			gfx_ops.PutPixel(x, y, GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][0], GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][1], GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][2]);
		}
	}
	gfx_ops.EndFrame();
}


void Gui::set_Target_SubGui(uint8_t _gui_elem, uint8_t _SubGui_elem) { gui_elem = _gui_elem; SubGui_elem = _SubGui_elem; }

uint8_t Gui::get_Gui_Elem() { return gui_elem; }

uint8_t Gui::get_SubGui_Elem() { return SubGui_elem; }


int16_t Gui::get_Mouse_Pos_at_GuiRegion() {
	uint16_t mouse_pos_x = app_window.mouse.GetPosX(), mouse_pos_y = app_window.mouse.GetPosY();

	for (uint16_t k = 0; k < (SubGui_data[gui_elem][SubGui_elem][4] * SubGui_data[gui_elem][SubGui_elem][5]); k++) {
		if (mouse_pos_x >= GuiRegion_data[gui_elem][SubGui_elem][k][0] && mouse_pos_x <= GuiRegion_data[gui_elem][SubGui_elem][k][1] &&
			mouse_pos_y >= GuiRegion_data[gui_elem][SubGui_elem][k][2] && mouse_pos_y <= GuiRegion_data[gui_elem][SubGui_elem][k][3]) {
			return k;
		}
	}
	return (-1);
}



void Gui::set_RGB_on_Gui(uint8_t* _rgb) {
	for (uint32_t k = 0; k < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); k++) {
		for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][k][a] = _rgb[a]; }
	}
}

void Gui::set_RGB_on_SubGui(uint8_t* _rgb) {
	for (uint16_t y = SubGui_data[gui_elem][SubGui_elem][1]; y < SubGui_data[gui_elem][SubGui_elem][3]; y++) {
		for (uint16_t x = SubGui_data[gui_elem][SubGui_elem][0]; x < SubGui_data[gui_elem][SubGui_elem][2]; x++) {
			for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _rgb[a]; }
		}
	}
}

void Gui::set_RGB_on_GuiRegion(uint16_t _GuiRegion_elem, uint8_t* _rgb) {
	for (uint16_t y = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2]; y <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][3]; y++) {
		for (uint16_t x = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0]; x <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][1]; x++) {
			for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _rgb[a]; }
		}
	}
}

void Gui::set_RGB_on_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t* _rgb) { for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _rgb[a]; } }

void Gui::set_RGB_on_SpriteRegion(uint8_t _sprite_region_id, uint8_t* _rgb) {
	for (uint32_t i = 0; i < (gfx_ops.ScreenWidth * gfx_ops.ScreenHeight); i++) {
		if (GuiSurface[gui_elem][i][6] == _sprite_region_id) { for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][i][a] = _rgb[a]; } }
	}
}

void Gui::set_RGB_on_GuiRegion_Border(uint16_t _GuiRegion_elem, uint8_t* _GuiRegion_border_rgb, bool _n, bool _e, bool _s, bool _w) {
	for (uint16_t y = (GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2] - SubGui_data[gui_elem][SubGui_elem][8]); y <= (GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][3] + SubGui_data[gui_elem][SubGui_elem][8]); y++) {
		for (uint16_t x = (GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0] - SubGui_data[gui_elem][SubGui_elem][8]); x <= (GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][1] + SubGui_data[gui_elem][SubGui_elem][8]); x++) {
			if (_n && y < GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2]) {
				for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _GuiRegion_border_rgb[a]; }
			}
			if (_e && x > GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][1]) {
				for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _GuiRegion_border_rgb[a]; }
			}
			if (_s && y > GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][3]) {
				for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _GuiRegion_border_rgb[a]; }
			}
			if (_w && x < GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0]) {
				for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][y * gfx_ops.ScreenWidth + x][a] = _GuiRegion_border_rgb[a]; }
			}
		}
	}
}


void Gui::set_Image_on_Gui(Image& _image) {
	if (gfx_ops.ScreenHeight >= _image.height && gfx_ops.ScreenWidth >= _image.width) {
		set_Image_at_Pixel((gfx_ops.ScreenWidth - _image.width) / 2, (gfx_ops.ScreenHeight - _image.height) / 2, _image);
	}
}

void Gui::set_Image_on_SubGui(Image& _image) {
	if (SubGui_data[gui_elem][SubGui_elem][3] - SubGui_data[gui_elem][SubGui_elem][1] >= _image.height && SubGui_data[gui_elem][SubGui_elem][2] - SubGui_data[gui_elem][SubGui_elem][0] >= _image.width) {
		set_Image_at_Pixel(SubGui_data[gui_elem][SubGui_elem][0] + ((SubGui_data[gui_elem][SubGui_elem][2] - SubGui_data[gui_elem][SubGui_elem][0]) - _image.width) / 2, SubGui_data[gui_elem][SubGui_elem][1] + ((SubGui_data[gui_elem][SubGui_elem][3] - SubGui_data[gui_elem][SubGui_elem][1]) - _image.height) / 2, _image);
	}
}

void Gui::set_Image_on_GuiRegion(uint16_t _GuiRegion_elem, Image& _image) {
	if (SubGui_data[gui_elem][SubGui_elem][9] >= _image.width && SubGui_data[gui_elem][SubGui_elem][9] >= _image.height) {
		set_Image_at_Pixel(GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0] + ((SubGui_data[gui_elem][SubGui_elem][9] - _image.width) / 2), GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2] + ((SubGui_data[gui_elem][SubGui_elem][9] - _image.height) / 2), _image);
	}
}

void Gui::set_Image_at_GuiRegion(uint16_t _GuiRegion_elem, Image& _image, std::string _origin) { set_Image_at_Pixel(GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0], GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2], _image, _origin); }

void Gui::set_Image_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Image& _image, std::string _origin) {
	if (_origin == "nw") {
		for (uint16_t y = 0; y < _image.height; y++) {
			for (uint16_t x = 0; x < _image.width; x++) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _image.width;
			_surface_y++;
		}
	}
	else if (_origin == "ne") {
		for (uint16_t y = 0; y < _image.height; y++) {
			for (int16_t x = _image.width - 1; x > -1; x--) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				_surface_x--;
			}
			_surface_x += _image.width;
			_surface_y++;
		}
	}
	else if (_origin == "sw") {
		for (int16_t y = _image.height - 1; y > -1; y--) {
			for (uint16_t x = 0; x < _image.width; x++) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _image.width;
			_surface_y--;
		}
	}
	else if (_origin == "se") {
		for (int16_t y = _image.height - 1; y > -1; y--) {
			for (int16_t x = _image.width - 1; x > -1; x--) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _image.rgb[_image.elem][y * _image.width + x][0]) {
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _image.rgb[_image.elem][y * _image.width + x][a + 1]; }
				}
				_surface_x--;
			}
			_surface_x += _image.width;
			_surface_y--;
		}
	}
}


void Gui::set_Sprite_on_Gui(Sprite& _sprite) {
	if (gfx_ops.ScreenHeight >= _sprite.height && gfx_ops.ScreenWidth >= _sprite.width) {
		set_Sprite_at_Pixel((gfx_ops.ScreenWidth - _sprite.width) / 2, (gfx_ops.ScreenHeight - _sprite.height) / 2, _sprite);
	}
}

void Gui::set_Sprite_on_SubGui(Sprite& _sprite) {
	if (SubGui_data[gui_elem][SubGui_elem][3] - SubGui_data[gui_elem][SubGui_elem][1] >= _sprite.height && SubGui_data[gui_elem][SubGui_elem][2] - SubGui_data[gui_elem][SubGui_elem][0] >= _sprite.width) {
		set_Sprite_at_Pixel(SubGui_data[gui_elem][SubGui_elem][0] + ((SubGui_data[gui_elem][SubGui_elem][2] - SubGui_data[gui_elem][SubGui_elem][0]) - _sprite.width) / 2, SubGui_data[gui_elem][SubGui_elem][1] + ((SubGui_data[gui_elem][SubGui_elem][3] - SubGui_data[gui_elem][SubGui_elem][1]) - _sprite.height) / 2, _sprite);
	}
}

void Gui::set_Sprite_on_GuiRegion(uint16_t _GuiRegion_elem, Sprite& _sprite) {
	if (SubGui_data[gui_elem][SubGui_elem][9] >= _sprite.width && SubGui_data[gui_elem][SubGui_elem][9] >= _sprite.height) {
		set_Sprite_at_Pixel(GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0] + ((SubGui_data[gui_elem][SubGui_elem][9] - _sprite.width) / 2), GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2] + ((SubGui_data[gui_elem][SubGui_elem][9] - _sprite.height) / 2), _sprite);
	}
}

void Gui::set_Sprite_at_GuiRegion(uint16_t _GuiRegion_elem, Sprite& _sprite, std::string _origin) { set_Sprite_at_Pixel(GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0], GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2], _sprite, _origin); }

void Gui::set_Sprite_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, std::string _origin) {
	if (_origin == "nw") {
		for (uint16_t y = 0; y < _sprite.height; y++) {
			for (uint16_t x = 0; x < _sprite.width; x++) {
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
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
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
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
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
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
				if (_surface_y < gfx_ops.ScreenHeight && _surface_x < gfx_ops.ScreenWidth && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 7] = GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
					for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][a + 1]; }
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
		if (GuiSurface[gui_elem][k][6] >= _sprite.id && GuiSurface[gui_elem][k][6] <= (_sprite.id + _sprite.total_regions - 1)) {
			GuiSurface[gui_elem][k][6] = 0;
			for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][k][a] = GuiSurface[gui_elem][k][a + 7]; }
		}
	}
}


void Gui::set_CollissionMap_Data_at_GuiRegion(uint16_t _GuiRegion_elem, uint8_t _id, bool _write_to_file) {
	for (uint16_t surface_y = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2]; surface_y <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][3]; surface_y++) {
		for (uint16_t surface_x = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0]; surface_x <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][1]; surface_x++) {
			set_CollissionMap_Data_at_Pixel(surface_x, surface_y, _id, _write_to_file);
		}
	}
}

void Gui::set_CollissionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t _id, bool _write_to_file) {
	GuiSurface[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6] = _id;

	if (_write_to_file) {
		std::ofstream GuiFile;
		GuiFile.open("CollisionMap_Data/CollisionMap_Data.txt", std::ios_base::app);
		if (GuiFile.is_open()) {
			GuiFile << "gui.set_Collission_Map_at_Pixel(" + std::to_string(_surface_x) + "," + std::to_string(_surface_y) + "," + std::to_string(_id) + ");" << "\n";
			GuiFile.close();
		}
	}

}

uint8_t Gui::get_CollissionMap_Data_at_GuiRegion(uint16_t _GuiRegion_elem) {
	for (uint16_t _surface_y = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2]; _surface_y <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][3]; _surface_y++) {
		for (uint16_t _surface_x = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0]; _surface_x <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][1]; _surface_x++) {
			if (GuiSurface[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6] > 0) return GuiSurface[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6];
		}
	}
	return 0;
}

uint8_t Gui::get_CollissionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y) { return GuiSurface[gui_elem][_surface_y * gfx_ops.ScreenWidth + _surface_x][6]; }


void Gui::save_GuiRegion(uint16_t _GuiRegion_elem, bool _include_GuiRegion_spacing) {

	uint16_t GuiRegion_spacing = 0;
	if (_include_GuiRegion_spacing) GuiRegion_spacing = SubGui_data[gui_elem][SubGui_elem][8];

	for (uint16_t y = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][2] - GuiRegion_spacing; y <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][3] + GuiRegion_spacing; y++) {
		for (uint16_t x = GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][0] - GuiRegion_spacing; x <= GuiRegion_data[gui_elem][SubGui_elem][_GuiRegion_elem][1] + GuiRegion_spacing; x++) {
			for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(y * gfx_ops.ScreenWidth + x)][a + 3] = GuiSurface[gui_elem][(y * gfx_ops.ScreenWidth + x)][a]; }
		}
	}
}

void Gui::load_GuiRegion(uint16_t _source_GuiRegion_elem, bool _include_GuiRegion_spacing, int16_t _target_GuiRegion_elem) {

	uint16_t GuiRegion_spacing = 0;
	if (_include_GuiRegion_spacing) GuiRegion_spacing = SubGui_data[gui_elem][SubGui_elem][8];
	if (_target_GuiRegion_elem < 0) _target_GuiRegion_elem = _source_GuiRegion_elem;

	uint16_t target_y = GuiRegion_data[gui_elem][SubGui_elem][_target_GuiRegion_elem][2] - GuiRegion_spacing;
	uint16_t target_x = GuiRegion_data[gui_elem][SubGui_elem][_target_GuiRegion_elem][0] - GuiRegion_spacing;

	for (uint16_t source_y = GuiRegion_data[gui_elem][SubGui_elem][_source_GuiRegion_elem][2] - GuiRegion_spacing; source_y <= GuiRegion_data[gui_elem][SubGui_elem][_source_GuiRegion_elem][3] + GuiRegion_spacing; source_y++) {
		for (uint16_t source_x = GuiRegion_data[gui_elem][SubGui_elem][_source_GuiRegion_elem][0] - GuiRegion_spacing; source_x <= GuiRegion_data[gui_elem][SubGui_elem][_source_GuiRegion_elem][1] + GuiRegion_spacing; source_x++) {
			for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(target_y * gfx_ops.ScreenWidth + target_x)][a] = GuiSurface[gui_elem][(source_y * gfx_ops.ScreenWidth + source_x)][a + 3]; }
			target_x++;
		}
		target_x = GuiRegion_data[gui_elem][SubGui_elem][_target_GuiRegion_elem][0] - GuiRegion_spacing;
		target_y++;
	}
}


void Gui::save_Pixel(uint16_t _surface_x, uint16_t _surface_y) {
	for (uint8_t a = 0; a < 3; a++) { GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a + 3] = GuiSurface[gui_elem][(_surface_y * gfx_ops.ScreenWidth + _surface_x)][a]; }
}

void Gui::load_Pixel(uint16_t _source_surface_x, uint16_t _source_surface_y, int16_t _target_surface_x, int16_t _target_surface_y) {
	if (_target_surface_x < 0) _target_surface_x = _source_surface_x;
	if (_target_surface_y < 0) _target_surface_y = _source_surface_y;

	for (uint8_t a = 0; a < 3; a++) {
		GuiSurface[gui_elem][(_target_surface_y * gfx_ops.ScreenWidth + _target_surface_x)][a] = GuiSurface[gui_elem][(_source_surface_y * gfx_ops.ScreenWidth + _source_surface_x)][a + 3];
	}
}