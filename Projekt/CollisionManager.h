/*
 * CollisionManager.h
 *
 *  Created on: 21.10.2013
 *      Author: Raphimulator
 */

#ifndef COLLISIONMANAGER_H_
#define COLLISIONMANAGER_H_

#include <windows.h>
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
	vector<Forces* > mCollisionForces;

	RigidRocket* mRocket;
	Vector3 collisionPoint;
	float collisiontime;

//---------------MEMBER FUNCTIONS------------
public:
	void update(float d_t);

	//Teste alles gegen Alles
	void collisionCheckAll();

	//Teste
	bool testObjectToObject();


	//Wende alle Gefundenen CollisionsKräfte an und leere den KraftVektor
	void applyAndEmptyCollisionForces();


	void setScene(SceneManager* scene);
	bool  isCollision ();
	void collisionCheck(float d_t);
	void explosion (float d_t);

};



#endif /* COLLISIONMANAGER_H_ */
