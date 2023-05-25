#pragma once
#include <stdint.h>

class RGB_Data {
private:
	friend class Gui;
	friend class Image;
	friend class Sprite;

	uint8_t total_elem_count = 0;
	uint16_t width = 0, height = 0;
	uint8_t elem = 0;
	uint8_t*** rgb = nullptr;

public:
	RGB_Data();
	~RGB_Data();
	void set_RGB_Data(uint8_t _total_elem_count, uint16_t _width, uint16_t _height, bool _Image = true);
};