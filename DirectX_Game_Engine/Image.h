#pragma once
#pragma warning(disable : 4018)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
#pragma warning(disable : 26451)
#include "Windows.h"
#include <stdint.h>
#include <string>
#include <fstream>
#include "RGB_Data.h"

class Image : public RGB_Data {
public:

	Image(std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t* _rgb = nullptr);

	Image& operator[](uint8_t _elem);

	void load_new_BMP(std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t* _rgb = nullptr);
	void set_RGB(uint8_t* _rgb);
	void set_Find_Replace_RGB(uint8_t* _find_rgb, uint8_t* _replace_rgb);
	void set_Complement_RGB(uint8_t* _find_rgb, uint8_t* _complement_rgb);
};