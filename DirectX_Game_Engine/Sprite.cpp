#include "Sprite.h"

Sprite::Sprite(uint8_t _id, uint8_t _widthwise_regions, uint8_t _heightwise_regions, std::string _filename, uint8_t _items_per_row, uint8_t _total_rows, uint8_t* _outline_rgb, uint8_t* _rgb) : RGB_Data() {

	if (_id > 1) {

		if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

			std::ifstream file(_filename, std::ios::binary);

			if (!file.fail()) {

				BITMAPFILEHEADER bmFileHeader;
				file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

				BITMAPINFOHEADER bmInfoHeader;
				file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

				if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {

					id = _id;
					total_regions = _widthwise_regions * _heightwise_regions;
					total_item_count = _items_per_row * _total_rows;

					uint16_t temp_width = bmInfoHeader.biWidth;
					while ((temp_width % _items_per_row) != 0) temp_width--;

					set_RGB_Data(total_item_count, (uint16_t)(temp_width / _items_per_row), (uint16_t)(bmInfoHeader.biHeight / _total_rows), true);
					file.close();
					load_new_BMP(_id, _widthwise_regions, _heightwise_regions, _filename, _items_per_row, _total_rows, _outline_rgb, _rgb);
				}
				else {
					file.close();
					exit(14); // Incorrect encoding or presence of compression
				}
			}
			else exit(15); // File failed to open
		}
		else exit(16); // File is not a BMP file type
	}
	else exit(17); // Incorrect Sprite id has been supplied
}

Sprite& Sprite::operator[](uint8_t _elem) { elem = _elem + 1; return *this; }


void Sprite::load_new_BMP(uint8_t _id, uint8_t _widthwise_regions, uint8_t _heightwise_regions, std::string _filename, uint8_t _items_per_row, uint8_t _total_rows, uint8_t* _outline_rgb, uint8_t* _rgb) {
	if (_id > 1) {

		if (total_item_count == _items_per_row * _total_rows + 1) {

			if (_filename.find(".bmp") == _filename.length() - 4 || _filename.find(".BMP") == _filename.length() - 4) {

				std::ifstream file(_filename, std::ios::binary);

				if (!file.fail()) {

					BITMAPFILEHEADER bmFileHeader;
					file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

					BITMAPINFOHEADER bmInfoHeader;
					file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

					if (bmInfoHeader.biBitCount == 24 && bmInfoHeader.biCompression == BI_RGB) {

						total_item_count = _items_per_row * _total_rows;

						uint16_t width_compensation_pixels = 0, temp_width = bmInfoHeader.biWidth;
						while ((temp_width % _items_per_row) != 0) { temp_width--;  width_compensation_pixels++; }

						if (width == (uint16_t)(temp_width / _items_per_row) && height == (uint16_t)(bmInfoHeader.biHeight / _total_rows)) {

							uint16_t rgb_x, rgb_y;
							uint16_t bmp_item_elem = 0, rgb_elem = 1, items_before_target_item = 0, items_after_target_item = 0, row_start = 0;
							uint8_t temp_file_rgb[3] = { 0,0,0 };
							uint16_t line_padding_bytes = ((4 - (bmInfoHeader.biWidth * 3) % 4) % 4);
							uint16_t line_item_bytes = 3 * width;
							uint16_t width_compensation_bytes = 3 * width_compensation_pixels;

							for (uint8_t bmp_y = _total_rows; bmp_y > 0; bmp_y--) {
								for (uint8_t bmp_x = 0; bmp_x < _items_per_row; bmp_x++) {

									bmp_item_elem = (bmp_y - 1) * _items_per_row + bmp_x;

									items_before_target_item = (bmp_item_elem % _items_per_row);
									items_after_target_item = (_items_per_row - items_before_target_item - 1);
									row_start = (uint16_t)(bmp_item_elem / _items_per_row);

									file.seekg(0, file.beg);
									file.seekg(bmFileHeader.bfOffBits + (row_start * height * (line_item_bytes * (items_before_target_item + 1 + items_after_target_item) + width_compensation_bytes + line_padding_bytes) + (items_before_target_item * line_item_bytes)), std::ios::cur);

									rgb_y = height;

									for (int y = height; y > 0; y--) {

										rgb_y--;
										rgb_x = 0;

										for (int x = 0; x < width; x++) {
											for (int a = 0; a < 3; a++) { temp_file_rgb[a] = file.get(); }

											if (_outline_rgb) {
												if (temp_file_rgb[0] != _outline_rgb[2] && temp_file_rgb[1] != _outline_rgb[1] && temp_file_rgb[2] != _outline_rgb[0]) {
													rgb[rgb_elem][rgb_y * width + rgb_x][0] = _id + ((uint8_t)(rgb_y / (height / _heightwise_regions)) * _widthwise_regions + (uint8_t)(rgb_x / (width / _widthwise_regions)));
												}
											}
											else {
												rgb[rgb_elem][rgb_y * width + rgb_x][0] = _id + ((uint8_t)(rgb_y / (height / _heightwise_regions)) * _widthwise_regions + (uint8_t)(rgb_x / (width / _widthwise_regions)));
											}

											if (rgb[rgb_elem][rgb_y * width + rgb_x][0]) {
												if (_rgb) for (int a = 1; a < 4; a++) { rgb[rgb_elem][rgb_y * width + rgb_x][a] = _rgb[a - 1]; }
												else for (int a = 1; a < 4; a++) { rgb[rgb_elem][rgb_y * width + rgb_x][a] = temp_file_rgb[2 - (a - 1)]; }
											}

											rgb_x++;
										}
										file.seekg((items_after_target_item * line_item_bytes) + width_compensation_bytes + line_padding_bytes + (items_before_target_item * line_item_bytes), std::ios::cur);
									}
									rgb_elem++;
								}
							}
							file.close();
						}
						else exit(13); // Width and/or height of bmp are not equal to opened file
					}
					else {
						file.close();
						exit(14); // Incorrect encoding or presence of compression
					}
				}
				else exit(15); // File failed to open
			}
			else exit(16); // File is not a BMP file type
		}
		else exit(17); // Number of portions of file not equivalent to number of allocated portions in memory
	}
	else exit(18); // Incorrect Sprite id has been supplied
}


void Sprite::set_RGB(uint8_t* _rgb) {
	for (int i = 0; i < height * width; i++) {
		if (rgb[elem][i][0]) { for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _rgb[a - 1]; } }
	}
}

void Sprite::set_Find_Replace_RGB(uint8_t* _find_rgb, uint8_t* _replace_rgb) {
	for (int i = 0; i < height * width; i++) {
		if (rgb[elem][i][0] && rgb[elem][i][1] == _find_rgb[0] && rgb[elem][i][2] == _find_rgb[1] && rgb[elem][i][3] == _find_rgb[2]) {
			for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _replace_rgb[a - 1]; }
		}
	}
}

void Sprite::set_Complement_RGB(uint8_t* _find_rgb, uint8_t* _complement_rgb) {
	for (int i = 0; i < height * width; i++) {
		if (rgb[elem][i][0] && rgb[elem][i][1] != _find_rgb[0] && rgb[elem][i][2] != _find_rgb[1] && rgb[elem][i][3] != _find_rgb[2]) {
			for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _complement_rgb[a - 1]; }
		}
	}
}

void Sprite::set_RGB_on_SpriteRegion(uint8_t _id, uint8_t* _rgb) {
	for (uint32_t i = 0; i < height * width; i++) {
		if (rgb[elem][i][0] == _id) { for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _rgb[a - 1]; } }
	}
}