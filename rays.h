#include <cstdint>


class Ray{
	public:
		float x1;
		float y1;
		float z1;
		float x2;
		float y2;
		float z2;
		//float length;
		Ray(float setX1=0, float setY1=0, float setZ1=0, float setX2=0, float setY2=0, float setZ2=0);
};

class CRay{
	public:
		/*float x1;
		float y1;
		float z1;
		float x2;
		float y2;
		float z2;*/
		Ray ray;
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
		void finishCast(bool setPos);
};


