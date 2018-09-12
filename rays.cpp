#include <cstdio>
#include "rays.h"


Color::Color(uint16_t setR, uint16_t setG, uint16_t setB, uint16_t setA){
	r = setR;
	g = setG;
	b = setB;
	a = setA;
}


Point::Point(float setX, float setY, float setZ){
	x = setX;
	y = setY;
	z = setZ;
}

Point& Point::operator+=(const Point& toAdd){
	x += toAdd.x;
	y += toAdd.y;
	z += toAdd.z;
	return *this;
}
Point& Point::operator-=(const Point& toSub){
	x -= toSub.x;
	y -= toSub.y;
	z -= toSub.z;
	return *this;
}
/*Point& Point::operator*=(const Point& toMult){
	x *= toMult.x;
	y *= toMult.y;
	z *= toMult.z;
	return *this;
}*/
Point& Point::operator*=(const float toMult){
	x *= toMult;
	y *= toMult;
	z *= toMult;
	return *this;
}
/*Point& Point::operator/=(const Point& toDiv){
	x /= toDiv.x;
	y /= toDiv.y;
	z /= toDiv.z;
	return *this;
}*/


Ray::Ray(Point setP1, Point setP2){
	p1 = setP1;
	p2 = setP2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
}

CRay::CRay(Ray setRay){
	//ray = Ray(Point(setX1, setY1, setZ1), Point(setX2, setY2, setZ2));
	ray = setRay;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
	color.r = 65535;
	color.g = 65535;
	color.b = 65535;
	color.a = 65535;
	escape = true;
}
void CRay::setColor(Color toSet, Point hit, float dist, bool ignoreDirection){
	if((ignoreDirection || (ray.p2.x>ray.p1.x==hit.x>ray.p1.x && ray.p2.y>ray.p1.y==hit.y>ray.p1.y && ray.p2.z>ray.p1.z==hit.z>ray.p1.z)) && dist<=setDist){
		setDist = dist;
		setPos = hit;
		if(toSet.a>0){
			setCastColor.r = toSet.r;
			setCastColor.g = toSet.g;
			setCastColor.b = toSet.b;
		}
		setCastColor.a = toSet.a;
	}
}
void CRay::finishCast(bool doSetPos){
	if(doSetPos){
		ray.p2 = setPos;
	}
	color.r = setCastColor.r;//*setCastColor.a*color.a/4294836225+color.r*(4294836225-setCastColor.a*color.a)/4294836225;
	color.g = setCastColor.g;//*setCastColor.a*color.a/4294836225+color.g*(4294836225-setCastColor.a*color.a)/4294836225;
	color.b = setCastColor.b;//*setCastColor.a*color.a/4294836225+color.b*(4294836225-setCastColor.a*color.a)/4294836225;
	color.a = color.a*(65535-setCastColor.a)/65535;
	if(color.a==0){escape = false;}
}


