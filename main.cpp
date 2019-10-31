//#define MAKE_IMAGE_SEQUENCE //if set, generates...

#ifdef WASM
	#include <emscripten.h>
	#include <emscripten/html5.h>
#endif
#include <SDL2/SDL.h>
#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cmath>
//#include <string>//needed to save to a file
#include "point.h"
#include "rays.h"
#include "camera.h"
#include "objects.h"
#include "world.h"

void set(int, int, Color);

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* buffer = nullptr;
Uint32* pixels;
bool quit = false;
SDL_Event event;

int windowWidth = 100;//600
int windowHeight = 100;//420
int windowSmallDim;//will be set to whichever window dimension is smaller
double FOVMultiplier = .5;//Multiplier for the field of view
//float mouseMoveStepX = 0.375;
//float mouseMoveStepY = 0.5;//These >>>>>>>will<<<<<<< determine the number of degrees of camera movement that correspond to 1 pixel of mouse movement

int frameCount = 0;
Uint32 lastTime;
Uint32 frameRate;
int mouseX = 0;
int mouseY = -windowHeight/2;
bool mousePressed;
//bool print;
//int detail = 1;
//int detailSq = detail*detail;

bool doControl = false;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<All these variables and related control code not needed when a program is built around this rendering engine
bool wDown = false;
bool aDown = false;
bool sDown = false;
bool dDown = false;
bool spaceDown = false;
bool shiftDown = false;

World world;


//setup function (called once before mainLoop starts being called)
void setup() {

	world = World(  Color( 0 )  );//Color( 38400, 51200, 65535, 65535 ) for blue background


	//The second paramater in Camera()--planeDist--is the only property of something in the world that should be set based on the actual screen.
	world.addCam(   new Camera(  Point( 0, UNIT/2, -UNIT*4 ),  (double)windowSmallDim/2 / FOVMultiplier,  0,  0,  0.05  )   );

	world.addLight(  new Light( Point( UNIT, UNIT*2, -UNIT*2 ), FloatColor( 1, 1, 3.5 ) )  );//light1
	world.addLight(  new Light( Point( -UNIT*2, UNIT*2, -UNIT*3 ), FloatColor( 6, 2, 1 ) )  );//light2
	world.addObj(  new Ball( world.lightList[0]->pos, UNIT/30, Color( 18724, 18724, 65535 ), false )  );//lightBall1
	world.addObj(  new Ball( world.lightList[1]->pos, UNIT/30, Color( 65535, 21845, 10923 ), false )  );//lightBall2

	world.addObj(  new Plane( 1, -UNIT, UNIT/4, Color( 65535, 65535, 65535 ), Color( 50000, 50000, 50000 )/*, true, 40000*/ )  );//testPlane1
	//world.addObj(  new Plane( 2, UNIT*2, UNIT/4, Color( 38400, 0, 38400 ), Color( 10000, 0, 20000 ) )  );//testPlane2
	//world.addObj(  new Plane( 2, UNIT*3, UNIT/4, Color( 0, 38400, 38400, 65535 ), Color( 0, 10000, 20000, 65535 ) )  );//testPlane3

	//world.addObj(  new Ball( Point( 0, 0, 0 ), UNIT/2, Color( 0 ), true, 50000 )  );//testBall1
	world.addObj(  new Ball( Point( -UNIT, UNIT, 0 ), UNIT/2, Color( 65535 ) )  );//testBall2

	//world.addObj(  new AxisBox( Point( -UNIT, 0, 0 ), Point( UNIT/2, UNIT/2, UNIT/2 ), Color( 10000, 0, 50000 ) )  );//testCube1

	/*int gridSize = 3;
	for( int x = 0; x<gridSize; x ++){
		for( int y = 0; y<gridSize; y ++){
			world.addObj( new Ball( Point( -UNIT/2+(x+0.5)*UNIT/gridSize, -UNIT/2+(y+0.5)*UNIT/gridSize, UNIT*1.5 ), UNIT/gridSize/2, Color( x*65535/gridSize, y*65535/gridSize, 30000, 65535 ), 30000 ) );//littleBall
		}
	}*/

	/*Tri triObj = */world.addObj(  new Tri( Point( UNIT/2, 0, 0 ), Point( 0, UNIT, UNIT/2 ), Point( UNIT*4/3, 0, 0 ), Color( 10000, 25600, 25600 ) )  );//testTri
	//world.addObj(  new Ball( Point( UNIT/2, 0, 0 ), UNIT/30, Color( 65535, 65535, 0 ), false )  );//TriVertex1
	//world.addObj(  new Ball( Point( 0, UNIT, UNIT/2 ), UNIT/30, Color( 65535, 65535, 0 ), false )  );//TriVertex2
	//world.addObj(  new Ball( Point( UNIT*4/3, 0, 0 ), UNIT/30, Color( 65535, 65535, 0 ), false )  );//TriVertex3
	//world.addObj(  new Ball( static_cast<Tri*>(world.objList[4])->p1 + static_cast<Tri*>(world.objList[4])->normal*UNIT, UNIT/30, Color( 65535, 65535, 0 ), false )  );//TriTest
}


//called once per frame, and deals with calling world.draw() and any logic other than rendeing
void draw() {
	//The following movement system should be improved to give same speed when moving forwad and sideways at the same time and when frame time is inconsistent
	if( doControl ){
		//Point moveFront = world.camList[0]->front * UNIT/10; moveFront.y = 0;
		if( wDown ){
			world.camList[0]->pos += ( world.camList[0]->front * UNIT/10 );
		}
		if( sDown ){
			world.camList[0]->pos -= ( world.camList[0]->front * UNIT/10 );
		}
		if( dDown ){
			world.camList[0]->pos += ( world.camList[0]->right * UNIT/10 );
		}
		if( aDown ){
			world.camList[0]->pos -= ( world.camList[0]->right * UNIT/10 );
		}
		if( spaceDown ){
			world.camList[0]->pos.set(  world.camList[0]->pos.x, world.camList[0]->pos.y + UNIT/10, world.camList[0]->pos.z );//Alternatively, pos += Point(0, UNIT/10, 0)
		}
		if( shiftDown ){
			world.camList[0]->pos.set(  world.camList[0]->pos.x, world.camList[0]->pos.y - UNIT/10, world.camList[0]->pos.z );
		}
	}

	//standard mouse control:
	if( doControl ){
		world.camList[0]->rotate(  -M_PI*3/4 * mouseX / windowWidth,  -M_PI*3/4 * mouseY / windowHeight  );
		//static_cast<Plane*>(world.objList[3])->dist = mouseX;
	}

	//world.camList[0]->move( Point( 0, UNIT, ( (double)mouseY * 8 / windowHeight - 4 ) * UNIT  ) );
	//camera.planeDist = windowWidth;//( 1 - (((double)mouseY) / windowHeight) ) * windowWidth;
	//camera.rotate( 0, 0 );
	//world.camList[0]->distortion = (double)mouseX / windowWidth;

	//camera.pos.z = -(mouseX+windowWidth/2)*4;
	//static_cast<Plane*>(world.objList[1])->dist = mouseY*4;
	//camera.pos.z = (windowWidth*frameCount/5);
	//static_cast<Plane*>(world.objList[2])->dist = (-21+frameCount)*windowHeight/20;

	//world.lightList[0]->pos.set(   sin( (frameCount)*M_PI/30 ) * UNIT,  cos( (frameCount)*M_PI/30 ) * UNIT,  world.lightList[0]->pos.z   );
	//static_cast<Ball*>(world.objList[1])->pos.x = mouseX*2.0/windowWidth;
	//static_cast<Ball*>(world.objList[0])->pos = world.lightList[0]->pos;//make lightBall follow the light

	//GIF animation rotating:
	//world.camList[0]->pos.set( UNIT * 4 * cos(frameCount*M_PI/30), UNIT, UNIT * 4 * abs( sin(frameCount*M_PI/30) ) );
	//world.camList[0]->rotate( M_PI*(15-abs(frameCount-30))/30, -M_PI/20 );

	world.draw( 0, pixels, windowWidth, windowHeight, 2, 1, windowWidth, windowHeight-1, 0, 1 );
}



//called repeatedly from main(), and handles calling draw(), drawing pixels[] to the screen, and input
void mainLoop(){
	if( SDL_GetTicks() != lastTime  ){
		frameRate = 1000 / ( SDL_GetTicks() - lastTime );
	}
	lastTime = SDL_GetTicks();
	/*if( mousePressed ){
		printf("Frame Rate:%i\n", frameRate);
	}*///<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<print framerate if mouse pressed

	draw();

	SDL_UpdateTexture( buffer, NULL, pixels, windowWidth * sizeof(Uint32) );
	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, buffer, NULL, NULL );
	SDL_RenderPresent( renderer );

	frameCount ++;

	#ifdef MAKE_IMAGE_SEQUENCE
		if(frameCount<=61){//This saves frames for an animation
			const char* name = ("frame_"+std::to_string(frameCount)+".bmp").c_str();
			SDL_SaveBMP(SDL_CreateRGBSurfaceFrom(pixels, windowWidth, windowHeight, 32, windowWidth*4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000), name);
			printf("Saved file %s\n", name);
		}
		else{quit = true;}
	#endif

	while( SDL_PollEvent( &event ) != 0 ){
		if( event.type == SDL_QUIT ){
			quit = true;
			break;
		}
		else if( event.type == SDL_MOUSEMOTION ){
			if(doControl){
				#ifdef WASM
					mouseX = event.motion.x-(windowWidth/2);
					mouseY = -event.motion.y+(windowHeight/2);
				#else
					mouseX += event.motion.x-(windowWidth/2);
					mouseY += -event.motion.y+(windowHeight/2);
					SDL_WarpMouseInWindow( window, windowWidth/2, windowHeight/2 );
				#endif
				//printf("mouseX=%f, mouseY=%f\n", mouseX*1.0/windowWidth, mouseY*1.0/windowHeight);
			}
		}
		else if( event.type == SDL_MOUSEBUTTONDOWN ){
			mousePressed = true;
			if( doControl ){
				const char* name = ("frame_"+std::to_string(frameCount)+".bmp").c_str();
				SDL_SaveBMP(SDL_CreateRGBSurfaceFrom(pixels, windowWidth, windowHeight, 32, windowWidth*4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000), name);
				printf("Saved file %s\n", name);
			}
			else{
				doControl = true;
				#ifdef WASM
					emscripten_request_pointerlock( "canvas", true );
				#else
					SDL_ShowCursor( SDL_DISABLE );
				#endif
			}
		}
		else if( event.type == SDL_MOUSEBUTTONUP ){
			mousePressed = false;
		}
		else if( event.type == SDL_KEYDOWN ){
			if( event.key.keysym.sym == SDLK_w ){ wDown = true; }
			else if( event.key.keysym.sym == SDLK_a ){ aDown = true; }
			else if( event.key.keysym.sym == SDLK_s ){ sDown = true; }
			else if( event.key.keysym.sym == SDLK_d ){ dDown = true; }
			else if( event.key.keysym.sym == SDLK_SPACE ){ spaceDown = true; }
			else if( event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT ){ shiftDown = true; }
			else if( event.key.keysym.sym == SDLK_ESCAPE ){
				doControl = false;
				#ifdef WASM
					emscripten_exit_pointerlock();
				#else
					SDL_ShowCursor( SDL_ENABLE );
				#endif
			}
		}
		else if( event.type == SDL_KEYUP ){
			if( event.key.keysym.sym == SDLK_w ){ wDown = false; }
			else if( event.key.keysym.sym == SDLK_a ){ aDown = false; }
			else if( event.key.keysym.sym == SDLK_s ){ sDown = false; }
			else if( event.key.keysym.sym == SDLK_d ){ dDown = false; }
			else if( event.key.keysym.sym == SDLK_SPACE ){ spaceDown = false; }
			else if( event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT ){ shiftDown = false; }
		}
		else if( event.type == SDL_WINDOWEVENT ){//This deals with window resizing
			if( event.window.event == SDL_WINDOWEVENT_RESIZED ){
				windowWidth = event.window.data1;
				windowHeight = event.window.data2;
				if( windowWidth < windowHeight ){ windowSmallDim = windowWidth; }
				else{ windowSmallDim = windowHeight; }
				world.camList[0]->planeDist = (double)windowSmallDim/2 / FOVMultiplier;//   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<THIS SHOULD BE CHANGED TO SOMETHING MORE UNIVERSAL (shouldn't be specific to a particular instance of the Camera object)
				delete pixels;
				pixels = new Uint32[ windowWidth * windowHeight ];
				SDL_DestroyTexture( buffer );
				buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
			}
		}
	}

}

//main function (called once on program start)
int main(/*int argc, char* args[]*/) {
	//SDL_Window* window = nullptr;
	//SDL_Renderer* renderer = nullptr;
	SDL_DisplayMode DM;
	//SDL_Texture* buffer = nullptr;
	//bool quit = false;
    //SDL_Event event;
	if(SDL_Init( SDL_INIT_VIDEO ) < 0){
		printf( "SDL could not initialize.  SDL_Error: %s\n", SDL_GetError() );
	}
	else{
		SDL_GetCurrentDisplayMode( 0, &DM );
		#ifdef WASM
			windowWidth = DM.w;
			windowHeight = DM.h;
		#else
			windowWidth = DM.w - 150;
			windowHeight = DM.h - 150;
		#endif
		if( windowWidth < windowHeight ){
			windowSmallDim = windowWidth;
			windowHeight = windowWidth;
		}
		else{
			windowSmallDim = windowHeight;
			windowWidth = windowHeight;
		}
		pixels = new Uint32[ windowWidth * windowHeight ];
		window = SDL_CreateWindow( "3D Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE );//used to end with "SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI" instead of "0".  For resizable, should be SDL_WINDOW_RESIZABLE.
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );

		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );
			buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
			setup();
			if( doControl ){
				#ifdef WASM
					emscripten_request_pointerlock( "canvas", true );
				#else
					SDL_ShowCursor( SDL_DISABLE );
				#endif
			}

			#ifdef WASM
				emscripten_set_main_loop( mainLoop, 0, 1 );
			#else
				while( !quit ){
					mainLoop();
				}
			#endif
		}
	}
	delete[] pixels;
	SDL_DestroyTexture( buffer );//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Program crashed on this line on exit a couple times
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}


//sets an individual pixel on the screen at position (x, y) to the given color---no longer used
/*void set( int x, int y, Color color ){
	if( x >= 0  &&  x < windowWidth  &&  y >= 0  &&  y < windowHeight  &&  (y*windowWidth)+x < windowWidth*windowHeight){
		pixels[ y * windowWidth  +  x ] = ( ( (int)sqrt( color.r ) ) << 16 ) + ( ( (int)sqrt( color.g ) ) << 8 ) + ( (int)sqrt( color.b ) );
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}*/


