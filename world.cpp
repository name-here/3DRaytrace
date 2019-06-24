#include <limits>//This library is used to get the double max value.
#include <cmath>//used for sqrt() in color setting
#include "point.h"
#include "rays.h"
#include "objects.h"
#include "camera.h"
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
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, false );
	}
	ray.intersect( 0, Color( 38400, 51200, 65535, 65535 ), Point( F_INFINITY, F_INFINITY, F_INFINITY ), F_INFINITY, Point(), true );
	ray.finishCast( true, Color( 65535, 65535, 65535 ) );
	if( !ray.escape ){
		{//brackets are to tell compiler temp is no longer needed after this
			Point temp = ray.ray.p1;
			for( auto i = lightList.begin(); i!=lightList.end(); ++i ){
				ray.ray.p1 = lightList[0]->pos;
				//ray.setDist = F_INFINITY;
				for( auto i = objList.begin(); i!=objList.end(); ++i ){
					(*i)->cast( ray, true );
				}
				//ray.finishCast( false );
			}
			ray.ray.p1 = temp;
		}
		if( ray.color.a > 0 && ray.bounceCount<MAX_BOUNCES && ray.normalVec!=Point() ){//Could also be reflect>0 if there are issues
			{//brackets are here to tell compiler that temp is no longer needed after this
				Point temp = ray.ray.p2;
				ray.ray.p2 +=   ray.ray.p2 - ray.ray.p1  -  ray.normalVec * 2 * dot(ray.ray.p2 - ray.ray.p1, ray.normalVec);
				ray.ray.p1 = temp;
			}
			ray.bounceCount ++;
			ray.normalVec = Point();
			ray.setDist = F_INFINITY;
			ray.escape = true;
			this->cast( ray );
		}
	}
}

void World::draw( unsigned int camNum, Uint32* pixels, unsigned int width, unsigned int height, unsigned int pixelSize, unsigned int detail, unsigned int drawWidth, unsigned int drawHeight, unsigned int startX, unsigned int startY ){
	if( drawWidth == 0 ){ drawWidth = width; }
	if( drawHeight == 0 ){drawHeight = height; }
	unsigned int detailSq = detail * detail;
	CRay cRay;
	uint32_t rTotal;
	uint32_t gTotal;
	uint32_t bTotal;
	for( unsigned int pxlX = 0; pxlX < drawWidth; pxlX += pixelSize ){
		for( unsigned int pxlY = 0; pxlY < drawHeight; pxlY += pixelSize ){
			if(   pxlX + startX < width  &&  pxlY + startY < height  &&  ( (pxlY + startY) * width) + pxlX + startX < width * height   ){
				rTotal = 0;
				gTotal = 0;
				bTotal = 0;
				for( unsigned int subX = 0; subX < detail; subX ++ ){
					for( unsigned int subY = 0; subY < detail; subY ++ ){
						camList[camNum]->getRay( cRay, pxlX - (double)drawWidth/2 + (double)subX / detail, pxlY - (double)drawHeight/2 + (double)subY / detail );
						this->cast( cRay );
						rTotal += cRay.color.r;
						gTotal += cRay.color.g;
						bTotal += cRay.color.b;
					}
				}
				for( unsigned int setSubX = 0; setSubX < pixelSize; setSubX ++ ){
					for( unsigned int setSubY = 0; setSubY < pixelSize; setSubY ++ ){
						if( pxlX + setSubX < drawWidth  &&  pxlY + setSubY < drawHeight ){
							pixels[ (height - pxlY - startY-1 + setSubY) * width  +  pxlX + startX + setSubX ] =  ( ( (int)sqrt( (int)(rTotal/detailSq) ) ) << 16 ) + ( ( (int)sqrt( (int)(gTotal/detailSq) ) ) << 8 ) + ( (int)sqrt( (int)(bTotal/detailSq) ) );
						}
					}
				}
			}
		}
	}
}

void World::addObj( Object* object ){
	object->id = objList.size()+1;
	objList.emplace_back( object );
}
void World::addLight( Light* light ){
	lightList.emplace_back( light );
}
void World::addCam( Camera* camera ){
	camList.emplace_back( camera );
}

World::~World(){
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		delete *i;
	}
	for( auto i = lightList.begin(); i!=lightList.end(); ++i ){
		delete *i;
	}
}


