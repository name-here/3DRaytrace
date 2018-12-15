#include <limits>//This library is used to get the double max value.
#include "rays.h"
#include "objects.h"
#include "world.h"

#define F_INFINITY std::numeric_limits<double>::infinity()


/*World::World( std::vector<Object*>&& setObjList )
	: objList( setObjList )
{
	light.x = 0;
	light.y = 1000;
	light.z = 0;
}*/
/*World::World(){
	
}*/
/*void World::cast( int camNum, double screenX, double screenY ){
	cast(camList[camNum]->getRay( CRay& ray, screenX, screenY ));
}*/
void World::cast( CRay& ray ){
	Point normalVec = Point();
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		normalVec = (*i)->cast( ray, false );
	}
	if( ray.bounceCount<15 && normalVec!=Point() ){
		//ray.ray = Ray(ray.ray.p2, ray.ray.p2+normalVec);//This should be the reflected ray
		ray.ray = Ray(Point(0, 0, 0), Point(0, 0, 1));
		ray.bounceCount ++;
		this->cast( ray );
	}
	ray.setColor( Color( 38400, 51200, 65535, 65535 ), Point( F_INFINITY, F_INFINITY, F_INFINITY ), F_INFINITY, true );
	ray.finishCast( true );
	ray.ray.p1 = lightList[0]->pos;
	//ray.setDist = F_INFINITY;
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, true );
	}
	//ray.finishCast( false );
}


