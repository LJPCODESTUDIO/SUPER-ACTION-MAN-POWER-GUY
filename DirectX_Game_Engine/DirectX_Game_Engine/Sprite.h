#pragma once
#include "Image.h"

class Sprite : public Image {
private:
	friend class Gui;
	uint8_t id, total_regions;

public:
	Sprite(uint8_t _sprite_id, std::string _filename, uint8_t* _outline_rgb = nullptr, uint8_t* _sprite_rgb = nullptr, uint8_t _sprites_per_row = 1, uint8_t _total_rows = 1, uint8_t _widthwise_regions = 1, uint8_t _heightwise_regions = 1);

	Sprite& operator[](uint8_t _elem);

	void set_RGB_on_SpriteRegion(uint8_t _region_id, uint8_t* _rgb);
};