#include <limits>//This library is used to get the double max value.
#include <cmath>//used for sqrt() in color setting
#include "point.h"
#include "rays.h"
#include "objects.h"
#include "camera.h"
#include "world.h"

#define F_INFINITY std::numeric_limits<double>::infinity()


inline double square( double num ){
	return num*num;
}
/*inline?*/ double dmod(double num1, double num2) {
    return num1  -  (int)(num1 / num2) * num2;
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
World::World( Color setBackgroundColor ){
	backgroundColor = setBackgroundColor;
}

/*void World::cast( int camNum, double screenX, double screenY ){
	cast(camList[camNum]->getRay( CRay& ray, screenX, screenY ));
}*/

void World::cast( CRay& ray ){
	for( auto i = objList.begin(); i!=objList.end(); ++i ){
		(*i)->cast( ray, false );
	}
	//ray.intersect( 0, Color( 38400, 51200, 65535, 65535 ), Point( F_INFINITY, F_INFINITY, F_INFINITY ), F_INFINITY, Point(), true );
	//ray.castBackground( backgroundColor );
	ray.finishCast( true, false );
	if( ray.escape ){
		ray.addColor( backgroundColor );
		ray.finishCast( false, true );
	}
	else if( ray.objLastHit->doLighting ){
		{//brackets are to tell compiler temp is no longer needed after this
			Point temp = ray.ray.p1;
			bool lightBlocked;
			for( auto i = lightList.begin(); i!=lightList.end(); ++i ){
				lightBlocked = false;
				ray.ray.p1 = (*i)->pos;
				//ray.setDist = F_INFINITY;
				for( auto i = objList.begin(); i!=objList.end(); ++i ){
					if(  (*i)->cast( ray, true )  ){//get rid of if(){} if possible (condition must somehow remain)
						lightBlocked = true;
					}
				}
				if( !lightBlocked ){
					FloatColor lightColor = (*i)->color;
					double lightAdjust = ray.ray.getLength() / UNIT; lightAdjust *= lightAdjust;//lightAdjust determines brightness adjustment of light based on distance to light source and angle of surface relative to light
					double cosAngleToNormal = dot(  (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(),  ray.normalVec  );
					if( cosAngleToNormal < 0 ){ cosAngleToNormal = 0;}
					lightAdjust /= cosAngleToNormal;
					/*Point test = (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength();
					if( test != Point() && ray.normalVec != Point() ){
						//printf("x: %f, y: %f, z: %f\n",test.x,test.y,test.z);
						//printf("x: %f, y: %f, z: %f\n",ray.normalVec.x,ray.normalVec.y,ray.normalVec.z);
						//printf("num: %f\n",dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec ));
						//printf("temp: %f\n",temp);
						//lightColor = FloatColor( 10 );
					}
					else{
						ray.addColor( Color(65535) );
					}*/

					lightColor.r /= lightAdjust;
					lightColor.g /= lightAdjust;
					lightColor.b /= lightAdjust;

					//temp /= abs(  dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec )  );
					//lightColor.r *= dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec);
					ray.addLight( lightColor );
				}
				else{
					//ray.addLight( FloatColor( (*i)->color.r/50, (*i)->color.g/50, (*i)->color.b/50 ) );
				}
				//ray.finishCast( false );
			}
			ray.ray.p1 = temp;
			ray.finishCast( false, true );
		}
		if( ray.color.a > 0 && ray.bounceCount<MAX_BOUNCES && ray.normalVec!=Point() ){//Could also be reflect>0 if there are issues
			//ray.addColor(  Color(  0,  0,  0,  (uint16_t)( ray.objLastHit->fresnel  *  sqrt( dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec) ) )  )  );//Adds darkness to account for Fresnel equations stuff

			{//brackets are here to tell compiler that temp is no longer needed after this
				Point temp = ray.ray.p2;
				ray.ray.p2 +=   ray.ray.p2 - ray.ray.p1  -  ray.normalVec * 2 * dot(ray.ray.p2 - ray.ray.p1, ray.normalVec);
				ray.ray.p1 = temp;
			}
			ray.bounceCount ++;
			ray.lightColor.r = 0;
			ray.lightColor.g = 0;
			ray.lightColor.b = 0;
			ray.normalVec = Point();
			ray.setDist = F_INFINITY;
			ray.escape = true;
			this->cast( ray );
		}
	}
	else{
		ray.addLight(  FloatColor( 1 )  );
		ray.finishCast( false, true );
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
						camList[camNum]->getRay( cRay,  pxlX  -  (double)drawWidth/2  +  (double)subX / detail,  pxlY  -  (double)drawHeight/2  +  (double)subY / detail );
						this->cast( cRay );
						rTotal += cRay.color.r;
						gTotal += cRay.color.g;
						bTotal += cRay.color.b;
					}
				}
				/*if( pxlY > drawHeight/2 ){//if grayscale, set each color channel to the average of all color channels
					uint64_t total = ( rTotal + gTotal + bTotal ) / 3;//total probably doesn't have to be 64 bits (otherwise could just put average in rTotal first and copy to other channels)
					if( total > 4294967295 ){ total = 4294967295; }
					rTotal = total;
					gTotal = total;
					bTotal = total;
				}*/
				for( unsigned int setSubX = 0; setSubX < pixelSize; setSubX ++ ){
					for( unsigned int setSubY = 0; setSubY < pixelSize; setSubY ++ ){
						if( pxlX + setSubX < drawWidth  &&  pxlY + setSubY < drawHeight ){
							pixels[  (height - pxlY - startY - setSubY) * width  +  pxlX + startX + setSubX  ] =
								( ( (int)sqrt( (int)(rTotal/detailSq) ) ) << 16 )   +   ( ( (int)sqrt( (int)(gTotal/detailSq) ) ) << 8 )   +   ( (int)sqrt( (int)(bTotal/detailSq) ) );
						}
					}
				}
			}
		}
	}
}

void World::addObj( Object* object ){
	//object->id = objList.size()+1;
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


