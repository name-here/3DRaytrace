/*
TODO:                                                                                    |𝔻𝕠𝕟𝕖𝕟𝕖𝕤𝕤|
	-classes.cpp line 182: add position to ball shadow casting(??)                       |StilToDo|
	-Find the normal of a triangle when it is created                                    |StilToDo|
	-Finish triangle.cast                                                                |StilToDo|
	-Make triangle.cast work for shadows                                                 |StilToDo|
	-Find the required vectors for the camera when it is created                         |StilToDo|
	-Finish camera.rotate and camera.getRay to make them actually work with rotation     |StilToDo|
	-Fix shadows!                                                                        |StilToDo|
	-Add shadows for plane.cast                                                          |StilToDo|
	-Restructure so that world object contains all objects to be cast                    |StilToDo|
	-Add reflections to all objects                                                      |StilToDo|
	-                                                                                    |_-_-_-_-|
	-                                                                                    |_-_-_-_-|
	-Avoid drawing spheres that are behind the camera                                    |PartDone|
	-Figure out mouseX and mouseY polarity                                               |  Done  |
	-Make the plane cast actually get what axis it is on from the variable               |  Done  |
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <limits>//This library is used to get the float max value.
#include "rays.h"
#include "camera.h"
#include "objects.h"

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
Camera camera(0, 0, 0, displayWidth, 0, -90);
CRay cRay(0, 0, 0, 0, 0, displayWidth);
Object* testTri = new Tri(0, 0, displayWidth*2, displayWidth, 0, displayWidth*2, displayWidth/2, displayWidth/2, displayWidth*5/2, 100, 100, 100, 255);
Object* testBall = new Ball(0, 0, -displayWidth, displayHeight/2, 255, 255, 255, 255, 0);
Object* lightBall = new Ball(lightX, lightY, lightZ, displayHeight/10, 255, 0, 0, 255, 0);
Object* testPlane = new Plane(1, 0, displayWidth/8, 0, 150, 150, 255, 150, 0, 150, 255, 127);


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
	lightBall->cast(cRay, false);
	testBall->cast(cRay, false);
	testPlane->cast(cRay, false);
	cRay.setColor(150, 200, 255, 255, F_INFINITY, F_INFINITY, F_INFINITY, F_INFINITY, true);
	cRay.finishCast(true);
	cRay.ray.x1 = lightX;
	cRay.ray.y1 = lightY;
	cRay.ray.z1 = lightZ;
	testBall->cast(cRay, true);
	/*if(cRay.escape){
		cRay.finishCast();
	}*/
	set(x+displayWidth/2, displayHeight-(y+displayHeight/2)/*optomization could cause an off-by-one error with some display sizes*/, cRay.r, cRay.g, cRay.b);
}

void draw(){
	camera.z = (mouseX+displayWidth/2)*4;
	static_cast<Plane*>(testPlane)->dist = mouseY*4;
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



int main(/*int argc, char* args[]*/){
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_DisplayMode DM;
	SDL_Texture* buffer = nullptr;
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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
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
							//printf("mouseX=%f, mouseY=%f\n", mouseX*1.0/displayWidth, mouseY*1.0/displayHeight);
					}
				}
			}
		}
	}
	delete testTri;
	delete testBall;
	delete lightBall;
	delete testPlane;
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


