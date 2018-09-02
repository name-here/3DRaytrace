#include <cstdint>
#include <vector>


class Object{
	public:
		virtual ~Object() = default;
		virtual void cast(CRay& ray, bool isShadow) = 0;
};

class World{
	public:
		Point light;
		std::vector<Object*> objList;
		//World(std::vector<Object*>&& setObjList);
		World();
		void cast(CRay& ray);
};

class Tri: public Object{
	public:
		Point p1;
		Point p2;
		Point p3;
		Point normal;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		Tri(Point setP1 = Point(), Point setP2 = Point(), Point setP3 = Point(), uint8_t setR = 0, uint8_t setG = 0, uint8_t setB = 0, uint8_t setA = 0);
		void cast(CRay& ray, bool isShadow);
};

class Ball: public Object{
	public:
		Point pos;
		float radius;
		float radiusSq;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		uint8_t reflect;
		Ball(Point setPos = Point(), float setRadius = 0, uint8_t setR = 0, uint8_t setG = 0, uint8_t setB = 0, uint8_t setA = 0, uint8_t setReflect = 0);
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
		uint8_t reflect;
		Plane(uint8_t setAxis = 0, float setDist = 0, float setGridSize = 1, uint8_t setR1 = 0, uint8_t setG1 = 0, uint8_t setB1 = 0, uint8_t setA1 = 0, uint8_t setR2 = 0, uint8_t setG2 = 0, uint8_t setB2 = 0, uint8_t setA2 = 0, uint8_t setReflect = 0);
		void cast(CRay& ray, bool isShadow);
};


