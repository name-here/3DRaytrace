#include <cmath>
#include <limits>//This library is used to get the double max value.
#include "point.h"
#include "rays.h"
#include "camera.h"

#define F_INFINITY std::numeric_limits<double>::infinity()


Camera::Camera( Point setPos, double setPlaneDist, double setAngleX, double setAngleY, double setDistortion ){
	planeDist = setPlaneDist;//planeDist effectively sets the field of view of the camera.
	distortion = setDistortion;
	pos = setPos;
	rotate( setAngleX, setAngleY );
}
/*void Camera::move( Point setPos ){
	pos = setPos;
}*/
void Camera::rotate( double setAngleX, double setAngleY ){//Can probably be optomized.  Also, should maybe be passed by reference?
	front.x = sin( setAngleX ) * cos( setAngleY );//front is a vector pointing in the direction that the camera is facing that extends to the plane the camera traces through.
	front.y = sin( setAngleY );
	front.z = cos( setAngleX ) * cos( setAngleY );
	up.x = sin( setAngleX ) * cos( M_PI/2 + setAngleY );//up is a unit vector pointing directly up from the camera.
	up.y = sin( M_PI/2 + setAngleY );
	up.z = cos( setAngleX ) * cos( M_PI/2 + setAngleY );
	//up.x = 0;
	//up.y = 1;
	//up.z = 0;
	right.x = sin( setAngleX + M_PI/2 );//right is a unit vector pointing directly to the right the camera.
	right.y = 0;
	right.z = cos( setAngleX + M_PI/2 );
}
void Camera::getRay( CRay& ray, double screenX, double screenY ){
	ray.ray.p1 = pos;
	ray.ray.p2 = pos  +  front * planeDist  +  right * screenX  +  up * screenY;
	ray.color.r = 0;
	ray.color.g = 0;
	ray.color.b = 0;
	ray.color.a = 65535;
	double temp = planeDist*planeDist  -  ( screenX*screenX + screenY*screenY ) * distortion;//square of the distance to the sphere being projected onto
	if( temp >= 0 ){
		ray.ray.p2 = pos  +  front * sqrt( temp )  +  right * screenX  +  up * screenY;//this projects rays through a sphere instead of a plane
	}
	else{
		ray.ray.p2 = pos;
		ray.color.a = 0;
	}
	ray.setPos.x = F_INFINITY;
	ray.setPos.y = F_INFINITY;
	ray.setPos.z = F_INFINITY;
	ray.setDist = F_INFINITY;
	ray.escape = true;
	ray.bounceCount = 0;
	ray.objLastHit = 0;//This value should not correspond to any object, as numbering should start at 1.
}


