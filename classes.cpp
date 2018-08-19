#define F_INFINITY std::numeric_limits<float>::infinity()

float square(float num){
	return num*num;
}
float dist3D(float x1, float y1, float z1, float x2, float y2, float z2){
	return sqrt(square(x2-x1)+square(y2-y1)+square(z2-z1));
}

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
void CRay::finishCast(bool setPos){
	if(setPos){
		x1 = setX;
		y1 = setY;
		z1 = setZ;
	}
	r = setR*setA*a/65025+r*(65025-setA*a)/65025;
	g = setG*setA*a/65025+g*(65025-setA*a)/65025;
	b = setB*setA*a/65025+b*(65025-setA*a)/65025;
	a = a*(255-setA)/255;
	if(a==0){escape = false;}
}
void CRay::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, float hitX, float hitY, float hitZ, float dist, bool ignoreDirection){
	if((ignoreDirection || (x2>x1==hitX>x1 && y2>y1==hitY>y1 && z2>z1==hitZ>z1)) && dist<=setDist && alpha>0){
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

Ray::Ray(float setX1, float setY1, float setZ1, float setX2, float setY2, float setZ2){
	x1 = setX1;
	y1 = setY1;
	z1 = setZ1;
	x2 = setX2;
	y2 = setY2;
	z2 = setZ2;
	//length = dist3D(x1, y1, z1, x2, y2, z2);
}


Camera::Camera(float setX, float setY, float setZ, float setPlaneDist, float setAngleX, float setAngleY){
	x = setX;
	y = setY;
	z = setZ;
	planeDist = setPlaneDist;
	xFront = 0;//xyzFront is a unit vector pointing in the direction that the camera is facing.
	yFront = 0;
	zFront = 0;
	xUp = 0;//xyzUp is a unit vector pointing directly up from the camera.
	yUp = 0;
	zUp = 0;
	xRight = 0;//xyzRight is a unit vector pointing directly to the right the camera.
	yRight = 0;
	zRight = 0;//These should be found instead of being set to 0.
}
void Camera::move(float setX, float setY, float setZ){
	x = setX;
	y = setY;
	x = setZ;
}
void Camera::rotate(float setAngleX, float setAngleY){
	//This should set the front, up, and right vectors.
}
void Camera::getRay(CRay& ray, float screenX, float screenY){
	ray.x1 = x;
	ray.y1 = y;
	ray.z1 = z;
	ray.x2 = x+screenY;
	ray.y2 = y-planeDist;
	ray.z2 = z+screenX;
	ray.r = 0;
	ray.g = 0;
	ray.b = 0;
	ray.a = 255;
	ray.setDist = F_INFINITY;
	ray.setX = x;
	ray.setY = y;
	ray.setZ = z;
	ray.setR = 0;
	ray.setG = 0;
	ray.setB = 0;
	ray.setA = 255;
	ray.escape = true;
}


void Object::cast(CRay& ray, bool isShadow){
	printf("Error in \"cast\" function implimentation(default was used).\n");
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
	normalX = 0;//These 3 values should actually be calculated instead of being set to 0.
	normalY = 0;
	normalX = 0;
	r = setR;
	g = setG;
	b = setB;
}
void Tri::cast(CRay& ray, bool isShadow){
	float pointX = ray.x2;
	float pointY = ray.y2;
	float triX1 = x1;
	float triY1 = y1;
	float triX2 = x2;
	float triY2 = y2;
	float triX3 = x3;
	float triY3 = y3;
	if(!(pointY-triY1<(triY2-triY1)*(pointX-triX1)/(triX2-triX1)^triX2<triX1) && !(pointY-triY2<(triY3-triY2)*(pointX-triX2)/(triX3-triX2)^triX3<triX2) && !(pointY-triY3<(triY1-triY3)*(pointX-triX3)/(triX1-triX3)^triX1<triX3)){
		ray.r = r;
		ray.g = g;
		ray.b = b;
		ray.escape = false;
	}
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
void Ball::cast(CRay& ray, bool isShadow){
	float lineX1 = dist3D(ray.x1, ray.y1, ray.z1, x, y, z);
	float dist1Sq = square(x-ray.x2)+square(y-ray.y2)+square(z-ray.z2);
	float dist2 = dist3D(ray.x2, ray.y2, ray.z2, ray.x1, ray.y1, ray.z1);
	float lineX2 = abs((dist1Sq-square(dist2)+square(lineX1))/(2*lineX1)); //this x and y, along with lineX1 /*and lineX2*/, define a ray in a 2D space that will actually be used to calculate the intersection between the original ray and the sphere.  The 2D plane is the plane that intersects the center of the sphere and both points on the 3D ray.
	float lineY2 = sqrt(dist1Sq-square(lineX2));
	float num1;
	float num2;
	float num3;
	if(lineY2!=0){
		num1 = (lineX2-lineX1)/lineY2;//The num+[number] variables store numbers that are used more than once, so that they don't have to be calculated 2 or 3 times.
		num2 = square(num1)+1;
		num3 = num2*radiusSq-lineX1*lineX1;
	}
	//if(print){printf("lineX2:%f, ray z:%f, lineY2:%f\n",lineX2,ray.z2,lineY2);}
	if(/*if ray hits 2D circle (slice of sphere)*/lineY2==0 || num3>=0){
		float num4;
		if(lineY2==0){num4 = radius;}
		else{num4 = (-lineX1*num1-sqrt(num3))/(num2);}
		float distance = sqrt(square(lineX1-sqrt(radiusSq-num4*num4))+num4*num4);
		float scale = distance/dist3D(ray.x1, ray.y1, ray.z1, ray.x2, ray.y2, ray.z2);// could also be "/ray.length" if ray.length gets implemented.
		if(isShadow){
			float hitX = ray.x1+(ray.x2-ray.x1)*scale;
			float hitY = ray.y1+(ray.y2-ray.y1)*scale;
			float hitZ = ray.z1+(ray.z2-ray.z1)*scale;
			/*The followint "if" statement determines if the hit location is actually between the light source and the point to cast the shadow on.*/
			if((hitX>ray.x1^hitX>ray.x2)&&(hitX<ray.x1^hitX<ray.x2) && (hitY>ray.y1^hitY>ray.y2)&&(hitY<ray.y1^hitY<ray.y2) && (hitZ>ray.z1^hitZ>ray.z2)&&(hitZ<ray.z1^hitZ<ray.z2)){//ray.x2>ray.x1==hitX>ray.x1 && ray.y2>ray.y1==hitY>ray.y1 && ray.z2>ray.z1==hitZ>ray.z1  &&  ray.x2>ray.x1==hitX<ray.x2 && ray.y2>ray.y1==hitY<ray.y2 && ray.z2>ray.z1==hitZ<ray.z2){
				//ray.setColor(0, 0, 0, 255, F_INFINITY, F_INFINITY, F_INFINITY, F_INFINITY, true);//The position for this should actually be set, but isn't yet
				ray.r *= 0.5;
				ray.g *= 0.5;
				ray.b *= 0.5;
			}
		}
		else{
			ray.setColor(r, g, b, a, ray.x1+(ray.x2-ray.x1)*scale, ray.y1+(ray.y2-ray.y1)*scale, ray.z1+(ray.z2-ray.z1)*scale, distance, false);
		}
	}
	//else if(lineY2<0.1){printf("lineY2=%f\n", lineY2);}
}

Plane::Plane(uint8_t setAxis, float setDist, float setGridSize, uint8_t setR1, uint8_t setG1, uint8_t setB1, uint8_t setA1, uint8_t setR2, uint8_t setG2, uint8_t setB2, uint8_t setA2){
	axis = setAxis;
	dist = setDist;
	gridSize = setGridSize;
	r1 = setR1;
	g1 = setG1;
	b1 = setB1;
	a1 = setA1;
	r2 = setR2;
	g2 = setG2;
	b2 = setB2;
	a2 = setA2;
}
void Plane::cast(CRay& ray, bool isShadow){//This assumes the plane is along the y axis, but should get the axis from the "axis" variable instead
	float dim1dist1;
	float dim1dist2;
	float dim2dist1;
	float dim2dist2;
	float dim3dist1;
	float dim3dist2;
	if(axis==0){
		dim1dist1 = ray.x1;
		dim1dist2 = ray.x2;
		dim2dist1 = ray.y1;
		dim2dist2 = ray.y2;
		dim3dist1 = ray.z1;
		dim3dist2 = ray.z2;
	}
	else if(axis==1){
		dim1dist1 = ray.y1;
		dim1dist2 = ray.y2;
		dim2dist1 = ray.z1;
		dim2dist2 = ray.z2;
		dim3dist1 = ray.x1;
		dim3dist2 = ray.x2;
	}
	else{
		dim1dist1 = ray.z1;
		dim1dist2 = ray.z2;
		dim2dist1 = ray.x1;
		dim2dist2 = ray.x2;
		dim3dist1 = ray.y1;
		dim3dist2 = ray.y2;
	}
	if((dim1dist2<dim1dist1)==(dist<dim1dist1)){
		float planeX = (dim3dist2-dim3dist1)*(dist-dim1dist1)/(dim1dist2-dim1dist1)+dim3dist1;//potential problem area with positivity of "...+ray.x1" at end instead on this line and next line
		float planeY = (dim2dist2-dim2dist1)*(dist-dim1dist1)/(dim1dist2-dim1dist1)+dim2dist1;
		//if(square(ray.x1-testBall.x)+square(ray.y1-testBall.y)+square(ray.z1-testBall.z)<=testBall.radiusSq){ray.setColor(255, 0, 0, 255);}
		if(abs((int)planeX)%(int)(gridSize*2)<gridSize^abs((int)planeY)%(int)(gridSize*2)<gridSize^planeX>0^planeY>0){ 
			if(axis==0){ray.setColor(r1, g1, b1, a1, dist, planeY, planeX, dist3D(dim3dist1, dim1dist1, dim2dist1, dist, planeY, planeX), false);}
			else if(axis==1){ray.setColor(r1, g1, b1, a1, planeX, dist, planeY, dist3D(dim3dist1, dim1dist1, dim2dist1, planeX, dist, planeY), false);}
			else{ray.setColor(r1, g1, b1, a1, planeY, planeX, dist, dist3D(dim3dist1, dim1dist1, dim2dist1, planeY, planeX, dist), false);}
		}
		else{
			if(axis==0){ray.setColor(r2, g2, b2, a2, dist, planeY, planeX, dist3D(dim3dist1, dim1dist1, dim2dist1, dist, planeY, planeX), false);}
			else if(axis==1){ray.setColor(r2, g2, b2, a2, planeX, dist, planeY, dist3D(dim3dist1, dim1dist1, dim2dist1, planeX, dist, planeY), false);}
			else{ray.setColor(r2, g2, b2, a2, planeY, planeX, dist, dist3D(dim3dist1, dim1dist1, dim2dist1, planeY, planeX, dist), false);}
		}
		ray.escape = false;
	}
}


