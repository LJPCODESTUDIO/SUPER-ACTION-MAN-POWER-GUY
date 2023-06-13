#pragma once
#pragma warning(disable : 4244)
#include "MainWindow.h"
#include "Image.h"
#include "Sprite.h"


class Gui {

private:

	int tempvar = 0;

	//DirectX functionality
	MainWindow& app_window;
	Graphics gfx_ops;

	//Window size
	uint16_t WindowWidth = gfx_ops.ScreenWidth;
	uint16_t WindowHeight = gfx_ops.ScreenHeight;

	//Gui reference variables
	uint8_t Gui_elem = 0, SubGui_elem = 0;

	//Gui specific array limits
	bool memory_buffer = false;
	uint8_t Gui_count;
	uint16_t** Gui_data = nullptr;

	//Gui, sub-gui and SubGuiRegion specifications
	uint16_t*** SubGui_data = nullptr;
	uint16_t**** SubGuiRegion_data = nullptr;

	//Gui surface data
	uint8_t*** GuiSurface = nullptr;

	//Counters
	uint32_t gui_region_counter = 0, surface_counter = 0;
	uint8_t gui_counter = 0, sub_gui_counter, rgb_counter = 0;
	int16_t X = 0, Y = 0;
	uint16_t x = 0, y = 0, SubGuiRegion_spacing = 0;

public:

	//Initialization and memory allocation
	Gui(class MainWindow& _app_window);
	
	//Memory de-allocation
	~Gui();


	//Returns the size of Gui-Region in Pixels
	uint16_t get_SubGuiRegion_Size();


	//Sets target Gui elements
	void set_Target_SubGui(uint8_t _Gui_elem = 0, uint8_t _SubGui_elem = 0);


	//Scrolling within a larger than a window Gui
	void set_SubGui_Pos_at_Pixel(uint16_t _surface_x, uint16_t _surface_y);


	//Returns current mouse position as Gui-Region elem. Returns -1 if Gui-Region elem not found.
	int32_t get_SubGuiRegion_at_Mouse_Pos();

	//Returns current mouse X position within Window regardless size of Gui
	int16_t get_Mouse_XPos_on_SubGui();

	//Returns current mouse Y position within Window regardless size of Gui
	int16_t get_Mouse_YPos_on_SubGui();




	//Refreshes Window
	void refresh_Gui();

	
	//Sets Gui color
	void set_RGB_on_Gui(uint8_t* _rgb);

	//Sets Sub-Gui color
	void set_RGB_on_SubGui(uint8_t* _rgb);

	//Sets Gui-Region color
	void set_RGB_on_SubGuiRegion(uint32_t _SubGuiRegion_elem, uint8_t* _rgb);

	//Sets Pixel color
	void set_RGB_on_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t* _rgb);

	//Sets Gui-Region border color (see default params)
	void set_RGB_on_SubGuiRegion_Border(uint32_t _SubGuiRegion_elem, uint8_t* _SubGuiRegion_border_rgb, bool _n = true, bool _e = true, bool _s = true, bool _w = true);
	


	//Sets BMP on within a Gui (see default param)
	void set_BMP_on_Gui(std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t _target_item = 0, uint8_t* _rgb = nullptr);

	//Sets BMP on within a Sub-Gui (see default param)
	void set_BMP_on_SubGui(std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t _target_item = 0, uint8_t* _rgb = nullptr);

	//Sets BMP on using Gui-Region addressing (see default param)
	void set_BMP_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t _target_item = 0, uint8_t* _rgb = nullptr);

	//Sets BMP on using Pixel addressing (see default param)
	void set_BMP_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, std::string _filename, uint8_t _items_per_row = 1, uint8_t _total_rows = 1, uint8_t* _outline_rgb = nullptr, uint8_t _target_item = 0, uint8_t* _rgb = nullptr);



	//Sets Image on within a Gui
	void set_Image_on_Gui(Image& _image);

	//Sets Image on within a Sub-Gui
	void set_Image_on_SubGui(Image& _image);

	//Sets Image on within a Gui-Region
	void set_Image_on_SubGuiRegion(uint32_t _SubGuiRegion_elem, Image& _image);

	//Sets Image on using Gui-Region addressing (see default param)
	void set_Image_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, Image& _image, std::string _origin = "nw", uint16_t _image_x = 0, uint16_t _image_y = 0);

	//Sets Image on using Pixel addressing (see default params)
	void set_Image_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Image& _image, std::string _origin = "nw", uint16_t _image_x = 0, uint16_t _image_y = 0);



	//Sets Sprite on within a Gui
	void set_Sprite_on_Gui(Sprite& _sprite);

	//Sets Sprite on within a Sub-Gui
	void set_Sprite_on_SubGui(Sprite& _sprite);

	//Sets Sprite on within a Gui-Region
	void set_Sprite_on_SubGuiRegion(uint32_t _SubGuiRegion_elem, Sprite& _sprite);

	//Sets Sprite on using Gui-Region addressing (see default param)
	void set_Sprite_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, Sprite& _sprite, std::string _origin = "nw");

	//Sets Sprite on using Pixel addressing (see default params)
	void set_Sprite_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, std::string _origin = "nw");

	//Turns latest set instance of an image off
	void set_Sprite_Off(Sprite& _sprite);
	


	//Sets Collision Map data using Gui-Region addressing
	void set_CollisionMap_Data_at_SubGuiRegion(uint32_t _SubGuiRegion_elem, bool _include_SubGuiRegion_spacing, uint8_t _id, bool _write_to_file = false);

	//Sets Collision Map data using Pixel addressing
	void set_CollisionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t _id, bool _write_to_file = false);


	//Gets Collision Map data using Gui-Region addressing
	uint8_t get_CollisionMap_Data_at_SubGuiRegion(uint32_t _SubGuiRegion_elem);

	//Gets Collision Map data using Pixel addressing
	uint8_t get_CollisionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y);

	//Checks for collision
	uint8_t get_CollisionMap_Data_at_SpriteRegion(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, uint8_t _region_id, std::string _origin = "nw");

	

	//Saves specified Gui-Region
	void save_SubGuiRegion(uint32_t _SubGuiRegion_elem, bool _include_SubGuiRegion_spacing = false);

	//Loads previously saved Gui-Region (see default params)
	void load_SubGuiRegion(uint32_t _source_SubGuiRegion_elem, bool _include_SubGuiRegion_spacing = false, int32_t _target_SubGuiRegion_elem = -1);
};