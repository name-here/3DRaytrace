#include <cstdint>


struct Color{
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t a;
	Color( uint16_t setR, uint16_t setG, uint16_t setB, uint16_t setA = 65535 );
	Color( uint16_t brightness, uint16_t setA = 65535 );
	Color();
};


struct Ray{
	Point p1;
	Point p2;
	//double length;
	Ray( Point setP1 = Point(), Point setP2 = Point() );
	bool pointsAt( Point point );
	bool inRange( Point point );
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
		unsigned int objLastHit;
		Point normalVec;
		CRay( Ray setRay = Ray() );
		void intersect( unsigned int id, Color toSet, Point hit, double dist, Point objNormalVec, bool ignoreDirection = false );//previously returned whether or not there was an intersection
		void finishCast( bool doSetPos, Color lightColor );
};


