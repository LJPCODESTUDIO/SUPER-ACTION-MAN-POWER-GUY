#include "RGB_Data.h"

RGB_Data::RGB_Data(){}

void RGB_Data::set_RGB_Data(uint8_t _total_item_count, uint16_t _width, uint16_t _height, bool _memory_buffer) {
	if (_memory_buffer) {
		elem = 1;
		total_item_count = _total_item_count + 1;
	}
	else total_item_count = _total_item_count;

	width = _width; 
	height = _height;

	rgb = new uint8_t **[total_item_count];
	for (uint8_t g = 0; g < total_item_count; g++) {
		rgb[g] = new uint8_t * [width * height];
		for (uint32_t i = 0; i < width * height; i++) {
			if (_memory_buffer && !g) rgb[g][i] = new uint8_t[3];
			else rgb[g][i] = new uint8_t[4];
			for (uint8_t a = 0; a < 3; a++) { rgb[g][i][a] = 0; }
		}
	}
}

RGB_Data::~RGB_Data() {
	if (rgb != nullptr) {
		for (uint8_t g = 0; g < total_item_count; g++) {
			for (uint32_t j = 0; j < (width * height); j++) {
				delete[] rgb[g][j]; rgb[g][j] = nullptr;
			} delete[] rgb[g]; rgb[g] = nullptr;
		} delete[] rgb; rgb = nullptr;
	}
}