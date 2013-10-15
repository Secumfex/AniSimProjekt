#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
	//---------------CONSTRUCTORS & DESTRUCTOR-----------------
	public:
		Camera(void);
		Camera(Vector3 pos, Vector3 lookAt);
		~Camera(void);

	//---------------MEMBER VARIABLES-----------------
	private:
		Vector3 mPosition;
		Vector3 mDirection;
		Vector3* mLookAt;
		Vector3 mUpVector;

	//---------------MEMBER FUNCTIONS-----------------
	public:

		inline void setPosition(Vector3 p){
			mPosition = p;
			mDirection = (-1.0) * mPosition;
			mDirection.normalize();
		}
		inline void setDirection(Vector3 d){
			mDirection = d;
			mDirection.normalize();
		}

		inline void setLookAt(Vector3 l){
			mLookAt = new Vector3(l.getX(), l.getY(), l.getZ());
		}
		inline void setLookAtPointer(Vector3* l){
			mLookAt = l;
		}
		inline Vector3 getPosition(void){return mPosition;}
		inline Vector3 getDirection(void){return mDirection;}
		inline Vector3 getLookAt(void){return *mLookAt;}
		

		void look(void);

};
#endif
