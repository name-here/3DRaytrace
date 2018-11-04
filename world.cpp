#include <limits>//This library is used to get the float max value.
#include "rays.h"
#include "objects.h"
#include "world.h"

#define F_INFINITY std::numeric_limits<float>::infinity()


/*World::World( std::vector<Object*>&& setObjList )
	: objList( setObjList )
{
	light.x = 0;
	light.y = 1000;
	light.z = 0;
}*/
/*World::World(){
	
}*/
/*void World::cast( int camNum, float screenX, float screenY ){
	cast(camList[camNum]->getRay( CRay& ray, screenX, screenY ));
}*/
void World::cast( CRay& ray ){
	ray.bounceCount ++;
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, false, *this );
	}
	ray.setColor( Color( 38400, 51200, 65535, 65535 ), Point( F_INFINITY, F_INFINITY, F_INFINITY ), F_INFINITY, true );
	ray.finishCast( true );
	ray.ray.p1 = lightList[0]->pos;
	//ray.setDist = F_INFINITY;
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, true, *this );
	}
	//ray.finishCast( false );
}


