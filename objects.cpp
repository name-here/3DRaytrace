#include <cmath>
#include <cstdio>
#include <limits>//This library is used to get the float max value.
#include "rays.h"
#include "objects.h"

#define F_INFINITY std::numeric_limits<float>::infinity()

float square(float num){
	return num*num;
}
float dist3D(Point p1, Point p2){
	return sqrt(square(p2.x-p1.x)+square(p2.y-p1.y)+square(p2.z-p1.z));
}


/*World::World(std::vector<Object*>&& setObjList)
	: objList(setObjList)
{
	light.x = 0;
	light.y = 1000;
	light.z = 0;
}*/
World::World(){
	light.x = 600;
	light.y = 600;
	light.z = 600;
}

void World::cast(CRay& ray){
	ray.bounceCount ++;
	for(auto i = objList.begin(); i!=objList.end(); ++i){
		(*i)->cast(ray, false, *this);
	}
	ray.setColor(Color(38400, 51200, 65535, 65535), Point(F_INFINITY, F_INFINITY, F_INFINITY), F_INFINITY, true);
	ray.finishCast(true);
	ray.ray.p1 = light;
	//ray.ray.p2 = Point(0, 0, -1000);
	//ray.setDist = F_INFINITY;
	for(auto i = objList.begin(); i!=objList.end(); ++i){
		(*i)->cast(ray, true, *this);
	}
	//ray.finishCast(false);
}


Tri::Tri(Point setP1, Point setP2, Point setP3, Color setColor, uint16_t setReflect){
	p1 = setP1;
	p2 = setP2;
	p3 = setP3;
	normal.x = 0;
	normal.y = 0;
	normal.z = 0;//These 3 values should actually be calculated instead of being set to 0.
	color = setColor;
	reflect = setReflect;
}
void Tri::cast(CRay& ray, bool isShadow, World& world){
	float pointX = ray.ray.p2.x;
	float pointY = ray.ray.p2.y;
	float triX1 = p1.x;
	float triY1 = p1.y;
	float triX2 = p2.x;
	float triY2 = p2.y;
	float triX3 = p3.x;
	float triY3 = p3.y;
	if(!(pointY-triY1<(triY2-triY1)*(pointX-triX1)/(triX2-triX1)^triX2<triX1) && !(pointY-triY2<(triY3-triY2)*(pointX-triX2)/(triX3-triX2)^triX3<triX2) && !(pointY-triY3<(triY1-triY3)*(pointX-triX3)/(triX1-triX3)^triX1<triX3)){
		if(isShadow){

		}
		else{
			ray.setColor(color, Point(F_INFINITY, F_INFINITY, F_INFINITY), F_INFINITY, false);
			ray.escape = false;
		}
	}
}

Ball::Ball(Point setPos, float setRadius, Color setColor, uint16_t setReflect){
	pos = setPos;
	radius = setRadius;
	radiusSq = setRadius*setRadius;
	color = setColor;
	reflect = setReflect;
}
void Ball::cast(CRay& ray, bool isShadow, World& world){
	float lineX1 = dist3D(ray.ray.p1, pos);
	float dist1Sq = square(pos.x-ray.ray.p2.x)+square(pos.y-ray.ray.p2.y)+square(pos.z-ray.ray.p2.z);
	float dist2 = dist3D(ray.ray.p1, ray.ray.p2);
	float lineX2 = /*abs*/((dist1Sq-square(dist2)+square(lineX1))/(2*lineX1)); //this x and y, along with lineX1 /*and lineX2*/, define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the sphere.  The 2D plane is the plane that intersects the center of the sphere and both points on the 3D ray.
	float lineY2 = sqrt(dist1Sq-square(lineX2));
	float num1;
	float num2;
	float num3;
	if(lineY2!=0){
		num1 = (lineX2-lineX1)/lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = square(num1)+1;
		num3 = num2*radiusSq-lineX1*lineX1;
	}
	//if(print){printf("lineX2:%f, ray z:%f, lineY2:%f\n",lineX2,ray.ray.p2.z,lineY2);}
	if(/*if ray hits 2D circle (slice of sphere)*/lineY2==0 || num3>=0){
		float num4;
		if(lineY2==0){num4 = radius;}
		else{num4 = (-lineX1*num1-sqrt(num3))/(num2);}
		float distance = sqrt(square(lineX1-sqrt(radiusSq-num4*num4))+num4*num4);
		float scale = distance/dist3D(ray.ray.p1, ray.ray.p2);// could also be "/ray.ray.length" if ray.length gets implemented.
		Point hit = ray.ray.p1+(ray.ray.p2-ray.ray.p1)*scale;
		if(isShadow){
			/*The followint "if" statement determines if the hit location is actually between the light source and the point to cast the shadow on.*/
			if( 	(hit.x>ray.ray.p1.x!=hit.x>ray.ray.p2.x) && //abs(hit.x-ray.ray.p2.x)>0.01 &&
					(hit.y>ray.ray.p1.y!=hit.y>ray.ray.p2.y) && //abs(hit.y-ray.ray.p2.y)>0.01 &&
					(hit.z>ray.ray.p1.z!=hit.z>ray.ray.p2.z) && //abs(hit.z-ray.ray.p2.z)>0.01 ){
					dist3D(hit, ray.ray.p2)>1){
				//ray.setColor(0, 0, 0, 255, ray.ray.p2, 0, true);//The position for this should actually be set, but isn't yet
				ray.color.r *= 0.5;
				ray.color.g *= 0.5;
				ray.color.b *= 0.5;
			}
		}
		else /*if(	(ray.ray.p2.x>ray.ray.p1.x==hit.x>ray.ray.p1.x) &&
					(ray.ray.p2.y>ray.ray.p1.x==hit.y>ray.ray.p1.y) &&
					(ray.ray.p2.z>ray.ray.p1.x==hit.z>ray.ray.p1.z) )*/{
			if(reflect>0 && ray.bounceCount<2){
				ray.setColor(Color(color.r, color.g, color.b, color.a*(65535-reflect)/65535), hit, distance, false);
				ray.finishCast(true);
				CRay copyRay;
				copyRay.bounceCount = ray.bounceCount;
				copyRay.ray.p1 = ray.ray.p2;

				world.cast(copyRay);
				ray.setColor(copyRay.color, hit, distance, false);
			}
			else{
				ray.setColor(color, hit, distance, false);
			}
		}
	}
	//else if(lineY2<0.1){printf("lineY2=%f\n", lineY2);}
}

Plane::Plane(uint8_t setAxis, float setDist, float setGridSize, Color setColor1, Color setColor2, uint16_t setReflect){
	axis = setAxis;
	dist = setDist;
	gridSize = setGridSize;
	color1 = setColor1;
	color2 = setColor2;
	reflect = setReflect;
}
void Plane::cast(CRay& ray, bool isShadow, World& world){
	Ray rotateRay;
	if(axis==0){
		rotateRay = Ray(Point(ray.ray.p1.x, ray.ray.p1.y, ray.ray.p1.z), Point(ray.ray.p2.x, ray.ray.p2.y, ray.ray.p2.z));
	}
	else if(axis==1){
		rotateRay = Ray(Point(ray.ray.p1.y, ray.ray.p1.z, ray.ray.p1.x), Point(ray.ray.p2.y, ray.ray.p2.z, ray.ray.p2.x));
	}
	else{
		rotateRay = Ray(Point(ray.ray.p1.z, ray.ray.p1.x, ray.ray.p1.y), Point(ray.ray.p2.z, ray.ray.p2.x, ray.ray.p2.y));
	}
	if(((rotateRay.p2.x<rotateRay.p1.x)==(dist<rotateRay.p1.x)) && abs(dist-rotateRay.p1.x)>0.5){
		float planeX = (rotateRay.p2.z-rotateRay.p1.z)*(dist-rotateRay.p1.x)/(rotateRay.p2.x-rotateRay.p1.x)+rotateRay.p1.z;//potential problem area with positivity of "...+ray.ray.p1.x" at end instead on this line and next line
		float planeY = (rotateRay.p2.y-rotateRay.p1.y)*(dist-rotateRay.p1.x)/(rotateRay.p2.x-rotateRay.p1.x)+rotateRay.p1.y;
		if(isShadow){

		}
		else{
			if(abs((int)planeX)%(int)(gridSize*2)<gridSize^abs((int)planeY)%(int)(gridSize*2)<gridSize^planeX>0^planeY>0){ 
				if(axis==0){
					ray.setColor(Color(color1.r, color1.g, color1.b, color1.a*(65535-reflect)/65535), Point(dist, planeY, planeX), dist3D(ray.ray.p1, Point(dist, planeY, planeX)), false);
				}
				else if(axis==1){
					ray.setColor(Color(color1.r, color1.g, color1.b, color1.a*(65535-reflect)/65535), Point(planeX, dist, planeY), dist3D(ray.ray.p1, Point(planeX, dist, planeY)), false);
				}
				else{
					ray.setColor(Color(color1.r, color1.g, color1.b, color1.a*(65535-reflect)/65535), Point(planeY, planeX, dist), dist3D(ray.ray.p1, Point(planeY, planeX, dist)), false);
				}
			}
			else{
				if(axis==0){
					ray.setColor(Color(color2.r, color2.g, color2.b, color2.a*(65535-reflect)/65535), Point(dist, planeY, planeX), dist3D(ray.ray.p1, Point(dist, planeY, planeX)), false);
				}
				else if(axis==1){
					ray.setColor(Color(color2.r, color2.g, color2.b, color2.a*(65535-reflect)/65535), Point(planeX, dist, planeY), dist3D(ray.ray.p1, Point(planeX, dist, planeY)), false);
				}
				else{
					ray.setColor(Color(color2.r, color2.g, color2.b, color2.a*(65535-reflect)/65535), Point(planeY, planeX, dist), dist3D(ray.ray.p1, Point(planeY, planeX, dist)), false);
				}
			}
			if(reflect>0 && ray.bounceCount<2){
				ray.finishCast(true);
				CRay copyRay;
				copyRay.bounceCount = ray.bounceCount;
				//ray.ray.p1.x = ray.ray.p2.x;
				//ray.ray.p1.y = ray.ray.p2.y;
				//ray.ray.p1.z = ray.ray.p2.z;
				/*if(axis==0){
					copyRay.ray.p1.x = rotateRay.p2.x;
					copyRay.ray.p1.y = rotateRay.p2.y;
					copyRay.ray.p1.z = rotateRay.p2.z;
					copyRay.ray.p2.x = rotateRay.p1.x;
					copyRay.ray.p2.y = rotateRay.p2.y*2-rotateRay.p1.y;
					copyRay.ray.p2.z = rotateRay.p2.z*2-rotateRay.p1.z;
				}
				if(axis==1){
					copyRay.ray.p1.x = rotateRay.p2.z;
					copyRay.ray.p1.y = rotateRay.p2.x;
					copyRay.ray.p1.z = rotateRay.p2.y;
					copyRay.ray.p2.x = rotateRay.p2.z*2-rotateRay.p1.z;
					copyRay.ray.p2.y = rotateRay.p1.x;
					copyRay.ray.p2.z = rotateRay.p2.y*2-rotateRay.p1.y;
				}
				if(axis==2){
					copyRay.ray.p1.x = rotateRay.p2.y;
					copyRay.ray.p1.y = rotateRay.p2.z;
					copyRay.ray.p1.z = rotateRay.p2.x;
					copyRay.ray.p2.x = rotateRay.p2.y*2-rotateRay.p1.y;
					copyRay.ray.p2.y = rotateRay.p2.z*2-rotateRay.p1.z;
					copyRay.ray.p2.z = rotateRay.p1.x;
				}*/
				copyRay.ray.p1 = ray.ray.p2;
				if(axis==0){
					copyRay.ray.p2.x = ray.ray.p1.x;
					copyRay.ray.p2.y = ray.ray.p2.y*2-ray.ray.p1.y;
					copyRay.ray.p2.z = ray.ray.p2.z*2-ray.ray.p1.z;
				}
				if(axis==1){
					copyRay.ray.p2.x = ray.ray.p2.x*2-ray.ray.p1.x;
					copyRay.ray.p2.y = ray.ray.p1.y;
					copyRay.ray.p2.z = ray.ray.p2.z*2-ray.ray.p1.z;
				}
				if(axis==2){
					copyRay.ray.p2.x = ray.ray.p2.x*2-ray.ray.p1.x;
					copyRay.ray.p2.y = ray.ray.p2.y*2-ray.ray.p1.y;
					copyRay.ray.p2.z = ray.ray.p1.z;
				}
				world.cast(copyRay);
				copyRay.finishCast(false);
				if(axis==0){
					ray.setColor(Color(copyRay.color.r, copyRay.color.g, copyRay.color.b, 65535), Point(dist, planeY, planeX), dist3D(ray.ray.p1, Point(dist, planeY, planeX)), false);
				}
				else if(axis==1){
					ray.setColor(Color(copyRay.color.r, copyRay.color.g, copyRay.color.b, 65535), Point(planeX, dist, planeY), dist3D(ray.ray.p1, Point(planeX, dist, planeY)), false);
				}
				else{
					ray.setColor(Color(copyRay.color.r, copyRay.color.g, copyRay.color.b, 65535), Point(planeY, planeX, dist), dist3D(ray.ray.p1, Point(planeY, planeX, dist)), false);
				}
			}
		}
		//ray.escape = false;
	}
}


