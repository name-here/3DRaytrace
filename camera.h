class Camera{
	public:
		Point pos;
		float planeDist;
		Point front;
		Point up;
		Point right;//These three(front, up, right) are used as vectors, not points/
		Camera(Point setPos = Point(), float setPlaneDist = 1, float setAngleX = 0, float setAngleY = 0);
		void move(Point setPos);
		void rotate(float setAngleX, float setAngleY);
		void getRay(CRay& ray, float screenX, float screenY);
};


