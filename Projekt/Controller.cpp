/*
 * Controller.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Raphimulator
 */


#include "Controller.h"

Controller::Controller() {
	// TODO Auto-generated constructor stub

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::resetObjectPointers(){
	if (mSceneManager != NULL){
		setRocketPointer( mSceneManager->getPlayerRocket() );
		if(rocketTracer!= NULL){
			rocketTracer->setTargetPointer(mRocketPointer->getPositionPointer());
		}
	}
}

void Controller:: setRocketPointer(RigidRocket* rocket){
	mRocketPointer = rocket;
}

void Controller:: setCameraPointer(Camera* cam){
	mCamera = cam;
}
void Controller:: setTracer(TimedPointTracer* tracer){
	rocketTracer = tracer;
}
void Controller:: setScene(SceneManager* scene){
	mSceneManager = scene;
}

void Controller::keyPressed(int key){
	if(mSceneManager ->getPlayerRocket()->isLaunched())
		launched(key);
	else if(!mSceneManager -> getPlayerRocket()->isLaunched())
		prelaunched(key);


	remaining(key);
}

void Controller:: prelaunched(int key){
	//Raketenstart
	Quaternion rot;
	if (key == ' '){
			if(mRocketPointer != NULL){
				mRocketPointer->launch();
			}
		}
	switch (key) {
	case GLUT_KEY_UP:
		rot = Quaternion(Vector3(1, 0, 0), 1);
		mRocketPointer->rotate(rot);
		break;
	case GLUT_KEY_DOWN:
		rot = Quaternion(Vector3(1, 0, 0), -1);
		mRocketPointer->rotate(rot);
		break;
	case GLUT_KEY_LEFT:
		rot = Quaternion(Vector3(0, 0, 1), 1);
		mRocketPointer->rotate(rot);
		break;
	case GLUT_KEY_RIGHT:
		rot = Quaternion(Vector3(0, 0, 1), -1);
		mRocketPointer->rotate(rot);
		break;
	}
}
void Controller:: launched(int key){}

void Controller:: remaining(int key){
	//Reset
	//HIER IS DER WURM DRIN
	if(key == 'r'){
		mSceneManager -> reset();

		//Lade neue Objekt Pointer, denn die alten sind futsch
		resetObjectPointers();

		if (mCamera != NULL){
			mCamera->setLookAt(Vector3(0,0,0));
			mCamera->setPosition(Vector3(0.0, 5.0, 20.0));
			mCamera->setDirection(Vector3(0.0, 0.0, -1.0));
		}
	}
	//Integration
	if(key == 'm'){
		mSceneManager->getPlayerRocket()->getRigidBodyPointer()->switchIntegrationMode();
	}

	//Raketenspur
	if(key == 't'){
		if (rocketTracer != NULL){
			rocketTracer->switchMode();
		}
	}
	//Raketenspur Leeren
		if(key == 'T'){
			if (rocketTracer != NULL){
				rocketTracer->reset();
			}
		}

	//Camera at Rakete
	if(key == 'c'){
		if (mCamera != NULL){
			Vector3* rocketPos = mSceneManager ->getPlayerRocket()->getPositionPointer();
			mCamera->setLookAtPointer(rocketPos);
		}
	}

	//Camera at Origin
	if(key == 'b'){
		if (mCamera != NULL){
			mCamera->setLookAt(Vector3(0,0,0));
			mCamera->setPosition(Vector3(0.0, 5.0, 20.0));
			mCamera->setDirection(Vector3(0.0, 0.0, -1.0));
		}
	}
}
