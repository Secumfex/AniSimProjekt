/*
 * CollisionManager.h
 *
 *  Created on: 21.10.2013
 *      Author: Raphimulator
 */

#ifndef COLLISIONMANAGER_H_
#define COLLISIONMANAGER_H_

#include "SceneManager.h"
#include "SimulationObjects.h"
#include "GL/glut.h"
#include <String.h>

using namespace std;

class CollisionManager {


//---------------CONSTRUCTORS & DESTRUCTOR------------
public:
	CollisionManager();
	virtual ~CollisionManager();

//---------------MEMBER VARIABLES------------
private:
	SceneManager* mSceneManager;
	RigidRocket* mRocket;
	int banana = 999;

//---------------MEMBER FUNCTIONS------------
public:
	void setScene(SceneManager* scene);
	bool  isCollision ();
	void collisionCheck();

};



#endif /* COLLISIONMANAGER_H_ */
