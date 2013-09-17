#include "Matrix3.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

//using namespace std;


Matrix3::Matrix3(void)
{
	setIdentity();
}

Matrix3::Matrix3(float a11, float a12, float a13,
				 float a21, float a22, float a23,
				 float a31, float a32, float a33)
{
	//row 1
	matrix_data[0][0] = a11;
	matrix_data[0][1] = a12;
	matrix_data[0][2] = a13;

	//row 2
	matrix_data[1][0] = a21;
	matrix_data[1][1] = a22;
	matrix_data[1][2] = a23;

	//row 3
	matrix_data[2][0] = a31;
	matrix_data[2][1] = a32;
	matrix_data[2][2] = a33;
}

Matrix3::~Matrix3(void)
{
}

Matrix3 Matrix3::operator*(const Matrix3 &matrix_operand) const
{
	Matrix3 temp_matrix;

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			temp_matrix.matrix_data[i][j] =
					matrix_data[i][0]*matrix_operand.matrix_data[0][j]
				+	matrix_data[i][1]*matrix_operand.matrix_data[1][j]
				+	matrix_data[i][2]*matrix_operand.matrix_data[2][j];
		}

	return(temp_matrix);
}

Matrix3 Matrix3::operator*(const float &float_operand) const
{
	Matrix3 temp_matrix;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			temp_matrix.matrix_data[i][j] = matrix_data[i][j]*float_operand;
		}

	return(temp_matrix);
}

Matrix3 operator*(const float &float_operand, const Matrix3 &matrix_operand)
{

	Matrix3 temp_matrix;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			temp_matrix.matrix_data[i][j] = matrix_operand.matrix_data[i][j]*float_operand;
		}

	return(temp_matrix);
}

Matrix3& Matrix3::operator*=(const float &float_operand)
{
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			matrix_data[i][j] = matrix_data[i][j]*float_operand;
		}

	return(*this);
}

Matrix3 Matrix3::operator+(const Matrix3 &matrix_operand) const //Addition
{
	Matrix3 temp_matrix;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			temp_matrix.matrix_data[i][j] = matrix_operand.matrix_data[i][j]
											+ matrix_data[i][j];
		}

	return(temp_matrix);
}

Matrix3& Matrix3::operator+=(const Matrix3 &matrix_operand)
{
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			matrix_data[i][j] = matrix_operand.matrix_data[i][j]
											+ matrix_data[i][j];
		}

	return(*this);
}

Matrix3 Matrix3::operator-(const Matrix3 &matrix_operand) const //Subtraktion
{
	Matrix3 temp_matrix;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			temp_matrix.matrix_data[i][j] = matrix_data[i][j]
											- matrix_operand.matrix_data[i][j];
		}

	return(temp_matrix);
}

Matrix3& Matrix3::operator-=(const Matrix3 &matrix_operand)
{
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			matrix_data[i][j] = matrix_data[i][j]
								- matrix_operand.matrix_data[i][j];
		}

	return(*this);
}

float& Matrix3::operator()(const int &zeile, const int &spalte)
{
	if(zeile>3 || spalte>3 || zeile<1 || spalte<1) //Befindet sich der index in der matrix ?
		cout<<"ERROR: Matrix Index out of Bounds"<<endl;
	else{
		return(matrix_data[zeile-1][spalte-1]);}
}

void Matrix3::setIdentity() //setzt die Matrix auf die Identit�t
{
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			if(j==i)
				matrix_data[i][j] = 1.0;
			else
				matrix_data[i][j] = 0.0;
		}
}

void Matrix3::setAllMatrixValues(float a11, float a12, float a13,
								 float a21, float a22, float a23,
								 float a31, float a32, float a33)
{
	matrix_data[0][0] = a11;
	matrix_data[0][1] = a12;
	matrix_data[0][2] = a13;

	matrix_data[1][0] = a21;
	matrix_data[1][1] = a22;
	matrix_data[1][2] = a23;

	matrix_data[2][0] = a31;
	matrix_data[2][1] = a32;
	matrix_data[2][2] = a33;

}

void Matrix3::transpose(void) //Transponiert die Matrix
{
	Matrix3 temp_matrix = *this;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			matrix_data[j][i] = temp_matrix.matrix_data[i][j];
		}

}

void Matrix3::invert(void) //invertiert die Matrix
{
    // Entsprechender Maple-Code
    // > A:=matrix(4,4,[a(1,1),a(1,2),a(1,3),a(1,4),a(2,1),a(2,2),a(2,3),a(2,4),a(3,1),
	//   a(3,2),a(3,3),a(3,4),a(4,1),a(4,2),a(4,3),a(4,4)]);
    // > det(A);
    // > adjoint(A);

	Matrix3 a= *this;
	float det; //Determinante

	//calculation of determinante for 3X3 matrices by the rule of the great Sarrus
	det = a(1,1)*a(2,2)*a(3,3) +
		  a(1,2)*a(2,3)*a(3,1) +
		  a(1,3)*a(2,1)*a(3,2) -
		  a(1,3)*a(2,2)*a(3,1) -
		  a(1,1)*a(2,3)*a(3,2) -
		  a(1,2)*a(2,1)*a(3,3);
	

	if(det != 0.0) //there exists a invers matrix
	{
		//creating the adjunct matrix of a
		Matrix3 adj(a(2,2)*a(3,3)-a(2,3)*a(3,2),
					-1*(a(1,2)*a(3,3)-a(1,3)*a(3,2)),
					a(1,2)*a(2,3)-a(1,3)*a(2,2),
					-1*(a(2,1)*a(3,3)-a(2,3)*a(3,1)),
					a(1,1)*a(3,3)-a(1,3)*a(3,1),
					-1*(a(1,1)*a(2,3)-a(1,3)*a(2,1)),
					a(2,1)*a(3,2)-a(2,2)*a(3,1),
					-1*(a(1,1)*a(3,2)-a(1,2)*a(2,2)),
					a(1,1)*a(2,2)-a(1,2)*a(2,1));

		(*this) = (1/det)*adj;

	}
	else
		cout<<"ERROR: Matrix could not be iverted, det == 0"<<endl;

}

void Matrix3::debugPrintToCerr(void) const //Nur f�r debug zwecke, gibt die matrix an cerr aus
{
	cerr << "\n("	<< matrix_data[0][0] << ","
					<< matrix_data[0][1] << ","
					<< matrix_data[0][2] << ")\n("

					<< matrix_data[1][0] << ","
					<< matrix_data[1][1] << ","
					<< matrix_data[1][2] << ")\n("

					<< matrix_data[2][0] << ","
					<< matrix_data[2][1] << ","
					<< matrix_data[2][2] << ")\n";

}
