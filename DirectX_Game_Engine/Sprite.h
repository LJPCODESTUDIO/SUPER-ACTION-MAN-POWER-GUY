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

class Sprite : public RGB_Data {
private:
	uint8_t total_regions = 1;
public:
	uint8_t id = 0;
	uint16_t x_coord = 0, y_coord = 0;
	std::string origin = "nw";
	bool displayed = false;

	Sprite(uint8_t _id, uint8_t _widthwise_regions, uint8_t _heightwise_regions, std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t * _outline_rgb = nullptr, uint8_t* _rgb = nullptr);

	Sprite& operator[](uint8_t _elem);

	void load_new_BMP(uint8_t _id, uint8_t _widthwise_regions, uint8_t _heightwise_regions, std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t* _rgb = nullptr);
	void set_RGB(uint8_t* _rgb);
	void set_Find_Replace_RGB(uint8_t* _find_rgb, uint8_t* _replace_rgb);
	void set_Complement_RGB(uint8_t* _find_rgb, uint8_t* _complement_rgb);
	void set_RGB_on_SpriteRegion(uint8_t _region_id, uint8_t* _rgb);
};