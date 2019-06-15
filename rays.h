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


struct Point{
	double x;
	double y;
	double z;
	Point( double setX = 0, double setY = 0, double setZ = 0 );
	void set( double setX, double setY, double setZ );
	void set();

	Point& operator+=( const Point& toAdd );
	Point& operator-=( const Point& toSub );
	//Point& operator*=( const Point& toMult );
	Point& operator*=( const double toMult );
	Point& operator/=( const double toDiv );
	double dot( const Point& vec );
	Point& cross( const Point& vec );
};

inline Point operator+( Point pos1, const Point& pos2 ){
	pos1 += pos2;
	return pos1;
}
inline Point operator-( Point pos1, const Point& pos2 ){
	pos1 -= pos2;
	return pos1;
}
/*inline Point operator*( Point pos1, const Point& pos2 ){
	pos1 *= pos2;
	return pos1;
}*/
inline Point operator*( Point pos, const double& num ){
	pos *= num;
	return pos;
}
inline Point operator/( Point pos, const double& num ){
	pos /= num;
	return pos;
}
inline double dot( Point v1, const Point& v2 ){
	return v1.dot( v2 );
}
inline Point cross( Point v1, const Point& v2 ){
	v1.cross( v2 );
	return v1;
}

inline bool operator==( const Point& point1, const Point& point2 ){
	return (point1.x == point2.x)  &&  (point1.y == point2.y)  &&  (point1.z == point2.z);
}
inline bool operator!=( const Point& point1, const Point& point2 ){
	return !( point1 == point2 );
}


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


