#include "MainWindow.h"
#include "Gui.h"
#include <fstream>


Gui::Gui(class MainWindow& _app_window) : app_window(_app_window), gfx_ops(_app_window) {

	//SETUP SUB-GUIS HERE************************************************************************************************************************//
	Gui_count = 4;

	Gui_data = new uint16_t * [Gui_count];
	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) { Gui_data[gui_counter] = new uint16_t[5]; }

	// First gui
	Gui_data[0][0] = WindowWidth;  // Width
	Gui_data[0][1] = WindowHeight; // Height
	Gui_data[0][2] = 0;            // Current x position
	Gui_data[0][3] = 0;            // Current y position
	Gui_data[0][4] = 1;            // Number of sub guis

	// Second gui
	Gui_data[1][0] = WindowWidth;  // Width
	Gui_data[1][1] = WindowHeight; // Height
	Gui_data[1][2] = 0;
	Gui_data[1][3] = 0;
	Gui_data[1][4] = 2;            // Number of guis

	// Third gui
	Gui_data[2][0] = WindowWidth;  // Width
	Gui_data[2][1] = WindowHeight; // Height
	Gui_data[2][2] = 0;            
	Gui_data[2][3] = 0;
	Gui_data[2][4] = 1;            // Number of guis

	// Fourth gui
	Gui_data[3][0] = 4096;         // Width
	Gui_data[3][1] = 4096;         // Height
	Gui_data[3][2] = 0;
	Gui_data[3][3] = 0;
	Gui_data[3][4] = 1;            // Number of guis


    //                                           0    1  2     3     4    5     6   7   8 
	uint16_t first_gui__first_SubGui[9] = {      0,   0, 1200, 900,  15,  11,   0,  0,  0};

	uint16_t second_gui__first_SubGui[9] = {     0,   0, 900,  900,  8,   8,    4,  4,  3};
	uint16_t second_gui__second_SubGui[9] = {    900, 0, 1200, 900,  12,  37,   5,  5,  1};

	uint16_t third_gui__first_SubGui[9] = {      0,   0, 1200, 900,  63,  47,   1,  4,  1};

	uint16_t fourth_gui__first_SubGui[9] = { 0,   0, 4096, 4096,  255,  255,   0,  0,  1 };

	SubGui_data = new uint16_t** [Gui_count];
	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		SubGui_data[gui_counter] = new uint16_t* [Gui_data[gui_counter][4]];
		for (sub_gui_counter = 0; sub_gui_counter < Gui_data[gui_counter][4]; sub_gui_counter++) { 
			SubGui_data[gui_counter][sub_gui_counter] = new uint16_t[10];

			if (gui_counter == 0 && sub_gui_counter == 0) for (rgb_counter = 0; rgb_counter < 9; rgb_counter++) { SubGui_data[gui_counter][sub_gui_counter][rgb_counter] = first_gui__first_SubGui[rgb_counter]; }
			if (gui_counter == 1 && sub_gui_counter == 0) for (rgb_counter = 0; rgb_counter < 9; rgb_counter++) { SubGui_data[gui_counter][sub_gui_counter][rgb_counter] = second_gui__first_SubGui[rgb_counter]; }
			if (gui_counter == 1 && sub_gui_counter == 1) for (rgb_counter = 0; rgb_counter < 9; rgb_counter++) { SubGui_data[gui_counter][sub_gui_counter][rgb_counter] = second_gui__second_SubGui[rgb_counter]; }
			if (gui_counter == 2 && sub_gui_counter == 0) for (rgb_counter = 0; rgb_counter < 9; rgb_counter++) { SubGui_data[gui_counter][sub_gui_counter][rgb_counter] = third_gui__first_SubGui[rgb_counter]; }
			if (gui_counter == 3 && sub_gui_counter == 0) for (rgb_counter = 0; rgb_counter < 9; rgb_counter++) { SubGui_data[gui_counter][sub_gui_counter][rgb_counter] = fourth_gui__first_SubGui[rgb_counter]; }
		}
	}

	//*********************************************************************************************************************************************//

	SubGuiRegion_data = new uint16_t*** [Gui_count];
	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		SubGuiRegion_data[gui_counter] = new uint16_t** [Gui_data[gui_counter][4]];
		for (sub_gui_counter = 0; sub_gui_counter < Gui_data[gui_counter][4]; sub_gui_counter++) {
			SubGuiRegion_data[gui_counter][sub_gui_counter] = new uint16_t*[SubGui_data[gui_counter][sub_gui_counter][4] * SubGui_data[gui_counter][sub_gui_counter][5]];
			for (gui_region_counter = 0; gui_region_counter < SubGui_data[gui_counter][sub_gui_counter][4] * SubGui_data[gui_counter][sub_gui_counter][5]; gui_region_counter++) {
				SubGuiRegion_data[gui_counter][sub_gui_counter][gui_region_counter] = new uint16_t[4];
				for (rgb_counter = 0; rgb_counter < 4; rgb_counter++) { SubGuiRegion_data[gui_counter][sub_gui_counter][gui_region_counter][rgb_counter] = 0; }
			}
		}
	}

	GuiSurface = new uint8_t**[Gui_count];
	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		GuiSurface[gui_counter] = new uint8_t* [Gui_data[gui_counter][0] * Gui_data[gui_counter][1]];
		for (surface_counter = 0; surface_counter < Gui_data[gui_counter][0] * Gui_data[gui_counter][1]; surface_counter++) {
			GuiSurface[gui_counter][surface_counter] = new uint8_t[10];
			for (rgb_counter = 0; rgb_counter < 10; rgb_counter++) { GuiSurface[gui_counter][surface_counter][rgb_counter] = 0; }                 
		}
	}

	float temp_SubGuiRegion_size_wrt_x = 0, temp_SubGuiRegion_size_wrt_y = 0;

	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		for (sub_gui_counter = 0; sub_gui_counter < Gui_data[gui_counter][4]; sub_gui_counter++) {
			temp_SubGuiRegion_size_wrt_y = ((float)((SubGui_data[gui_counter][sub_gui_counter][3] - SubGui_data[gui_counter][sub_gui_counter][1]) - SubGui_data[gui_counter][sub_gui_counter][7] - SubGui_data[gui_counter][sub_gui_counter][8] - (SubGui_data[gui_counter][sub_gui_counter][5] * SubGui_data[gui_counter][sub_gui_counter][8])) / SubGui_data[gui_counter][sub_gui_counter][5]);
			temp_SubGuiRegion_size_wrt_x = ((float)((SubGui_data[gui_counter][sub_gui_counter][2] - SubGui_data[gui_counter][sub_gui_counter][0]) - SubGui_data[gui_counter][sub_gui_counter][6] - SubGui_data[gui_counter][sub_gui_counter][8] - (SubGui_data[gui_counter][sub_gui_counter][4] * SubGui_data[gui_counter][sub_gui_counter][8])) / SubGui_data[gui_counter][sub_gui_counter][4]);

			if (temp_SubGuiRegion_size_wrt_y > 0 && temp_SubGuiRegion_size_wrt_y == temp_SubGuiRegion_size_wrt_x) SubGui_data[gui_counter][sub_gui_counter][9] = temp_SubGuiRegion_size_wrt_x;
			else if (temp_SubGuiRegion_size_wrt_y > 0 && temp_SubGuiRegion_size_wrt_y < temp_SubGuiRegion_size_wrt_x) SubGui_data[gui_counter][sub_gui_counter][9] = temp_SubGuiRegion_size_wrt_y;
			else if (temp_SubGuiRegion_size_wrt_x > 0 && temp_SubGuiRegion_size_wrt_y > temp_SubGuiRegion_size_wrt_x) SubGui_data[gui_counter][sub_gui_counter][9] = temp_SubGuiRegion_size_wrt_x;

			for (y = 0; y < SubGui_data[gui_counter][sub_gui_counter][5]; y++) {
				for (x = 0; x < SubGui_data[gui_counter][sub_gui_counter][4]; x++) {
					SubGuiRegion_data[gui_counter][sub_gui_counter][y * SubGui_data[gui_counter][sub_gui_counter][4] + x][0] = (SubGui_data[gui_counter][sub_gui_counter][0] + SubGui_data[gui_counter][sub_gui_counter][6] + SubGui_data[gui_counter][sub_gui_counter][8] + (x * (SubGui_data[gui_counter][sub_gui_counter][9] + SubGui_data[gui_counter][sub_gui_counter][8])));
					SubGuiRegion_data[gui_counter][sub_gui_counter][y * SubGui_data[gui_counter][sub_gui_counter][4] + x][1] = SubGuiRegion_data[gui_counter][sub_gui_counter][y * SubGui_data[gui_counter][sub_gui_counter][4] + x][0] + SubGui_data[gui_counter][sub_gui_counter][9] - 1;

					SubGuiRegion_data[gui_counter][sub_gui_counter][y * SubGui_data[gui_counter][sub_gui_counter][4] + x][2] = (SubGui_data[gui_counter][sub_gui_counter][1] + SubGui_data[gui_counter][sub_gui_counter][7] + SubGui_data[gui_counter][sub_gui_counter][8] + (y * (SubGui_data[gui_counter][sub_gui_counter][9] + SubGui_data[gui_counter][sub_gui_counter][8])));
					SubGuiRegion_data[gui_counter][sub_gui_counter][y * SubGui_data[gui_counter][sub_gui_counter][4] + x][3] = SubGuiRegion_data[gui_counter][sub_gui_counter][y * SubGui_data[gui_counter][sub_gui_counter][4] + x][2] + SubGui_data[gui_counter][sub_gui_counter][9] - 1;
				}
			}
		}
	}
}

Gui::~Gui() {
	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		for (sub_gui_counter = 0; sub_gui_counter < Gui_data[gui_counter][4]; sub_gui_counter++) {
			for (gui_region_counter = 0; gui_region_counter < SubGui_data[gui_counter][sub_gui_counter][4] * SubGui_data[gui_counter][sub_gui_counter][5]; gui_region_counter++) {
				delete[] SubGuiRegion_data[gui_counter][sub_gui_counter][gui_region_counter]; SubGuiRegion_data[gui_counter][sub_gui_counter][gui_region_counter] = nullptr;
			} delete[] SubGuiRegion_data[gui_counter][sub_gui_counter]; SubGuiRegion_data[gui_counter][sub_gui_counter] = nullptr;
		} delete[] SubGuiRegion_data[gui_counter]; SubGuiRegion_data[gui_counter] = nullptr;
	} delete[] SubGuiRegion_data; SubGuiRegion_data = nullptr;

	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		for (sub_gui_counter = 0; sub_gui_counter < Gui_data[gui_counter][4]; sub_gui_counter++) { 
			delete[] SubGui_data[gui_counter][sub_gui_counter]; 
			SubGui_data[gui_counter][sub_gui_counter] = nullptr;
		} delete[] SubGui_data[gui_counter]; SubGui_data[gui_counter] = nullptr;
	} delete[] SubGui_data; SubGui_data = nullptr;

	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) {
		for (surface_counter = 0; surface_counter < Gui_data[gui_counter][0] * Gui_data[gui_counter][1]; surface_counter++) {
			delete[] GuiSurface[gui_counter][surface_counter]; GuiSurface[gui_counter][surface_counter] = nullptr;
		} delete[] GuiSurface[gui_counter]; GuiSurface[gui_counter] = nullptr;
	} delete[] GuiSurface; GuiSurface = nullptr;

	for (gui_counter = 0; gui_counter < Gui_count; gui_counter++) { delete[] Gui_data[gui_counter]; Gui_data[gui_counter] = nullptr; }
	delete[] Gui_data; Gui_data = nullptr;
}


void Gui::set_Target_SubGui(uint8_t _Gui_elem, uint8_t _SubGui_elem){ Gui_elem = _Gui_elem; SubGui_elem = _SubGui_elem; }


uint16_t Gui::get_SubGuiRegion_Size() { return SubGui_data[Gui_elem][SubGui_elem][9]; }

void Gui::set_SubGui_Pos_at_Pixel(uint16_t _surface_x, uint16_t _surface_y) { Gui_data[Gui_elem][2] = _surface_x; Gui_data[Gui_elem][3] = _surface_y; }


int32_t Gui::get_SubGuiRegion_at_Mouse_Pos() {
	for (gui_region_counter = 0; gui_region_counter < SubGui_data[Gui_elem][SubGui_elem][4] * SubGui_data[Gui_elem][SubGui_elem][5]; gui_region_counter++) {
		if (get_Mouse_XPos_on_SubGui() >= SubGuiRegion_data[Gui_elem][SubGui_elem][gui_region_counter][0] && get_Mouse_XPos_on_SubGui() <= SubGuiRegion_data[Gui_elem][SubGui_elem][gui_region_counter][1] &&
			get_Mouse_YPos_on_SubGui() >= SubGuiRegion_data[Gui_elem][SubGui_elem][gui_region_counter][2] && get_Mouse_YPos_on_SubGui() <= SubGuiRegion_data[Gui_elem][SubGui_elem][gui_region_counter][3]) {
			return gui_region_counter;
		}
	}
	return (-1);
}

int16_t Gui::get_Mouse_XPos_on_SubGui() { 
	if (app_window.mouse.GetPosX() + Gui_data[Gui_elem][2] >= SubGui_data[Gui_elem][SubGui_elem][0] && app_window.mouse.GetPosX() + Gui_data[Gui_elem][2] <= SubGui_data[Gui_elem][SubGui_elem][2]) {
		return app_window.mouse.GetPosX() + Gui_data[Gui_elem][2];
	}
	return (-1);
}

int16_t Gui::get_Mouse_YPos_on_SubGui() {
	if (app_window.mouse.GetPosY() + Gui_data[Gui_elem][3] >= SubGui_data[Gui_elem][SubGui_elem][1] && app_window.mouse.GetPosY() + Gui_data[Gui_elem][3] <= SubGui_data[Gui_elem][SubGui_elem][3]) {
		return app_window.mouse.GetPosY() + Gui_data[Gui_elem][3];
	}
	return (-1);
}



void Gui::refresh_Gui() {

	gfx_ops.BeginFrame();
	if (WindowWidth == Gui_data[Gui_elem][0] && WindowHeight == Gui_data[Gui_elem][1]) {
		for (y = 0; y < WindowHeight; y++) {
			for (x = 0; x < WindowWidth; x++) {
				gfx_ops.PutPixel(x, y, GuiSurface[Gui_elem][y * WindowWidth + x][0], GuiSurface[Gui_elem][y * WindowWidth + x][1], GuiSurface[Gui_elem][y * WindowWidth + x][2]);
			}
		}
	}
	else {
		for (y = 0; y < WindowHeight; y++) {
			for (x = 0; x < WindowWidth; x++) {
				for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) {
					gfx_ops.PutPixel(x, y, GuiSurface[Gui_elem][Gui_data[Gui_elem][3] * Gui_data[Gui_elem][0] + Gui_data[Gui_elem][2]][0], GuiSurface[Gui_elem][Gui_data[Gui_elem][3] * Gui_data[Gui_elem][0] + Gui_data[Gui_elem][2]][1], GuiSurface[Gui_elem][Gui_data[Gui_elem][3] * Gui_data[Gui_elem][0] + Gui_data[Gui_elem][2]][2]);
				}
				Gui_data[Gui_elem][2]++;
			}
			Gui_data[Gui_elem][2] -= WindowWidth;
			Gui_data[Gui_elem][3]++;
		}
		Gui_data[Gui_elem][3] -= WindowHeight;
	}
	gfx_ops.EndFrame();
}


void Gui::set_RGB_on_Gui(uint8_t* _rgb) {
	for (surface_counter = 0; surface_counter < Gui_data[Gui_elem][0] * Gui_data[Gui_elem][1]; surface_counter++) {
		for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][surface_counter][rgb_counter] = _rgb[rgb_counter]; }
	}
}

void Gui::set_RGB_on_SubGui(uint8_t* _rgb) {
	for (y = SubGui_data[Gui_elem][SubGui_elem][1]; y < SubGui_data[Gui_elem][SubGui_elem][3]; y++) {
		for (x = SubGui_data[Gui_elem][SubGui_elem][0]; x < SubGui_data[Gui_elem][SubGui_elem][2]; x++) {
			for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter] = _rgb[rgb_counter]; }
		}
	}
}

void Gui::set_RGB_on_SubGuiRegion(uint32_t _SubGuiRegion_elem, uint8_t* _rgb) {
	for (y = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2]; y <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][3]; y++) {
		for (x = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0]; x <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][1]; x++) {
			for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter] = _rgb[rgb_counter]; }
		}
	}
}

void Gui::set_RGB_on_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t* _rgb) { for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][rgb_counter] = _rgb[rgb_counter]; } }

void Gui::set_RGB_on_SubGuiRegion_Border(uint32_t _SubGuiRegion_elem, uint8_t* _SubGuiRegion_border_rgb, bool _n, bool _e, bool _s, bool _w) {
	for (y = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2] - SubGui_data[Gui_elem][SubGui_elem][8]; y <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][3] + SubGui_data[Gui_elem][SubGui_elem][8]; y++) {
		for (x = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0] - SubGui_data[Gui_elem][SubGui_elem][8]; x <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][1] + SubGui_data[Gui_elem][SubGui_elem][8]; x++) {
			if (_n && y < SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2]) {
				for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter] = _SubGuiRegion_border_rgb[rgb_counter]; }
			}
			if (_e && x > SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][1]) {
				for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter] = _SubGuiRegion_border_rgb[rgb_counter]; }
			}
			if (_s && y > SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][3]) {
				for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter] = _SubGuiRegion_border_rgb[rgb_counter]; }
			}
			if (_w && x < SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0]) {
				for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter] = _SubGuiRegion_border_rgb[rgb_counter]; }
			}
		}
	}
}



void Gui::set_BMP_on_Gui(std::string _filename, uint8_t _items_per_row, uint8_t _total_rows, uint8_t* _outline_rgb, uint8_t _target_item, uint8_t* _rgb) {
	if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

		std::ifstream file(_filename, std::ios::binary);

		if (!file.fail()) {

			BITMAPFILEHEADER bmFileHeader;
			file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

			BITMAPINFOHEADER bmInfoHeader;
			file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

			if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {
				if (Gui_data[Gui_elem][1] >= bmInfoHeader.biHeight && Gui_data[Gui_elem][0] >= bmInfoHeader.biWidth) {
					file.close();
					set_BMP_at_Pixel((Gui_data[Gui_elem][0] - bmInfoHeader.biWidth) / 2, (Gui_data[Gui_elem][1] - bmInfoHeader.biHeight) / 2, _filename, _items_per_row, _total_rows, _outline_rgb, _target_item, _rgb);
				}
			}
			else {
				file.close();
				exit(24); // Incorrect encoding or presence of compression
			}
		}
		else exit(25); // File failed to open
	}
	else exit(26); // File is not a BMP file type
}

void Gui::set_BMP_on_SubGui(std::string _filename, uint8_t _items_per_row, uint8_t _total_rows, uint8_t* _outline_rgb, uint8_t _target_item, uint8_t* _rgb) {
	if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

		std::ifstream file(_filename, std::ios::binary);

		if (!file.fail()) {

			BITMAPFILEHEADER bmFileHeader;
			file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

			BITMAPINFOHEADER bmInfoHeader;
			file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

			if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {
				if (SubGui_data[Gui_elem][SubGui_elem][3] - SubGui_data[Gui_elem][SubGui_elem][1] >= bmInfoHeader.biHeight && SubGui_data[Gui_elem][SubGui_elem][2] - SubGui_data[Gui_elem][SubGui_elem][0] >= bmInfoHeader.biWidth) {
					file.close();
					set_BMP_at_Pixel(SubGui_data[Gui_elem][SubGui_elem][0] + (SubGui_data[Gui_elem][SubGui_elem][2] - SubGui_data[Gui_elem][SubGui_elem][0] - bmInfoHeader.biWidth) / 2, SubGui_data[Gui_elem][SubGui_elem][1] + (SubGui_data[Gui_elem][SubGui_elem][3] - SubGui_data[Gui_elem][SubGui_elem][1] - bmInfoHeader.biHeight) / 2, _filename, _items_per_row, _total_rows, _outline_rgb, _target_item, _rgb);
				}
			}
			else {
				file.close();
				exit(24); // Incorrect encoding or presence of compression
			}
		}
		else exit(25); // File failed to open
	}
	else exit(26); // File is not a BMP file type
}

void Gui::set_BMP_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, std::string _filename, uint8_t _items_per_row, uint8_t _total_rows, uint8_t* _outline_rgb, uint8_t _target_item, uint8_t* _rgb) {
	set_BMP_at_Pixel(SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0], SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2], _filename, _items_per_row, _total_rows, _outline_rgb, _target_item, _rgb);
}

void Gui::set_BMP_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, std::string _filename, uint8_t _items_per_row, uint8_t _total_rows, uint8_t* _outline_rgb, uint8_t _target_item, uint8_t* _rgb) {

	if (_filename == "Images/funky_chars.bmp") {
		int hello = 0;
	}


	if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

		std::ifstream file(_filename, std::ios::binary);

		if (!file.fail()) {

			BITMAPFILEHEADER bmFileHeader;
			file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

			BITMAPINFOHEADER bmInfoHeader;
			file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

			if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {

				uint16_t width_compensation_pixels = 0, temp_width = bmInfoHeader.biWidth;
				while (temp_width % _items_per_row) { temp_width--;  width_compensation_pixels++; }
				
				uint16_t width = (uint16_t)(temp_width / _items_per_row);
				uint16_t width_compensation_bytes = 3 * width_compensation_pixels;
				uint16_t line_item_bytes = width * 3;
				uint8_t items_before_target_item = _target_item % _items_per_row;
				uint8_t items_after_target_item = _items_per_row - items_before_target_item - 1;
				uint8_t row_start = _total_rows - (uint8_t)(_target_item / _items_per_row) - 1;
				uint8_t line_padding_bytes = ((4 - (bmInfoHeader.biWidth * 3) % 4) % 4);
				uint8_t temp_file_rgb[3] = { 0,0,0 };

				file.seekg(0, file.beg);

				file.seekg(bmFileHeader.bfOffBits + (row_start * (uint16_t)(bmInfoHeader.biHeight / _total_rows) * (line_item_bytes * (items_before_target_item + 1 + items_after_target_item) + width_compensation_bytes + line_padding_bytes) + (items_before_target_item * line_item_bytes)), std::ios::cur);

				_surface_y += (bmInfoHeader.biHeight / _total_rows) - 1;

				for (y = bmInfoHeader.biHeight / _total_rows; y > 0; y--) {
					for (x = 0; x < width; x++) {

						for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { temp_file_rgb[rgb_counter] = file.get(); }

						if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0]) {
							if (_outline_rgb) {
								if (temp_file_rgb[0] != _outline_rgb[2] && temp_file_rgb[1] != _outline_rgb[1] && temp_file_rgb[2] != _outline_rgb[0]) {
									if (_rgb) for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _rgb[rgb_counter]; }
									else for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = temp_file_rgb[2 - rgb_counter]; }
								}
							}
							else for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = temp_file_rgb[2 - rgb_counter]; }
						}
						_surface_x++;
					}
					file.seekg((items_after_target_item * line_item_bytes) + width_compensation_bytes + line_padding_bytes + (items_before_target_item * line_item_bytes), std::ios::cur);

					_surface_x -= width;
					_surface_y--;
				}
				file.close();
			}
			else {
				file.close();
				exit(24); // Incorrect encoding or presence of compression
			}
		}
		else exit(25); // File failed to open
	}
	else exit(26); // File is not a BMP file type
}



void Gui::set_Image_on_Gui(Image& _image) {
	if (Gui_data[Gui_elem][1] >= _image.height && Gui_data[Gui_elem][0] >= _image.width) {
		set_Image_at_Pixel((Gui_data[Gui_elem][0] - _image.width) / 2, (Gui_data[Gui_elem][1] - _image.height) / 2, _image);
	}
}

void Gui::set_Image_on_SubGui(Image& _image) {
	if (SubGui_data[Gui_elem][SubGui_elem][3] - SubGui_data[Gui_elem][SubGui_elem][1] >= _image.height && SubGui_data[Gui_elem][SubGui_elem][2] - SubGui_data[Gui_elem][SubGui_elem][0] >= _image.width) {
		set_Image_at_Pixel(SubGui_data[Gui_elem][SubGui_elem][0] + (SubGui_data[Gui_elem][SubGui_elem][2] - SubGui_data[Gui_elem][SubGui_elem][0] - _image.width) / 2, SubGui_data[Gui_elem][SubGui_elem][1] + (SubGui_data[Gui_elem][SubGui_elem][3] - SubGui_data[Gui_elem][SubGui_elem][1] - _image.height) / 2, _image);
	}
}

void Gui::set_Image_on_SubGuiRegion(uint32_t _SubGuiRegion_elem, Image& _image) {
	if (SubGui_data[Gui_elem][SubGui_elem][9] >= _image.width && SubGui_data[Gui_elem][SubGui_elem][9] >= _image.height) {
		set_Image_at_Pixel(SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0] + ((SubGui_data[Gui_elem][SubGui_elem][9] - _image.width) / 2), SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2] + ((SubGui_data[Gui_elem][SubGui_elem][9] - _image.height) / 2), _image);
	}
}

void Gui::set_Image_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, Image& _image, std::string _origin, uint16_t _image_x, uint16_t _image_y) { set_Image_at_Pixel(SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0], SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2], _image, _origin, _image_x, _image_y); }

void Gui::set_Image_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Image& _image, std::string _origin, uint16_t _image_x, uint16_t _image_y) {

	if (_origin == "nw") {
		for (y = _image_y; y < _image.height; y++) {
			for (x = _image_x; x < _image.width; x++) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _image.rgb[_image.elem][y * _image.width + x][0]) {
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _image.rgb[_image.elem][y * _image.width + x][rgb_counter + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _image.width;
			_surface_y++;
		}
	}
	else if (_origin == "ne") {
		if (!_image_x) _image_x = _image.width - 1;
		for (y = _image_y; y < _image.height; y++) {
			for (X = _image_x; X > -1; X--) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _image.rgb[_image.elem][y * _image.width + X][0]) {
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _image.rgb[_image.elem][y * _image.width + X][rgb_counter + 1]; }
				}
				_surface_x--;
			}
			_surface_x += _image.width;
			_surface_y++;
		}
	}
	else if (_origin == "sw") {
		if (!_image_y) _image_y = _image.height - 1;
		for (Y = _image_y; Y > -1; Y--) {
			for (x = _image_x; x < _image.width; x++) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _image.rgb[_image.elem][Y * _image.width + x][0]) {
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _image.rgb[_image.elem][Y * _image.width + x][rgb_counter + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _image.width;
			_surface_y--;
		}
	}
	else if (_origin == "se") {
		if (!_image_x) _image_x = _image.width - 1;
		if (!_image_y) _image_y = _image.height - 1;
		for (Y = _image_y; Y > -1; Y--) {
			for (X = _image_x; X > -1; X--) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _image.rgb[_image.elem][Y * _image.width + X][0]) {
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _image.rgb[_image.elem][Y * _image.width + X][rgb_counter + 1]; }
				}
				_surface_x--;
			}
			_surface_x += _image.width;
			_surface_y--;
		}
	}
}



void Gui::set_Sprite_on_Gui(Sprite& _sprite) {
	if (Gui_data[Gui_elem][1] >= _sprite.height && Gui_data[Gui_elem][0] >= _sprite.width) {
		set_Sprite_at_Pixel((Gui_data[Gui_elem][0] - _sprite.width) / 2, (Gui_data[Gui_elem][1] - _sprite.height) / 2, _sprite);
	}
}

void Gui::set_Sprite_on_SubGui(Sprite& _sprite) {
	if (SubGui_data[Gui_elem][SubGui_elem][3] - SubGui_data[Gui_elem][SubGui_elem][1] >= _sprite.height && SubGui_data[Gui_elem][SubGui_elem][2] - SubGui_data[Gui_elem][SubGui_elem][0] >= _sprite.width) {
		set_Sprite_at_Pixel(SubGui_data[Gui_elem][SubGui_elem][0] + ((SubGui_data[Gui_elem][SubGui_elem][2] - SubGui_data[Gui_elem][SubGui_elem][0]) - _sprite.width) / 2, SubGui_data[Gui_elem][SubGui_elem][1] + ((SubGui_data[Gui_elem][SubGui_elem][3] - SubGui_data[Gui_elem][SubGui_elem][1]) - _sprite.height) / 2, _sprite);
	}
}

void Gui::set_Sprite_on_SubGuiRegion(uint32_t _SubGuiRegion_elem, Sprite& _sprite) {
	if (SubGui_data[Gui_elem][SubGui_elem][9] >= _sprite.width && SubGui_data[Gui_elem][SubGui_elem][9] >= _sprite.height) {
		set_Sprite_at_Pixel(SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0] + ((SubGui_data[Gui_elem][SubGui_elem][9] - _sprite.width) / 2), SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2] + ((SubGui_data[Gui_elem][SubGui_elem][9] - _sprite.height) / 2), _sprite);
	}
}

void Gui::set_Sprite_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, Sprite& _sprite, std::string _origin) { set_Sprite_at_Pixel(SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0], SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2], _sprite, _origin); }

void Gui::set_Sprite_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, std::string _origin) {

	_sprite.displayed = true;
	_sprite.x_coord = _surface_x;
	_sprite.y_coord = _surface_y;
	_sprite.origin = _origin;

	if (_origin == "nw") {
		for (y = 0; y < _sprite.height; y++) {
			for (x = 0; x < _sprite.width; x++) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
					GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][0];
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { _sprite.rgb[0][y * _sprite.width + x][rgb_counter] = GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter]; }
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _sprite.rgb[_sprite.elem][y * _sprite.width + x][rgb_counter + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _sprite.width;
			_surface_y++;
		}
	}
	else if (_origin == "ne") {
		for (y = 0; y < _sprite.height; y++) {
			for (X = _sprite.width - 1; X > -1; X--) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][y * _sprite.width + X][0]) {
					GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][6] = _sprite.rgb[_sprite.elem][y * _sprite.width + X][0];
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { _sprite.rgb[0][y * _sprite.width + X][rgb_counter] = GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter]; }
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _sprite.rgb[_sprite.elem][y * _sprite.width + X][rgb_counter + 1]; }
				}
				_surface_x--;
			}
			_surface_x += _sprite.width;
			_surface_y++;
		}
	}
	else if (_origin == "sw") {
		for (Y = _sprite.height - 1; Y > -1; Y--) {
			for (x = 0; x < _sprite.width; x++) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][Y * _sprite.width + x][0]) {
					GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][6] = _sprite.rgb[_sprite.elem][Y * _sprite.width + x][0];
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { _sprite.rgb[0][Y * _sprite.width + x][rgb_counter] = GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter]; }
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _sprite.rgb[_sprite.elem][Y * _sprite.width + x][rgb_counter + 1]; }
				}
				_surface_x++;
			}
			_surface_x -= _sprite.width;
			_surface_y--;
		}
	}
	else if (_origin == "se") {
		for (Y = _sprite.height - 1; Y > -1; Y--) {
			for (X = _sprite.width - 1; X > -1; X--) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][Y * _sprite.width + X][0]) {
					GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][6] = _sprite.rgb[_sprite.elem][Y * _sprite.width + X][0];
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { _sprite.rgb[0][Y * _sprite.width + X][rgb_counter] = GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter]; }
					for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][rgb_counter] = _sprite.rgb[_sprite.elem][Y * _sprite.width + X][rgb_counter + 1]; }
				}
				_surface_x--;
			}
			_surface_x += _sprite.width;
			_surface_y--;
		}
	}
}

void Gui::set_Sprite_Off(Sprite& _sprite) {

	if (_sprite.displayed) {
		if (_sprite.origin == "nw") {
			for (y = 0; y < _sprite.height; y++) {
				for (x = 0; x < _sprite.width; x++) {
					if (_sprite.y_coord < Gui_data[Gui_elem][1] && _sprite.x_coord < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0]) {
						GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][6] = 0;
						for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][rgb_counter] = _sprite.rgb[0][y * _sprite.width + x][rgb_counter]; }
					}
					_sprite.x_coord++;
				}
				_sprite.x_coord -= _sprite.width;
				_sprite.y_coord++;
			}
		}
		else if (_sprite.origin == "ne") {
			for (y = 0; y < _sprite.height; y++) {
				for (X = _sprite.width - 1; X > -1; X--) {
					if (_sprite.y_coord < Gui_data[Gui_elem][1] && _sprite.x_coord < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][y * _sprite.width + X][0]) {
						GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][6] = 0;
						for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][rgb_counter] = _sprite.rgb[0][y * _sprite.width + X][rgb_counter]; }
					}
					_sprite.x_coord--;
				}
				_sprite.x_coord += _sprite.width;
				_sprite.y_coord++;
			}
		}
		else if (_sprite.origin == "sw") {
			for (Y = _sprite.height - 1; Y > -1; Y--) {
				for (x = 0; x < _sprite.width; x++) {
					if (_sprite.y_coord < Gui_data[Gui_elem][1] && _sprite.x_coord < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][Y * _sprite.width + x][0]) {
						GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][6] = 0;
						for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][rgb_counter] = _sprite.rgb[0][Y * _sprite.width + x][rgb_counter]; }
					}
					_sprite.x_coord++;
				}
				_sprite.x_coord -= _sprite.width;
				_sprite.y_coord--;
			}
		}
		else if (_sprite.origin == "se") {
			for (Y = _sprite.height - 1; Y > -1; Y--) {
				for (X = _sprite.width - 1; X > -1; X--) {
					if (_sprite.y_coord < Gui_data[Gui_elem][1] && _sprite.x_coord < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][Y * _sprite.width + X][0]) {
						GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][6] = 0;
						for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][_sprite.y_coord * Gui_data[Gui_elem][0] + _sprite.x_coord][rgb_counter] = _sprite.rgb[0][Y * _sprite.width + X][rgb_counter]; }
					}
					_sprite.x_coord--;
				}
				_sprite.x_coord += _sprite.width;
				_sprite.y_coord--;
			}
		}

		_sprite.displayed = false;
		_sprite.x_coord = 0;
		_sprite.y_coord = 0;
		_sprite.origin = "nw";
	}
}



void Gui::set_CollisionMap_Data_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, bool _include_SubGuiRegion_spacing, uint8_t _id, bool _write_to_file) {
	if (_include_SubGuiRegion_spacing) SubGuiRegion_spacing = SubGui_data[Gui_elem][SubGui_elem][8];
	else SubGuiRegion_spacing = 0;

	for (y = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2] - SubGuiRegion_spacing; y <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][3] + SubGuiRegion_spacing; y++) {
		for (x = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0] - SubGuiRegion_spacing; x <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][1] + SubGuiRegion_spacing; x++) {
			set_CollisionMap_Data_at_Pixel(x, y, _id, _write_to_file);
		}
	}
}

void Gui::set_CollisionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t _id, bool _write_to_file) { 
	GuiSurface[Gui_elem][_surface_y * Gui_data[Gui_elem][0] + _surface_x][6] = _id;

	if (_write_to_file) {
		std::ofstream GuiFile;
		GuiFile.open("CollisionMap_Data/CollisionMap_Data.txt", std::ios_base::app);
		if (GuiFile.is_open()) {
			GuiFile << "gui.set_Collision_Map_at_Pixel(" + std::to_string(_surface_x) + "," + std::to_string(_surface_y) + "," + std::to_string(_id) + ");" << "\n";
			GuiFile.close();
		}
	}

}

uint8_t Gui::get_CollisionMap_Data_at_SubGuiRegion(uint32_t _SubGuiRegion_elem) {
	for (y = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2]; y <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][3]; y++) {
		for (x = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0]; x <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][1]; x++) {
			if (GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][6] > 0) return GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][6];
		}
	}
	return 0;
}

uint8_t Gui::get_CollisionMap_Data_at_Pixel(uint16_t _x, uint16_t _y) { return GuiSurface[Gui_elem][_y * Gui_data[Gui_elem][0] + _x][6]; }

uint8_t Gui::get_CollisionMap_Data_at_SpriteRegion(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, uint8_t _region_id, std::string _origin) {

	if (_origin == "nw") {
		for (y = 0; y < _sprite.height; y++) {
			for (x = 0; x < _sprite.width; x++) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][y * _sprite.width + x][0] == _region_id) {
					if (GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6] != 0) return GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6];
				}
				_surface_x++;
			}
			_surface_x -= _sprite.width;
			_surface_y++;
		}
	}
	else if (_origin == "ne") {
		for (y = 0; y < _sprite.height; y++) {
			for (X = _sprite.width - 1; X > -1; X--) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][y * _sprite.width + X][0] == _region_id) {
					if (GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6] != 0) return GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6];
				}
				_surface_x--;
			}
			_surface_x += _sprite.width;
			_surface_y++;
		}
	}
	else if (_origin == "sw") {
		for (Y = _sprite.height - 1; Y > -1; Y--) {
			for (x = 0; x < _sprite.width; x++) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][Y * _sprite.width + x][0] == _region_id) {
					if (GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6] != 0) return GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6];
				}
				_surface_x++;
			}
			_surface_x -= _sprite.width;
			_surface_y--;
		}
	}
	else if (_origin == "se") {
		for (Y = _sprite.height - 1; Y > -1; Y--) {
			for (X = _sprite.width - 1; X > -1; X--) {
				if (_surface_y < Gui_data[Gui_elem][1] && _surface_x < Gui_data[Gui_elem][0] && _sprite.rgb[_sprite.elem][Y * _sprite.width + X][0] == _region_id) {
					if (GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6] != 0) return GuiSurface[Gui_elem][(_surface_y * Gui_data[Gui_elem][0] + _surface_x)][6];
				}
				_surface_x--;
			}
			_surface_x += _sprite.width;
			_surface_y--;
		}
	}

	return 0;
}



void Gui::save_SubGuiRegion(uint32_t _SubGuiRegion_elem, bool _include_SubGuiRegion_spacing) {
	if (_include_SubGuiRegion_spacing) SubGuiRegion_spacing = SubGui_data[Gui_elem][SubGui_elem][8];
	else SubGuiRegion_spacing = 0;

	for (y = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][2] - SubGuiRegion_spacing; y <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][3] + SubGuiRegion_spacing; y++) {
		for (x = SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][0] - SubGuiRegion_spacing; x <= SubGuiRegion_data[Gui_elem][SubGui_elem][_SubGuiRegion_elem][1] + SubGuiRegion_spacing; x++) {
			for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter + 3] = GuiSurface[Gui_elem][y * Gui_data[Gui_elem][0] + x][rgb_counter]; }
		}
	}
}

void Gui::load_SubGuiRegion(uint32_t _source_SubGuiRegion_elem, bool _include_SubGuiRegion_spacing, int32_t _target_SubGuiRegion_elem) {

	uint16_t SubGuiRegion_spacing = 0;
	if (_include_SubGuiRegion_spacing) SubGuiRegion_spacing = SubGui_data[Gui_elem][SubGui_elem][8];
	if (_target_SubGuiRegion_elem < 0) _target_SubGuiRegion_elem = _source_SubGuiRegion_elem;

	uint16_t target_y = SubGuiRegion_data[Gui_elem][SubGui_elem][_target_SubGuiRegion_elem][2] - SubGuiRegion_spacing;
	uint16_t target_x = SubGuiRegion_data[Gui_elem][SubGui_elem][_target_SubGuiRegion_elem][0] - SubGuiRegion_spacing;

	for (uint16_t source_y = SubGuiRegion_data[Gui_elem][SubGui_elem][_source_SubGuiRegion_elem][2] - SubGuiRegion_spacing; source_y <= SubGuiRegion_data[Gui_elem][SubGui_elem][_source_SubGuiRegion_elem][3] + SubGuiRegion_spacing; source_y++) {
		for (uint16_t source_x = SubGuiRegion_data[Gui_elem][SubGui_elem][_source_SubGuiRegion_elem][0] - SubGuiRegion_spacing; source_x <= SubGuiRegion_data[Gui_elem][SubGui_elem][_source_SubGuiRegion_elem][1] + SubGuiRegion_spacing; source_x++) {
			for (rgb_counter = 0; rgb_counter < 3; rgb_counter++) { GuiSurface[Gui_elem][target_y * Gui_data[Gui_elem][0] + target_x][rgb_counter] = GuiSurface[Gui_elem][source_y * Gui_data[Gui_elem][0] + source_x][rgb_counter + 3]; }
			target_x++;
		}
		target_x = SubGuiRegion_data[Gui_elem][SubGui_elem][_target_SubGuiRegion_elem][0] - SubGuiRegion_spacing;
		target_y++;
	}
}