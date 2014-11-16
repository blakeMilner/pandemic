#ifndef GUI_SETTINGS_HPP
#define GUI_SETTINGS_HPP

#include <QColor>
#include <map>

#include "../main/enums/object_enum.hpp"
#include "../main/settings/map_settings.hpp"

using namespace std;

namespace GUI_settings{
    // settings for size of map
    static Pair<int> ROI_coors = Pair<int>(0,0);
    static Pair<int> ROI_dims = Map_settings::map_len;

    // settings for frames per sec
	static int fps = 1;
	static int ms_per_frame = (1000/fps);
	static const int MIN_FPS = 1;
	static const int MAX_FPS = 60;

    // settings for game resolution
	static const int MIN_pix_per_symbol = 1;
    static const int MAX_pix_per_symbol = 100;
    static int mainView_pps = MIN_pix_per_symbol; // pix per symbol

    // settings for panning through map
    static int pan_pps = 3; // pix per symbol per pan
    static int ms_per_pan = 30;

    // settings for settings table!
    static QStringList populate_settings(){
        QStringList ll;
//        ll << "World X width" << "World Y Width" << ""

//              int Map_settings::min_human_density = 6;
//              int Map_settings::max_human_density = 12;
//              int Map_settings::min_zombie_density = 4;
//              int Map_settings::max_zombie_density = 8;

//              // BUILDING GENERATION SETTINGS
//              const int Map_settings::MAX_LENGTH_DIFF = 3;
//              int Map_settings::min_building_density = 10;
//              int Map_settings::max_building_density = 35;

//              int Map_settings::min_footprint_len = 14; // possible put these as pairs???
//              int Map_settings::max_footprint_len = 32;
//              int Map_settings::min_building_len = 10;

//              ;
        return ll;
    }

    static const int NUM_SETTINGS = 10;
    static const QStringList settings_label_list = GUI_settings::populate_settings();

    // Definitions for QT RGB colors
#define QT_BLACK 			qRgb(0,0,0)
#define QT_GREEN 			qRgb(0,128,0)
#define QT_YELLOW 			qRgb(255,255,0)
#define QT_RED 				qRgb(255,0,0)
#define QT_WHITE 			qRgb(255,255,255)
#define QT_BURNT_BROWN 		qRgb(217,120,17)
#define QT_RUSTIC_SILVER 	qRgb(125, 120, 112)
#define QT_DARK_TURQUOISE 	qRgb(7,59,49)

    /* color mapping for different block types */
    static map<Map_symbol, QRgb> init_map(){
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

    // relative paths for button icons
	static const char* play_icon_path = "play.bmp";
	static const char* pause_icon_path = "pause.bmp";
	static const char* reset_icon_path = "reset.bmp";
    static const char* next_icon_path = "next.bmp";
}

#endif
