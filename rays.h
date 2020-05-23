#include <cstdint>
#include "color.h"//<<<<<<<<<<<<<<<<<<<<<<<<<<<<THIS SHOULDN'T BE HERE!!!

class Object;



class Ray{
	public:
		Point p1;
		Point p2;
		Ray( Point setP1 = Point(), Point setP2 = Point() );
		bool pointsAt( Point point );
		bool inRange( Point point );
		double getLength();
		double cosAngleToUVec( Point normalRay );//used to get the cosine of the angle between this ray and a unit vector (vector of length 1), such as a normal vector
		Ray& normalize();
};

double cosAngleBetween( Ray ray1, Ray ray2 );


class CRay{//This type (casting ray) is used for casting out from the camera and checking against objects.  Its variables are used to keep track of relevant information.
	public://<<<<<<<<<<<<<<<<<<<<<<<Should be a struct?
		Ray ray;

		Color castColor;//color gotten from casting against objects (color to be mixed in)
		Color color;
		uint16_t colorMixLeft;//the amount of the ray's final color that is still left to be calculated and mixed in.  This is used to weight mixing more colors with the ray's current color variable.

		//double length;

		double currentIOR;//Index Of Refraction of the material the ray is currently in
		double nextIOR;//used to get Index Of Refraction of next object to be in, and used with currentIOR to calculate reflections/refractions

		Point hitPos;
		double hitDist;// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<These should be renamed (to better describe use/values, or should be made private)

		bool escape;

		//FloatColor lightColor;

		uint32_t bounceCount;//Make smaller to reflect max bounces (max possible value)?

		Object* objLastHit = nullptr;
		Point normalVec;//normal vector of the last object hit (ObjLastHit) at the intersection point

		CRay( Ray setRay = Ray() );
		bool intersect( Object* object, Color toSet, Point hit, double dist, Point objNormalVec );//previously returned whether or not there was an intersection
		//void addLight( FloatColor lightColor );//adds light for the current cast ( to be added with finishCast(___, true) )
		//void castBackground( Color backgroundColor );
		void addColor( Color addColor, uint16_t addColorAlpha = 65535, FloatColor addLightColor = FloatColor( 1 ) );
		//void finishCast( bool doSetPos, bool doSetColor );
};


