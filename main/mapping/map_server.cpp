#include "map.hpp"

namespace CS = Char_stats;

MapServer* MapServer::p_Instance = NULL;
Map* MapServer::currmap = NULL;

MapServer::MapServer()
{
	// create LUT for block occlusion at max visual radius

	Pair<int> dims = 2 * CS::MAX_VIS_RAD + 1;

	BLOCKS_OCCLUDED = new vector<Pair<int> >* [dims.x];
	for(int x = 0; x < dims.x; x++){
		BLOCKS_OCCLUDED[x] = new vector<Pair<int> > [dims.y];
	}

	Map_symbol next_sym;
	Pair<float> corners[4];
	float diff, max_diff;
	Pair<int> max_pair;
	Pair<float> block; // blocks that might be occluded
	float block_dist;

	// loop through each block, marking out blocks it occludes
	for(int x = -CS::MAX_VIS_RAD; x <= CS::MAX_VIS_RAD; x++){
	for(int y = -CS::MAX_VIS_RAD; y <= CS::MAX_VIS_RAD; y++){
		block_dist = Pair<float>(x, y).mag();

		// compute vectors that go to block corners
		corners[0] = Pair<float>(x - 0.5, y - 0.5);
		corners[1] = Pair<float>(x + 0.5, y - 0.5);
		corners[2] = Pair<float>(x - 0.5, y + 0.5);
		corners[3] = Pair<float>(x + 0.5, y + 0.5);

		// compute and compare all combos, find pair with largest angle
		max_diff = 0;
		max_pair.set(1,2);
		for(int s = 0; s < 4; s++){
		for(int e = s + 1; e < 4; e++){
			diff = corners[s].angle_with(corners[e]);
			if(diff > max_diff){
				max_diff = diff;
				max_pair.set(s,e);
			}
		}}

		// loop through all blocks, see if they are within largest vectors
		for(int xx = -CS::MAX_VIS_RAD; xx <= CS::MAX_VIS_RAD; xx++){
		for(int yy = -CS::MAX_VIS_RAD; yy <= CS::MAX_VIS_RAD; yy++){
			block = Pair<float>(xx, yy);
			if( block.is_between(corners[max_pair.x], corners[max_pair.y]) and block.mag() > block_dist ){
				BLOCKS_OCCLUDED[x + CS::MAX_VIS_RAD][y + CS::MAX_VIS_RAD]
				 .push_back( Pair<int>(xx, yy) );
			}
		}
		}

		// sort based on maximum element size (to disregard blocks outside vision later)
		std::sort(BLOCKS_OCCLUDED[x + CS::MAX_VIS_RAD][y + CS::MAX_VIS_RAD].begin(),
			BLOCKS_OCCLUDED[x + CS::MAX_VIS_RAD][y + CS::MAX_VIS_RAD].end(),
			Max_mag_cmp<int>());

	}}
}

MapServer::~MapServer(){
	for(int x = 0; x < Char_stats::MAX_VIS_RAD; x++){
		delete [] BLOCKS_OCCLUDED[x];
	}

	delete BLOCKS_OCCLUDED;
}

void MapServer::delete_instance(){
	if(this->p_Instance){
		delete this->p_Instance;
	}
}

// singleton method for this class - call to get lone pointer
MapServer* MapServer::Instance(){
	if(!p_Instance){
		p_Instance = new MapServer;
	}

	return( p_Instance );
}

void MapServer::set_map(Map* map){
	currmap = map;
}

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

void MapServer::empty_field(Map_symbol** char_vision, Pair<int> dims){
	for(int y = 0; y < dims.y; y++){
	for(int x = 0; x < dims.x; x++){
		char_vision[x][y] = EMPTY;
	}}
}

void MapServer::copy_field(Map_symbol** char_vision, Pair<int> coor, Pair<int> vis_rad){
	Map_symbol next_sym;
	Pair<int> dims = 2*vis_rad + 1;
	// offset to disregard precomputed blocks outside current radius
	Pair<int> offset = CS::MAX_VIS_RAD - vis_rad;

	empty_field(char_vision, dims);

	for(int y = 0; y < dims.y; y++){
	for(int x = 0; x < dims.x; x++){
		// TODO: replace is_occluded with has_type(next_sym, BLOCKING)
		next_sym = MapServer::get_symbol(coor.x + x, coor.y + y);

		// place block if it already hasn't been marked as occluded
		if(char_vision[x][y] != OCCLUDED){
			char_vision[x][y] = next_sym;
		}


			// mark out occluded blocks if current block is occluded
			if( NAV::block_is(char_vision[x][y], OCCLUDED) ){
				for(vector<Pair<int> >::iterator it = BLOCKS_OCCLUDED[x + offset.x][y + offset.y].begin();
												it != BLOCKS_OCCLUDED[x + offset.x][y + offset.y].end(); it++){

					// keep removing blocks until we're outside visual radius
					if(*it + vis_rad < dims and *it + vis_rad >= 0)
						char_vision[(*it).x + vis_rad.x][(*it).y + vis_rad.y] = OCCLUDED;
					else
						break;
				}
			}

	}}

}

void MapServer::move_character(Character* char_ptr, Nav_symbol dir){
	Pair<int> a = char_ptr->get_coor() + NAV::get_direction(dir);
	if(currmap != NULL){
		currmap->move_character(char_ptr, a);
	}
}

void MapServer::bite_player(Pair<int> pos){
	if(currmap->get_symbol(pos) == HUMAN){
		currmap->infect_player(currmap->get_ID(pos));
	}
}

void MapServer::convert_infected(Infected* infected){
	currmap->convert_infected((Character *) infected);
}
