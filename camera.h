class Camera{
	public:
		float x;
		float y;
		float z;
		float planeDist;
		float xFront;
		float yFront;
		float zFront;
		float xUp;
		float yUp;
		float zUp;
		float xRight;
		float yRight;
		float zRight;
		Camera(float setX, float setY, float setZ, float setPlaneDist, float setAngleX, float setAngleY);
		void move(float setX, float setY, float setZ);
		void rotate(float setAngleX, float setAngleY);
		void getRay(CRay& ray, float screenX, float screenY);
};


