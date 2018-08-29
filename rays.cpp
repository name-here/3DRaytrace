#include "rays.h"


Ray::Ray(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2){
	x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
}

CRay::CRay(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2){
	/*x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;*/
	ray = Ray(setX1, setY1, setZ1, setX2, setY2, setZ2);
	//length = dist3D(x1, y1, z1, x2, y2, z2);
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	escape = true;
}
void CRay::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, float hitX, float hitY, float hitZ, float dist, bool ignoreDirection){
	if((ignoreDirection || (ray.x2>ray.x1==hitX>ray.x1 && ray.y2>ray.y1==hitY>ray.y1 && ray.z2>ray.z1==hitZ>ray.z1)) && dist<=setDist){
		setDist = dist;
		setX = hitX;
		setY = hitY;
		setZ = hitZ;
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
		ray.x2 = setX;
		ray.y2 = setY;
		ray.z2 = setZ;
	}
	r = setR*setA*a/65025+r*(65025-setA*a)/65025;
	g = setG*setA*a/65025+g*(65025-setA*a)/65025;
	b = setB*setA*a/65025+b*(65025-setA*a)/65025;
	a = a*(255-setA)/255;
	if(a==0){escape = false;}
}


