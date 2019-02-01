#include <vector>

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


