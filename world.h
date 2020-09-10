#include <SDL2/SDL.h>
#include <vector>

const double UNIT = 100;//This is determines the size of the units used for size and positioning of all objects.
const double INTERSECT_ERR = UNIT/10000;//Intersection margin of error (for shapes that need it).  Rays must start this distance away from an object to hit it, to avoid rays bouncing off the same point twice.
const uint32_t MAX_DEPTH = 15;//This is the maximum number of bounces/reflections/refractions/iterations the program will calculate.  Further potential ray interactions are ignored.

class Light;
class Object;
class Camera;

class World;


struct WorldDrawArgs{
	World* world;
	unsigned int camNum;//index of the camera to be used (in camList)
	Uint32* texture;//array of color values (in the form RRGGBB, with blue in least significant bits, and most significant 8 bits being unused so far), formatted as a list of rows (along the x direction)
	unsigned int textureWidth;
	unsigned int textureHeight;
	unsigned int pixelSize;//size in pixels of the square blocks that make up the rendered image
	unsigned int detail;//will cast detail^2 rays for each pixel, and average the results
	unsigned int drawWidth;
	unsigned int drawHeight;//width(x) and height(y) of the area to draw to (used to draw to small sectiosn of the texture)
	unsigned int startX;
	unsigned int startY;//Defines the upper left corner of the area to draw to
	bool centerView;//If true, rectngle defined by startX, startY, drawWidth, and drawHeight will show the center of the camera's view, rather than a cut out portion centered at the texture's center
};


class World{
	public:
		std::vector<Light*> lightList;
		std::vector<Object*> objList;
		std::vector<Camera*> camList;

		Color backgroundColor;
		double airIOR;//the index of refraction of the air (assumed to be the same everywhere)

		//World( std::vector<Object*>&& setObjList );
		//World();
		World( Color setBackgroundColor = Color(), double setAirIOR = 1 );

		void addObj( Object* object );
		void addLight( Light* light );
		void addCam( Camera* camera );//These add functions should return a pointer to the thing added, or an ID for accessing or removing it.

		void cast( CRay& ray );
		void recast( CRay& ray );

		void doDifuseReflect( CRay& ray );
		void doRefractReflect( CRay& ray );
		void doDirectLight( CRay& ray, uint16_t mixAmount );
		void doReflect( CRay& ray );

		static void draw( WorldDrawArgs args ); //( unsigned int camNum, Uint32* pixels, unsigned int width, unsigned int height, unsigned int pixelSize = 1, unsigned int detail = 1, unsigned int drawWidth = 0, unsigned int drawHeight = 0, unsigned int startX = 0, unsigned int startY = 0 );
		void drawExpanded( unsigned int camNum, Uint32* texture, unsigned int textureWidth, unsigned int textureHeight, unsigned int pixelSize = 1, unsigned int detail = 1, unsigned int drawWidth = 0, unsigned int drawHeight = 0, unsigned int startX = 0, unsigned int startY = 0, bool centerView = false );
		~World();
};


