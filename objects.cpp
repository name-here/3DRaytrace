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



Tri::Tri( Point setP1, Point setP2, Point setP3, Color setColor, bool setDoLighting, uint16_t setRoughness, uint16_t setOpacity, double setIOR ){
	p1 = setP1;
	p2 = setP2;
	p3 = setP3;
	normal = ( p1 - p3 ).cross( p2 - p1 );
	normal /= normal.magnitude();

	yVector = p3 - p2;
	xVector = cross( yVector,  normal );
	length = ( p2 - p1 ).dot( xVector );
	p2Y = ( p2 - p1 ).dot( yVector );
	p3Y = ( p3 - p1 ).dot( yVector );

	color = setColor;
	doLighting = setDoLighting;
	roughness = setRoughness;
	opacity = setOpacity;
	IOR = setIOR;
}

bool Tri::cast( CRay& ray, bool isShadow, bool isInside ){
	//int normalFlip = isInside * 2  -  1;
	if(  ray.ray.cosAngleToUVec( normal ) * ( 1 - isInside*2 )  <=  0  ){
		double rayLength = ray.ray.getLength();
		Point rayUVec = (ray.ray.p2 - ray.ray.p1) / rayLength;//unit vector in direction of ray.ray (length 1)
		double distance = ( p1 - ray.ray.p1 ).dot( normal )  /  rayUVec.dot( normal );
		if( distance >= 0 ){//should be just ( distance > 0 ) ????
			Point hit = ray.ray.p1 + (rayUVec * distance);
			double pointX = ( hit - p1 ).dot( xVector );//(hit - p1).dot(p2 - p1);
			double pointY = ( hit - p1 ).dot( yVector );//(hit - p1).dot(p3 - p1);//these two maybe could be using 2D operations somehow?

			if(   ( pointX >= 0 )  !=  ( pointX >= length )   &&  ( pointY >= pointX * p2Y / length )  !=  ( pointY >= pointX * p3Y / length )   ){//checks if point in 2D plane is actually inside triangle (ie. does the ray hit the triangle?)
			//stuff that doesn't work: (pointX > 0  &&  pointY > 0),   (pointX >= 0)  ==  (p2 - p1).dot(p3 - p1) >= 0
			//incomplete stuff: pointY > (p3 - p1).dot(p3 - p2) != pointY > (p2 - p1).dot(p3 - p2)  &&  pointX > 0 //!= could also be ^
				if( !isShadow ){
					//Point l1 = ( ray.ray.p2 - ray.ray.p1 );
					//l1 /= l1.magnitude();
					//double divisor = l1.dot( normal );
					//double distance = ( p1 - ray.ray.p1 ).dot( normal )  /  divisor;

					return ray.intersect( this, color, hit, distance, normal );
				}
				else if( distance <= rayLength - INTERSECT_ERR ){
					return true;
				}
			}
		}
	}
	return false;
}


Ball::Ball( Point setPos, double setRadius, Color setColor, bool setDoLighting, uint16_t setRoughness, uint16_t setOpacity, double setIOR ){
	pos = setPos;
	radius = setRadius;
	radiusSq = setRadius*setRadius;
	color = setColor;
	doLighting = setDoLighting;
	roughness = setRoughness;
	opacity = setOpacity;
	IOR = setIOR;
}

bool Ball::cast( CRay& ray, bool isShadow, bool isInside ){
	char flipNormals = 1 - isInside*2;
	double lineX1 = dist3D( ray.ray.p1, pos );
	double dist1Sq = dist3DSq( pos, ray.ray.p2 ); //square( pos.x - ray.ray.p2.x )  +  square( pos.y - ray.ray.p2.y )  +  square( pos.z - ray.ray.p2.z );
	double rayLength = dist3D( ray.ray.p1, ray.ray.p2 );
	double lineX2 = /*abs(*/( dist1Sq - (rayLength*rayLength) + (lineX1*lineX1) )  /  ( lineX1*2 ); //this x and y, along with lineX1 (coordinates chosen so Y1 is 0), define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the sphere.  The 2D plane is the plane that intersects the center of the sphere and both points on the 3D ray.
	double lineY2 = sqrt( dist1Sq - (lineX2*lineX2) );
	double num1;
	double num2;
	double num3;
	if(lineY2!=0){
		num1 = (lineX2-lineX1) / lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = num1*num1+1;
		num3 = (num2*radiusSq) - (lineX1*lineX1);
	}
	if(  lineX1 > lineX2/*Avoids intersections behind the camera*/  &&  ( lineX1 >= radius  ||  isInside )/*This check should maybe be earlier?*/  &&  ( lineY2 == 0  ||  num3 >= 0 )  ){//This checks if ray hits 2D circle (slice of sphere)
		double num4;
		double distance;
		if( lineY2 == 0 ){
			num4 = radius;
			distance = lineX1 - radius;
		}
		else{
			num4 =  (   -lineX1 * num1  -  sqrt( num3 ) * flipNormals   )   /   num2;
			distance = sqrt(   square( num1 * num4 )  +  num4 * num4   );
		}

		if(isShadow){// The stuff in here can be simplified, and all the stuff for calculating the 3D intersection can be moved into the else{}
			return  (distance <= rayLength)   &&   ( ray.objLastHit != this  ||  lineX2*lineX2 + lineY2*lineY2 >= square(radius + INTERSECT_ERR) );
		}
		else{ //if(  ray.objLastHit != this  ||  dist3DSq( hit, ray.ray.p1 ) >= INTERSECT_ERR  ){//This if statement may not even be needed!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			//ray.intersect( this, Color( (hit.x+1)*65535, (hit.y+1)*65535, (hit.z+1)*65535, color.a * ( 65535-roughness ) / 65535 ), hit, distance, Point( (hit - pos) / radius ), false );
			/*if( roughness > 0 ){
				return Point( (hit-pos) / radius );
			}*/  //no longer returned this way.
			double scale = distance / rayLength;// could also be "/ ray.ray.length" if Ray.length gets implemented.
			Point hit = ray.ray.p1 + ( (ray.ray.p2-ray.ray.p1)*scale );
			return ray.intersect(  this,  color,  hit,  distance,  (hit - pos) / radius * flipNormals  );
		}
	}
	return false;
}


Tube::Tube( Ray setLine, double setRadius, Color setColor, bool setDoLighting, uint16_t setRoughness, uint16_t setOpacity, double setIOR ){
	this->setLine( setLine );
	radius = setRadius;
	radiusSq = setRadius*setRadius;
	color = setColor;
	doLighting = setDoLighting;
	roughness = setRoughness;
	opacity = setOpacity;
	IOR = setIOR;
}

void Tube::setLine( Ray setLine ){
	line = setLine;
	lineVec = line.p2 - line.p1;
	lineVecUnit = lineVec / lineVec.magnitude();
	length = lineVec.magnitude();
	if( length >= INTERSECT_ERR ){
		lineVec /= length;
	}
	else{
		length = 0;
	}
}

bool Tube::cast( CRay& ray, bool isShadow, bool isInside ){//This is test version of the function, ONLY INTENDED TO TEST MY MATH
	char flipNormals = 1 - isInside*2;

	Point p1Vec = line.p1 - ray.ray.p1;
	Point p2Vec = line.p1 - ray.ray.p2;//vectors pointing from p1 of cylinder to p1 and p2 of ray
	Point axisY = cross( p1Vec, lineVec ).normalize();
	Point axisX = cross( lineVec, axisY ).normalize();

	double lineX1 = p1Vec.dot( axisX );
	double lineX2 = p2Vec.dot( axisX );
	double lineY2 = abs( p2Vec.dot( axisY ) );

	double rayLength2DSq = square(lineX1 - lineX2) + lineY2*lineY2;

	double num1;//2D slope (delta X / delta Y)
	double num2;
	double num3;
	if(lineY2!=0){
		num1 = (lineX2-lineX1) / lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = num1*num1+1;
		num3 = (num2*radiusSq) - (lineX1*lineX1);
	}

	if(  lineX1 > lineX2/*Avoids intersections behind the camera*/  &&  ( lineX1 >= radius  ||  isInside )/*This check should maybe be earlier?*/  &&  ( lineY2 == 0  ||  num3 >= 0 )  ){//This checks if ray hits 2D circle (slice of sphere)
		double num4;
		double dist2DSq;
		if( lineY2 == 0 ){
			num4 = radius;
			dist2DSq = square( lineX1 - radius );
		}
		else{
			num4 =  (   -lineX1 * num1  -  sqrt( num3 ) * flipNormals   )   /   num2;
			dist2DSq = square(num1 * num4)  +  num4*num4;
		}

		double scale = sqrt(dist2DSq) / sqrt( rayLength2DSq );
		Point hit = ray.ray.p1 + ( (ray.ray.p2-ray.ray.p1)*scale );
		double distanceAlong =  lineVecUnit.dot( hit - line.p1 );
		if(  distanceAlong > 0  &&  distanceAlong < length  ){
			if( isShadow ){
				return  (dist2DSq <= rayLength2DSq)   &&   ( ray.objLastHit != this  ||  lineX2*lineX2 + lineY2*lineY2 >= square(radius + INTERSECT_ERR) );
			}
			else{
				double distance = dist3D( hit, ray.ray.p1 );//Could also be:  distance = ray.ray.getLength() * scale;
				Point normal = hit - line.p1;
				normal -=  lineVecUnit  *  normal.dot( lineVecUnit );
				normal.normalize();
				normal *= flipNormals;
				return ray.intersect(  this,  color,  hit,  distance,  normal  );
			}
		}
	}
	return false;

	//Calculates intersections as if the cylinder was a place facing towards ray.ray.p1
	/*if(   lineX1 > lineX2   &&   ( lineX1 >= radius  ||  isInside )   &&   lineY2 * lineX1 / (lineX1 - lineX2)  <  radius   ){
		if( isShadow ){
			return  lineX2 <= INTERSECT_ERR  ||  ( lineX2 <= 0 && ray.objLastHit != this );
		}
		else{
			//double rayLength2D = sqrt( rayLength2DSq );
			//double dist2D = rayLength2D * lineX1 / (lineX1 - lineX2);
			double scale = lineX1 / (lineX1 - lineX2);
			double distance = ray.ray.getLength() * scale;
			Point hit = ray.ray.p1 + ( (ray.ray.p2-ray.ray.p1)*scale );
			Point normal = ray.ray.p1 - line.p1;
			normal.z = 0;
			normal.normalize();
			return ray.intersect(  this,  color,  hit,  distance,  normal  );
		}
	}
	return false;*/
}

/*bool Tube::cast( CRay& ray, bool isShadow, bool isInside ){//This is test version of the function, ONLY INTENDED TO TEST MY MATH
	Point p1Vec = ray.ray.p1 - line.p1;
	Point p2Vec = ray.ray.p2 - line.p1;//vectors pointing from p1 of cylinder to p1 and p2 of ray
	Point axisY = Point(0, UNIT, 0);//cross( p1Vec, lineVec ).normalize();
	Point axisX = Point(UNIT, 0, 0);//cross( lineVec, axisY ).normalize();

	double lineX1 = p1Vec.x;//dot( axisX );
	double lineX2 = p2Vec.x;//dot( axisX );
	double lineY2 = p2Vec.y;//dot( axisY );

	double hit2DY = lineY2 / (lineX1 - lineX2) * lineX1;//Y position of the intersection in the axes defined above
	double dist2Dsq = lineX1*lineX1 + hit2DY*hit2DY;
	//double dist2D = sqrt( dist2Dsq );
	double hitDistZ =   ray.ray.p1.dot( lineVec )   +   ( ray.ray.p2 - ray.ray.p1 ).dot( lineVec )  /  ( lineX1 - lineX2 )  *  lineX1;  //  /  sqrt( square(lineX2 - lineX1) + lineY2*lineY2 )  *  sqrt( lineX1*lineX1 + hit2DY*hit2DY );

	if(  abs(hit2DY) < radius  &&  (lineX1 - lineX2 > 0) == (lineX1 > 0)  ){
		if( isShadow ){
			return (lineX1 > 0) != (lineX2 > 0);
		}
		else{
			double distance = sqrt( dist2Dsq + hitDistZ*hitDistZ );
			return ray.intersect(  this,  Color( (uint16_t)(lineY2/UNIT/10*65535), (uint16_t)(lineY2/UNIT/10*65535), (uint16_t)(lineY2/UNIT/10*65535) ),  ray.ray.p1 + (ray.ray.p2 - ray.ray.p1) / ray.ray.getLength() * distance,  distance,  axisX  );
		}
	}
	return false;
}*/

/*bool Tube::cast( CRay& ray, bool isShadow, bool isInside ){
	char flipNormals = 1 - isInside*2;

	Point p1Vec = ray.ray.p1 - line.p1;
	Point p2Vec = ray.ray.p2 - line.p1;//vectors pointing from p1 of cylinder to p1 and p2 of ray
	Point axisY = cross( p1Vec, lineVec ).normalize();
	Point axisX = cross( lineVec, axisY ).normalize();
	
	//double dist1Sq = dist3DSq( line.p1, ray.ray.p2 );
	//double rayLength = dist3D( ray.ray.p1, ray.ray.p2 );
	//double lineX1 = dist3D( ray.ray.p1, line.p1 );
	//double lineX2 = ( dist1Sq - (rayLength*rayLength) + (lineX1*lineX1) )  /  ( lineX1*2 );
	//double lineY2 = sqrt( dist1Sq - (lineX2*lineX2) );

	double lineX1 = p1Vec.dot( axisX );
	double lineX2 = p2Vec.dot( axisX );
	double lineY2 = p2Vec.dot( axisY );
	//printf( "lineX1 - lineX2: %f\n", lineX1 - lineX2 );
	//The above X1, X2, and Y2 (coordinates chosen so Y1 is 0), define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the tube.  The 2D plane is the plane that intersects the center of the tube and both points on the 3D ray.
	
	double num1;
	double num2;
	double num3;//optimize these by creating the variables with the object (maybe, since won't work for multithreading)
	if(lineY2!=0){
		num1 = (lineX2-lineX1) / lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = num1*num1+1;
		num3 = (num2*radiusSq) - (lineX1*lineX1);
	}
	if(  ( lineX1 >= radius  ||  isInside )  &&  ( lineY2 == 0  ||  num3 >= 0 )  ){//This checks if ray hits 2D circle (slice of sphere).  Check of ( lineX1 >= radius  ||  isInside ) should maybe be earlier?
		double num4;
		double distance;
		if( lineY2 == 0 ){
			num4 = radius;
			distance = lineX1 - radius;
		}
		else{
			num4 =  (   -lineX1 * num1  -  sqrt( num3 ) * flipNormals   )   /   num2;
			distance = sqrt(   square( num1 * num4 )  +  num4 * num4   );
		}
		double scale =  distance  /  ( ray.ray.p2 - ray.ray.p1 ).magnitude();// could also be "/ ray.ray.length" if Ray.length gets implemented.
		Point hit =  ray.ray.p1  +  ( (ray.ray.p2 - ray.ray.p1) * scale );
		Point hitVec = hit - line.p1;//vector pointing from p1 of cylinder to ray intersection
		double distAlong = hitVec.dot( lineVec );//distance along the center line of the tube from line.p1 to hit
		if(   distAlong >= 0  &&  distAlong <= length   &&   abs(  square( hit.dot(axisX) ) + square( hit.dot(axisY) )  -  radiusSq  )  <=  INTERSECT_ERR   ){ //Avoids intersections behind the camera by making sure hit point is within INTERSECT_ERR of the surface (maybe INTERSECT_ERR should be squared?)
			if(isShadow){
				//The followint "if" statement determines if the hit location is actually between the light source and the point to cast the shadow on.
				if( 	( (hit.x > ray.ray.p1.x)  !=  (hit.x > ray.ray.p2.x) ) &&
						( (hit.y > ray.ray.p1.y)  !=  (hit.y > ray.ray.p2.y) ) &&
						( (hit.z > ray.ray.p1.z)  !=  (hit.z > ray.ray.p2.z) ) &&
						dist3DSq( hit, ray.ray.p2 ) >= INTERSECT_ERR ){

					return true;
				}
			}
			else{ //if( dist3DSq( hit, ray.ray.p1 ) >= INTERSECT_ERR ){//This if statement may not even be needed!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				//printf( "Should be true: %i\n", lineX1 > lineX2 || lineX1 < radius );
				Point normal = ( axisX * hitVec.dot(axisX) + axisY * hitVec.dot(axisY) ).normalize() * flipNormals;
				return ray.intersect(  this,  Color( (uint16_t)(normal.x*65535), (uint16_t)(normal.y*65535), (uint16_t)(normal.z*65535) ),  hit,  distance,  normal  );
			}
		}
	}
	return false;
}*/

/*bool Tube::cast( CRay& ray, bool isShadow, bool isInside ){
	char flipNormals = 1 - isInside*2;

	Point p1Vec = ray.ray.p1 - line.p1;
	Point p2Vec = ray.ray.p2 - line.p1;//vectors pointing from p1 of cylinder to p1 and p2 of ray
	Point axisY = cross( p1Vec, lineVec ).normalize();
	Point axisX = cross( axisY, lineVec ).normalize();

	double lineX1 = p1Vec.dot( axisX );
	double lineX2 = p2Vec.dot( axisX );
	double lineY2 = p2Vec.dot( axisY );
	//The above X1, X2, and Y2 (coordinates chosen so Y1 is 0), define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the tube.  The 2D plane is the plane that intersects the center of the tube and both points on the 3D ray.

	//double lineX1 = dist3D( ray.ray.p1, line.p1 );
	//double dist1Sq = dist3DSq( line.p1, ray.ray.p2 );
	double rayLength = dist3D( ray.ray.p1, ray.ray.p2 );
	//double lineX2 = ( dist1Sq - (rayLength*rayLength) + (lineX1*lineX1) )  /  ( lineX1*2 ); //this x and y, along with lineX1 (coordinates chosen so Y1 is 0), define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the sphere.  The 2D plane is the plane that intersects the center of the sphere and both points on the 3D ray.
	//double lineY2 = sqrt( dist1Sq - (lineX2*lineX2) );
	double num1;
	double num2;
	double num3;//optimize these by creating the variables with the object (maybe, since won't work for multithreading)
	if(lineY2!=0){
		num1 = (lineX2-lineX1) / lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = num1*num1+1;
		num3 = (num2*radiusSq) - (lineX1*lineX1);
	}
	if(  ( lineX1 >= radius  ||  isInside )  &&  ( lineY2 == 0  ||  num3 >= 0 )  ){//This checks if ray hits 2D circle (slice of sphere).  Check of ( lineX1 >= radius  ||  isInside ) should maybe be earlier?
		double num4;
		double distance;
		if( lineY2 == 0 ){
			num4 = radius;
			distance = lineX1 - radius;
		}
		else{
			num4 =  (   -lineX1 * num1  -  sqrt( num3 ) * flipNormals   )   /   num2;
			distance = sqrt(   square( num1 * num4 )  +  num4 * num4   );
		}
		double scale = distance / rayLength;// could also be "/ ray.ray.length" if Ray.length gets implemented.
		Point hit = ray.ray.p1 + ( (ray.ray.p2-ray.ray.p1)*scale );

		//if(  abs( dist3DSq( line.p1, hit ) - radiusSq )  <=  INTERSECT_ERR  ){ //Avoids intersections behind the camera by making sure hit point is within INTERSECT_ERR of the surface
			if(isShadow){
				//The followint "if" statement determines if the hit location is actually between the light source and the point to cast the shadow on.
				if( 	( (hit.x > ray.ray.p1.x)  !=  (hit.x > ray.ray.p2.x) ) &&
						( (hit.y > ray.ray.p1.y)  !=  (hit.y > ray.ray.p2.y) ) &&
						( (hit.z > ray.ray.p1.z)  !=  (hit.z > ray.ray.p2.z) ) &&
						dist3DSq( hit, ray.ray.p2 ) >= INTERSECT_ERR ){
					return true;
				}
			}
			else if(  ray.objLastHit != this  ||  dist3DSq( hit, ray.ray.p1 ) >= INTERSECT_ERR  ){//This if statement may not even be needed!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				return ray.intersect(  this,  Color( (uint16_t)(hit.x/UNIT*65535), (uint16_t)(hit.y/UNIT*65535), (uint16_t)(hit.z/UNIT*65535) ),  hit,  distance,  (hit - line.p1) / radius * flipNormals  );
			}
		//}
	}
	return false;
}*/


AxisBox::AxisBox( Point setPos, Point setSize, Color setColor, bool setDoLighting, uint16_t setRoughness, uint16_t setOpacity, double setIOR ){
	pos = setPos;
	size[0] = setSize.x;
	size[1] = setSize.y;
	size[2] = setSize.z;
	color = setColor;
	doLighting = setDoLighting;
	roughness = setRoughness;
	opacity = setOpacity;
	IOR = setIOR;
}

bool AxisBox::cast( CRay& ray, bool isShadow, bool isInside ){//replace this logic with looping through sideNormals[], and using cosAngleToUVec(normal) <= 0 check for each.
	for( int i = 0; i < 3; i ++ ){
		if(  castSidePair( ray, i, isShadow, isInside )  ){ return true; }
	}
	return false;



	/*Point sideNormal;

	if(  ( ray.ray.p2.x > ray.ray.p1.x )  ==  ( pos.x > ray.ray.p1.x )  ){
		sideNormal.set( -1, 0, 0 );
	}
	else{
		sideNormal.set( 1, 0, 0 );
	}
	if(  castSidePair( ray, isShadow, sideNormal )  ){ return true; }

	if(  ( ray.ray.p2.y > ray.ray.p1.y )  ==  ( pos.y > ray.ray.p1.y )  ){
		sideNormal.set( 0, -1, 0 );
	}
	else{
		sideNormal.set( 0, 1, 0 );
	}
	if(  castSidePair( ray, isShadow, sideNormal )  ){ return true; }

	if(  ( ray.ray.p2.z > ray.ray.p1.z )  ==  ( pos.z > ray.ray.p1.z )  ){
		sideNormal.set( 0, 0, -1 );
	}
	else{
		sideNormal.set( 0, 0, 1 );
	}
	if(  castSidePair( ray, isShadow, sideNormal )  ){ return true; }*/


	/*double totalDistOverLength = ( pos.z - ray.ray.p1.z ) / ( ray.ray.p2.z - ray.ray.p1.z );
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
			ray.intersect(  this,  Color( color.r, color.g, color.b ),  intersection,  dist3D( ray.ray.p1, Point(hitX, hitY, pos.z) ),  Point( 0, 0, -1 )  );
			return true;
		}
	}
	return false;*/
}

bool AxisBox::castSidePair( CRay& ray, unsigned char axis/*should just be int?*/, bool isShadow, bool isInside ){//<<<<<<largely copied from Tri::cast(), so should either have shared function, or optimize this using it's specific use for a cube
	if( axis >= 3 ){ return false; }//prevent trying to read outside the array if given invalid input

	Point sideNormal;
	if(  ray.ray.cosAngleToUVec( sideNormals[axis] )  <=  0  ){
		sideNormal = sideNormals[axis];
	}
	else{
		sideNormal = sideNormals[axis] * -1;
	}
	Point sideCenter = pos + sideNormal * size[axis] * ( 1 - isInside*2 );

	double rayLength = ray.ray.getLength();
	Point rayUVec = (ray.ray.p2 - ray.ray.p1) / rayLength;//unit vector in direction of ray.ray (length 1)
	double distance = (sideCenter - ray.ray.p1 ).dot( sideNormal )  /  rayUVec.dot( sideNormal );
	if( distance >= 0 ){
		Point hit = ray.ray.p1 + (rayUVec * distance);
		double pointX = ( hit - sideCenter ).dot(  sideNormals[ (axis + 1) % 3 ]  );
		double pointY = ( hit - sideCenter ).dot(  sideNormals[ (axis + 2) % 3 ]  );
		if(  abs( pointX ) <= size[ (axis + 1) % 3 ]  &&  abs( pointY ) <= size[ (axis + 2) % 3 ]  ){
			if( !isShadow ){
				return ray.intersect( this, color, hit, distance, sideNormal );
			}
			else if( distance <= rayLength - INTERSECT_ERR ){
				return true;
			}
		}
	}
	return false;
}


Plane::Plane( uint8_t setAxis, double setDist, double setGridSize, Color setColor1, Color setColor2, bool setDoLighting, uint16_t setRoughness, uint16_t setOpacity, double setIOR ){
	axis = setAxis;
	dist = setDist;
	gridSize = setGridSize;
	color1 = setColor1;
	color2 = setColor2;
	doLighting = setDoLighting;
	roughness = setRoughness;
	opacity = setOpacity;
	IOR = setIOR;
}

bool Plane::cast( CRay& ray, bool isShadow, bool isInside ){
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
				return ray.intersect(  this,  Color( color1.r, color1.g, color1.b ),  Point( dist, planeY, planeX ),  dist3D( ray.ray.p1, Point( dist, planeY, planeX ) ),  Point( 1, 0, 0 ) * normalFlip  );
			}
			else if( axis == 1 ){
				return ray.intersect(  this,  Color( color1.r, color1.g, color1.b ),  Point( planeX, dist, planeY ),  dist3D( ray.ray.p1, Point( planeX, dist, planeY ) ),  Point( 0, 1, 0 ) * normalFlip  );
			}
			else{
				return ray.intersect(  this,  Color( color1.r, color1.g, color1.b ),  Point( planeY, planeX, dist ),  dist3D( ray.ray.p1, Point( planeY, planeX, dist ) ),  Point( 0, 0, 1 ) * normalFlip  );
			}
		}
		else{
			if( axis == 0 ){
				return ray.intersect(  this,  Color( color2.r, color2.g, color2.b ),  Point( dist, planeY, planeX ),  dist3D( ray.ray.p1, Point( dist, planeY, planeX ) ),  Point( 1, 0, 0 ) * normalFlip  );
			}
			else if( axis == 1 ){
				return ray.intersect(  this,  Color( color2.r, color2.g, color2.b ),  Point( planeX, dist, planeY ),  dist3D( ray.ray.p1, Point( planeX, dist, planeY ) ),  Point( 0, 1, 0 ) * normalFlip  );
			}
			else{
				return ray.intersect(  this,  Color( color2.r, color2.g, color2.b ),  Point( planeY, planeX, dist ),  dist3D( ray.ray.p1, Point( planeY, planeX, dist ) ),  Point( 0, 0, 1 ) * normalFlip  );
			}
		}
		/*if( roughness > 0 ){
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


