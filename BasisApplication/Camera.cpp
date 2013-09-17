#include "Camera.h"
#include "GL/glut.h"

Camera::Camera(void)
{
	mPosition = Vector3(0.0, 5.0, 5.0);
	mDirection = Vector3(0.0, 0.0, -1.0);
	mLookAt = mPosition + mDirection;
	mUpVector = Vector3(0.0, 1.0, 0.0);
}


Camera::Camera(Vector3 pos, Vector3 lookAt){
	mPosition = pos;
	mLookAt = lookAt;
	mDirection = (mLookAt - mPosition);
	mDirection.normalize();
	mUpVector = Vector3(0.0, 1.0, 0.0);
}


Camera::~Camera(void)
{
}


void Camera::look(void){
	
	gluLookAt(mPosition.getX(), mPosition.getY(), mPosition.getZ(),
			  mLookAt.getX(), mLookAt.getY(), mLookAt.getZ(),
			  mUpVector.getX(), mUpVector.getY(), mUpVector.getZ());
}