//#define MAKE_IMAGE_SEQUENCE = 60 //if set, generates an image sequence with a number of images equal to the value of MAKE_IMAGE_SEQUENCE
//#define SAVE_IMAGE_ON_CLICK//if set, image saves an image of what is on screen on mouse click, or if not set, prints framerate to console on click

#ifdef WASM
	#include <emscripten.h>
	#include <emscripten/html5.h>
#endif
#include <SDL2/SDL.h>
#include <thread>
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
double FOVMultiplier = 0.5;//Multiplier for the field of view

int defaultPixelSize = 4;//to store the pixelSize to change back to after releasing [Q] key
int pixelSize = defaultPixelSize;//size of rendered pixels (number of (real) pixels across for chunky displayed pixels)
int smallPixelSize = 1;//changed to after pressing [Q] key
int detail = 1;//grid size of rendered points within each pixel, so the square of this number is the number of rays cast for each pixel — more is much slower, but makes edges smoother

int frameCount = 0;
Uint32 lastTicks;
Uint32 thisTicks;
Uint32 frameRate;
Uint32 minFrameTicks = 40;//equivalent of capping framerate, but defined in ticks per frame

int mouseX = 0;
int mouseY = -windowHeight/2;
bool mousePressed;
//float mouseMoveStepX = 0.375;
//float mouseMoveStepY = 0.5;//These >>>>>>>will<<<<<<< determine the number of degrees of camera movement that correspond to 1 pixel of mouse movement

//bool print;
//int detail = 1;
//int detailSq = detail*detail;

bool doControl = false;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<All these variables and related control code not needed when a program is built around this rendering engine
//bool qDown = false;
bool wDown = false;
bool aDown = false;
bool sDown = false;
bool dDown = false;
bool spaceDown = false;
bool shiftDown = false;

World world;


//setup function (called once before mainLoop starts being called)
void setup() {

	world = World();//Color( 38400, 51200, 65535, 65535 ) for blue background


	//The second paramater in Camera()--planeDist--is the only property of something in the world that should be set based on the actual screen.
	world.addCam(   new Camera(  Point( 0, UNIT/2, -UNIT*4 ),  (double)windowSmallDim/2 / FOVMultiplier,  0,  0,  0/*was 0.05*/  )   );

	world.addLight(  new Light( Point( UNIT, UNIT*2, -UNIT*2 ), FloatColor( 1, 1, 3.5 ) )  );//light1
	world.addLight(  new Light( Point( -UNIT*2, UNIT*2, -UNIT*3 ), FloatColor( 6, 2, 1 ) )  );//light2
	//world.addLight(  new Light( Point( -UNIT, UNIT, UNIT/100 ), FloatColor( 5, 5, 0.5 ) )  );//light3
	//world.addLight(  new Light( Point( 0, UNIT, -UNIT*5/2 ), FloatColor( -2 ) )  );//light4
	world.addObj(  new Ball( world.lightList[0]->pos, UNIT/30, Color( 18724, 18724, 65535 ), false )  );//lightBall1
	world.addObj(  new Ball( world.lightList[1]->pos, UNIT/30, Color( 65535, 21845, 10923 ), false )  );//lightBall2
	//world.addObj(  new Ball( world.lightList[2]->pos, UNIT/30, Color( 65535, 65535, 0 ), false )  );//lightBall3
	//world.addObj(  new Ball( world.lightList[3]->pos, UNIT/30, Color( 10000 ), false )  );//lightBall3

	world.addObj(  new Plane( 1, -UNIT, UNIT/4, Color( 65535, 65535, 65535 ), Color( 50000, 50000, 50000 )/*, true, 40000*/ )  );//testPlane1
	world.addObj(  new Plane( 2, UNIT*2, UNIT/4, Color( 38400, 0, 38400 ), Color( 10000, 0, 20000 ) )  );//testPlane2
	//world.addObj(  new Plane( 2, UNIT*3, UNIT/4, Color( 0, 38400, 38400, 65535 ), Color( 0, 10000, 20000, 65535 ) )  );//testPlane3

	world.addObj(  new Ball( Point( 0, 0, 0 ), UNIT/2, Color( 30000 ), true, 0 )  );//testBall1
	world.addObj(  new Ball( Point( -UNIT, UNIT, 0 ), UNIT/2, Color( 65535 ), true )  );//testBall2

	world.addObj(  new Tri( Point( UNIT/2, 0, UNIT ), Point( 0, UNIT, UNIT*9/10 ), Point( UNIT*4/3, 0, UNIT ), Color( 10000, 25600, 25600 ), true, 0 )  );//testTri

	world.addObj(  new AxisBox( Point( -UNIT, 0, -UNIT ), Point( UNIT/2, UNIT/2, UNIT/5 ), Color( 10000, 2000, 10000 ), true, 0, 5000, 1.5 )  );//testBox1
	//world.addObj(  new AxisBox( Point( -UNIT/2, 0, -UNIT ), Point( UNIT/4, UNIT/4, UNIT/4 ), Color( 10000, 2000, 10000 ), true, 0, 1000, 2 )  );//testBox2

	//world.addObj(   new Tube(  Ray( Point(0, 0, UNIT*-3), Point(0, 0, UNIT*-2) ),  UNIT/5,  Color( 65535, 0, 32767 ),  false  )   );//testTube

	//testTriCube:
	/*world.addObj(  new Tri( Point( 0, 0, 0 ), Point( 0, UNIT, 0 ), Point( UNIT, UNIT, 0 ) )  );
	world.addObj(  new Tri( Point( 0, 0, 0 ), Point( UNIT, UNIT, 0 ), Point( UNIT, 0, 0 ) )  );
	world.addObj(  new Tri( Point( UNIT, 0, 0 ), Point( UNIT, UNIT, 0 ), Point( UNIT, UNIT, UNIT ) )  );
	world.addObj(  new Tri( Point( UNIT, 0, 0 ), Point( UNIT, UNIT, UNIT ), Point( UNIT, 0, UNIT ) )  );
	world.addObj(  new Tri( Point( UNIT, 0, UNIT ), Point( UNIT, UNIT, UNIT ), Point( 0, UNIT, UNIT ) )  );
	world.addObj(  new Tri( Point( UNIT, 0, UNIT ), Point( 0, UNIT, UNIT ), Point( 0, 0, UNIT ) )  );
	world.addObj(  new Tri( Point( 0, 0, UNIT ), Point( 0, UNIT, UNIT ), Point( 0, UNIT, 0 ) )  );
	world.addObj(  new Tri( Point( 0, 0, UNIT ), Point( 0, UNIT, 0 ), Point( 0, 0, 0 ) )  );
	world.addObj(  new Tri( Point( 0, UNIT, 0 ), Point( 0, UNIT, UNIT ), Point( UNIT, UNIT, UNIT ) )  );
	world.addObj(  new Tri( Point( 0, UNIT, 0 ), Point( UNIT, UNIT, UNIT ), Point( UNIT, UNIT, 0 ) )  );
	world.addObj(  new Tri( Point( 0, 0, UNIT ), Point( 0, 0, 0 ), Point( UNIT, 0, 0 ) )  );
	world.addObj(  new Tri( Point( 0, 0, UNIT ), Point( UNIT, 0, 0 ), Point( UNIT, 0, UNIT ) )  );*/

	/*int gridSize = 3;
	for( int x = 0; x<gridSize; x ++){
		for( int y = 0; y<gridSize; y ++){
			world.addObj( new Ball( Point( -UNIT/2+(x+0.5)*UNIT/gridSize, -UNIT/2+(y+0.5)*UNIT/gridSize, UNIT*1.5 ), UNIT/gridSize/2, Color( x*65535/gridSize, y*65535/gridSize, 30000 ), 30000 ) );//littleBall
		}
	}*/

	
	//the following code traces out a ray and adds tube shapes
	/*int segments = 1;

	world.addCam(  new Camera( Point(0, UNIT/2, -UNIT*4), UNIT, 0, -M_PI/20 )  );
	CRay testRay;
	world.camList[1]->getRay( testRay, 0, 0 );
	Ball* points[segments+1];
	Tube* lines[segments];

	for( int i = 0; i < segments+1; i ++ ){
		points[i] = new Ball( Point(), UNIT/10, Color( 0, 65535, 0 ), false );
	}
	for( int i = 0; i < segments; i ++ ){
		lines[i] = new Tube( Ray(), UNIT/10, Color( 65535, 0, 0 ), false );
	}

	testRay.bounceCount = MAX_DEPTH;
	world.cast( testRay );
	lines[0]->setLine(  Ray( testRay.ray.p1, testRay.hitPos )  );
	points[0]->pos = testRay.ray.p1;
	points[1]->pos = testRay.hitPos;

	for( int i = 1; i < segments; i ++){
		testRay.bounceCount = MAX_DEPTH - 2;
		world.recast( testRay );
		lines[i]->setLine(  Ray( lines[i-1]->line.p2, testRay.hitPos )  );
		points[i+1]->pos = testRay.hitPos;
	}

	for( int i = 0; i < segments+1; i ++){
		//world.addObj( points[i] );
	}
	for( int i = 0; i < segments; i ++ ){
		world.addObj( lines[i] );
	}*/
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
		world.camList[0]->rotate(  M_PI*3/4 * mouseX / windowWidth,  M_PI*3/4 * mouseY / windowHeight  );
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


	WorldDrawArgs args;
	args.world = &world;
	args.camNum = 0;
	args.texture = pixels;
	args.textureWidth = windowWidth;
	args.textureHeight = windowHeight;
	args.pixelSize = pixelSize;
	args.detail = detail;
	args.centerView = false;

	int xGrid = 3;
	int yGrid = 3;
	std::thread** drawThreads = new std::thread*[ xGrid * yGrid ];
	for( int x = 0; x < xGrid; x ++ ){
		for( int y = 0; y < yGrid; y ++){
			args.startX = windowWidth * x / xGrid;
			args.startY = windowWidth * y / yGrid;
			args.drawWidth = windowWidth * (x+1) / xGrid  -  args.startX;
			args.drawHeight = windowHeight * (y+1) / yGrid  -  args.startY;
			drawThreads[ y * xGrid  +  x ] = new std::thread( world.draw, args );
			//std::thread drawThread( world.draw, args );
		}
	}

	for( int i = 0; i < xGrid * yGrid; i ++){
		drawThreads[i]->join();
	}

	//world.drawExpanded( 0, pixels, windowWidth, windowHeight, pixelSize, detail );
}



//called repeatedly from main(), and handles calling draw(), drawing pixels[] to the screen, and input
void mainLoop(){
	thisTicks = SDL_GetTicks();
	if( thisTicks != lastTicks  ){
		frameRate = 1000 / ( thisTicks - lastTicks );
		if( mousePressed ){
			printf("Max potential frame rate:%i\n", frameRate);
		}
	}
	if( thisTicks - lastTicks  <  minFrameTicks ){//if() around delay may not be necessary
		SDL_Delay( minFrameTicks + lastTicks - thisTicks );
	}
	/*else{
		printf("FrameTicks:%i\n", thisTicks - lastTicks);
	}*/
	lastTicks = SDL_GetTicks();
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
		if(frameCount<=MAKE_IMAGE_SEQUENCE){//This saves frames for an animation
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
				#ifdef SAVE_IMAGE_ON_CLICK
					const char* name = ("frame_"+std::to_string(frameCount)+".bmp").c_str();
					SDL_SaveBMP(SDL_CreateRGBSurfaceFrom(pixels, windowWidth, windowHeight, 32, windowWidth*4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000), name);
					printf("Saved file %s\n", name);
				#endif
				//printf("Frame rate:%i\n", frameRate);
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
			if( event.key.keysym.sym == SDLK_q ){
				pixelSize = smallPixelSize;
				//detail = 2;
			}
			else if( event.key.keysym.sym == SDLK_w ){ wDown = true; }
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
			if( event.key.keysym.sym == SDLK_q ){
				pixelSize = defaultPixelSize;
				//detail = 1;
			}
			else if( event.key.keysym.sym == SDLK_w ){ wDown = false; }
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


