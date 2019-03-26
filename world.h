#include <vector>

const double UNIT = 100;//This is determines the size of the units used for size and positioning of all objects.
const double INTERSECT_ERR = UNIT/10000;//Intersection margin of error (for shapes that need it).  Rays must start this distance away from an object to hit it, to avoid rays bouncing off the same point twice.

struct Light;
class Object;

class World{
	public:
		std::vector<Light*> lightList;
		std::vector<Object*> objList;
		//std::vector<Camera*> camList;
		//World( std::vector<Object*>&& setObjList );
		//World();
		void addObj( Object* object );
		void addLight( Light* light );
		void cast( CRay& ray );
		~World();
};


