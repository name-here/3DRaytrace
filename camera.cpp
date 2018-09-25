//#include <cmath>
#include <limits>//This library is used to get the float max value.
#include "rays.h"
#include "camera.h"

#define F_INFINITY std::numeric_limits<float>::infinity()


Camera::Camera(Point setPos, float setPlaneDist, float setAngleX, float setAngleY){
	planeDist = setPlaneDist;
	move(setPos);
	rotate(setAngleX, setAngleY);
}
void Camera::move(Point setPos){
	pos = setPos;
}
void Camera::rotate(float setAngleX, float setAngleY){//This should set the front, up, and right vectors.
	front.x = 0;//front is a unit vector pointing in the direction that the camera is facing.
	front.y = 0;
	front.z = 0;
	up.x = 0;//up is a unit vector pointing directly up from the camera.
	up.y = 0;
	up.z = 0;
	right.x = 0;//right is a unit vector pointing directly to the right the camera.
	right.y = 0;
	right.z = 0;//These should all be found instead of being set to 0.
}
void Camera::getRay(CRay& ray, float screenX, float screenY){
	ray.ray.p1 = pos;
	ray.ray.p2.x = pos.x+screenX;
	ray.ray.p2.y = pos.y+screenY;
	ray.ray.p2.z = pos.z+planeDist;
	ray.color.r = 0;
	ray.color.g = 0;
	ray.color.b = 0;
	ray.color.a = 65535;
	ray.setPos.x = F_INFINITY;
	ray.setPos.y = F_INFINITY;
	ray.setPos.z = F_INFINITY;
	ray.setDist = F_INFINITY;
	ray.escape = true;
	ray.bounceCount = 0;
}


