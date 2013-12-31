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

Building::Building(Pair coor, Pair dimens, queue<Pair>& vertices):
Obstacle(coor, dimens, vertices, BUILDING)
{

}
