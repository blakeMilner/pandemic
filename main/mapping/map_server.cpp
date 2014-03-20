#include "map.hpp"

Map* MapServer::currmap = NULL;

Map_symbol MapServer::get_symbol(int x, int y){
	// is this hurting performance?
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

void MapServer::copy_field(Map_symbol** o, Pair<int> coor, Pair<int> vis_rad){
	// TODO: perhaps replace corner/angle computation with LUT?



	// compute blocks that a block will occlude beforehand
	Pair<int> dims = 2*vis_rad + 1;

	for(int y = 0; y < dims.y; y++){
	for(int x = 0; x < dims.x; x++){
		o[x][y] = MapServer::get_symbol(coor.x + x, coor.y + y);
	}}

	Map_symbol next_sym;
	Pair<float> corners[4];
	float diff, max_diff;
	Pair<int> max_pair;
	Pair<float> block; // blocks that might be occluded
	float block_dist;

	// loop through each block, marking out blocks it occludes
	for(int x = -vis_rad.x; x <= vis_rad.x; x++){
	for(int y = -vis_rad.y; y <= vis_rad.y; y++){
		next_sym = MapServer::get_symbol(coor.x + x + vis_rad.x, coor.y + y + vis_rad.y);
		block_dist = Pair<float>(x, y).mag();

		// if this block is occluded then find other blocks it occludes
		// TODO: replace is_occluded with has_type(next_sym, BLOCKING)
		if(NAV::is_occluded(next_sym)){
			corners[0] = Pair<float>(x - 0.5, y - 0.5);
			corners[1] = Pair<float>(x + 0.5, y - 0.5);
			corners[2] = Pair<float>(x - 0.5, y + 0.5);
			corners[3] = Pair<float>(x + 0.5, y + 0.5);

			// compute all combos
			max_diff = 0;
			max_pair.set(1,2);
			for(int s = 0; s < 4; s++){
			for(int e = s + 1; e < 4; e++){
				diff = corners[s].angle_with(corners[e]);
				if(diff > max_diff){
					max_diff = diff;
					max_pair.set(s,e);
				}
			}
			}

			// loop through all blocks, see if they are within largest vectors
			for(int xx = -vis_rad.x; xx <= vis_rad.x; xx++){
			for(int yy = -vis_rad.y; yy <= vis_rad.y; yy++){
				// don't check if block is already occluded
				if(o[xx + vis_rad.x][yy + vis_rad.y] == OCCLUDED){
					continue;
				}

				block = Pair<float>(xx, yy);
				if( block.is_between(corners[max_pair.x], corners[max_pair.y]) and block.mag() > block_dist ){
					o[xx + vis_rad.x][yy + vis_rad.y] = OCCLUDED;
				}
			}
			}
		}
	}}
}

void MapServer::move_character(Character* this_ptr, Nav_symbol dir){
	Pair<int> a = this_ptr->get_coor() + NAV::get_direction(dir);
	if(currmap != NULL){
		currmap->move_character(this_ptr, a);
	}
}

void MapServer::bite_player(Pair<int> pos){
	if(currmap->get_symbol(pos) == HUMAN){
		currmap->infect_player(currmap->get_ID(pos));
	}
}
