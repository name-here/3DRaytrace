#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <string>//needed to save to a file
#include "rays.h"
#include "camera.h"
#include "objects.h"
#include "world.h"

void set(int, int, Color);

int windowWidth;//600
int windowHeight;//420
int windowSmallDim;//will be set to whichever window dimension is smaller
float FOVMultiplier = 0.8;//Multiplier for the field of view
Uint32* pixels;
int frameCount = 0;
Uint32 lastTime;
Uint32 frameRate;
int mouseX = 0;
int mouseY = -windowHeight/2;
bool mousePressed;
//bool print;
//int detail = 1;
//int detailSq = detail*detail;

bool doControl = true;
bool wDown = false;
bool aDown = false;
bool sDown = false;
bool dDown = false;

World world;

Object* testTri = new Tri( Point( 0, 0, UNIT*2 ), Point( UNIT, 0, UNIT*2 ), Point( UNIT/2, UNIT/2, UNIT*5/2 ), Color( 25600, 25600, 25600, 65535 ) );


void setup() {



	//The second paramater in Camera()--planeDist--is the only property of something in the world that should be set based on the actual screen.
	world.addCam(  new Camera( Point( 0, UNIT, -UNIT*3 ), windowSmallDim / FOVMultiplier, 0, 0 )  );


	world.addLight(  new Light( Point( 0, 0, -UNIT*2 ), Color( 65535, 65535, 65535 ) )  );//light1

	//world.addObj( new Ball( world.lightList[0]->pos, UNIT/10, Color( 65535, 0, 0 ), 0));//lightBall
	world.addObj(  new Plane( 1, -UNIT, UNIT/4, Color( 65535, 65535, 65535, 65535 ), Color( 50000, 50000, 50000 ) )  );//testPlane1
	world.addObj(  new Plane( 3, UNIT*2, UNIT/4, Color( 38400, 0, 38400, 65535 ), Color( 10000, 0, 20000 ) )  );//testPlane2
	//world.addObj(  new Plane( 1, UNIT*3, UNIT/4, Color( 0, 38400, 38400, 65535 ), Color( 0, 10000, 20000, 65535 ) )  );//testPlane3
	world.addObj(  new Ball( Point( 0, 0, 0 ), UNIT/2, Color( 65535, 65535 ), 65535 )  );//testBall1
	//world.addObj(  new Ball( Point( UNIT, UNIT, 0 ), UNIT/2, Color( 30000, 65535, 65535, 65535 ), 30000 )  );//testBall2
	world.addObj(  new AxisBox( Point( UNIT, UNIT, 0 ), Point( UNIT/2, UNIT/2, UNIT/2 ), Color( 10000, 0, 50000 ) )  );//testCube1
	/*int gridSize = 2;
	for( int x = 0; x<gridSize; x ++){
		for( int y = 0; y<gridSize; y ++){
			world.addObj( new Ball( Point( -UNIT/2+(x+0.5)*UNIT/gridSize, -UNIT/2+(y+0.5)*UNIT/gridSize, UNIT*1.5 ), UNIT/gridSize/2, Color( x*65535/gridSize, y*65535/gridSize, 30000, 65535 ), 30000 ) );//littleBall
		}
	}*/
}


void draw() {
	//The following movement system should be improved to account for same aligned and diagonal speed, movement relative to camera orientation, and frame time differences
	if(doControl){
		if( wDown ){
			world.camList[0]->move( world.camList[0]->pos + ( world.camList[0]->front * UNIT/10 ) );
		}
		if( aDown ){
			world.camList[0]->move(  world.camList[0]->pos - ( world.camList[0]->right * UNIT/10 )  );
		}
		if( sDown ){
			world.camList[0]->move( world.camList[0]->pos - ( world.camList[0]->front * UNIT/10 ) );
		}
		if( dDown ){
			world.camList[0]->move(  world.camList[0]->pos + ( world.camList[0]->right * UNIT/10 )  );
		}
	}

	//world.camList[0]->move( Point( 0, UNIT, ( (double)mouseY * 8 / windowHeight - 4 ) * UNIT  ) );
	//camera.planeDist = windowWidth;//( 1 - (((double)mouseY) / windowHeight) ) * windowWidth;
	//camera.rotate( 0, 0 );
	world.camList[0]->rotate( -M_PI*3/4*mouseX/windowWidth, -M_PI*mouseY/windowHeight );

	//camera.pos.z = -(mouseX+windowWidth/2)*4;
	//static_cast<Plane*>(world.objList[1])->dist = mouseY*4;
	//camera.pos.z = (windowWidth*frameCount/5);
	//static_cast<Plane*>(world.objList[2])->dist = (-21+frameCount)*windowHeight/20;

	world.lightList[0]->pos.x = sin( (frameCount)*M_PI/30 ) * UNIT;
	world.lightList[0]->pos.y = cos( (frameCount)*M_PI/30 ) * UNIT;
	//static_cast<Ball*>(world.objList[1])->pos.x = mouseX*2.0/windowWidth;
	//static_cast<Ball*>(world.objList[0])->pos = world.lightList[0]->pos;//make lightBall follow the light

	//GIF animation rotating:
	//world.camList[0]->move( Point( UNIT * 4 * cos(frameCount*M_PI/30), UNIT, UNIT * -4 * abs( sin(frameCount*M_PI/30) ) ) );
	//world.camList[0]->rotate( M_PI*(15-abs(frameCount-30))/30, -M_PI/20 );

	world.draw( 0, pixels, windowWidth, windowHeight, 2, 1, windowWidth, windowHeight-1, 0, 1 );
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
		if( windowWidth < windowHeight ){ windowSmallDim = windowWidth; }
		else{ windowSmallDim = windowHeight; }
		pixels = new Uint32[ windowWidth * windowHeight ];
		window = SDL_CreateWindow( "3D Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE );//used to end with "SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI" instead of "0".  For resizable, should be SDL_WINDOW_RESIZABLE.
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );

		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );
			buffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight );
			setup();
			if(doControl){
				SDL_ShowCursor( SDL_DISABLE );//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Probably not needed when program is build around rendering engine
			}
			while( !quit ){
				if( SDL_GetTicks() != lastTime  ){
					frameRate = 1000 / ( SDL_GetTicks() - lastTime );
				}
				lastTime = SDL_GetTicks();
				if( mousePressed ){
					printf("Frame Rate:%i\n", frameRate);
				}
				draw();
				SDL_UpdateTexture( buffer, NULL, pixels, windowWidth * sizeof(Uint32) );
				SDL_RenderClear( renderer );
				SDL_RenderCopy( renderer, buffer, NULL, NULL );
				SDL_RenderPresent( renderer );

				frameCount ++;

				/*if(frameCount<=61){//This saves frames for an animation
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
						if(doControl){
							mouseX += event.motion.x-(windowWidth/2);
							mouseY += -event.motion.y+(windowHeight/2);
							SDL_WarpMouseInWindow( window, windowWidth/2, windowHeight/2 );
							//printf("mouseX=%f, mouseY=%f\n", mouseX*1.0/windowWidth, mouseY*1.0/windowHeight);
						}
					}
					else if( event.type == SDL_MOUSEBUTTONDOWN ){
						mousePressed = true;
					}
					else if( event.type == SDL_MOUSEBUTTONUP ){
						mousePressed = false;
					}
					else if( event.type == SDL_KEYDOWN ){
						if( event.key.keysym.sym == SDLK_w ){ wDown = true; }
						if( event.key.keysym.sym == SDLK_a ){ aDown = true; }
						if( event.key.keysym.sym == SDLK_s ){ sDown = true; }
						if( event.key.keysym.sym == SDLK_d ){ dDown = true; }
					}
					else if( event.type == SDL_KEYUP ){
						if( event.key.keysym.sym == SDLK_w ){ wDown = false; }
						if( event.key.keysym.sym == SDLK_a ){ aDown = false; }
						if( event.key.keysym.sym == SDLK_s ){ sDown = false; }
						if( event.key.keysym.sym == SDLK_d ){ dDown = false; }
					}
					else if( event.type == SDL_WINDOWEVENT ){//This deals with window resizing
						if( event.window.event == SDL_WINDOWEVENT_RESIZED ){
							windowWidth = event.window.data1;
							windowHeight = event.window.data2;
							if( windowWidth < windowHeight ){ windowSmallDim = windowWidth; }
							else{ windowSmallDim = windowHeight; }
							world.camList[0]->planeDist = windowSmallDim / FOVMultiplier;//   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<THIS SHOULD BE CHANGED TO SOMETHING MORE UNIVERSAL
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
		pixels[ y * windowWidth  +  x ] = ( ( (int)sqrt( color.r ) ) << 16 ) + ( ( (int)sqrt( color.g ) ) << 8 ) + ( (int)sqrt( color.b ) );
	}
	else{ printf( "Tried to draw pixel out of bounds at (%i, %i)\n", x, y ); }
}


