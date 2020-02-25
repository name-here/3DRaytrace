#include "point.h"
#include<cmath>

Point::Point(double setX, double setY, double setZ ){
	x = setX;
	y = setY;
	z = setZ;
}
void Point::set( double setX, double setY, double setZ ){
	x = setX;
	y = setY;
	z = setZ;
}
void Point::set(){
	x = 0;
	y = 0;
	z = 0;
}

double Point::magnitude(){
	return sqrt( x*x + y*x + z*z );
}

Point& Point::normalize(){
	double length = sqrt( x*x + y*x + z*z );
	this->x/=length;
	this->y/=length;
	this->z/=length;
	return *this;
}

Point& Point::operator+=( const Point& toAdd ){
	x += toAdd.x;
	y += toAdd.y;
	z += toAdd.z;
	return *this;
}
Point& Point::operator-=( const Point& toSub ){
	x -= toSub.x;
	y -= toSub.y;
	z -= toSub.z;
	return *this;
}
Point& Point::operator*=( const double toMult ){
	x *= toMult;
	y *= toMult;
	z *= toMult;
	return *this;
}
Point& Point::operator/=( const double toDiv ){
	x /= toDiv;
	y /= toDiv;
	z /= toDiv;
	return *this;
}
double Point::dot( const Point& vec ){
	return x * vec.x  +  y * vec.y  +  z * vec.z;
}
Point& Point::cross( const Point& vec ){
	double tempX = y * vec.z  -  z * vec.y;
	double tempY = z * vec.x  -  x * vec.z;
	z = x * vec.y  -  y * vec.x;
	x = tempX;
	y = tempY;
	return *this;
}


