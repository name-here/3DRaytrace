#include <cstdint>


struct Color{
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t a;
	Color( uint16_t setR = 0, uint16_t setG = 0, uint16_t setB = 0, uint16_t setA = 0 );
};


struct Point{
	float x;
	float y;
	float z;
	Point( float setX = 0, float setY = 0, float setZ = 0 );

	Point& operator+=( const Point& toAdd );
	Point& operator-=( const Point& toSub );
	//Point& operator*=( const Point& toMult );
	Point& operator*=( const float toMult );
	//Point& operator/=( const Point& toDiv );
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
inline Point operator*( Point pos, float num ){
	pos *= num;
	return pos;
}
/*inline Point operator/( Point pos1, const Point& pos2 ){
	pos1 /= pos2;
	return pos1;
}*/


struct Ray{
	Point p1;
	Point p2;
	//float length;
	Ray( Point setP1 = Point(), Point setP2 = Point() );
};

class CRay{
	Color setCastColor;
	public:
		Ray ray;
		Color color;
		//float length;
		Point setPos;
		float setDist;
		bool escape;
		uint32_t bounceCount;
		CRay( Ray setRay = Ray() );
		void setColor( Color toSet, Point hit, float dist, bool ignoreDirection );
		void finishCast( bool setPos );
};


