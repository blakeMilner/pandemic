#include "factory.hpp"
#include <iostream>

namespace MS = Map_settings;

Human* Factory::make_human(int xstart, int ystart){
	return(	new Human(
				random_pair(xstart, xstart + MS::region_len, ystart, ystart + MS::region_len)
			 ));
}


// NEED TO MAKE STRUCTURE FOR ALL
list<Obstacle> Factory::make_buildings(list<Obstacle>& ob, int num_b, Pair reg_by_reg_coor){
//	Pair curr_abs_coor = reg_by_reg_coor * MS::region_len;
//	int max_len = MS::max_building_len;
//	int min_len = MS::min_building_len;
//	int reg_len = MS::region_len;
//
//	Pair reg_obs_coors, abs_obs_coors, obs_dims;
//	queue<Pair> vert;
	list<Obstacle> newbuilds;
//
//	// need to find a better way to do this (perhaps init helpers at start and delete at finish?)
//	if(~coor_is_alloced)
//		init_poss_coors();
//	set_poss_coors(curr_abs_coor);
//
//	// generate dimensions for desired number of buildings
//	list<Building> poss_build;
//	queue<Pair> vertices;
//	Pair ran_dims;
//	for(int i = 0; i < num_b; i++){
//		// temporary
//		ran_dims = random_pair(min_len, max_len, min_len, max_len);
//		vertices.push(Pair(0, 0));
//		vertices.push(Pair(ran_dims.x - 1, 0));
//		vertices.push(Pair(ran_dims.x - 1, ran_dims.y - 1));
//		vertices.push(Pair(0, ran_dims.y - 1));
//		//
//
//		poss_build.push_front(
//				Building(vertices, ran_dims, Pair(0,0),	Pair(0,0), reg_by_reg_coor));
//
//		vertices = queue<Pair>();
//	}
//	poss_build.sort(Dimension_cmp()); // sort by area
//
//	// remove current obstacles from coordinates - only square dimensions
//	list<Obstacle>::iterator ito;
//	for(ito = ob.begin(); ito != ob.end(); ito++){
//		cout << "OLD" << endl;
//		abs_obs_coors = ito->get_rel_coor();
//		reg_obs_coors = ito->get_reg_coor();
//		obs_dims = ito->get_dimensions();
//
//		stamp_poss_coors(reg_obs_coors - reg_by_reg_coor, obs_dims, abs_obs_coors);
//	}
//
//	list<Building>::iterator itc;
//	Pair rand_coor, build_dims;
//
//	// loop through generated buildings trying to fit them in
//	for(itc = poss_build.begin(); itc != poss_build.end(); itc++){
//		build_dims = itc->get_dimensions();
//		for(int tryc = 0; tryc < MAX_TRIES; tryc++){
//			rand_coor = random_pair(0, reg_len - 1, 0, reg_len - 1);
//
//			if(placement_is_clear(build_dims, rand_coor)){
//				itc->set_rel_coor(rand_coor); // need to combine these..
//				itc->set_abs_coor(curr_abs_coor + rand_coor);
//
//				newbuilds.push_front(*itc);
//
//				stamp_poss_coors(Pair(0,0), build_dims, rand_coor);
//				break;
//			}
//
//		}
//	}
//cout << "++++++++++++++++++++++" << endl;
	return(newbuilds);
}

// POSSIBLE ALTERNATIVE STRUCTURE: GIVE REFERENCE TO MAP AND
// GENERATE REGIONS SIMULTANEOUSLY

void Factory::init_poss_coors(){
	if(~coor_is_alloced){
		poss_coors = new char*[3*MS::region_len];
		for (int i = 0; i < 3*MS::region_len; i++){
			poss_coors[i] = new char[3*MS::region_len];
		}

		coor_is_alloced = true;
		coor_is_deleted = false;
	}
}

void Factory::set_poss_coors(Pair rc){
	int newx, newy;
// need to only portions of regions that could cause trouble
	for (int i = 0; i < 3*MS::region_len; i++){
		for(int j = 0; j < 3*MS::region_len; j++){
			newx = rc.x - MS::region_len + j;
			newy = rc.y - MS::region_len + i;
			if((newx >= 0) && (newy >= 0) && (newx < MS::map_len[0]) && (newy < MS::map_len[1])){
				poss_coors[j][i] = VALID_COOR;
			}
			else{
				poss_coors[j][i] = INVALID_COOR;
			}
		}
	}
}

void Factory::stamp_poss_coors(Pair reg_offset, Pair dims, Pair coor){
	Pair offset = (reg_offset + 1)*MS::region_len;
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			poss_coors[offset.x + coor.x + x][offset.y + coor.y + y] = INVALID_COOR;
		}
	}
	for(int i = 0; i < 3*MS::region_len; i++){
		for(int j = 0; j < 3*MS::region_len; j++){
			cout << poss_coors[i][j];
		}
		cout << endl;
	}
	cout << endl << dims.x << " " << dims.y << endl;

}

Pair Factory::find_empty_path(int x_search, int y_search, int xincr, int yincr){
	Pair empty = Pair(0,0);
	int i = 0;

	while(poss_coors[MS::region_len + x_search + i*xincr][MS::region_len + y_search + i*yincr] != INVALID_COOR){
		empty.x += xincr;
		empty.y += yincr;

		empty++;
		i++;
	}

	return(empty);
}

bool Factory::placement_is_clear(Pair dims, Pair coor){
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			if(poss_coors[MS::region_len + coor.x + x][MS::region_len + coor.y + y] == INVALID_COOR)
				return false;
		}
	}

	return true;
}

void Factory::delete_poss_coors(){
	if(~coor_is_deleted){
		for(int i = 0; i < 3*MS::region_len; i++){
			delete [] poss_coors[i];
		}
		delete poss_coors;

		coor_is_deleted = true;
	}
}
