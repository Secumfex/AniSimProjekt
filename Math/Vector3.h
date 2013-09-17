#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

//#include "Quaternion.h"
#include "Quaternion.h"




class Quaternion;
class Matrix3;

//Class for a Vector with 3 Elements

class Vector3
{
	//---------------CONSTRUCTORS & DESTRUCTOR-------------------------------------------
	public:
		Vector3(float arg_x=0, float arg_y=0, float arg_z=0);
		~Vector3(void);

		friend class Matrix3; //Damit operatoren der Matrizen auf Vektoren zugreifen können

	//---------------MEMBER VARIABLES-------------------------------------------------------
	protected:
		float x, y, z;  //the three Elements of the Vector

	//--------------MEMBER FUNCTIONS----------------------------------------------------------

	public:
		//Vektoraddition
		Vector3 operator+(const Vector3 &operand_vector) const; //Vektoraddition
		Vector3& operator+=(const Vector3 &operand_vector);
		Vector3 operator-(const Vector3 &operand_vector) const;
		Vector3& operator-=(const Vector3 &operand_vector);
		Vector3& operator=(const Vector3 &operand_vector);
		float operator*(const Vector3 &operand_vector) const; //Skalarprodukt
		Vector3 operator*(const float &scale_factor) const;//Multiplikation von scalar von rechts
		bool operator == (const Vector3 operand);
		bool operator != (const Vector3 operand);
		friend Vector3 operator*(const float &scale_factor, const Vector3 &operand_vector);
		 //Dasselbe von links
		Vector3& operator*=(const float &scale_factor);
		friend Vector3 operator*(Matrix3 &operand_matrix, const Vector3 &operand_vector);

		 //Multiplikation von Matrix mit Vector
		Vector3 cross(const Vector3 &operand_vector);
		 //Berechnet das Kreuzprodukt aktueller_vektor x operand_vector,
		 //das Ergebnis liegt im aktuellen Vektor
		static Vector3 crossProduct(const Vector3 &operand1, const Vector3 &operand2);
		 //Berechnet das Kreuzprodukt zweier Vektoren und liefert das Ergebnis als Vektor
		 //zurück [static]

		//gets & sets
		inline float getX(void) const {return x;}
		inline float getY(void) const {return y;}
		inline float getZ(void) const {return z;}
		inline void setX(const float arg_x=0){x = arg_x;}
		inline void setY(const float arg_y=0){y = arg_y;}
		inline void setZ(const float arg_z=0){z = arg_z;}

	#ifdef _DEBUG
		void normalize(void); //Normalisiert den Vektor auf die Länge 1
	#else
		inline void normalize(void){
			float vector_length = length();
			x/=vector_length;
			y/=vector_length;
			z/=vector_length;
		}
	#endif

		float length(void) const; //Liefert die Länge des Vektors




		/**rotates the vector around the given axis through the origin about the given angle in degrees
		 uses quaternion rotation
		 v'_rot = r * v' * r~, where v' is the vector as a quaternion, r ist the rotation quaternion,
		 r~ the conjungate complex of the quaternion and v'rot is the rotated vector as a quaternion
		returns the rotated vector*/
		Vector3 rotate(Vector3 axis, float angle);

		//rotates the vector with respect to the given quaternion
		Vector3 rotate(Quaternion r);

		//DEBUG FUNKTIONEN
		void debugPrintToCerr(void) const; //Nur für debug zwecke, gibt den vektor an cerr aus
		void debugPrintToCout(void) const; //Nur für debug zwecke, gibt den vektor an cout aus

};





#endif
