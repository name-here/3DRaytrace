#include <limits>//This library is used to get the double max value.
#include "rays.h"
#include "objects.h"
#include "world.h"

#define F_INFINITY std::numeric_limits<double>::infinity()

#include <cmath>//these are temporary
inline double square( double num ){
	return num*num;
}
inline double dist3DSq( Point p1, Point p2 ){
	return square(p2.x-p1.x) + square(p2.y-p1.y) + square(p2.z-p1.z);
}
inline double dist3D( Point p1, Point p2 ){
	return sqrt( dist3DSq( p1, p2 ) );
}

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
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, false );
	}
	ray.intersect( 0, Color( 38400, 51200, 65535, 65535 ), Point( F_INFINITY, F_INFINITY, F_INFINITY ), F_INFINITY, Point(), true );
	ray.finishCast( true );
	if( ray.bounceCount<3 && ray.normalVec!=Point() ){//Could also be reflect>0 if there are issues
		{//brackets are here to tell compiler that temp is no longer needed after this
			Point temp = ray.ray.p2;
			ray.ray.p2 +=   ray.ray.p2 - ray.ray.p1  -  ray.normalVec * 2 * dot(ray.ray.p2 - ray.ray.p1, ray.normalVec);
			ray.ray.p1 = temp;
		}
		ray.bounceCount ++;
		ray.normalVec = Point();
		//this->cast( ray );
	}
	ray.ray.p1 = lightList[0]->pos;
	//ray.setDist = F_INFINITY;
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, true );
	}
	//ray.finishCast( false );
}

void World::addObj( Object* object ){
	object->id = objList.size()+1;
	objList.emplace_back( object );
}
void World::addLight( Light* light ){
	lightList.emplace_back( light );
}

World::~World(){
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		delete *i;
	}
	for( auto i = lightList.begin(); i!=lightList.end(); ++i ){
		delete *i;
	}
}


