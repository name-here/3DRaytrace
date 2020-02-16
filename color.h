#include <cstdint>


class Color{
	public:
		uint16_t r;
		uint16_t g;
		uint16_t b;
		Color( uint16_t setR, uint16_t setG, uint16_t setB );
		Color( uint16_t brightness );
		Color();
};

class FloatColor{//used for colors with no maximum and/or minimum value
	public:
		double r;
		double g;
		double b;
		FloatColor( double setR, double setG, double setB );
		FloatColor( double brightness );
		FloatColor();
};