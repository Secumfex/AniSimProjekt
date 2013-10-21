/*
 * Controller.h
 *
 *  Created on: 17.10.2013
 *      Author: Raphimulator
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_


#include "SimulationObjects.h"
#include "GL/glut.h"
#include "Camera.h"
#include "UtilityClasses.h"
#include "SceneManager.h"

using namespace std;

class Controller {
	//---------------CONSTRUCTORS & DESTRUCTOR------------
public:
	Controller();
	virtual ~Controller();
	//---------------MEMBER VARIABLES------------
private:
	RigidRocket* mRocketPointer;
	Camera* mCamera;
	TimedPointTracer* rocketTracer;
	SceneManager* mSceneManager;
	//---------------MEMBER FUNCTIONS------------
	public:
		void resetObjectPointers();

		void setRocketPointer(RigidRocket* rocket);
		void setCameraPointer(Camera* cam);
		void setTracer(TimedPointTracer* tracer);
		void setScene(SceneManager* scene);

		void keyPressed(int key);

		void prelaunched(int key);
		void launched(int key);
		void remaining(int key);
};



 /* namespace std */

#endif /* CONTROLLER_H_ */
