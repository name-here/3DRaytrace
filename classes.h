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
		void finishCast(bool setPos);
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


class Camera{
	public:
		float x;
		float y;
		float z;
		float planeDist;
		float xFront;
		float yFront;
		float zFront;
		float xUp;
		float yUp;
		float zUp;
		float xRight;
		float yRight;
		float zRight;
		Camera(float setX, float setY, float setZ, float setPlaneDist, float setAngleX, float setAngleY);
		void move(float setX, float setY, float setZ);
		void rotate(float setAngleX, float setAngleY);
		void getRay(CRay& ray, float screenX, float screenY);
};


class Object{
	public:
		virtual void cast(CRay& ray, bool isShadow);
};

class Tri: public Object{
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
		float normalX;
		float normalY;
		float normalZ;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		Tri(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2, float setX3, float setY3, float setZ3, uint8_t setR, uint8_t setG, uint8_t setB);
		void cast(CRay& ray, bool isShadow);
};

class Ball: public Object{
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
		void cast(CRay& ray, bool isShadow);
};

class Plane: public Object{
	public:
		uint8_t axis;
		float dist;
		float gridSize;
		uint8_t r1;
		uint8_t g1;
		uint8_t b1;
		uint8_t a1;
		uint8_t r2;
		uint8_t g2;
		uint8_t b2;
		uint8_t a2;
		Plane(uint8_t setAxis, float setDist, float setGridSize, uint8_t setR1, uint8_t setG1, uint8_t setB1, uint8_t setA1, uint8_t setR2, uint8_t setG2, uint8_t setB2, uint8_t setA2);
		void cast(CRay& ray, bool isShadow);
};

#include "classes.cpp"