CRay::CRay(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2){
	x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	escape = true;
}
void CRay::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, float hitX, float hitY, float hitZ, float dist, bool ignoreDirection){
	if((ignoreDirection || (x2>x1^hitX>x1 && y2>y1^hitY>y1 && z2>z1^hitZ>z1)) && dist<setDist){
		setDist = dist;
		setX = hitX;
		setY = hitY;
		setZ = hitZ;
		setR = red;
		setG = green;
		setB = blue;
		setA = alpha;
	}
}
void CRay::finishCast(){
	x1 = setX;
	y1 = setY;
	z1 = setZ;
	r = setR*setA*a/65025+r*(65025-setA*a)/65025;
	g = setG*setA*a/65025+g*(65025-setA*a)/65025;
	b = setB*setA*a/65025+b*(65025-setA*a)/65025;
	a = a*(255-setA)/255;
	if(a==0){escape = false;}
}

Ray::Ray(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2){
	x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
}

Tri::Tri(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2, float setX3, float setY3, float setZ3, uint8_t setR, uint8_t setG, uint8_t setB){
	x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;
	x3 = setX3;
	y3 = setY3;
	z3 = setZ3;
	r = setR;
	g = setG;
	b = setB;
}

Ball::Ball(float setX, float setY, float setZ, float setRadius, uint8_t setR, uint8_t setG, uint8_t setB, uint8_t setA){
	x = setX;
	y = setY;
	z = setZ;
	radius = setRadius;
	radiusSq = setRadius*setRadius;
	r = setR;
	g = setG;
	b = setB;
	a = setA;
}