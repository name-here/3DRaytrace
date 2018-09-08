#include <cstdint>


struct Point{
	float x;
	float y;
	float z;
	Point(float setX = 0, float setY = 0, float setZ = 0);
};


struct Ray{
	Point p1;
	Point p2;
	//float length;
	Ray(Point setP1 = Point(), Point setP2 = Point());
};

class CRay{
	uint8_t setR;
	uint8_t setG;
	uint8_t setB;
	uint8_t setA;
	public:
		Ray ray;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		//float length;
		Point setPos;
		float setDist;
		bool escape;
		uint32_t bounceCount;
		CRay(Ray setRay = Ray());
		void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, Point hit, float dist, bool ignoreDirection);
		void finishCast(bool setPos);
};


