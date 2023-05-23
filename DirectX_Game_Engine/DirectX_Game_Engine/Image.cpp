#include <fstream>
#include "Image.h"

Image::Image(std::string _filename, uint8_t* _outline_rgb, uint8_t* _image_rgb, uint8_t _images_per_row, uint8_t _total_rows) {

	if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

		std::ifstream file(_filename, std::ios::binary);

		if (!file.fail()) {

			BITMAPFILEHEADER bmFileHeader;
			file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

			BITMAPINFOHEADER bmInfoHeader;
			file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

			if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {

				total_elem_count = _images_per_row * _total_rows;
				width = (uint16_t)(bmInfoHeader.biWidth / _images_per_row);
				height = (uint16_t)(bmInfoHeader.biHeight / _total_rows);

				rgb = new uint8_t * *[total_elem_count];
				for (uint16_t i = 0; i < total_elem_count; i++) {
					rgb[i] = new uint8_t * [width * height];
					for (uint32_t j = 0; j < (width * height); j++) {
						rgb[i][j] = new uint8_t[4];
						for (uint8_t a = 0; a < 4; a++) { rgb[i][j][a] = 0; }
					}
				}

				uint16_t rgb_x, rgb_y;
				uint16_t bmp_item_elem = 0, rgb_elem = 0, items_before_target_item = 0, items_after_target_item = 0, row_start = 0;
				uint8_t temp_file_rgb[3] = { 0,0,0 };
				uint16_t line_padding_bytes = ((4 - (bmInfoHeader.biWidth * 3) % 4) % 4);
				uint16_t line_item_bytes = (3 * width);

				for (uint8_t bmp_y = _total_rows; bmp_y > 0; bmp_y--) {
					for (uint8_t bmp_x = 0; bmp_x < _images_per_row; bmp_x++) {

						bmp_item_elem = (bmp_y - 1) * _images_per_row + bmp_x;

						items_before_target_item = (bmp_item_elem % _images_per_row);
						items_after_target_item = (_images_per_row - items_before_target_item - 1);
						row_start = (uint16_t)(bmp_item_elem / _images_per_row);

						file.seekg(0, file.beg);
						file.seekg(bmFileHeader.bfOffBits + (row_start * height * (line_item_bytes * (items_before_target_item + 1 + items_after_target_item) + line_padding_bytes) + (items_before_target_item * line_item_bytes)), std::ios::cur);

						rgb_y = height;

						for (int y = height; y > 0; y--) {

							rgb_y--;
							rgb_x = 0;

							for (int x = 0; x < width; x++) {
								for (int a = 0; a < 3; a++) { temp_file_rgb[a] = file.get(); }

								if (_outline_rgb) {
									if (temp_file_rgb[0] != _outline_rgb[2] && temp_file_rgb[1] != _outline_rgb[1] && temp_file_rgb[2] != _outline_rgb[0]) rgb[rgb_elem][rgb_y * width + rgb_x][0] = 1;
								}
								else rgb[rgb_elem][rgb_y * width + rgb_x][0] = 1;

								if (rgb[rgb_elem][rgb_y * width + rgb_x][0]) {
									if (_image_rgb) for (int a = 1; a < 4; a++) { rgb[rgb_elem][rgb_y * width + rgb_x][a] = _image_rgb[a - 1]; }
									else for (int a = 1; a < 4; a++) { rgb[rgb_elem][rgb_y * width + rgb_x][a] = temp_file_rgb[2 - (a - 1)]; }
								}
								rgb_x++;
							}
							file.seekg((items_after_target_item * line_item_bytes) + line_padding_bytes + (items_before_target_item * line_item_bytes), std::ios::cur);
						}
						rgb_elem++;
					}
				}
			}
			else exit(1);
		}
		else exit(1);
	}
}

Image::Image(uint8_t _sprite_id, std::string _filename, uint8_t* _outline_rgb, uint8_t* _image_rgb, uint8_t _images_per_row, uint8_t _total_rows, uint8_t _widthwise_regions, uint8_t _heightwise_regions) {

	if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

		std::ifstream file(_filename, std::ios::binary);

		if (!file.fail()) {

			BITMAPFILEHEADER bmFileHeader;
			file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

			BITMAPINFOHEADER bmInfoHeader;
			file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

			if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {

				total_elem_count = _images_per_row * _total_rows;
				width = (uint16_t)(bmInfoHeader.biWidth / _images_per_row);
				height = (uint16_t)(bmInfoHeader.biHeight / _total_rows);

				rgb = new uint8_t * *[total_elem_count];
				for (uint16_t i = 0; i < total_elem_count; i++) {
					rgb[i] = new uint8_t * [width * height];
					for (uint32_t j = 0; j < (width * height); j++) {
						rgb[i][j] = new uint8_t[4];
						for (uint8_t a = 0; a < 4; a++) { rgb[i][j][a] = 0; }
					}
				}

				uint16_t rgb_x, rgb_y;
				uint16_t bmp_item_elem = 0, rgb_elem = 0, items_before_target_item = 0, items_after_target_item = 0, row_start = 0;
				uint8_t temp_file_rgb[3] = { 0,0,0 };
				uint16_t line_padding_bytes = ((4 - (bmInfoHeader.biWidth * 3) % 4) % 4);
				uint16_t line_item_bytes = (3 * width);

				for (uint8_t bmp_y = _total_rows; bmp_y > 0; bmp_y--) {
					for (uint8_t bmp_x = 0; bmp_x < _images_per_row; bmp_x++) {

						bmp_item_elem = (bmp_y - 1) * _images_per_row + bmp_x;

						items_before_target_item = (bmp_item_elem % _images_per_row);
						items_after_target_item = (_images_per_row - items_before_target_item - 1);
						row_start = (uint16_t)(bmp_item_elem / _images_per_row);

						file.seekg(0, file.beg);
						file.seekg(bmFileHeader.bfOffBits + (row_start * height * (line_item_bytes * (items_before_target_item + 1 + items_after_target_item) + line_padding_bytes) + (items_before_target_item * line_item_bytes)), std::ios::cur);

						rgb_y = height;

						for (int y = height; y > 0; y--) {

							rgb_y--;
							rgb_x = 0;

							for (int x = 0; x < width; x++) {
								for (int a = 0; a < 3; a++) { temp_file_rgb[a] = file.get(); }

								if (_outline_rgb) {
									if (temp_file_rgb[0] != _outline_rgb[2] && temp_file_rgb[1] != _outline_rgb[1] && temp_file_rgb[2] != _outline_rgb[0]) {
										rgb[rgb_elem][rgb_y * width + rgb_x][0] = _sprite_id + ((uint8_t)(rgb_y / (height / _heightwise_regions)) * _widthwise_regions + (uint8_t)(rgb_x / (width / _widthwise_regions)));
									}
								}
								else {
									rgb[rgb_elem][rgb_y * width + rgb_x][0] = _sprite_id + ((uint8_t)(rgb_y / (height / _heightwise_regions)) * _widthwise_regions + (uint8_t)(rgb_x / (width / _widthwise_regions)));
								}

								if (rgb[rgb_elem][rgb_y * width + rgb_x][0]) {
									if (_image_rgb) for (int a = 1; a < 4; a++) { rgb[rgb_elem][rgb_y * width + rgb_x][a] = _image_rgb[a - 1]; }
									else for (int a = 1; a < 4; a++) { rgb[rgb_elem][rgb_y * width + rgb_x][a] = temp_file_rgb[2 - (a - 1)]; }
								}
								rgb_x++;
							}
							file.seekg((items_after_target_item * line_item_bytes) + line_padding_bytes + (items_before_target_item * line_item_bytes), std::ios::cur);
						}
						rgb_elem++;
					}
				}
			}
			else exit(1);
		}
		else exit(1);
	}
}

Image::~Image() {
	for (uint16_t i = 0; i < total_elem_count; i++) {
		for (uint32_t j = 0; j < (width * height); j++) { delete[] rgb[i][j]; rgb[i][j] = nullptr; }
		delete[] rgb[i]; rgb[i] = nullptr;
	} delete[] rgb; rgb = nullptr;
}

Image& Image::operator[](uint8_t _elem) { elem = _elem; return *this; }

uint16_t Image::get_Width() { return width; }

uint16_t Image::get_Height() { return height; }

void Image::set_RGB(uint8_t* _rgb) {
	for (int i = 0; i < height * width; i++) {
		if (rgb[elem][i][0]) { for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _rgb[a - 1]; } }
	}
}

void Image::set_Find_Replace_RGB(uint8_t* _find_rgb, uint8_t* _replace_rgb) {
	for (int i = 0; i < height * width; i++) {
		if (rgb[elem][i][0] && rgb[elem][i][1] == _find_rgb[0] && rgb[elem][i][2] == _find_rgb[1] && rgb[elem][i][3] == _find_rgb[2]) {
			for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _replace_rgb[a - 1]; }
		}
	}
}

void Image::set_Complement_RGB(uint8_t* _find_rgb, uint8_t* _complement_rgb) {
	for (int i = 0; i < height * width; i++) {
		if (rgb[elem][i][0] && rgb[elem][i][1] != _find_rgb[0] && rgb[elem][i][2] != _find_rgb[1] && rgb[elem][i][3] != _find_rgb[2]) {
			for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _complement_rgb[a - 1]; }
		}
	}
}