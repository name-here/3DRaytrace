#include <vector>

struct Light;
class Object;

class World{
	public:
		//std::vector<Camera*> camList;
		std::vector<Object*> objList;
		//World( std::vector<Object*>&& setObjList );
		std::vector<Light*> lightList;
		//World();
		void cast( CRay& ray );
};