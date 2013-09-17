#include "Vector3.h"
#include "Matrix3.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;


Vector3::Vector3(float arg_x, float arg_y, float arg_z)
{
	x = arg_x;
	y = arg_y;
	z = arg_z;

	
}

Vector3::~Vector3(void)
{
}


Vector3 Vector3::operator+(const Vector3 &operand_vector) const //Vektoraddition
{
	Vector3 temp_vector;

	temp_vector.x = x + operand_vector.x;
	temp_vector.y = y + operand_vector.y;
	temp_vector.z = z + operand_vector.z;

	return temp_vector;
}


Vector3& Vector3::operator+=(const Vector3 &operand_vector)
{
	x += operand_vector.x;
	y += operand_vector.y;
	z += operand_vector.z;

	return(*this);
}


Vector3 Vector3::operator-(const Vector3 &operand_vector) const
{
	Vector3 temp_vector;
	temp_vector.x = x - operand_vector.x;
	temp_vector.y = y - operand_vector.y;
	temp_vector.z = z - operand_vector.z;

	return temp_vector;
}

Vector3& Vector3::operator-=(const Vector3 &operand_vector)
{
	x -= operand_vector.x;
	y -= operand_vector.y;
	z -= operand_vector.z;

	return(*this);
}


Vector3& Vector3::operator=(const Vector3 &operand_vector){
	x = operand_vector.x;
	y = operand_vector.y;
	z = operand_vector.z;

	return(*this);
}

float Vector3::operator*(const Vector3 &operand_vector) const //Dot Product
{
	float temp_scalar;

	temp_scalar =	x*operand_vector.x +
					y*operand_vector.y +
					z*operand_vector.z;
	return temp_scalar;
}

Vector3 Vector3::operator*(const float &scale_factor) const
{
	Vector3 temp_vector;
	temp_vector.x = x*scale_factor;
	temp_vector.y = y*scale_factor;
	temp_vector.z = z*scale_factor;

	return(temp_vector);
}

Vector3 operator*(const float &scale_factor, const Vector3 &operand_vector)
{

	Vector3 temp_vector;
	temp_vector.setX(operand_vector.getX()*scale_factor);
	temp_vector.setY(operand_vector.getY()*scale_factor);
	temp_vector.setZ(operand_vector.getZ()*scale_factor);

	return(temp_vector);
}

Vector3& Vector3::operator*=(const float &scale_factor)
{
	x = x*scale_factor;
	y = y*scale_factor;
	z = z*scale_factor;

	return(*this);
}



Vector3 operator*(Matrix3 &operand_matrix, const Vector3 &operand_vector)
{

	Vector3 temp_vector;
	temp_vector.setX(operand_matrix(1,1)*operand_vector.getX() +
					 operand_matrix(1,2)*operand_vector.getY() +
					 operand_matrix(1,3)*operand_vector.getZ());

	temp_vector.setY(operand_matrix(2,1)*operand_vector.getX() +
					 operand_matrix(2,2)*operand_vector.getY() +
					 operand_matrix(2,3)*operand_vector.getZ());

	temp_vector.setZ(operand_matrix(3,1)*operand_vector.getX() +
					 operand_matrix(3,2)*operand_vector.getY() +
					 operand_matrix(3,3)*operand_vector.getZ());

	return temp_vector;
}

bool Vector3::operator !=(const Vector3 operand){
    return ((*this).x != operand.getX() &&
            (*this).y != operand.getY() &&
            (*this).z != operand.getZ());
}

bool Vector3::operator ==(const Vector3 operand){
    return ((*this).x == operand.getX() &&
            (*this).y == operand.getY() &&
            (*this).z == operand.getZ());
}


Vector3 Vector3::cross(const Vector3 &operand_vector)
{
	Vector3 temp_vector;
	temp_vector.x = (y*operand_vector.z)-
					(z*operand_vector.y);
	temp_vector.y = (z*operand_vector.x)-
					(x*operand_vector.z);
	temp_vector.z = (x*operand_vector.y)-
					(y*operand_vector.x);


	return temp_vector;
}

#ifdef _DEBUG
void Vector3::normalize(void) //Normalisiert den Vektor auf die Länge 1
{

	float vector_length = length();
	if(vector_length != 0.0000)
	{
		x/=vector_length;
		y/=vector_length;
		z/=vector_length;
	}
	else{
		//throw NormalizedZeroVector();
		cout<<"ERROR: Vector has length 0, no normalization possible!!"<<endl;
	}
}
#endif

float Vector3::length(void) const //Liefert die Länge des Vektors
{
	return(sqrt(x*x+y*y+z*z));
}


Vector3 Vector3::rotate(Vector3 axis, float angle){

	Quaternion r(axis, angle); //sets a rotation quaternion via constructor
	
	return (*this).rotate(r);
}

Vector3 Vector3::rotate(Quaternion r){
	Quaternion r_conj = r.conjugate();
	Quaternion p(0.0, (*this).x, (*this).y, (*this).z);
	Quaternion p_rot = r * p * r_conj;

	return Vector3(p_rot.getX(), p_rot.getY(), p_rot.getZ());
}


void Vector3::debugPrintToCerr(void) const
{
	cerr	<< "(" << x << ","
			<< y << ","
			<< z << ")\n";

}

void Vector3::debugPrintToCout(void) const
{
	cout	<< "(" << x << ","
			<< y << ","
			<< z << ")\n";

}
