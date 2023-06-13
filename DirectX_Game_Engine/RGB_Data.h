#pragma once
#include <stdint.h>

class RGB_Data {
public:
	uint8_t elem = 0;
	uint8_t total_item_count = 0;
	uint16_t width = 0, height = 0;
	uint8_t*** rgb = nullptr;

public:
	RGB_Data();
	~RGB_Data();
	void set_RGB_Data(uint8_t _total_item_count, uint16_t _width, uint16_t _height, bool _memory_buffer = false);
};