#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"

//Class for 3 X 3 Matrices


class Matrix3
{
public:

	
	//---------------CONSTRUCTORS & DESTRUCTOR-----------------
	public:
		Matrix3(); //Standardm�ssig wird die Identit�tsmatrix initialisiert
		Matrix3(float a11, float a12, float a13,
				float a21, float a22, float a23,
				float a31, float a32, float a33);
			//Initialisiert die Matrix mit den gegebenen Elementen

		//virtual ~Matrix3(); // [Destruktor von abgeleiteter Klasse und ~Matrix3() aufgerufen]
		~Matrix3();

	//--------------MEMBER VARIABLES----------------------------
	private:
		float matrix_data[3][3]; //Enth�lt die Matrix


	//--------------OPERATORS------------------------------------
	public:
		Matrix3 operator*(const Matrix3 &matrix_operand) const; 
		Matrix3 operator*(const float &float_operand) const;  
		friend Matrix3 operator*(const float &float_operand, const Matrix3 &matrix_operand);
		Matrix3& operator*=(const float &float_operand); 
		Matrix3 operator+(const Matrix3 &matrix_operand) const;  
		Matrix3& operator+=(const Matrix3 &matrix_operand);
		Matrix3 operator-(const Matrix3 &matrix_operand) const; 
		Matrix3& operator-=(const Matrix3 &matrix_operand);
		float& operator()(const int &zeile, const int &spalte);
			
	//---------------MEMBER FUNCTIONS----------------------------------
		void setIdentity(); 
		void setAllMatrixValues(float a11, float a12, float a13,
								float a21, float a22, float a23,
								float a31, float a32, float a33);
			//setzt alle Matrixwerte neu
		void transpose(void); //Transponiert die Matrix [Durch R�ckgabe z.B.
		
		void invert(void); //invertiert die Matrix

		void debugPrintToCerr(void) const; //Nur f�r debug zwecke, gibt die matrix an cerr aus
};


#endif
