#include <cstdint>
//#include "rays.h"


struct Light{
	Point pos;
	Color color;
	Light(Point setPos = Point(), Color setColor = Color());
};

class Object{
	public:
		virtual ~Object() = default;
		virtual void cast( CRay& ray, bool isShadow ) = 0;
		uint16_t reflect;
		unsigned int id;
};
class Tri: public Object{
	public:
		Point p1;
		Point p2;
		Point p3;
		Point normal;
		Color color;
		Tri( Point setP1 = Point(), Point setP2 = Point(), Point setP3 = Point(), Color setColor = Color(), uint16_t setReflect = 0 );
		void cast( CRay& ray, bool isShadow );
};
class Ball: public Object{
	public:
		Point pos;
		double radius;
		double radiusSq;
		Color color;
		Ball( Point setPos = Point(), double setRadius = 0, Color setColor = Color(), uint16_t setReflect = 0 );
		void cast( CRay& ray, bool isShadow );
};
class Plane: public Object{
	public:
		uint8_t axis;
		double dist;
		double gridSize;
		Color color1;
		Color color2;
		Plane( uint8_t setAxis = 0, double setDist = 0, double setGridSize = 1, Color setColor1 = Color(), Color setColor2 = Color(), uint16_t setReflect = 0 );
		void cast( CRay& ray, bool isShadow );
};


