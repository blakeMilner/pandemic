#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <iostream>
#include <cmath>

#define DEFAULT_X_VALUE -1
#define DEFAULT_Y_VALUE -1

#define UNIT_BOUND 0.31623

using namespace std;

// TODO: make these classes a single templated class?

class Pair;

class flPair{
public:
	flPair();
	flPair(Pair&);
	flPair(float, float);

	bool within(const Pair&, const Pair&);

	void set(int, int);
	float mag();
	float angle();
	Pair sgn();
	Pair round();

	float x, y;
};
flPair operator/ (flPair, float);
flPair operator/ (float, flPair);

class Pair{
public:
	Pair(const Pair& p);
	Pair();
	Pair(int);
	Pair(int, int);

	Pair operator++(int);
	Pair operator+=(const Pair&);
	Pair operator-=(const Pair&);
	Pair operator+(const Pair&);
	Pair operator+(float);
	Pair operator* (const Pair&);

	void set(int, int);
	float mag();
	float angle();
	Pair sgn();
	flPair unit_v();

	int x, y;
};

Pair operator* (Pair, float);
Pair operator* (float, Pair);
Pair operator/ (Pair, float);
Pair operator/ (float, Pair);
Pair operator-(const Pair&, const Pair&);
Pair operator-(float, const Pair&);
Pair operator-(const Pair&, float);
bool operator== (const Pair& p1, const Pair& p2);
bool operator!= (const Pair& p1, const Pair& p2);
ostream& operator<< (ostream &os, const Pair &p);

int sgn(float);


#endif
