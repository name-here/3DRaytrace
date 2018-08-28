#include <cmath>
#include <iostream>
#include "rays.h"
#include "objects.h"


float square(float num){
	return num*num;
}
float dist3D(float x1, float y1, float z1, float x2, float y2, float z2){
	return sqrt(square(x2-x1)+square(y2-y1)+square(z2-z1));
}


World::World(CRay* setObjList[]){
	//objList = setObjList;
}
void World::cast(CRay& ray){

}


void Object::cast(CRay& ray, bool isShadow){
	printf("Error in \"cast\" function implimentation(default was used).\n");
}

Tri::Tri(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2, float setX3, float setY3, float setZ3, uint8_t setR, uint8_t setG, uint8_t setB, uint8_t setA){
	x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;
	x3 = setX3;
	y3 = setY3;
	z3 = setZ3;
	normalX = 0;//These 3 values should actually be calculated instead of being set to 0.
	normalY = 0;
	normalX = 0;
	r = setR;
	g = setG;
	b = setB;
	a = setA;
}
void Tri::cast(CRay& ray, bool isShadow){
	float pointX = ray.ray.x2;
	float pointY = ray.ray.y2;
	float triX1 = x1;
	float triY1 = y1;
	float triX2 = x2;
	float triY2 = y2;
	float triX3 = x3;
	float triY3 = y3;
	if(!(pointY-triY1<(triY2-triY1)*(pointX-triX1)/(triX2-triX1)^triX2<triX1) && !(pointY-triY2<(triY3-triY2)*(pointX-triX2)/(triX3-triX2)^triX3<triX2) && !(pointY-triY3<(triY1-triY3)*(pointX-triX3)/(triX1-triX3)^triX1<triX3)){
		ray.r = r;
		ray.g = g;
		ray.b = b;
		ray.escape = false;
	}
}

Ball::Ball(float setX, float setY, float setZ, float setRadius, uint8_t setR, uint8_t setG, uint8_t setB, uint8_t setA, uint8_t setReflect){
	x = setX;
	y = setY;
	z = setZ;
	radius = setRadius;
	radiusSq = setRadius*setRadius;
	r = setR;
	g = setG;
	b = setB;
	a = setA;
	reflect = setReflect;
}
void Ball::cast(CRay& ray, bool isShadow){
	float lineX1 = dist3D(ray.ray.x1, ray.ray.y1, ray.ray.z1, x, y, z);
	float dist1Sq = square(x-ray.ray.x2)+square(y-ray.ray.y2)+square(z-ray.ray.z2);
	float dist2 = dist3D(ray.ray.x2, ray.ray.y2, ray.ray.z2, ray.ray.x1, ray.ray.y1, ray.ray.z1);
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
	//if(print){printf("lineX2:%f, ray z:%f, lineY2:%f\n",lineX2,ray.ray.z2,lineY2);}
	if(/*if ray hits 2D circle (slice of sphere)*/lineY2==0 || num3>=0){
		float num4;
		if(lineY2==0){num4 = radius;}
		else{num4 = (-lineX1*num1-sqrt(num3))/(num2);}
		float distance = sqrt(square(lineX1-sqrt(radiusSq-num4*num4))+num4*num4);
		float scale = distance/dist3D(ray.ray.x1, ray.ray.y1, ray.ray.z1, ray.ray.x2, ray.ray.y2, ray.ray.z2);// could also be "/ray.length" if ray.length gets implemented.
		float hitX = ray.ray.x1+(ray.ray.x2-ray.ray.x1)*scale;
		float hitY = ray.ray.y1+(ray.ray.y2-ray.ray.y1)*scale;
		float hitZ = ray.ray.z1+(ray.ray.z2-ray.ray.z1)*scale;
		if(isShadow){
			/*The followint "if" statement determines if the hit location is actually between the light source and the point to cast the shadow on.*/
			if( 	(hitX>ray.ray.x1!=hitX>ray.ray.x2) && //abs(hitX-ray.ray.x2)>0.01 &&
					(hitY>ray.ray.y1!=hitY>ray.ray.y2) && //abs(hitY-ray.ray.y2)>0.01 &&
					(hitZ>ray.ray.z1!=hitZ>ray.ray.z2) && //abs(hitZ-ray.ray.z2)>0.01 ){
					dist3D(hitX, hitY, hitZ, ray.ray.x2, ray.ray.y2, ray.ray.z2)>1 ){
				//ray.setColor(0, 0, 0, 255, F_INFINITY, F_INFINITY, F_INFINITY, F_INFINITY, true);//The position for this should actually be set, but isn't yet
				ray.r *= 0.5;
				ray.g *= 0.5;
				ray.b *= 0.5;
			}
		}
		else/* if(	(ray.ray.x2>ray.ray.x1==hitX>ray.ray.x1) &&
					(ray.ray.y2>ray.ray.x1==hitY>ray.ray.y1) &&
					(ray.ray.z2>ray.ray.x1==hitZ>ray.ray.z1) )*/{
			if(reflect>0){

			}
			else{
				ray.setColor(r, g, b, a, ray.ray.x1+(ray.ray.x2-ray.ray.x1)*scale, ray.ray.y1+(ray.ray.y2-ray.ray.y1)*scale, ray.ray.z1+(ray.ray.z2-ray.ray.z1)*scale, distance, false);
			}
		}
	}
	//else if(lineY2<0.1){printf("lineY2=%f\n", lineY2);}
}

Plane::Plane(uint8_t setAxis, float setDist, float setGridSize, uint8_t setR1, uint8_t setG1, uint8_t setB1, uint8_t setA1, uint8_t setR2, uint8_t setG2, uint8_t setB2, uint8_t setA2, uint8_t setReflect){
	axis = setAxis;
	dist = setDist;
	gridSize = setGridSize;
	r1 = setR1;
	g1 = setG1;
	b1 = setB1;
	a1 = setA1;
	r2 = setR2;
	g2 = setG2;
	b2 = setB2;
	a2 = setA2;
	reflect = setReflect;
}
void Plane::cast(CRay& ray, bool isShadow){//This assumes the plane is along the y axis, but should get the axis from the "axis" variable instead
	float dim1dist1;
	float dim1dist2;
	float dim2dist1;
	float dim2dist2;
	float dim3dist1;
	float dim3dist2;
	CRay bounceRay(0, 0, 0, 0, 0, 0);
	if(reflect>0){
		bounceRay = ray;
	}
	if(axis==0){
		dim1dist1 = ray.ray.x1;
		dim1dist2 = ray.ray.x2;
		dim2dist1 = ray.ray.y1;
		dim2dist2 = ray.ray.y2;
		dim3dist1 = ray.ray.z1;
		dim3dist2 = ray.ray.z2;
	}
	else if(axis==1){
		dim1dist1 = ray.ray.y1;
		dim1dist2 = ray.ray.y2;
		dim2dist1 = ray.ray.z1;
		dim2dist2 = ray.ray.z2;
		dim3dist1 = ray.ray.x1;
		dim3dist2 = ray.ray.x2;
	}
	else{
		dim1dist1 = ray.ray.z1;
		dim1dist2 = ray.ray.z2;
		dim2dist1 = ray.ray.x1;
		dim2dist2 = ray.ray.x2;
		dim3dist1 = ray.ray.y1;
		dim3dist2 = ray.ray.y2;
	}
	if((dim1dist2<dim1dist1)==(dist<dim1dist1)){
		float planeX = (dim3dist2-dim3dist1)*(dist-dim1dist1)/(dim1dist2-dim1dist1)+dim3dist1;//potential problem area with positivity of "...+ray.ray.x1" at end instead on this line and next line
		float planeY = (dim2dist2-dim2dist1)*(dist-dim1dist1)/(dim1dist2-dim1dist1)+dim2dist1;
		//if(square(ray.ray.x1-testBall.x)+square(ray.ray.y1-testBall.y)+square(ray.ray.z1-testBall.z)<=testBall.radiusSq){ray.setColor(255, 0, 0, 255);}
		if(abs((int)planeX)%(int)(gridSize*2)<gridSize^abs((int)planeY)%(int)(gridSize*2)<gridSize^planeX>0^planeY>0){ 
			if(axis==0){
				ray.setColor(r1, g1, b1, a1*(255-reflect)/255, dist, planeY, planeX, dist3D(dim1dist1, dim2dist1, dim3dist1, dist, planeY, planeX), false);
			}
			else if(axis==1){
				ray.setColor(r1, g1, b1, a1*(255-reflect)/255, planeX, dist, planeY, dist3D(dim3dist1, dim1dist1, dim2dist1, planeX, dist, planeY), false);
			}
			else{
				ray.setColor(r1, g1, b1, a1*(255-reflect)/255, planeY, planeX, dist, dist3D(dim2dist1, dim3dist1, dim1dist1, planeY, planeX, dist), false);
			}
		}
		else{
			if(axis==0){
				ray.setColor(r2, g2, b2, a2*(255-reflect)/255, dist, planeY, planeX, dist3D(dim1dist1, dim2dist1, dim3dist1, dist, planeY, planeX), false);
			}
			else if(axis==1){
				ray.setColor(r2, g2, b2, a2*(255-reflect)/255, planeX, dist, planeY, dist3D(dim3dist1, dim1dist1, dim2dist1, planeX, dist, planeY), false);
			}
			else{
				ray.setColor(r2, g2, b2, a2*(255-reflect)/255, planeY, planeX, dist, dist3D(dim2dist1, dim3dist1, dim1dist1, planeY, planeX, dist), false);
			}
		}
		ray.escape = false;
	}
}


