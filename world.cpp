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
/* double dmod(double num1, double num2) {// (should be inline?)
    return num1  -  (int)(num1 / num2) * num2;
} */

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
	if( ray.colorMixLeft > 0 ){
		for( auto i = objList.begin(); i!=objList.end(); ++i ){
			(*i)->cast( ray, false );//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Either World::cast() should use the bool returned by cast (removing need for CRay.escape?), or it should probably return void!
		}

		ray.ray.p2 = ray.hitPos;

		if( ray.escape ){
			ray.addColor( backgroundColor );
			//ray.addColor( ray.castColor, ray.colorMixLeft, FloatColor(0) );
		}

		else if( ray.objLastHit->doLighting ){
			if( ray.objLastHit->opacity == 65535 ){//is opaque
				doDifuseReflect( ray );
			}
			else{
				doRefractReflect( ray );
			}
		}

		else{
			//ray.addLight(  FloatColor( 1 )  );
			ray.addColor( ray.castColor );

		}
	}
}


void World::recast( CRay& ray ){
	ray.bounceCount ++;
	//ray.lightColor.r = 0;
	//ray.lightColor.g = 0;
	//ray.lightColor.b = 0;
	ray.normalVec = Point();
	ray.hitDist = F_INFINITY;
	ray.escape = true;
	cast( ray );
}


void World::doDifuseReflect( CRay& ray ){ //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Should also take angle into account for mixing between diffuse and specular lighting, and a whole bunch of other factors!
	if( ray.objLastHit->roughness > 0 ){
		doDirectLight( ray, ray.objLastHit->roughness );
	}
	if(  ray.bounceCount < MAX_DEPTH  &&  ray.objLastHit->roughness < 65535  ){
		doReflect( ray );
	}
}


void World::doRefractReflect( CRay& ray ){
	if(  ray.bounceCount < MAX_DEPTH  /*&&  ray.objLastHit->roughness == 0  This is the only case that's being calculated, but no need to do nothing just because we have other values*/  ){
		if( ray.objLastHit->opacity > 0 ){
			Ray originalRay = ray.ray;
			Point originalHit = ray.hitPos;
			double originalHitDist = ray.hitDist;

			uint16_t refractMixAmount = (uint32_t)ray.colorMixLeft * ( 65535 - ray.objLastHit->opacity ) / 65535;
			ray.colorMixLeft -= refractMixAmount;
			doReflect( ray );
			ray.colorMixLeft += refractMixAmount;

			ray.ray = originalRay;
			ray.hitPos = originalHit;
			ray.hitDist = originalHitDist;
		}


		//Point refractDir = ;//direction of ray after refraction

		Point offset = (ray.hitPos - ray.ray.p1) / ray.hitDist * INTERSECT_ERR * 2;//used to offset the ray so that it is fully inside the shape (to prevent double intersections)
		ray.ray.p2 =  ray.hitPos * 2  -  ray.ray.p1;
		ray.ray.p1 =  ray.hitPos  +  offset;
		recast( ray );
	}
}


void World::doDirectLight( CRay& ray, uint16_t mixAmount ){
	Point temp = ray.ray.p1;
	bool lightBlocked;
	FloatColor lightTotal;
	for( auto i = lightList.begin(); i!=lightList.end(); ++i ){
	//if(  dot( ((*i)->pos - ray.ray.p2), ray.normalVec )  >=  0  ){//may not actually improve performance, but is meant to
		lightBlocked = false;
		ray.ray.p1 = (*i)->pos;
		//ray.hitDist = F_INFINITY;
		for( auto i = objList.begin(); !lightBlocked && i!=objList.end(); ++i ){//  If cast() needed to run for every object, this would need to be changed, and the line below set to: lightBlocked =  lightBlocked  ||  (*i)->cast( ray, true );
			lightBlocked = (*i)->cast( ray, true );
		}
		if( !lightBlocked ){
			FloatColor lightColor = (*i)->color;
			double lightAdjust = ray.ray.getLength() / UNIT; lightAdjust *= lightAdjust;//lightAdjust determines brightness adjustment of light based on distance to light source and angle of surface relative to light
			double cosAngleToNormal = dot(  (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(),  ray.normalVec  );
			if( cosAngleToNormal < 0 ){ cosAngleToNormal = 0;}
			lightAdjust /= cosAngleToNormal;

			lightColor.r /= lightAdjust;
			lightColor.g /= lightAdjust;
			lightColor.b /= lightAdjust;

			//temp /= abs(  dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec )  );
			//lightColor.r *= dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec);
			lightTotal.r += lightColor.r;
			lightTotal.g += lightColor.g;
			lightTotal.b += lightColor.b;
			//ray.addLight( lightColor );
		}
	//}
	}
	ray.ray.p1 = temp;
	ray.addColor( ray.castColor, mixAmount, lightTotal );
}


void World::doReflect( CRay& ray ){ //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SHOULD TAKE ROUGHNESS INTO ACCOUNT FOR REFLECTIONS OFF ROUGH SURFACES
	//ray.addColor(  Color(  0,  0,  0,  (uint16_t)( ray.objLastHit->fresnel  *  sqrt( dot( (ray.ray.p1 - ray.ray.p2) / ray.ray.getLength(), ray.normalVec) ) )  )  );//Adds darkness to account for Fresnel equations stuff

	{//brackets are here to tell compiler that temp is no longer needed after this
		Point temp = ray.ray.p2;
		ray.ray.p2 +=   ray.ray.p2 - ray.ray.p1  -  ray.normalVec * 2 * dot(ray.ray.p2 - ray.ray.p1, ray.normalVec);
		ray.ray.p1 = temp;
	}
	recast( ray );
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

				//if( pxlX > drawWidth/2 ){//if grayscale, set each color channel to the average of all color channels
					/*uint64_t total = ( rTotal + gTotal + bTotal ) / 3;//total probably doesn't have to be 64 bits (otherwise could just put average in rTotal first and copy to other channels)
					if( total > 4294967295 ){ total = 4294967295; }
					rTotal = total * pxlX / drawWidth  -  rTotal * pxlX / drawWidth + rTotal;
					gTotal = total * pxlX / drawWidth  -  gTotal * pxlX / drawWidth + gTotal;
					bTotal = total * pxlX / drawWidth  -  bTotal * pxlX / drawWidth + bTotal;*/
				//}

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
	for( auto i = camList.begin(); i!=camList.end(); ++i ){
		delete *i;
	}
}


