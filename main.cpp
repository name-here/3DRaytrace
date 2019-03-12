#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdint>
#include <cmath>
//#include <string>
#include "rays.h"
#include "camera.h"
#include "objects.h"
#include "world.h"

void set(int, int, Color);

int windowWidth;//600
int windowHeight;//420
Uint32* pixels;
int frameCount = 0;
Uint32 lastTime;
Uint32 frameRate;
int mouseX = 0;
int mouseY = -windowHeight/2;
bool mousePressed;
//bool print;
int detail = 1;
int detailSq = detail*detail;

double scale = 100;

World world;
//The second paramater in Camera()--planeDist--is the only property of something in the world that should be set based on the actual screen.
Camera camera;
CRay cRay( Ray( Point( 0, 0, 0 ), Point( 0, 0, scale ) ) );
Object* testTri = new Tri( Point( 0, 0, scale*2 ), Point( scale, 0, scale*2 ), Point( scale/2, scale/2, scale*5/2 ), Color( 25600, 25600, 25600, 65535 ) );


void setup() {
	camera = Camera( Point( 0, 0, -scale*5 ), windowWidth, 0, 0 );


	world.addLight( new Light( Point( 0, scale, scale ), Color( 65535, 65535, 65535, 65535 ) ) );//light1

	//world.addObj( new Ball( world.lightList[0]->pos, scale/10, Color( 65535, 0, 0 ), 0));//lightBall
	world.addObj( new Plane( 1, -scale, scale/4, Color( 65535, 65535, 65535, 65535 ), Color( 50000, 50000, 50000, 65535 ), 20000 ) );//testPlane1
	world.addObj( new Plane( 3, scale*2, scale/4, Color( 38400, 0, 38400, 65535 ), Color( 10000, 0, 20000, 65535 ), 20000 ) );//testPlane2
	//world.addObj( new Plane( 1, scale*2, scale/4, Color( 0, 38400, 38400, 65535 ), Color( 0, 10000, 20000, 65535 ), 0 ) );//testPlane3
	world.addObj( new Ball( Point( 0, 0, 0 ), scale/2, Color( 65535, 65535, 65535, 65535 ), 10000 ) );//testBall
	int gridSize = 3;
	for( int x = 0; x<gridSize; x ++){
		/*for( int y = 0; y<gridSize; y ++){
			world.addObj( new Ball( Point( -scale/2+(x+0.5)*scale/gridSize, -scale/2+(y+0.5)*scale/gridSize, scale*1.5 ), scale/gridSize/2, Color( x*65535/gridSize, y*65535/gridSize, 30000, 65535 ), 0 ) );//littleBall
		}*/
	}
}


void renderPixel( int x, int y ) {
	//print = x==0 && y==0 && frameCount==15;
	uint32_t rTotal = 0;
	uint32_t gTotal = 0;
	uint32_t bTotal = 0;
	for( int subX = 0; subX<detail; subX ++ ) {
		for( int subY = 0; subY<detail; subY ++ ) {
			camera.getRay( cRay, x+subX*1.0/detail, y+subY*1.0/detail );
			world.cast( cRay );
			rTotal += cRay.color.r;
			gTotal += cRay.color.g;
			bTotal += cRay.color.b;
		}
	}
	set(x+windowWidth/2, windowHeight-1-(y+windowHeight/2)/*optimizing here could cause an off-by-one error with some display sizes*/, Color((int)(rTotal/detailSq), (int)(gTotal/detailSq), (int)(bTotal/detailSq)));
}

void draw() {
	//camera.move( Point( 0, 0, (((double)mouseY) / windowHeight * scale) ) );
	//camera.planeDist = windowWidth;//( 1 - (((double)mouseY) / windowHeight) ) * windowWidth;
	//camera.rotate( 0, 0 );
	camera.rotate( -M_PI*3/2*mouseX/windowWidth, -M_PI*mouseY/windowHeight);

	//camera.pos.z = -(mouseX+windowWidth/2)*4;
	//static_cast<Plane*>(world.objList[1])->dist = mouseY*4;
	//camera.pos.z = (windowWidth*frameCount/5);
	//static_cast<Plane*>(world.objList[2])->dist = (-21+frameCount)*windowHeight/20;

	world.lightList[0]->pos.x = (sin(frameCount*M_PI/20) + 1) * scale;
	world.lightList[0]->pos.y = cos(frameCount*M_PI/20) * scale;
	//static_cast<Ball*>(world.objList[1])->pos.x = mouseX*2.0/windowWidth;
	//static_cast<Ball*>(world.objList[0])->pos = world.lightList[0]->pos;//make lightBall follow the light
	for(int y = windowHeight-1; y>=0; y --){
		for(int x = 0; x<windowWidth; x ++){
			renderPixel(x-windowWidth/2, y-windowHeight/2);
			/*if( (y > windowHeight*(sin(x*M_PI*3/windowWidth)+1)/2-mouseY) ^ (y > windowHeight/2) ){
				set( x, y, Color(65535, 30000, 10000) );
			}
			else{
				set( x, y, Color(0, 0, 0) );
			}*/
		}
	}
}



int main(/*int argc, char* args[]*/) {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_DisplayMode DM;
	SDL_Texture* buffer = nullptr;
	bool quit = false;
    SDL_Event event;
	if(SDL_Init( SDL_INIT_VIDEO ) < 0){
		printf( "SDL could not initialize.  SDL_Error: %s\n", SDL_GetError() );
	}
	else{
		SDL_GetCurrentDisplayMode( 0, &DM );
		windowWidth = DM.w/2;
		windowHeight = DM.h/2;
		pixels = new Uint32[ windowWidth * windowHeight ];
		window = SDL_CreateWindow( "3D Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE );//used to end with "SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI" instead of "0".  For resizable, should be SDL_WINDOW_RESIZABLE.
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );

		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );
			buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
			setup();
			while(!quit){
				if( SDL_GetTicks() != lastTime  ){
					frameRate = 1000 / ( SDL_GetTicks() - lastTime );
				}
				lastTime = SDL_GetTicks();
				if( mousePressed ){
					printf("Frame Rate:%i\n", frameRate);
				}
				draw();
				SDL_UpdateTexture( buffer, NULL, pixels, windowWidth*sizeof(Uint32) );
				SDL_RenderClear( renderer );
				SDL_RenderCopy( renderer, buffer, NULL, NULL );
				SDL_RenderPresent( renderer );

				frameCount ++;

				/*if(frameCount<=20){
					const char* name = ("frame_"+std::to_string(frameCount)+".bmp").c_str();
					SDL_SaveBMP(SDL_CreateRGBSurfaceFrom(pixels, windowWidth, windowHeight, 32, windowWidth*4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000), name);
					printf("Saved file %s\n", name);
				}
				else{quit = true;}*/

				while( SDL_PollEvent( &event ) != 0 ){
					if( event.type == SDL_QUIT ){
						quit = true;
						break;
					}
					else if( event.type == SDL_MOUSEMOTION ){
						mouseX = event.motion.x-(windowWidth/2);
						mouseY = -event.motion.y+(windowHeight/2);
						//printf("mouseX=%f, mouseY=%f\n", mouseX*1.0/windowWidth, mouseY*1.0/windowHeight);
					}
					else if( event.type == SDL_MOUSEBUTTONDOWN ){
						mousePressed = true;
					}
					else if( event.type == SDL_MOUSEBUTTONUP ){
						mousePressed = false;
					}
					else if( event.type == SDL_WINDOWEVENT ){//This deals with window resizing
						if( event.window.event == SDL_WINDOWEVENT_RESIZED ){
							windowWidth = event.window.data1;
							windowHeight = event.window.data2;
							camera.planeDist = windowWidth;
							delete pixels;
							pixels = new Uint32[ windowWidth * windowHeight ];
							SDL_DestroyTexture( buffer );
							buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
						}
					}
				}
			}
		}
	}
	delete testTri;
	delete[] pixels;
	SDL_DestroyTexture( buffer );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}

void set( int x, int y, Color color ){
	if( x >= 0  &&  x < windowWidth  &&  y >= 0  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight){
		pixels[ (y*windowWidth)+x ] = ( ( (int)sqrt( color.r ) ) << 16 ) + ( ( (int)sqrt( color.g ) ) << 8 ) + ( (int)sqrt( color.b ) );
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


