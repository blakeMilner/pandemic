#ifndef GUI_SETTINGS_HPP
#define GUI_SETTINGS_HPP

#include <QColor>
#include <map>

#include "../main/enums/object_enum.hpp"

using namespace std;

namespace GUI_settings{
	static Pair<int> ROI_coors = Pair<int>(0,0);
	static Pair<int> ROI_dims = Pair<int>(20,20);
	static const Pair<int> MAX_ROI_DIMS = Pair<int>(1000,1000);

	static int fps = 2;
	static int ms_per_frame = (1000/fps);
	static const int MAX_FPS = 60;

	static int pix_per_symbol = 10;
	static const int MAX_pix_per_symbol = 100;

	static map<Map_symbol, QRgb> init_map(){
		// SET DEFAULT MAP SYMBOL COLOR MAPPINGS
		// TODO: make this more elegant
			map<Map_symbol, QRgb> color_map;

			color_map[INVALID] 		= qRgb(0,0,0);			// BLACK
			color_map[HUMAN]		= qRgb(0,128,0);		// GREEN
			color_map[ZOMBIE]		= qRgb(255,0,0);		// RED
			color_map[CHARACTER]	= qRgb(0,0,0);
			color_map[BUILDING]		= qRgb(255,255,255); 	// WHITE
			color_map[OBJECT]		= qRgb(0,0,0);
			color_map[OBSTACLE]		= qRgb(0,0,0);
			color_map[OCCLUDED]		= qRgb(0,0,0);
			color_map[EMPTY] 		= qRgb(0,0,0);

			return color_map;
		}

	static map<Map_symbol, QRgb> color_map = GUI_settings::init_map();

};

#endif
