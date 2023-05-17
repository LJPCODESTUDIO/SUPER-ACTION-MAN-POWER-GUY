#pragma once
#include "MainWindow.h"
#include "Image.h"
#include "Sprite.h"

class Gui {

private:
	//DirectX functionality
	MainWindow& app_window;
	Graphics gfx_ops;

	//Gui reference variables
	uint8_t gui_elem = 0, subgui_elem = 0;

	//Gui specific array limits
	uint8_t total_gui_count;
	uint8_t** subgui_count = nullptr;

	//Gui, sub-gui and led specifications
	uint16_t*** subgui_data = nullptr;
	uint16_t**** subgui_led_data = nullptr;

	//Gui surface data
	uint8_t*** GS = nullptr;

public:

	//Gui class memory allocation and initialization
	Gui(class MainWindow& _app_window);
	
	//Gui class memory de-allocation and de-initialization
	~Gui();

	//Refreshes gui
	void refresh_Gui();

	void set_Target_SubGui(uint8_t _gui_elem, uint8_t _subgui_elem);

	//Getter: Gets current Gui element
	uint8_t get_Gui_Elem();

	//Getter: Gets current SubGiu element
	uint8_t get_SubGui_Elem();

	//Set image using pixel addressing
	void set_Image_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Image& _image);


	//Sets gui background color
	void set_RGB_on_Gui(uint8_t* _rgb);

	//Sets LED color
	void set_RGB_on_LED(uint16_t _led_elem, uint8_t* _rgb);

	void set_RGB_on_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t* _rgb);

	void set_RGB_on_Sprite_Region_ID(uint8_t _sprite_region_id, uint8_t* _rgb);


	//Sets LED image (see default param)
	void set_Image_on_LED(uint16_t _led_elem, Image& _image);


	//Sets LED border color (see default params)
	void set_RGB_on_LED_Border(uint16_t _led_elem, uint8_t* _led_border_rgb, bool _n = true, bool _e = true, bool _s = true, bool _w = true);

	//Saves specified LED
	void save_LED(uint16_t _led_elem);

	//Loads previously saved LED
	void load_LED(uint16_t _source_led_elem, int16_t _target_led_elem = -1);



	//Sets gui background image (see default param)
	void set_Image_on_Gui(Image& _image);


	//Sets subgui background color
	void set_RGB_on_SubGui(uint8_t* _rgb);

	//Sets subgui background image (see default param)
	void set_Image_on_SubGui(Image& _image);

	
	void set_Image_at_LED(uint16_t _led_elem, Image& _image);


	//Sets sprite on using gui addressing (see default param)
	void set_Sprite_on_Gui(Sprite& _sprite);

	//Sets sprite on using sub-sui addressing (see default param)
	void set_Sprite_on_SubGui(Sprite& _sprite);


	//Sets sprite on using LED addressing (see default param)
	void set_Sprite_at_LED(uint16_t _led_elem, Sprite& _sprite);

	//Sets sprite on using pixel addressing (see default params)
	void set_Sprite_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, std::string _origin = "nw");

	//Turns sprite off
	void set_Sprite_Off(Sprite& _sprite);

	void set_Collission_Map_at_LED(uint16_t _led_elem, uint8_t _region_id);
	void set_Collission_Map_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t _region_id);

	uint8_t get_Collission_Map_at_LED(uint16_t _led_elem);
	uint8_t get_Collission_Map_at_Pixel(uint16_t _surface_x, uint16_t _surface_y);

	//Returns current mouse position as LED elem. Returns -1 if LED elem not found.
	int16_t get_Mouse_Pos_at_LED();
	uint16_t get_LED_Size();

	void write_to_Text_File(std::string _function_call);
	void save_Pixel(uint16_t _surface_x, uint16_t _surface_y);
	void load_Pixel(uint16_t _source_surface_x, uint16_t _source_surface_y, int16_t _target_surface_x = -1, int16_t _target_surface_y = -1);
};

// uint16_t**** sub_gui_data
// g is cycling through each gui with total_gui_count as the limit
// i is cycling through each gui with gui_count[i][0] as the limit
// j is cycling through each sub-gui with gui_count[i][1] as the limit
// 
// sub_gui_data[g][i][j][0]  - xwise/widthwise pixel marking start of sub-gui
// sub_gui_data[g][i][j][1]  - ywise/heightwise pixel marking start of sub-gui
// sub_gui_data[g][i][j][2]  - xwise/widthwise pixel marking end of sub-gui
// sub_gui_data[g][i][j][3]  - ywise/heightwise pixel marking end of sub-gui
// sub_gui_data[g][i][j][4]  - total xwise/widthwise leds
// sub_gui_data[g][i][j][5]  - total ywise/heightwise leds
// sub_gui_data[g][i][j][6]  - shift right/xwise/widthwise pixels
// sub_gui_data[g][i][j][7]  - shift down/ywise/heightwise pixels
// sub_gui_data[g][i][j][8]  - led spacing pixels
// sub_gui_data[g][i][j][9]  - led size in pixels
// sub_gui_data[g][i][j][10] - R value of sub-gui background
// sub_gui_data[g][i][j][11] - G value of sub-gui background
// sub_gui_data[g][i][j][12] - B value of sub-gui background
// sub_gui_data[g][i][j][13] - R value of led rgb
// sub_gui_data[g][i][j][14] - G value of led rgb
// sub_gui_data[g][i][j][15] - B value of led rgb

// uint16_t***** sub_gui_led_data
// g is cycling through each gui with total_gui_count as the limit
// i is cycling through each gui with gui_count[i][0] as the limit
// j is cycling through each sub-gui with gui_count[i][1] as the limit
// k is cycling through each led in the sub_gui with (sub_gui_data[g][i][j][4] * sub_gui_data[g][i][j][5]) as the limit
// sub_gui_led_data[g][i][j][k][0] - xwise/widthwise pixel marking start of each led in sub-gui
// sub_gui_led_data[g][i][j][k][1] - xwise/widthwise pixel marking end of each led in sub-gui
// sub_gui_led_data[g][i][j][k][2] - ywise/heightwise pixel marking start of each led in sub-gui
// sub_gui_led_data[g][i][j][k][3] - ywise/heightwise pixel marking end of each led in sub-gui