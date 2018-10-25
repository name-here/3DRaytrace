#include <cstdint>
#include <vector>


struct Light;
class Object;

class World{
	public:
		std::vector<Object*> objList;
		//World( std::vector<Object*>&& setObjList );
		std::vector<Light*> lightList;
		//World();
		void cast( CRay& ray );
};

struct Light{
	Point pos;
	Color color;
	Light(Point setPos = Point(), Color setColor = Color());
};

class Object{
	public:
		virtual ~Object() = default;
		virtual void cast( CRay& ray, bool isShadow, World& world ) = 0;
};
class Tri: public Object{
	public:
		Point p1;
		Point p2;
		Point p3;
		Point normal;
		Color color;
		uint16_t reflect;
		Tri( Point setP1 = Point(), Point setP2 = Point(), Point setP3 = Point(), Color setColor = Color(), uint16_t setReflect = 0 );
		void cast( CRay& ray, bool isShadow, World& world );
};
class Ball: public Object{
	public:
		Point pos;
		float radius;
		float radiusSq;
		Color color;
		uint16_t reflect;
		Ball( Point setPos = Point(), float setRadius = 0, Color setColor = Color(), uint16_t setReflect = 0 );
		void cast( CRay& ray, bool isShadow, World& world );
};
class Plane: public Object{
	public:
		uint8_t axis;
		float dist;
		float gridSize;
		Color color1;
		Color color2;
		uint16_t reflect;
		Plane( uint8_t setAxis = 0, float setDist = 0, float setGridSize = 1, Color setColor1 = Color(), Color setColor2 = Color(), uint16_t setReflect = 0 );
		void cast( CRay& ray, bool isShadow, World& world );
};


