/*
TODO:                                                                                    |Doneness|
	-Line 64: add position                                                               |????????|
	-Figure out mouseX and mouseY polarity                                               |MabyDone|
	-Avoid drawing spheres that are behind the camera                                    |ProbDone|
	-Find the normal of a triangle when it is created                                    |Not Done|
	-Finish triangle.cast                                                                |Not Done|
	-Find the required vectors for the camera when it is created                         |Not Done|
	-Finish camera.rotate and camera.getRay to make them actually work with rotation     |Not Done|
	-Make the plane cast actually get what axis it is on from the variable               |_-_-_-_-|
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include <limits>//This library is used to get the float max value.
#include <iostream>
#include "classes.h"

#define F_INFINITY std::numeric_limits<float>::infinity()

void set(int, int, uint8_t, uint8_t, uint8_t);
//float square(float num);
//float dist3D(float x1, float y1, float z1, float x2, float y2, float z2);

int displayWidth = 1000;
int displayHeight = 700;
Uint32* pixels = new Uint32[displayWidth*displayHeight];
int frameCount = 0;
int mouseX;
int mouseY;
float lightX = 0;
float lightY = displayHeight;
float lightZ = 0;
bool print;
Object test = Ball(0, 0, -displayWidth, displayHeight/2, 0, 255, 100, 255);
Camera camera(0, displayWidth*3, 0, displayWidth, 0, 0);
CRay cRay(0, 0, 0, 0, 0, displayWidth);
Tri testTri(0, 0, displayWidth*2, displayWidth, 0, displayWidth*2, displayWidth/2, displayWidth/2, displayWidth*5/2, 100, 100, 100);
Ball testBall(0, 0, -displayWidth, displayHeight/2, 0, 255, 100, 255);
Ball lightBall(lightX, lightY, lightZ, displayHeight/10, 255, 0, 0, 255);
Plane testPlane(1, 0, displayWidth/2, 0, 0, 0, 255, 150, 0, 150, 255);


/*CRay test2D(CRay ray){
	if(square(ray.x2)+square(ray.y2)<square(displayHeight/2)){
		if(ray.y2<sin(ray.x2*15.7079632679/displayWidth-frameCount*displayWidth/50)*displayHeight/4){
			ray.r = 50;
			ray.g = 200;
			ray.b = 20;
		}
		else{
			ray.r = 200;
			ray.g = 20;
			ray.b = 50;
		}
		ray.escape = false;
	}
	return ray;
}*/


void renderPixel(int x, int y){
	print = y==0 && frameCount==15;
	camera.getRay(cRay, x, y);
	//testTri.cast(cRay, false);
	lightBall.cast(cRay, false);
	testBall.cast(cRay, false);
	testPlane.cast(cRay, false);
	cRay.setColor(150, 200, 255, 255, F_INFINITY, F_INFINITY, F_INFINITY, F_INFINITY, true);
	cRay.finishCast(true);
	cRay.x2 = lightX;
	cRay.y2 = lightY;
	cRay.z2 = lightZ;
	testBall.cast(cRay, true);
	/*if(cRay.escape){
		cRay.finishCast();
	}*/
	set(x+displayWidth/2, displayHeight-(y+displayHeight/2)/*optomization could cause an off-by-one error with some display sizes*/, cRay.r, cRay.g, cRay.b);
}

void draw(){
	-camera.z = mouseX*4;
	//lightBall.x = lightX;
	//lightBall.y = lightY;
	//lightBall.z = lightZ;
	//testBall.z = (mouseX)*10;
	for(int y = displayHeight; y>0; y --){
		for(int x = 0; x<displayWidth; x ++){
			renderPixel(x-displayWidth/2, y-displayHeight/2);
			//set(x, y, 255, 0, 255);
		}
	}
}



int main(int argc, char* args[]){
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_DisplayMode DM;
	SDL_Texture* buffer;
	bool quit = false;
    SDL_Event event;
	SDL_GetCurrentDisplayMode(0, &DM);
	printf("width: %i, ", DM.w);
	printf("height: %i\n", DM.h);
	//displayWidth = DM.w;
	//displayHeight = DM.h;
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		printf("SDL could not initialize.  SDL_Error: %s\n", SDL_GetError());
	}
	else{
		window = SDL_CreateWindow("3D Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayWidth, displayHeight, 0);//used to end with "SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI" instead of "0"
		if(window==NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else{
			renderer = SDL_CreateRenderer(window, -1, 0);//last "0" is for flags, and was previously "SDL_RENDERER_PRESENTVSYNC"
			buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, displayWidth, displayHeight);
			while(!quit){
				draw();
				//memset(pixels, 255, displayWidth*displayHeight*sizeof(Uint32));
				SDL_UpdateTexture(buffer, NULL, pixels, displayWidth*sizeof(Uint32));
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, buffer, NULL, NULL);
				SDL_RenderPresent(renderer);
				frameCount ++;
				while(SDL_PollEvent(&event)!=0){
					switch(event.type){
						case SDL_QUIT:
							quit = true;
							break;
						case SDL_MOUSEMOTION:
							mouseX = event.motion.x-displayWidth/2;
							mouseY = -event.motion.y+displayHeight/2;
							printf("mouseX%f\n", mouseX*1.0/displayWidth);
					}
				}
			}
		}
	}
	delete[] pixels;
	SDL_DestroyTexture(buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Done\n");
	return 0;
}

void set(int x, int y, uint8_t r, uint8_t g, uint8_t b){
	if(x>=0 && x<displayWidth && y>=0 && y<displayHeight){pixels[y*displayWidth+x] = (r<<16)+(g<<8)+b;}
	else{printf("Tried to draw pixel out of bounds at (%i, %i)\n", x, y);}
}
/*float square(float num){
	return num*num;
}
float dist3D(float x1, float y1, float z1, float x2, float y2, float z2){
	return sqrt(square(x2-x1)+square(y2-y1)+square(z2-z1));
}*/


