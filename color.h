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

		FloatColor& operator+=( const FloatColor& toAdd );
		FloatColor& operator-=( const FloatColor& toSub );
		FloatColor& operator*=( const FloatColor& toMult );
		FloatColor& operator*=( const double toMult );
		FloatColor& operator/=( const double toDiv );
};

inline FloatColor operator+( FloatColor color1, const FloatColor& color2 ){
	color1 += color2;
	return color1;
}
inline FloatColor operator-( FloatColor color1, const FloatColor& color2 ){
	color1 -= color2;
	return color1;
}
inline FloatColor operator*( FloatColor color1, const FloatColor& color2 ){
	color1 *= color2;
	return color1;
}
inline FloatColor operator*( FloatColor color, const double& num ){
	color *= num;
	return color;
}
inline FloatColor operator/( FloatColor color, const double& num ){
	color /= num;
	return color;
}


