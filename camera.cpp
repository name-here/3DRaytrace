#include <cmath>
#include <iostream>
#include <limits>//This library is used to get the float max value.
#include "rays.h"
#include "camera.h"

#define F_INFINITY std::numeric_limits<float>::infinity()


Camera::Camera(float setX, float setY, float setZ, float setPlaneDist, float setAngleX, float setAngleY){
	planeDist = setPlaneDist;
	move(setX, setY, setZ);
	rotate(setAngleX, setAngleY);
}
void Camera::move(float setX, float setY, float setZ){
	x = setX;
	y = setY;
	x = setZ;
}
void Camera::rotate(float setAngleX, float setAngleY){//This should set the front, up, and right vectors.
	xFront = 0;//xyzFront is a unit vector pointing in the direction that the camera is facing.
	yFront = 0;
	zFront = 0;
	xUp = 0;//xyzUp is a unit vector pointing directly up from the camera.
	yUp = 0;
	zUp = 0;
	xRight = 0;//xyzRight is a unit vector pointing directly to the right the camera.
	yRight = 0;
	zRight = 0;//These should all be found instead of being set to 0.
}
void Camera::getRay(CRay& ray, float screenX, float screenY){
	ray.ray.x1 = x;
	ray.ray.y1 = y;
	ray.ray.z1 = z;
	ray.ray.x2 = x+screenX;
	ray.ray.y2 = y+screenY;
	ray.ray.z2 = z-planeDist;
	ray.r = 0;
	ray.g = 0;
	ray.b = 0;
	ray.a = 255;
	ray.setDist = F_INFINITY;
	ray.setX = x;
	ray.setY = y;
	ray.setZ = z;
	ray.setR = 0;
	ray.setG = 0;
	ray.setB = 0;
	ray.setA = 255;
	ray.escape = true;
}


