#ifndef MREGION_HPP
#define MREGION_HPP

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include "../objects/character.hpp"
#include "../objects/obstacle.hpp"

using namespace std;

class mRegion{
public:
	mRegion(int, int, int);
	~mRegion();

	void insert_character(Character*);
	void insert_obstacle(Obstacle*);
	void remove_character(Character*);
	void remove_obstacle(Obstacle*);
	void switch_character(Character*, Character*);

	Pair<int> get_coor();
	int get_length();
private:
	Pair<int> coor;
	list<Obstacle*> obstacles;
	list<Character*> characters;
	int len;

};

#endif



//
//#ifndef MREGION_HPP
//#define MREGION_HPP
//
//#include <list>
//#include <map>
//#include "../objects/character.hpp"
//#include "../objects/obstacle.hpp"
//
//using namespace std;
//
//typedef struct linked_list{
//	Character* obj;
//	linked_list* next;
//} linked_list;
//
//class mRegion{
//public:
//	mRegion(int, int, int);
//	~mRegion();
//	void insert_character(Character*);
//	void insert_obstacle(Obstacle*);
//	void remove_character(Character*);
//	void remove_obstacle(Obstacle*);
//	void insert_node(linked_list**, Character*);
//	void delete_node(linked_list**, Character*);
//	linked_list* new_node(Character*);
//	Pair get_coor();
//	int get_length();
//	void exec();
//private:
//	Pair coor;
//	list<Obstacle*> obstacles;
//	linked_list* characters_start;
//	int len;
//
//	linked_list* chars;
//	void start_char_iter();
//	pObject* get_next_char();
//};
//
//#endif
