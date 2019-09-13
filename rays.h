#include <cstdint>

class Object;


class Color{
	public:
		uint16_t r;
		uint16_t g;
		uint16_t b;
		uint16_t a;
		Color( uint16_t setR, uint16_t setG, uint16_t setB, uint16_t setA = 65535 );
		Color( uint16_t brightness, uint16_t setA = 65535 );
		Color();
};


class Ray{
	public:
		Point p1;
		Point p2;
		Ray( Point setP1 = Point(), Point setP2 = Point() );
		bool pointsAt( Point point );
		bool inRange( Point point );
		double getLength();
		Ray& normalize();
};

class CRay{//This type (casting ray) is used for casting out from the camera and checking against objects.  Its variables are used to keep track of relevant information.
	Color setCastColor;
	public:
		Ray ray;
		Color color;
		//double length;
		double currentIOR;//Index Of Refraction of the material the ray is currently in
		double nextIOR;//used to get Index Of Refraction of next object to be in, and used with currentIOR to calculate reflections/refractions
		Point setPos;
		double setDist;
		bool escape;
		uint32_t bounceCount;
		Object* objLastHit;
		Point normalVec;
		CRay( Ray setRay = Ray() );
		void intersect( Object* object, Color toSet, Point hit, double dist, Point objNormalVec /*, bool ignoreDirection = false*/ );//previously returned whether or not there was an intersection
		void castSky( Color skyColor );
		void addColor( Color addColor, Color addColorLight = Color(65535, 65535, 65535) );
		void finishCast( bool doSetPos, Color lightColor );
};


