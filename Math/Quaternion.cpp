#include "Quaternion.h"


//implementation of the Quaternion class

//------------CONSTRUCTORS-------------
Quaternion::Quaternion(void)
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Quaternion::Quaternion(float wi, float xi, float yi, float zi){
	w = wi;
	x = xi;
	y = yi;
	z = zi;
}

Quaternion::Quaternion(float wi, Vector3 u){
	w = wi;
	x = u.getX();
	y = u.getY();
	z = u.getZ();
}


Quaternion::Quaternion(Vector3 axis, float angle){
	(*this) = getRotationQuaternion(axis, angle);

}


Quaternion::Quaternion(Matrix3 r){
	
	float trace = r(1,1) + r(2,2) + r(3,3); 
	if( trace > 0 ) {
		float s = 0.5f / sqrtf(trace+ 1.0f);
		w = 0.25f / s;
		x = ( r(3,2) - r(2,3) ) * s;
		y = ( r(1,3) - r(3,1) ) * s;
		z = ( r(2,1) - r(1,2) ) * s;
	} 
	else {
		if ( r(1,1) > r(2,2) && r(1,1) > r(3,3) ) {
			float s = 2.0f * sqrtf( 1.0f + r(1,1) - r(2,2) - r(3,3));
			w = (r(3,2) - r(2,3)) / s;
			x = 0.25f * s;
			y = (r(1,2) + r(2,1) ) / s;
			z = (r(1,3) - r(3,1) ) / s;
		} 
		else if (r(2,2) > r(3,3)) {
			float s = 2.0f * sqrtf( 1.0f + r(2,2) - r(1,1) - r(3,3));
			w = (r(1,3) - r(3,1) ) / s;
			x = (r(1,2) + r(2,1) ) / s;
			y = 0.25f * s;
			z = (r(2,3) + r(3,2) ) / s;
		} 
		else {
			float s = 2.0f * sqrtf( 1.0f + r(3,3) - r(1,1) - r(2,2) );
			w = (r(2,1) - r(1,2) ) / s;
			x = (r(1,3) + r(3,1) ) / s;
			y = (r(2,3) + r(3,1) ) / s;
			z = 0.25f * s;
		}
	}

}


Quaternion::Quaternion(const Quaternion& q){
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}


//default destructor
Quaternion::~Quaternion(void)
{
}

//-----------------OPERATORS---------------------

Quaternion Quaternion::operator = (const Quaternion& q){
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;

	return (*this);
}

Quaternion Quaternion::operator + (const Quaternion& q){
	return Quaternion(w+q.w, x+q.x, y+q.y, z+q.z);
}

Quaternion Quaternion::operator - (const Quaternion& q){
	return Quaternion(w-q.w, x-q.x, y-q.y, z-q.z);
}

Quaternion Quaternion::operator * (const Quaternion& q){
	return Quaternion(
		w*q.w - x*q.x - y*q.y - z*q.z,
		w*q.x + x*q.w + y*q.z - z*q.y,
		w*q.y + y*q.w + z*q.x - x*q.z,
		w*q.z + z*q.w + x*q.y - y*q.x);
}

Quaternion Quaternion::operator * (const float skalar){

	return Quaternion(skalar*w, skalar*x, skalar*y, skalar*z);
}

Quaternion Quaternion::operator / (Quaternion& q){
	return ((*this) * (q.inverse()));
}

Quaternion& Quaternion::operator += (const Quaternion& q){
		w += q.w;
		x += q.x;
		y += q.y;
		z += q.z;

		return (*this);
}

Quaternion& Quaternion::operator -= (const Quaternion& q){
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;

	return (*this);
}

Quaternion& Quaternion::operator *= (const Quaternion& q){
	float w_val = w*q.w - x*q.x - y*q.y - z*q.z;
	float x_val = w*q.x + x*q.w + y*q.z - z*q.y;
	float y_val = w*q.y + y*q.w + z*q.x - x*q.z;
	float z_val = w*q.z + z*q.w + x*q.y - y*q.x;

	w = w_val;
	x = x_val;
	y = y_val;
	z = z_val;

	return (*this);
}

Quaternion& Quaternion::operator /= (Quaternion& q){
	(*this) = (*this)*q.inverse();
	return (*this);
}

bool Quaternion::operator != (const Quaternion& q){
	return (w!=q.w || x!=q.x || y!=q.y || z!=q.z) ? true : false;
}

bool Quaternion::operator == (const Quaternion& q){
	return (w==q.w && x==q.x && y==q.y && z==q.z) ? true : false;
}

//-----------------MEMBER FUNCTIONS----------------------

float Quaternion::norm(){
	return (w*w + x*x + y*y + z*z);
}

float Quaternion::magnitude(){
	return sqrt(norm());
}

Quaternion Quaternion::scale(float s){
	return Quaternion(w*s, x*s, y*s, z*s);
}

Quaternion Quaternion::inverse(){
	return conjugate().scale(1/norm());
}

Quaternion Quaternion::conjugate(){
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::unitQuaternion(){
	return (*this).scale(1/(*this).magnitude());
}

Quaternion Quaternion::getRotationQuaternion(Vector3 axis, float angle){
	//converting the angle to radians
	//radian = degrees * PI/180
	//made two optimizations:
	//first i precomputed PI/180 with my pocket calculator (10 digits) for speeding
	// (one division is saved), that gives me 0.017453283
	//then i realized that i always need angle/2 for quaternion calculation, so the multiplication
	//by 0.5 is invariant and i can also precompute this by my pocked calculator,
	//so 0.017453283 * 0.5 = 0.008726646 (another division is saved)
	float r_angle_half = angle*0.008726646f;

	axis.normalize();
	return Quaternion(cos(r_angle_half),
							sin(r_angle_half)*axis.getX(),
							sin(r_angle_half)*axis.getY(),
							sin(r_angle_half)*axis.getZ());
}


///computes a 3X3 rotation matrix corrosponding to the quaternion
Matrix3 Quaternion::computeRotationMatrix(void){

	//doing some precalculations for speeding purposes
	float z_sqr_2 = (*this).z * (*this).z * 2;
	float y_sqr_2 = (*this).y * (*this).y * 2;
	float x_sqr_2 = (*this).x * (*this).x * 2;
	float xy_2 = (*this).x * (*this).y * 2;
	float xz_2 = (*this).x * (*this).z * 2;
	float yz_2 = (*this).y * (*this).z * 2;
	float wx_2 = (*this).w * (*this).x * 2;
	float wy_2 = (*this).w * (*this).y * 2;
	float wz_2 = (*this).w * (*this).z * 2;


	return Matrix3(1 - y_sqr_2 - z_sqr_2, xy_2 - wz_2, xz_2 + wy_2,
				   xy_2 + wz_2, 1 - x_sqr_2 - z_sqr_2, yz_2 - wx_2,
				   xz_2 - wy_2, yz_2 + wx_2, 1 - x_sqr_2 - y_sqr_2);


	
}



Vector3 Quaternion::getAxis(void){
	if(fabs(w) == 1.0){ //axis not defined (zero rotation angle)
		return Vector3(0.0, 1.0, 0.0);
	}
	else{
		return (1.0f / sqrt(1.0f - w * w)) * Vector3(x, y, z);
	}
}


float Quaternion::getAngle(void){
	return 2.0f * (acos(w) * 57.295779513082320876798154814105f); //convert acos(w) from radians to degree by multiplying with PI / 180
}


void Quaternion::debugPrintToCerr(void){
	cerr<<"("<<w<<","<<x<<","<<y<<","<<z<<")\n";
}





