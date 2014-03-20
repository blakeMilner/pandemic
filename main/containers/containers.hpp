#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <iostream>
#include <cmath>

#define DEFAULT_X_VALUE -1
#define DEFAULT_Y_VALUE -1

#define UNIT_BOUND 0.31623

using namespace std;


template<typename T>
class Pair{
public:
	T x, y;

	Pair(): x(DEFAULT_X_VALUE), y(DEFAULT_Y_VALUE) 	{}
	Pair(const Pair& p){ x = p.x, y = p.y; }
	Pair(T xy):	x(xy), y(xy) {} // assume that both x and y are equal
	Pair(T xcoor, T ycoor): x(xcoor), y(ycoor) {}

	Pair operator-() const{
		Pair p;
		p.x = - this->x;
		p.y = - this->y;
		return p;
	}

	Pair operator++(int){
		Pair p = *this;

		(*this).x++;
		(*this).y++;

		return p;
	}

	Pair operator+=(const Pair& p){
		this->x += p.x;
		this->y += p.y;

		return(*this);
	}

	Pair operator-=(const Pair& p){
		this->x -= p.x;
		this->y -= p.y;

		return(*this);
	}

	Pair operator+(const Pair& p){
		return Pair(p.x + (*this).x, p.y + (*this).y	);
	}

	Pair operator+(float a){
		return Pair(a + (*this).x, a + (*this).y);
	}

	Pair operator-(float f){
		return Pair(this->x - f, this->y - f);
	}

	Pair operator-(const Pair& p){
		return Pair(this->x - p.x, this->y - p.y);
	}

	Pair operator* (const Pair& p){
		Pair pp = *this;
		pp.x *= p.x;
		pp.y *= p.y;
	    return(pp);
	}

	Pair operator* (float a){
		Pair v = *this;
		v.x *= a;
		v.y *= a;
	    return(v);
	}

	Pair operator/ (float a)
	{
		Pair p = *this;
		p.x /= a;
		p.y /= a;
	    return(p);
	}

	bool operator== (const Pair& p){
		return((this->x == p.x) && (this->y == p.y));
	}

	bool operator!= (const Pair& p){
		return((this->x != p.x) || (this->y != p.y));
	}

	// WARNING: THIS OPERATOR RETURNS TRUE IF CONDITION IS MET FOR BOTH X AND Y
	bool operator<= (const Pair& p){
		return((this->x <= p.x) and (this->y <= p.y));
	}

	// set elements
	void set(T nx, T ny){
		x = nx;
		y = ny;
	}

	// x and y of vector is between x and y of given vectors, respectively
	bool within(const Pair& p1, const Pair& p2){
		return(p1.x <= x and x <= p2.x and p1.y <= y and y <= p2.y);
	}

	// test if any element of x is outside beginning (p1) and end (p2)
	bool outside(const Pair& p1, const Pair& p2){
		return(p1.x > x or x > p2.x or p1.y > y or y > p2.y);
	}

	// scalar magnitude
	float mag(){ return sqrt(pow(x,2) + pow(y,2)); }

	// dot product
	float dot(const Pair& a){
		return(this->x * a.x + this->y * a.y);
	}

	// 2-D cross product
	float cross(Pair& a){
		return (this->x * a.y - this->y * a.x);
	}

	// angle pair makes in cartesian coordinates
	float angle() { return atan2(y, x); }

	// angle vector makes with another vector
	float angle_with(Pair<T>& vec){
		return acos(dot(vec) / (mag() * vec.mag()));
	}

	// vector is between two other vectors, testing cross products
	bool is_between(Pair& p1, Pair& p2){
		return (p1.cross(*this) * p1.cross(p2) > 0) and (p2.cross(*this) * p2.cross(p1) > 0);
	}

	// returns sign of each entry
	Pair<int> sgn(){ return Pair((x > 0) - (x < 0), (y > 0) - (y < 0)); }

	// returns unit vector
	Pair<float> unit_v() { float m = mag(); return Pair<float>(x/m, y/m); }

	// round to int
	Pair<int> round(){ return Pair(static_cast<int>(floor(x + 0.5f)), static_cast<int>(floor(y + 0.5f))); }
};


template<typename T>
Pair<T> operator* (float a, Pair<T>& v){
	Pair<T> p = v;
	p.x *= a;
	p.y *= a;
    return(p);
}

template<typename T>
ostream& operator<< (ostream &os, Pair<T>& p){
	return(os << "(" << p.x << "\t,  " << p.y << "\t)");
}

// to sort by area
template<typename T>
struct Dimension_cmp {
    bool operator()(Pair<T> a, Pair<T> b) const {
        return (a.x*a.y < b.x*b.y);
    }
};

#endif

