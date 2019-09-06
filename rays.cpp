#include <cstdio>
#include <cmath>
#include <limits>//This library is used to get the double max value.
#include "point.h"
#include "rays.h"
#include "world.h"

#define F_INFINITY std::numeric_limits<double>::infinity()

inline double square( double num ){
	return num*num;
}

inline double dist3DSq( Point p1, Point p2 ){//returns the square of the distance between 2 3D points
	return square(p2.x-p1.x) + square(p2.y-p1.y) + square(p2.z-p1.z);
}
inline double dist3D( Point p1, Point p2 ){
	return sqrt( dist3DSq( p1, p2 ) );
}


Color::Color( uint16_t setR, uint16_t setG, uint16_t setB, uint16_t setA ){
	r = setR;
	g = setG;
	b = setB;
	a = setA;
}

Color::Color( uint16_t brightness, uint16_t setA ){
	r = brightness;
	g = brightness;
	b = brightness;
	a = setA;
}

Color::Color(){
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}


Ray::Ray( Point setP1, Point setP2 ){
	p1 = setP1;
	p2 = setP2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
}

bool Ray::pointsAt( Point point ){//Make sure point is in fromt of ray (on each axis, from ray p1 to ray p2 is the same direction as ray p1 to point)
	return	(  ( p2.x >= p1.x )  ==  ( point.x >= p1.x )  )  &&
			(  ( p2.y >= p1.y )  ==  ( point.y >= p1.y )  )  &&
			(  ( p2.z >= p1.z )  ==  ( point.z >= p1.z )  )  &&
			dist3DSq( p1, point ) > INTERSECT_ERR;
}

bool Ray::inRange( Point point ){
	return 	(  ( point.x >= p1.x )  ==  ( point.x <= p2.x )  )  &&
			(  ( point.y >= p1.y )  ==  ( point.y <= p2.y )  )  &&
			(  ( point.z >= p1.z )  ==  ( point.z <= p2.z )  )  &&
			dist3DSq( p2, point ) > INTERSECT_ERR;
}

Ray& Ray::normalize(){
	double length = dist3D( p1, p2 );
	p2.x = ( p2.x - p1.x ) / length  +  p1.x;
	p2.y = ( p2.y - p1.y ) / length  +  p1.y;
	p2.x = ( p2.z - p1.z ) / length  +  p1.z;
	return *this;
}


CRay::CRay( Ray setRay ){
	ray = setRay;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 65535;
	setPos.x = F_INFINITY;
	setPos.y = F_INFINITY;
	setPos.z = F_INFINITY;
	setDist = F_INFINITY;
	escape = true;
	bounceCount = 0;
}

void CRay::intersect( unsigned int id, Color toSet, Point hit, double dist, Point objNormalVec/*, bool ignoreDirection*/ ){
	//if( (ignoreDirection  ||  /*ray.pointsAt( hit )*/ true ) && //this line is probably unnecessary (if something fails this test, something probably went wrong elsewhere)
	if( dist <= setDist ){
		objLastHit = id;
		normalVec = objNormalVec;
		setDist = dist;
		setPos = hit;
		//if( toSet.a > 0 ){
			setCastColor.r = toSet.r;
			setCastColor.g = toSet.g;
			setCastColor.b = toSet.b;
		//}
		setCastColor.a = toSet.a;
		//return true;
	}
	//return false;
}

void CRay::castSky( Color skyColor ){
	if( setDist >= F_INFINITY ){
		objLastHit = 0;
		normalVec.x = 0;
		normalVec.y = 0;
		normalVec.z = 0;
		setCastColor = skyColor;
		setCastColor.a = 65535;
	}
}

void CRay::finishCast( bool doSetPos, Color lightColor ){
	if( doSetPos ){
		ray.p2 = setPos;
	}
	color.r +=/*removed stupid-seeming color mixing stuff and changed to +=*/ (uint32_t)( (uint64_t)setCastColor.r * setCastColor.a * color.a / 4294836225 ) * lightColor.r / 65535   /*+   (uint64_t)color.r * ( 4294836225  -  (uint32_t)setCastColor.a * color.a / 4294836225 )*/;
	color.g +=/*removed stupid-seeming color mixing stuff and changed to +=*/ (uint32_t)( (uint64_t)setCastColor.g * setCastColor.a * color.a / 4294836225 ) * lightColor.g / 65535   /*+   (uint64_t)color.g * ( 4294836225  -  (uint32_t)setCastColor.a * color.a / 4294836225 )*/;
	color.b +=/*removed stupid-seeming color mixing stuff and changed to +=*/ (uint32_t)( (uint64_t)setCastColor.b * setCastColor.a * color.a / 4294836225 ) * lightColor.b / 65535   /*+   (uint64_t)color.b * ( 4294836225  -  (uint32_t)setCastColor.a * color.a / 4294836225 )*/;
	color.a = (uint32_t)color.a * (65535 - setCastColor.a) / 65535;
 	 if( setDist < F_INFINITY ){ escape = false; }
}


