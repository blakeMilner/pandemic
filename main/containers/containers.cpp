#include "containers.hpp"

flPair::flPair(): x(DEFAULT_X_VALUE), y(DEFAULT_Y_VALUE) 	{}
flPair::flPair(Pair& p): x(p.x), y(p.y) {}
flPair::flPair(float newx, float newy): x(newx), y(newy) {}

flPair operator/ (flPair p, float a)
{
	p.x /= a;
	p.y /= a;

    return(p);
}

flPair operator/ (float a, flPair p)
{
	p.x /= a;
	p.y /= a;

    return(p);
}


// PAIR //


Pair::Pair(const Pair& p){
	x = p.x, y = p.y;
}

Pair::Pair():
	x(DEFAULT_X_VALUE), y(DEFAULT_Y_VALUE) 	{}

Pair::Pair(int xy):
x(xy), y(xy) {} // assume that both x and y are equal

Pair::Pair(int xcoor, int ycoor):
x(xcoor), y(ycoor)
{

}
Pair Pair::operator+=(const Pair& p){
	this->x += p.x;
	this->y += p.y;

	return(*this);
}

Pair Pair::operator-=(const Pair& p){
	this->x -= p.x;
	this->y -= p.y;

	return(*this);
}

Pair Pair::operator++(int){
	Pair p = *this;

	(*this).x++;
	(*this).y++;

	return p;
}

Pair Pair::operator+(const Pair& p){
	return Pair(p.x + (*this).x, p.y + (*this).y	);
}

Pair Pair::operator+(float a){
	return Pair(a + (*this).x, a + (*this).y);
}

Pair operator-(const Pair& p1, const Pair& p2){
	return Pair(p1.x - p2.x, p1.y - p2.y);
}

Pair operator-(float f, const Pair& p){
	return Pair(f - p.x, f - p.y);
}

Pair operator-(const Pair& p, float f){
	return Pair(p.x - f, p.y - f);
}

Pair Pair::operator* (const Pair& p)
{
	Pair pp = *this;

	pp.x *= p.x;
	pp.y *= p.y;

    return(pp);
}

Pair operator* (Pair p, float a)
{
	p.x *= a;
	p.y *= a;

    return(p);
}

Pair operator* (float a, Pair p)
{
	p.x *= a;
	p.y *= a;

    return(p);
}

Pair operator/ (Pair p, float a)
{
	p.x /= a;
	p.y /= a;

    return(p);
}

Pair operator/ (float a, Pair p)
{
	p.x /= a;
	p.y /= a;

    return(p);
}

bool operator== (const Pair& p1, const Pair& p2){
	return((p1.x == p2.x) && (p1.y == p2.y));
}

bool operator!= (const Pair& p1, const Pair& p2){
	return((p1.x != p2.x) || (p1.y != p2.y));
}

ostream& operator<< (ostream &os, const Pair &p){
	return(os << "(" << p.x << "\t,  " << p.y << "\t)");
}

void Pair::set(int nx, int ny){
	x = nx;
	y = ny;
}

// INCLUSIVE
bool flPair::within(const Pair& p1, const Pair& p2){
	return(p1.x <= x and x <= p2.x and p1.y <= y and y <= p2.y);
}

float Pair::mag(){ return sqrt(pow(x,2) + pow(y,2)); }
float Pair::angle() { return atan2(y, x); }
Pair Pair::sgn(){ return Pair((x > 0) - (x < 0), (y > 0) - (y < 0)); }
flPair Pair::unit_v() { float m = mag(); return flPair(x/m, y/m); }
Pair flPair::round(){ return Pair(static_cast<int>(floor(x + 0.5f)), static_cast<int>(floor(y + 0.5f))); }

int sgn(float x){ return (x > 0) - (x < 0); }
