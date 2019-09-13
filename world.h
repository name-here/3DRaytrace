#include <SDL2/SDL.h>
#include <vector>

const double UNIT = 100;//This is determines the size of the units used for size and positioning of all objects.
const double INTERSECT_ERR = UNIT/10000;//Intersection margin of error (for shapes that need it).  Rays must start this distance away from an object to hit it, to avoid rays bouncing off the same point twice.
const int MAX_BOUNCES = 15;//This is the maximum number of bounces the program will calculate.  Further potential reflections are ignored.

class Light;
class Object;
class Camera;


class World{
	public:
		std::vector<Light*> lightList;
		std::vector<Object*> objList;
		std::vector<Camera*> camList;
		//World( std::vector<Object*>&& setObjList );
		//World();
		void addObj( Object* object );
		void addLight( Light* light );
		void addCam( Camera* camera );//these add functions should return a pointer to the thing added, or an ID for accessing or removing it.
		void cast( CRay& ray );
		void draw( unsigned int camNum, Uint32* pixels, unsigned int width, unsigned int height, unsigned int pixelSize = 1, unsigned int detail = 1, unsigned int drawWidth = 0, unsigned int drawHeight = 0, unsigned int startX = 0, unsigned int startY = 0 );
		~World();
};


