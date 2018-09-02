#include "rays.h"


Point::Point(float setX, float setY, float setZ){
	x = setX;
	y = setY;
	z = setZ;
}


Ray::Ray(Point setP1, Point setP2){
	p1 = setP1;
	p2 = setP2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
}

CRay::CRay(Ray setRay){
	//ray = Ray(Point(setX1, setY1, setZ1), Point(setX2, setY2, setZ2));
	ray = setRay;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	escape = true;
}
void CRay::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, Point hit, float dist, bool ignoreDirection){
	if((ignoreDirection || (ray.p2.x>ray.p1.x==hit.x>ray.p1.x && ray.p2.y>ray.p1.y==hit.y>ray.p1.y && ray.p2.z>ray.p1.z==hit.z>ray.p1.z)) && dist<=setDist){
		setDist = dist;
		setPos = hit;
		if(alpha>0){
			setR = red;
			setG = green;
			setB = blue;
		}
		setA = alpha;
	}
}
void CRay::finishCast(bool setPos){
	if(setPos){
		ray.p2 = setPos;
	}
	r = setR*setA*a/65025+r*(65025-setA*a)/65025;
	g = setG*setA*a/65025+g*(65025-setA*a)/65025;
	b = setB*setA*a/65025+b*(65025-setA*a)/65025;
	a = a*(255-setA)/255;
	if(a==0){escape = false;}
}


