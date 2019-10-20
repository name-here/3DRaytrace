#include <cmath>
#include <cstdio>
#include <limits>//This library is used to get the double max value.
#include "point.h"
#include "rays.h"
#include "objects.h"
#include "world.h"//Won't compile without this for some mysterious reason (figure out how to remove)

#define F_INFINITY std::numeric_limits<double>::infinity()

inline double square( double num ){
	return num*num;
}
inline double dist3DSq( Point p1, Point p2 ){//returns the square of the distance between 2 3D points
	return square(p2.x-p1.x) + square(p2.y-p1.y) + square(p2.z-p1.z);
}
inline double dist3D( Point p1, Point p2 ){//returns the distance between 2 3D points
	return sqrt( dist3DSq( p1, p2 ) );
}


Light::Light( Point setPos, FloatColor setColor ){
	pos = setPos;
	color = setColor;
}



Tri::Tri( Point setP1, Point setP2, Point setP3, Color setColor, bool setDoLighting, uint16_t setReflect, uint16_t setFresnel ){
	p1 = setP1;
	p2 = setP2;
	p3 = setP3;
	normal = Point();//This should actually be calculated instead of being set to 0.
	color = setColor;
	doLighting = setDoLighting;
	reflect = setReflect;
	fresnel = setFresnel;
}

bool Tri::cast( CRay& ray, bool isShadow ){
	double pointX = ray.ray.p2.x;
	double pointY = ray.ray.p2.y;
	double triX1 = p1.x;
	double triY1 = p1.y;
	double triX2 = p2.x;
	double triY2 = p2.y;
	double triX3 = p3.x;
	double triY3 = p3.y;//These represeent the points that make up the triangle when projected onto the plane of the triangle.
						//They are not currently set right, and some of the calculations should be done when the triangle is created
	if( 	!( (pointY-triY1 < (triY2-triY1)*(pointX-triX1)/(triX2-triX1))  ^  (triX2 < triX1) ) &&
			!( (pointY-triY2 < (triY3-triY2)*(pointX-triX2)/(triX3-triX2))  ^  (triX3 < triX2) ) &&
			!( (pointY-triY3 < (triY1-triY3)*(pointX-triX3)/(triX1-triX3))  ^  (triX1 < triX3) ) ){//This is temporary (not finished), and detects if
		if(isShadow){
			
		}
		else{
			ray.intersect(  this,  color,  Point( F_INFINITY, F_INFINITY, F_INFINITY ),  F_INFINITY,  normal  );
			ray.escape = false;
		}
		return true;
	}
	return false;
}


Ball::Ball( Point setPos, double setRadius, Color setColor, bool setDoLighting, uint16_t setReflect, uint16_t setFresnel ){
	pos = setPos;
	radius = setRadius;
	radiusSq = setRadius*setRadius;
	color = setColor;
	doLighting = setDoLighting;
	reflect = setReflect;
	fresnel = setFresnel;
}
bool Ball::cast( CRay& ray, bool isShadow ){
	double lineX1 = dist3D( ray.ray.p1, pos );
	double dist1Sq = dist3DSq(pos, ray.ray.p2); //square( pos.x - ray.ray.p2.x )  +  square( pos.y - ray.ray.p2.y )  +  square( pos.z - ray.ray.p2.z );
	double dist2 = dist3D( ray.ray.p1, ray.ray.p2 );
	double lineX2 = /*abs(*/( dist1Sq - (dist2*dist2) + (lineX1*lineX1) )  /  ( lineX1*2 ); //this x and y, along with lineX1 /*and lineX2*/, define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the sphere.  The 2D plane is the plane that intersects the center of the sphere and both points on the 3D ray.
	double lineY2 = sqrt( dist1Sq - (lineX2*lineX2) );
	double num1;
	double num2;
	double num3;//optimize these by creating the variables with the object (maybe, since won't work for multithreading)
	if(lineY2!=0){
		num1 = (lineX2-lineX1) / lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = num1*num1+1;
		num3 = (num2*radiusSq) - (lineX1*lineX1);
	}
	if(/*if ray hits 2D circle (slice of sphere)*/lineY2==0 || num3>=0){
		double num4;
		if( lineY2 == 0 ){ num4 = radius; }
		else{ num4 = ((-lineX1*num1) - sqrt(num3)) / (num2); }
		double distance = sqrt( square( lineX1-sqrt( radiusSq - (num4*num4) ) ) + (num4*num4) );
		double scale = distance / dist3D(ray.ray.p1, ray.ray.p2);// could also be "/ray.ray.length" if ray.length gets implemented.
		Point hit = ray.ray.p1 + ( (ray.ray.p2-ray.ray.p1)*scale );
		if( dist3DSq( pos, hit ) - radiusSq <= INTERSECT_ERR ){//<<<<<<<<<<<<<<<<<<<<<<<<<<<figure out what this does!!
			if(isShadow){
				/*The followint "if" statement determines if the hit location is actually between the light source and the point to cast the shadow on.*/
				if( 	( (hit.x > ray.ray.p1.x)  !=  (hit.x > ray.ray.p2.x) ) && //abs(hit.x-ray.ray.p2.x)>0.01 &&
						( (hit.y > ray.ray.p1.y)  !=  (hit.y > ray.ray.p2.y) ) && //abs(hit.y-ray.ray.p2.y)>0.01 &&
						( (hit.z > ray.ray.p1.z)  !=  (hit.z > ray.ray.p2.z) ) && //abs(hit.z-ray.ray.p2.z)>0.01 ){
						dist3DSq( hit, ray.ray.p2 ) >= INTERSECT_ERR ){
					//ray.intersect(0, 0, 0, 255, ray.ray.p2, 0, true);//The position for this should actually be set, but isn't yet
					//ray.color.r = (ray.color.r + color.r)/2;
					//ray.color.g = (ray.color.g + color.g)/2;
					//ray.color.b = (ray.color.b + color.b)/2;

					return true;
				}
			}
			else if( dist3DSq( hit, ray.ray.p1 ) >= INTERSECT_ERR ){//This if statement may not even be needed!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				ray.intersect(  this,  Color( color.r, color.g, color.b, color.a * (65535 - reflect) / 65535 ),  hit,  distance,  Point( (hit - pos) / radius )  );
				//ray.intersect( this, Color( (hit.x+1)*65535, (hit.y+1)*65535, (hit.z+1)*65535, color.a * ( 65535-reflect ) / 65535 ), hit, distance, Point( (hit - pos) / radius ), false );
				/*if( reflect > 0 ){
					return Point( (hit-pos) / radius );
				}*/  //no longer returned this way.
				return true;
			}
		}
	}
	return false;
}


AxisBox::AxisBox( Point setPos, Point setSize, Color setColor, bool setDoLighting, uint16_t setReflect, uint16_t setFresnel ){
	pos = setPos;
	size = setSize;
	color = setColor;
	doLighting = setDoLighting;
	reflect = setReflect;
	fresnel = setFresnel;
}

bool AxisBox::cast( CRay& ray, bool isShadow ){
	double totalDistOverLength = ( pos.z - ray.ray.p1.z ) / ( ray.ray.p2.z - ray.ray.p1.z );
	double hitX = ( ray.ray.p2.x - ray.ray.p1.x ) * totalDistOverLength + ray.ray.p1.x;
	double hitY = ( ray.ray.p2.y - ray.ray.p1.y ) * totalDistOverLength + ray.ray.p1.y;
	if(  abs( hitX - pos.x ) < size.x  &&  abs( hitY - pos.y ) < size.y  &&  abs(pos.z - ray.ray.p1.z) > INTERSECT_ERR  ){
		Point intersection = Point( hitX, hitY, pos.z );
		if( isShadow ){
			if( ray.ray.inRange( intersection ) ){
				return true;
			}
		}
		else if( ray.ray.pointsAt( intersection ) ){
			ray.intersect(  this,  Color( color.r, color.g, color.b, color.a * ( 65535-reflect ) / 65535 ),  intersection,  dist3D( ray.ray.p1, Point(hitX, hitY, pos.z) ),  Point( 0, 0, -1 )  );
			return true;
		}
	}
	return false;
}


Plane::Plane( uint8_t setAxis, double setDist, double setGridSize, Color setColor1, Color setColor2, bool setDoLighting, uint16_t setReflect, uint16_t setFresnel ){
	axis = setAxis;
	dist = setDist;
	gridSize = setGridSize;
	color1 = setColor1;
	color2 = setColor2;
	doLighting = setDoLighting;
	reflect = setReflect;
	fresnel = setFresnel;
}

bool Plane::cast( CRay& ray, bool isShadow ){
	Ray rotateRay;
	if( axis == 0 ){
		rotateRay = ray.ray;
	}
	else if( axis == 1 ){
		rotateRay.p1.x = ray.ray.p1.y;
		rotateRay.p1.y = ray.ray.p1.z;
		rotateRay.p1.z = ray.ray.p1.x;
		rotateRay.p2.x = ray.ray.p2.y;
		rotateRay.p2.y = ray.ray.p2.z;
		rotateRay.p2.z = ray.ray.p2.x;
	}
	else{
		rotateRay.p1.x = ray.ray.p1.z;
		rotateRay.p1.y = ray.ray.p1.x;
		rotateRay.p1.z = ray.ray.p1.y;
		rotateRay.p2.x = ray.ray.p2.z;
		rotateRay.p2.y = ray.ray.p2.x;
		rotateRay.p2.z = ray.ray.p2.y;
	}

	if( isShadow ){
		if(  (rotateRay.p1.x < dist)  !=  (rotateRay.p2.x < dist)  &&  abs(rotateRay.p2.x - dist) > INTERSECT_ERR  ){
			return true;
		}
	}
	else if(  (rotateRay.p2.x < rotateRay.p1.x) == (dist < rotateRay.p1.x)  &&  abs(rotateRay.p1.x - dist) > INTERSECT_ERR  ){//&&  ( abs(dist-rotateRay.p1.x) > 0.5 ) ){
		double planeX = ( ( rotateRay.p2.z - rotateRay.p1.z ) * ( dist - rotateRay.p1.x ) / ( rotateRay.p2.x - rotateRay.p1.x ) )  +  ( rotateRay.p1.z );//potential problem area with positivity of "...+ray.ray.p1.x" at end instead on this line and next line
		double planeY = ( ( rotateRay.p2.y - rotateRay.p1.y ) * ( dist - rotateRay.p1.x ) / ( rotateRay.p2.x - rotateRay.p1.x ) )  +  ( rotateRay.p1.y );
		char normalFlip = 2 * ( rotateRay.p1.x > dist )  -  1;
		if(  (int)(planeX/gridSize)%2 == 0  ^  (int)(planeY/gridSize)%2 == 0  ^  planeX > 0  ^  planeY > 0  ){
			if( axis == 0 ){
				ray.intersect(  this,  Color( color1.r, color1.g, color1.b, (color1.a) * (65535 - reflect) / 65535 ),  Point( dist, planeY, planeX ),  dist3D( ray.ray.p1, Point( dist, planeY, planeX ) ),  Point( 1, 0, 0 ) * normalFlip  );
				return true;
			}
			else if( axis == 1 ){
				ray.intersect(  this,  Color( color1.r, color1.g, color1.b, (color1.a) * (65535 - reflect) / 65535 ),  Point( planeX, dist, planeY ),  dist3D( ray.ray.p1, Point( planeX, dist, planeY ) ),  Point( 0, 1, 0 ) * normalFlip  );
				return true;
			}
			else{
				ray.intersect(  this,  Color( color1.r, color1.g, color1.b, (color1.a) * (65535 - reflect) / 65535 ),  Point( planeY, planeX, dist ),  dist3D( ray.ray.p1, Point( planeY, planeX, dist ) ),  Point( 0, 0, 1 ) * normalFlip  );
				return true;
			}
		}
		else{
			if( axis == 0 ){
				ray.intersect(  this,  Color( color2.r, color2.g, color2.b, (color2.a) * (65535 - reflect) / 65535 ),  Point( dist, planeY, planeX ),  dist3D( ray.ray.p1, Point( dist, planeY, planeX ) ),  Point( 1, 0, 0 ) * normalFlip  );
				return true;
			}
			else if( axis == 1 ){
				ray.intersect(  this,  Color( color2.r, color2.g, color2.b, (color2.a) * (65535 - reflect) / 65535 ),  Point( planeX, dist, planeY ),  dist3D( ray.ray.p1, Point( planeX, dist, planeY ) ),  Point( 0, 1, 0 ) * normalFlip  );
				return true;
			}
			else{
				ray.intersect(  this,  Color( color2.r, color2.g, color2.b, (color2.a) * (65535 - reflect) / 65535 ),  Point( planeY, planeX, dist ),  dist3D( ray.ray.p1, Point( planeY, planeX, dist ) ),  Point( 0, 0, 1 ) * normalFlip  );
				return true;
			}
		}
		/*if( reflect > 0 ){
			if( axis == 0 ){
				return Point( 1, 0, 0 );
			}
			else if( axis == 1 ){
				return Point( 0, 1, 0 );
			}
			else{
				return Point( 0, 0, 1 );
			}
		}*/
	}
	return false;
}


