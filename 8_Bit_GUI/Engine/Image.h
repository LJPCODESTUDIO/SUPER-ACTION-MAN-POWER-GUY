#pragma once
#include <stdint.h>
#include <string>

class Image {
private:
	friend class Sprite;
	friend class Gui;
	uint8_t total_elem_count = 0;
	uint16_t width = 0, height = 0;
	uint8_t elem = 0;
	uint8_t*** rgb = nullptr;

public:
	Image(std::string _filename, uint8_t* _outline_rgb = nullptr, uint8_t* _image_rgb = nullptr, uint8_t _images_per_row = 1, uint8_t _total_rows = 1);
	Image(uint8_t _sprite_id, std::string _filename, uint8_t* _outline_rgb = nullptr, uint8_t* _image_rgb = nullptr, uint8_t _images_per_row = 1, uint8_t _total_rows = 1, uint8_t _widthwise_regions = 1, uint8_t _heightwise_regions = 1);
	~Image();

	Image& operator[](uint8_t _elem);

	uint16_t get_Width();
	uint16_t get_Height();

	void set_RGB(uint8_t* _rgb);
	void set_Find_Replace_RGB(uint8_t* _find_rgb, uint8_t* _replace_rgb);
	void set_Complement_RGB(uint8_t* _find_rgb, uint8_t* _complement_rgb);
};