#pragma once
#pragma warning(disable : 4244)
#include "MainWindow.h"
#include "Image.h"
#include "Sprite.h"

class Gui {

private:
	//DirectX functionality
	MainWindow& app_window;
	Graphics gfx_ops;

	//Gui reference variables
	uint8_t gui_elem = 0, SubGui_elem = 0;

	//Gui specific array limits
	uint8_t total_gui_count;
	uint8_t** SubGui_count = nullptr;

	//Gui, sub-gui and GuiRegion specifications
	uint16_t*** SubGui_data = nullptr;
	uint16_t**** GuiRegion_data = nullptr;

	//Gui surface data
	uint8_t*** GuiSurface = nullptr;

public:

	//Initialization and memory allocation
	Gui(class MainWindow& _app_window);
	
	//Memory de-allocation
	~Gui();


	//Returns the size of Gui-Region in Pixels
	uint16_t get_GuiRegion_Size();


	//Refreshes Window
	void refresh_Gui();


	//Sets target Gui and Sub-Gui elements
	void set_Target_SubGui(uint8_t _gui_elem, uint8_t _SubGui_elem);

	//Gets target Gui element
	uint8_t get_Gui_Elem();

	//Gets target Sub-Giu element
	uint8_t get_SubGui_Elem();



	//Returns current mouse position as Gui-Region elem. Returns -1 if Gui-Region elem not found.
	int16_t get_Mouse_Pos_at_GuiRegion();


	
	//Sets Gui color
	void set_RGB_on_Gui(uint8_t* _rgb);

	//Sets Sub-Gui color
	void set_RGB_on_SubGui(uint8_t* _rgb);

	//Sets Gui-Region color
	void set_RGB_on_GuiRegion(uint16_t _GuiRegion_elem, uint8_t* _rgb);

	//Sets Pixel color
	void set_RGB_on_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t* _rgb);

	//Sets Gui-Region border color (see default params)
	void set_RGB_on_GuiRegion_Border(uint16_t _GuiRegion_elem, uint8_t* _GuiRegion_border_rgb, bool _n = true, bool _e = true, bool _s = true, bool _w = true);

	//Sets Sprite-Region color
	void set_RGB_on_SpriteRegion(uint8_t _sprite_region_id, uint8_t* _rgb);



	//Sets Image within a Gui
	void set_Image_on_Gui(Image& _image);

	//Sets Image within a Sub-Gui
	void set_Image_on_SubGui(Image& _image);

	//Sets Image within a Gui-Region
	void set_Image_on_GuiRegion(uint16_t _GuiRegion_elem, Image& _image);

	//Sets Image using Gui-Region addressing (see default param)
	void set_Image_at_GuiRegion(uint16_t _GuiRegion_elem, Image& _image, std::string _origin = "nw");

	//Set Image using Pixel addressing (see default param)
	void set_Image_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Image& _image, std::string _origin = "nw");
	


	//Sets Sprite on within a Gui
	void set_Sprite_on_Gui(Sprite& _sprite);

	//Sets Sprite on within a Sub-Gui
	void set_Sprite_on_SubGui(Sprite& _sprite);

	//Sets Sprite on within a Gui-Region
	void set_Sprite_on_GuiRegion(uint16_t _GuiRegion_elem, Sprite& _sprite);

	//Sets Sprite on using Gui-Region addressing (see default param)
	void set_Sprite_at_GuiRegion(uint16_t _GuiRegion_elem, Sprite& _sprite, std::string _origin = "nw");

	//Sets Sprite on using Pixel addressing (see default params)
	void set_Sprite_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, Sprite& _sprite, std::string _origin = "nw");

	//Turns sprite off
	void set_Sprite_Off(Sprite& _sprite);
	


	//Sets Collision Map data using Gui-Region addressing
	void set_CollissionMap_Data_at_GuiRegion(uint16_t _GuiRegion_elem, uint8_t _id, bool _write_to_file = false);

	//Sets Collision Map data using Pixel addressing
	void set_CollissionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y, uint8_t _id, bool _write_to_file = false);


	//Gets Collision Map data using Gui-Region addressing
	uint8_t get_CollissionMap_Data_at_GuiRegion(uint16_t _GuiRegion_elem);

	//Gets Collision Map data using Pixel addressing
	uint8_t get_CollissionMap_Data_at_Pixel(uint16_t _surface_x, uint16_t _surface_y);

	

	//Saves specified Gui-Region
	void save_GuiRegion(uint16_t _GuiRegion_elem, bool _include_GuiRegion_spacing = false);

	//Loads previously saved Gui-Region (see default params)
	void load_GuiRegion(uint16_t _source_GuiRegion_elem, bool _include_GuiRegion_spacing = false, int16_t _target_GuiRegion_elem = -1);



	//Saves specified Pixel
	void save_Pixel(uint16_t _surface_x, uint16_t _surface_y);

	//Loads previously saved Pixel (see default params)
	void load_Pixel(uint16_t _source_surface_x, uint16_t _source_surface_y, int16_t _target_surface_x = -1, int16_t _target_surface_y = -1);
};