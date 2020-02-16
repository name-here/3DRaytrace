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