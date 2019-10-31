#include <cstdint>
//#include "rays.h"


class Light{
	public:
		Point pos;
		FloatColor color;
		Light (Point setPos = Point(), FloatColor setColor = FloatColor() );
};


class Object{
	public:
		virtual ~Object() = default;
		virtual bool cast( CRay& ray, bool isShadow ) = 0;//isShadow is used if only checking for intersection (used for lighting)
		bool doLighting;
		uint16_t reflect;//reflectiveness (0-65535)
		uint16_t fresnel;//strength of effect making reflective objects darker for higher angles of incidence (0-65535, higher is darker)
		double indexOfRefraction;//index of refraction of material
};

class Tri: public Object{
	public:
		//Point pos;
		Point normal;
		Point p1;
		Point p2;
		Point p3;

		Point yVector;
		Point xVector;
		double length;
		double p2Y;
		double p3Y;

		Color color;
		Tri( Point setP1 = Point(), Point setP2 = Point(), Point setP3 = Point(), Color setColor = Color( 65535 ), bool setDoLighting = true, uint16_t setReflect = 0, uint16_t setFresnel = 49151 );
		bool cast( CRay& ray, bool isShadow );
};

class Ball: public Object{
	public:
		Point pos;
		double radius;
		double radiusSq;
		Color color;
		Ball( Point setPos = Point(), double setRadius = 0, Color setColor = Color( 65535 ), bool setDoLighting = true, uint16_t setReflect = 0, uint16_t setFresnel = 49151 );
		bool cast( CRay& ray, bool isShadow );
};

class AxisBox: public Object{
	public:
		Point pos;//at the center of the cube
		Point size;//x, y, and z distance from center to faces
		Color color;
		AxisBox( Point setPos = Point(), Point setSize = Point(), Color setColor = Color( 65535 ), bool setDoLighting = true, uint16_t setReflect = 0, uint16_t setFresnel = 49151 );
		bool cast( CRay& ray, bool isShadow );
};

class Plane: public Object{
	public:
		uint8_t axis;
		double dist;
		double gridSize;
		Color color1;
		Color color2;
		Plane( uint8_t setAxis = 0, double setDist = 0, double setGridSize = 1, Color setColor1 = Color(), Color setColor2 = Color( 65535 ), bool setDoLighting = true, uint16_t setReflect = 0, uint16_t setFresnel = 49151 );
		bool cast( CRay& ray, bool isShadow );
};


