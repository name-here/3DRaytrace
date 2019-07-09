struct Point{//<<<<<<<<<<<<Maybe make into class?
	double x;
	double y;
	double z;
	Point( double setX = 0, double setY = 0, double setZ = 0 );
	void set( double setX, double setY, double setZ );
	void set();

	Point& operator+=( const Point& toAdd );
	Point& operator-=( const Point& toSub );
	//Point& operator*=( const Point& toMult );
	Point& operator*=( const double toMult );
	Point& operator/=( const double toDiv );
	double dot( const Point& vec );
	Point& cross( const Point& vec );
};

inline Point operator+( Point pos1, const Point& pos2 ){
	pos1 += pos2;
	return pos1;
}
inline Point operator-( Point pos1, const Point& pos2 ){
	pos1 -= pos2;
	return pos1;
}
/*inline Point operator*( Point pos1, const Point& pos2 ){
	pos1 *= pos2;
	return pos1;
}*/
inline Point operator*( Point pos, const double& num ){
	pos *= num;
	return pos;
}
inline Point operator/( Point pos, const double& num ){
	pos /= num;
	return pos;
}
inline double dot( Point v1, const Point& v2 ){
	return v1.dot( v2 );
}
inline Point cross( Point v1, const Point& v2 ){
	v1.cross( v2 );
	return v1;
}

inline bool operator==( const Point& point1, const Point& point2 ){
	return (point1.x == point2.x)  &&  (point1.y == point2.y)  &&  (point1.z == point2.z);
}
inline bool operator!=( const Point& point1, const Point& point2 ){
	return !( point1 == point2 );
}


