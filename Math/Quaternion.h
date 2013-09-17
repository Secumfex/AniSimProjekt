#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"
#include "Matrix3.h"

#include <iostream>
#include <math.h>

using namespace std;

class Vector3;
class Matrix3;

//class which represents a Quaternion (w,x,y,z)
//w,x,y,z are floating point values with float precision


class Quaternion
{
	public:
		//---CONSTRUCTORS & DESTRUCTOR----
		//Quaternion
		// -default constructor
		// -creates a new quaternion with all parts equal to zero
		Quaternion(void);

		//Quaternion
		// -constructor
		// -parametes : w, x, y, z elements of the quaternion
		// -creates a new quaternion based on the elements passed in
		Quaternion(float wi, float xi, float yi, float zi);

		//Quaternion
		// -constructor
		// -parametes : w, Vector3 u
		// -creates a new quaternion based on the elements passed in and
		//  generates the values for x,y,z from u
		Quaternion(float wi, Vector3 u);

		//Quaternion
		// -constructor
		// -parameters : axis, angle
		// -creates a new rotation quaternion based on axis and angle
		Quaternion(Vector3 axis, float angle);

		Quaternion(Matrix3 r);

		//Quaternion
		// -copy constructor
		// -parameters : const quaternion q
		// -creates a new quaternion based on the quaternion passed in
		Quaternion(const Quaternion& q);

		//default destructor
		~Quaternion(void);


	private:
		float w, x, y, z;



	public:
		//-------OPERATORS---------------------
		Quaternion operator = (const Quaternion& q);
		Quaternion operator + (const Quaternion& q);
		Quaternion operator - (const Quaternion& q);
		Quaternion operator * (const Quaternion& q);
		Quaternion operator * (const float skalar);
		Quaternion operator / (Quaternion& q);
		Quaternion& operator += (const Quaternion& q);
		Quaternion& operator -= (const Quaternion& q);
		Quaternion& operator *= (const Quaternion& q);
		Quaternion& operator /= (Quaternion& q);
		bool operator != (const Quaternion& q);
		bool operator == (const Quaternion& q);

		//-------MEMBER FUNCTIONS-----------------
		float norm();
		float magnitude();
		Quaternion scale(float s);
		Quaternion inverse();
		Quaternion conjugate();
		Quaternion unitQuaternion();
		Quaternion getRotationQuaternion(Vector3 axis, float angle);
		Matrix3 computeRotationMatrix(void);
		void debugPrintToCerr(void);

		
		inline float getW(void){return w;}
		inline float getX(void){return x;}
		inline float getY(void){return y;}
		inline float getZ(void){return z;}

		inline void setW(const float arg){w = arg;}
		inline void setX(const float arg){x = arg;}
		inline void setY(const float arg){y = arg;}
		inline void setZ(const float arg){z = arg;}

		Vector3 getAxis(void); ///returns normalized rotation axis
		float getAngle(void); ///returns angle in degrees
	

};


#endif
