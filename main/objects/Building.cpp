#include "Building.hpp"
#include <iostream>

Building::Building():
Obstacle()
{
}

Building::Building(const Building& b):
Obstacle(b)
{
}

Building::Building(Pair<int> coor, Pair<int> dimens, queue<Pair<int> >& vertices):
Obstacle(coor, dimens, vertices, BUILDING)
{

}
