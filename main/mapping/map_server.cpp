#include "map.hpp"


Map* MapServer::currmap = NULL;

Map_symbol MapServer::get_symbol(int x, int y){
	if(currmap != NULL){
		return(currmap->get_symbol(x,y));
	}
	else{
		return(INVALID);
	}
}

int MapServer::get_ID(int x, int y){
	if(currmap != NULL){
		return(currmap->get_ID(x,y));
	}
	else{
		return(INVALID_ID);
	}
}

void MapServer::copy_field(Map_symbol** o, Pair coor, Pair vis_rad){
	Pair dims = 2*vis_rad + 1;

	for(int x = 0; x < dims.x; x++){
	for(int y = 0; y < dims.y; y++){
		o[x][y] = MapServer::get_symbol(coor.x + x, coor.y + y);
	}}

//	Map_symbol next_sym;
//	flPair fl_pos, change;
//	Pair cartes_pos, pos, rnd_coor;
//	Pair dims = 2*vis_rad + 1;
//
//	for(int x = 0; x < dims.x; x++){
//	for(int y = 0; y < dims.y; y++){
//		next_sym = MapServer::get_symbol(coor.x + x, coor.y + y);
//
//		// skip if it has already been marked occluded
//		if(o[x][y] != OCCLUDED){
//			o[x][y] = next_sym;
//		}
//		// remove occluded blocks if next symbol causes occlusion
//		if(NAV::is_occluded(next_sym)){
//			// find unit vector so we know what direction
//			fl_pos = pos = Pair(x,y);
//			cartes_pos = pos - vis_rad;
//			change = cartes_pos.unit_v() / 10;
//
//			// get ourselves up to the next block so we don't occlude the parent
//			while(fl_pos.round() == pos){
//				fl_pos.x += change.x; fl_pos.y -= change.y;
//			}
//
//			// trace out along direction and occlude blocks that we find
//			while(fl_pos.within(Pair(0,0), dims - 1)){
//				rnd_coor = fl_pos.round();
//				o[rnd_coor.x][rnd_coor.y] = OCCLUDED;
//				fl_pos.x += change.x; fl_pos.y -= change.y;
//			}
//		}
//	}}
}

void MapServer::move_character(Character* this_ptr, Nav_symbol dir){
	Pair a = this_ptr->get_coor() + NAV::get_direction(dir);
	if(currmap != NULL){
		currmap->move_character(this_ptr, a);
	}
}

void MapServer::bite_player(Pair pos){
	if(currmap->get_symbol(pos) == HUMAN){
		currmap->infect_player(currmap->get_ID(pos));
	}
}
