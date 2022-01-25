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
World::World( Color setBackgroundColor, double setAirIOR ){
	backgroundColor = setBackgroundColor;
	airIOR = setAirIOR;
}



/*void World::cast( int camNum, double screenX, double screenY ){
	cast(camList[camNum]->getRay( CRay& ray, screenX, screenY ));
}*/

double World::cast( CRay& ray ){
	if( ray.colorMixLeft > 0 ){

		bool refractOut = false;
		if(  ray.objLastHit != nullptr  &&  ray.objLastHit->opacity < 65535  &&  ray.objLastHit->doLighting  ){
			if( ray.objLastHit->cast( ray, false, true ) ){//this logic does not work for multiple intersecting refractive objects (might be fine, though)
				ray.nextIOR = airIOR;
				refractOut = true;
			}
		}

		for( auto i = objList.begin(); i!=objList.end(); ++i ){
			//if(  (*i) != ray.objLastHit  ){ //removed because it's unnecessary
				(*i)->cast( ray, false );//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Either World::cast() should use the bool returned by cast (removing need for CRay.escape?), or it should probably return void!
			//}
		}

		ray.ray.p2 = ray.hitPos;//is this necessary?  Should other things be updated to use hitPos instead?

		if( ray.escape ){
			ray.addColor( backgroundColor );
			//ray.addColor( ray.castColor, ray.colorMixLeft, FloatColor(0) );

			return F_INFINITY;
		}

		else if( ray.objLastHit->doLighting ){
			if( ray.objLastHit->opacity == 65535 ){//is opaque
				doDifuseReflect( ray );
			}
			else{
				if( !refractOut ){
					ray.nextIOR = ray.objLastHit->IOR;
				}
				doRefractReflect( ray );
			}
		}

		else{
			//ray.addLight(  FloatColor( 1 )  );
			ray.addColor( ray.castColor );
			//ray.addColor(   (  65535  -  (uint16_t)( ray.hitDist * 65535 / (UNIT*10) )  )  *  ( ray.hitDist <= UNIT*10 )   );
			//ray.addColor(  (uint16_t)( UNIT * 65535 / (ray.hitDist + UNIT) )  );

			return ray.hitDist;
		}
	}

	return 0;
}


double World::recast( CRay& ray ){
	ray.bounceCount ++;
	//ray.lightColor.r = 0;
	//ray.lightColor.g = 0;
	//ray.lightColor.b = 0;
	ray.currentIOR = ray.nextIOR;
	ray.normalVec = Point();
	ray.hitDist = F_INFINITY;
	ray.escape = true;
	return cast( ray );
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
		//uint16_t reflectAmmount = 0;
		if( ray.objLastHit->opacity > 0 ){
			/*Ray originalRay = ray.ray;
			Point originalHit = ray.hitPos;
			double originalHitDist = ray.hitDist;

			uint16_t refractMixAmount = (uint32_t)ray.colorMixLeft * ( 65535 - ray.objLastHit->opacity ) / 65535;
			ray.colorMixLeft -= refractMixAmount;
			doReflect( ray );
			ray.colorMixLeft += refractMixAmount;

			ray.ray = originalRay;
			ray.hitPos = originalHit;
			ray.hitDist = originalHitDist;*/

			if( false ){//if refraction is not needed (total internal reflection case)
				ray.nextIOR = ray.currentIOR;
				ray.normalVec *= -1;
				doReflect( ray );
				return;
			}

			uint16_t reflectAmmount = ray.objLastHit->opacity;//this should be properly calculated, taking angle into account

			CRay reflectRay = CRay();
			reflectRay.ray = ray.ray;
			reflectRay.normalVec = ray.normalVec;

			uint32_t keepBounces = (uint64_t)( MAX_DEPTH - ray.bounceCount )  *  ( 65535 - reflectAmmount) / 65535;
			reflectRay.bounceCount = ray.bounceCount + keepBounces;

			doReflect( reflectRay );

			ray.bounceCount = reflectRay.bounceCount - keepBounces;

			ray.addColor( reflectRay.color, reflectAmmount );
		}


		{//None of these variables are needed once the new ray is constructed, so limit their scope
			double IORRatio = (ray.currentIOR / ray.nextIOR);
			Point rayDir = (ray.hitPos - ray.ray.p1) / ray.hitDist;//direction of ray before refraction

			double c1 = -dot( ray.normalVec, rayDir );
			double c2 = sqrt(  1  -  IORRatio*IORRatio * ( 1 - c1*c1 )  )  ;//*  -1 * (c1 < 0);//last bit is to correct the sometimes inverted output.  I'm not sure if this is right, since I didn't derive the equations, but it's definately wrong without it.
			Point refractDir = IORRatio  *  (  rayDir  +  ray.normalVec * c1  )  -  ray.normalVec * c2;//direction of ray after refraction

			Point offset = refractDir * INTERSECT_ERR/* *2 */;//used to offset the ray so that it is fully inside the shape (to prevent double intersections)
		
			ray.ray.p2 =  ray.hitPos + refractDir;
			ray.ray.p1 =  ray.hitPos + offset;
		}
		recast( ray );
	}
}


void World::doDirectLight( CRay& ray, uint16_t mixAmount ){
	//ray.addColor( Color( (uint16_t)((ray.hitPos.x - ray.ray.p1.x)/UNIT*65535), (uint16_t)((ray.hitPos.y - ray.ray.p1.y)/UNIT*65535), (uint16_t)((ray.hitPos.z - ray.ray.p1.z)/UNIT*65535) ), mixAmount );
	//return;//these 2 lines make it just show x, y, and z distance instead of doing lighting

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
		ray.ray.p1 = temp;//  +  (ray.ray.p2 - temp) * INTERSECT_ERR/* *2 */;//used to offset the ray so that it is fully outside the shape (to prevent double intersections)
	}
	recast( ray );
}



void World::draw( WorldDrawArgs args ){
	unsigned int detailSq = args.detail * args.detail;
	CRay cRay;
	double depthTotal;
	uint32_t rTotal;
	uint32_t gTotal;
	uint32_t bTotal;
	for(   unsigned int pxlX = 0;   pxlX < args.drawWidth  &&  pxlX + args.startX < args.textureWidth;   pxlX += args.pixelSize   ){
		for(   unsigned int pxlY = 0;   pxlY < args.drawHeight  &&  pxlY + args.startY < args.textureHeight;   pxlY += args.pixelSize   ){
			depthTotal = 0;
			rTotal = 0;
			gTotal = 0;
			bTotal = 0;
			for( unsigned int subX = 0; subX < args.detail; subX ++ ){
				for( unsigned int subY = 0; subY < args.detail; subY ++ ){
					//camList[args.camNum]->getRay( cRay,  pxlX  -  (double)args.drawWidth/2  +  (double)subX / args.detail,  (double)args.drawHeight/2  -  pxlY  +  (double)subY / args.detail );
					args.world->camList[args.camNum]->getRay( cRay,
							(!args.centerView) * args.startX  +  pxlX  -  (double)(args.centerView ? args.drawWidth : args.textureWidth) / 2  +  ((double)subX + 0.5) / args.detail,
							(double)(args.centerView ? args.drawHeight : args.textureHeight) / 2  -  (args.centerView ? 0 : args.startY)  -  pxlY  +  ((double)subY + 0.5) / args.detail );
					cRay.currentIOR = args.world->airIOR;
					cRay.nextIOR = args.world->airIOR;
					depthTotal += args.world->cast( cRay );
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

			for(  unsigned int setSubX = 0;  setSubX < args.pixelSize  &&  pxlX + setSubX < args.drawWidth  &&  args.startX + pxlX + setSubX < args.textureWidth;  setSubX ++  ){
				for(  unsigned int setSubY = 0;  setSubY < args.pixelSize  &&  pxlY + setSubY < args.drawHeight  &&  args.startY + pxlY + setSubY < args.textureHeight;  setSubY ++  ){
					int index =  (args.startY + pxlY + setSubY) * args.textureWidth  +  args.startX + pxlX + setSubX;
					args.depthTexture[ index ] = depthTotal / detailSq;
					args.texture[ index ] =
						( ( (int)sqrt( (int)(rTotal/detailSq) ) ) << 16 )  +
						( ( (int)sqrt( (int)(gTotal/detailSq) ) ) << 8 )   +
						( (int)sqrt( (int)(bTotal/detailSq) ) );//Should the inner (int) casts be (uint16_t) instead?
				}
			}
		}
	}
}


void World::drawExpanded( unsigned int camNum, Uint32* texture, unsigned int textureWidth, unsigned int textureHeight, double* depthTexture, unsigned int pixelSize, unsigned int detail, unsigned int drawWidth, unsigned int drawHeight, unsigned int startX, unsigned int startY, bool centerView ){
	WorldDrawArgs args;
	args.world = this;
	args.camNum = camNum;
	args.texture = texture;
	args.textureWidth = textureWidth;
	args.textureHeight = textureHeight;
	args.depthTexture = depthTexture;
	args.pixelSize = pixelSize;
	args.detail = detail;
	if( drawWidth == 0 ){ args.drawWidth = textureWidth; }
	else{ args.drawWidth = drawWidth; }
	if( drawHeight == 0 ){ args.drawHeight = textureHeight; }
	else{ args.drawHeight = drawHeight; }
	args.startX = startX;
	args.startY = startY;
	args.centerView = centerView;

	draw( args );
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


World::~World(){//this segfaults if the object was deleted by something else first
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


