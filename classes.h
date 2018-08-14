class CRay{
	public:
		float x1;
		float y1;
		float z1;
		float x2;
		float y2;
		float z2;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		//float length;
		float setDist;
		float setX;
		float setY;
		float setZ;
		uint8_t setR;
		uint8_t setG;
		uint8_t setB;
		uint8_t setA;
		bool escape;
		CRay(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2);
		void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, float hitX, float hitY, float hitZ, float dist, bool ignoreDirection);
		void finishCast();
};
class Ray{
	public:
		float x1;
		float y1;
		float z1;
		float x2;
		float y2;
		float z2;
		//float length;
		Ray(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2);
};

class Tri{
	public:
		float x1;
		float y1;
		float z1;
		float x2;
		float y2;
		float z2;
		float x3;
		float y3;
		float z3;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		Tri(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2, float setX3, float setY3, float setZ3, uint8_t setR, uint8_t setG, uint8_t setB);
};
class Ball{
	public:
		float x;
		float y;
		float z;
		float radius;
		float radiusSq;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		Ball(float setX, float setY, float setZ, float setRadius, uint8_t setR, uint8_t setG, uint8_t setB, uint8_t setA);
};

#include "classes.cpp"