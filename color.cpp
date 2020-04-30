#include "color.h"



Color::Color( uint16_t setR, uint16_t setG, uint16_t setB ){
	r = setR;
	g = setG;
	b = setB;
}

Color::Color( uint16_t brightness ){
	r = brightness;
	g = brightness;
	b = brightness;
}

Color::Color(){
	r = 0;
	g = 0;
	b = 0;
}



FloatColor::FloatColor( double setR, double setG, double setB ){
	r = setR;
	g = setG;
	b = setB;
}

FloatColor::FloatColor( double brightness ){
	r = brightness;
	g = brightness;
	b = brightness;
}

FloatColor::FloatColor(){
	r = 0;
	g = 0;
	b = 0;
}


FloatColor& FloatColor::operator+=( const FloatColor& toAdd ){
	r += toAdd.r;
	g += toAdd.g;
	b += toAdd.b;
	return *this;
}

FloatColor& FloatColor::operator-=( const FloatColor& toSub ){
	r -= toSub.r;
	g -= toSub.g;
	b -= toSub.b;
	return *this;
}

FloatColor& FloatColor::operator*=( const FloatColor& toMult ){
	r *= toMult.r;
	g *= toMult.g;
	b *= toMult.b;
	return *this;
}

FloatColor& FloatColor::operator*=( const double toMult ){
	r *= toMult;
	g *= toMult;
	b *= toMult;
	return *this;
}

FloatColor& FloatColor::operator/=( const double toDiv ){
	r /= toDiv;
	g /= toDiv;
	b /= toDiv;
	return *this;
}


