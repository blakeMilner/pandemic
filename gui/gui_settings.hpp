#ifndef GUI_SETTINGS_HPP
#define GUI_SETTINGS_HPP

#include <QColor>
#include <map>

#include "../main/enums/object_enum.hpp"

using namespace std;

namespace GUI_settings{
    static Pair<int> ROI_coors = Pair<int>(0,0);
	static const Pair<int> MIN_ROI_DIMS = Pair<int>(0,0);
	static const Pair<int> MAX_ROI_DIMS = Pair<int>(5000,5000); // need to put in check that this isn't over actual dims
	static Pair<int> ROI_dims = MIN_ROI_DIMS;

	static int fps = 1;
	static int ms_per_frame = (1000/fps);
	static const int MIN_FPS = 1;
	static const int MAX_FPS = 60;

	static const int MIN_pix_per_symbol = 1;
    static const int MAX_pix_per_symbol = 10;
    static int mainView_pps = MIN_pix_per_symbol; // pix per symbol

    static int pan_pps = 3; // pix per symbol per pan
    static int ms_per_pan = 30;

#define QT_BLACK 			qRgb(0,0,0)
#define QT_GREEN 			qRgb(0,128,0)
#define QT_YELLOW 			qRgb(255,255,0)
#define QT_RED 				qRgb(255,0,0)
#define QT_WHITE 			qRgb(255,255,255)
#define QT_BURNT_BROWN 		qRgb(217,120,17)
#define QT_RUSTIC_SILVER 	qRgb(125, 120, 112)
#define QT_DARK_TURQUOISE 	qRgb(7,59,49)

	static map<Map_symbol, QRgb> init_map(){
        // SET DEFAULT MAP SYMBOL COLOR MAPPINGS
		map<Map_symbol, QRgb> color_map;

        // yet another reason to have an enum class...
        color_map[INVALID]          = QT_BLACK;
        color_map[HUMAN]            = QT_GREEN;
        color_map[INFECTED]         = QT_YELLOW;
        color_map[ZOMBIE]           = QT_RED;
        color_map[CHARACTER]        = QT_BLACK;
        color_map[BUILDING]         = QT_WHITE;
        color_map[BUILDING_DOOR]    = QT_BURNT_BROWN;
        color_map[BUILDING_WALL]    = QT_RUSTIC_SILVER;
        color_map[BUILDING_INDOOR]  = QT_DARK_TURQUOISE;
        color_map[OBJECT]           = QT_BLACK;
        color_map[OBSTACLE]     	= QT_BLACK;
        color_map[OCCLUDED]     	= QT_BLACK;
        color_map[EMPTY]        	= QT_BLACK;

		return color_map;
	}
	static map<Map_symbol, QRgb> color_map = GUI_settings::init_map();

	static const char* play_icon_path = "play.bmp";
	static const char* pause_icon_path = "pause.bmp";
	static const char* reset_icon_path = "reset.bmp";
    static const char* next_icon_path = "next.bmp";
}

#endif
