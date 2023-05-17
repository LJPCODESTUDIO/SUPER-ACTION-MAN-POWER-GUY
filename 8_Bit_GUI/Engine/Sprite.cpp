#include "Sprite.h"

Sprite::Sprite(uint8_t _sprite_id, std::string _filename, uint8_t* _outline_rgb, uint8_t* _sprite_rgb, uint8_t _sprites_per_row, uint8_t _total_rows, uint8_t _widthwise_regions, uint8_t _heightwise_regions)
	:
	Image(_sprite_id, _filename, _outline_rgb, _sprite_rgb, _sprites_per_row, _total_rows, _widthwise_regions, _heightwise_regions)
{
	id = _sprite_id;
	total_regions = _widthwise_regions * _heightwise_regions;
}

Sprite& Sprite::operator[](uint8_t _elem) { elem = _elem; return *this; }

void Sprite::set_RGB_on_Sprite_Region_ID(uint8_t _region_id, uint8_t* _rgb) {
	for (uint32_t i = 0; i < height * width; i++) {
		if (rgb[elem][i][0] == _region_id) { for (int a = 1; a < 4; a++) { rgb[elem][i][a] = _rgb[a - 1]; } }
	}
}