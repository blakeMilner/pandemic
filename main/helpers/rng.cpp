#include "rng.hpp"

Pair<int> RNG::random_pair(int xstart, int xend, int ystart, int yend){
	int xran = xstart + (int) (rand() % (xend - xstart));
	int yran = ystart + (int) (rand() % (yend - ystart));

	return(Pair<int>(xran, yran));
}

Pair<int> RNG::random_pair(Pair<int> start, Pair<int> end){
	return(random_pair(start.x, end.x, start.y, end.y));
}

int RNG::random_num(int start, int end){
	return(start + (int) (rand() % (end - start)));
}

bool RNG::yes_or_no(float chance){
	int num = random_num(0, FLOAT_PRECISION);
	if(num < chance*FLOAT_PRECISION) return true;
	else return false;
}
