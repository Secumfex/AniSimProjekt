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
#include "Forces.h"
#include <vector>
#include "SceneConstraints.h"


using namespace std;

class CollisionManager {


//---------------CONSTRUCTORS & DESTRUCTOR------------
public:
	CollisionManager();
	virtual ~CollisionManager();

//---------------MEMBER VARIABLES------------
private:
	SceneManager* mSceneManager;
	vector<Force* > mCollisionForces;

	//Ab welcher Nähe wird eine Kraft registriert
	float mMassPointEpsilon;

	RigidRocket* mRocket;

//---------------MEMBER FUNCTIONS------------
public:
	void update(float d_t);

	//Teste alles gegen Alles
	void collisionCheckAll();

	//Teste SimualtionObject gegen SimulationObject
	void collisionCheckObjectAgainstObject(SimulationObject* lhs, SimulationObject* rhs);
	//Teste SimulationObject gegen Waend
	void collisionCheckObjectAgainstSceneWalls(SimulationObject* object);
	void collisionCheckPhysicsAgainstSceneWalls(Physics* physics);
	//Teste Massepunkt gegen Massepunkt
	void collisionCheckPhysicsAgainstPhysics(Physics* lhs, Physics* rhs);
	//Wende alle Gefundenen CollisionsKräfte an und leere den KraftVektor
	void applyAndEmptyCollisionForces(float d_t);


	void setScene(SceneManager* scene);
	bool  isCollision ();
	void collisionCheck(float d_t);
	void collisionHoleCheck(float d_t);
	void collisionWallCheck(float d_t);

};



#endif /* COLLISIONMANAGER_H_ */
