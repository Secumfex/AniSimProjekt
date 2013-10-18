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
	}
}

void Controller:: setRocketPointer(Rocket* rocket){
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
	if (key == ' '){
			if(mRocketPointer != NULL){
				mRocketPointer->launch();
			}
		}
	switch (key) {
		case GLUT_KEY_UP :
			mRocketPointer -> setDirection(mRocketPointer -> getDirection().rotate(Vector3(1,0,0),1));
				break;
		case GLUT_KEY_DOWN :
			mRocketPointer -> setDirection(mRocketPointer -> getDirection().rotate(Vector3(1,0,0),-1));
				break;
		case GLUT_KEY_LEFT :
			mRocketPointer -> setDirection(mRocketPointer -> getDirection().rotate(Vector3(0,0,1),1));
				break;
		case GLUT_KEY_RIGHT :
			mRocketPointer -> setDirection(mRocketPointer -> getDirection().rotate(Vector3(0,0,1),-1));
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

		if(rocketTracer!= NULL){
			rocketTracer = NULL;
		}

		if (mCamera != NULL){
			mCamera->setLookAt(Vector3(0,0,0));
			mCamera->setPosition(Vector3(0.0, 5.0, 5.0));
			mCamera->setDirection(Vector3(0.0, 0.0, -1.0));
		}

	}
	//Integration
	if(key == 'm'){
		mSceneManager->getPlayerRocket()->getPhysics()->switchIntegrationMode();
	}

	//Raketenspur
	if(key == 't'){
		rocketTracer = new TimedPointTracer(mSceneManager -> getPlayerRocket()->getPhysics()->getPositionPointer(),1000);
	}

	//Camera at Rakete
	if(key == 'c'){
		if (mCamera != NULL){
			Vector3* rocketPos = mSceneManager ->getPlayerRocket()->getPhysics()->getPositionPointer();
			mCamera->setLookAtPointer(rocketPos);
		}
	}

	//Camera at Origin
	if(key == 'b'){
		if (mCamera != NULL){
			mCamera->setLookAt(Vector3(0,0,0));
			mCamera->setPosition(Vector3(0.0, 5.0, 5.0));
			mCamera->setDirection(Vector3(0.0, 0.0, -1.0));
		}
	}
}
