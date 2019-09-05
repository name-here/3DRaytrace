class Camera{
	public:
		Point pos;
		double planeDist;
		double distortion;

		Point front;
		Point up;
		Point right;//These three(front, up, right) are used as vectors, not points/
		Camera( Point setPos = Point(), double setPlaneDist = 1, double setAngleX = 0, double setAngleY = 0, double setDistortion = 0 );
		//void move( Point setPos );
		void rotate( double setAngleX, double setAngleY );
		void getRay( CRay& ray, double screenX, double screenY );
};


