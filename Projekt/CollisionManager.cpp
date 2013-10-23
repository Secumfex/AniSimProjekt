/*
 * CollisionManager.cpp
 *
 *  Created on: 21.10.2013
 *      Author: Raphimulator
 */

#include "CollisionManager.h"

using namespace std;

CollisionManager::CollisionManager() {
	mMassPointEpsilon = 0.20;
}

CollisionManager::~CollisionManager() {
	// TODO Auto-generated destructor stub
}
void CollisionManager:: setScene(SceneManager* scene){
	mSceneManager = scene;
	mRocket = mSceneManager->getPlayerRocket();
}

bool CollisionManager:: isCollision (){
	return mRocket -> isCrashed();
}


void CollisionManager:: collisionCheck(float d_t){
	collisionHoleCheck(d_t);
	collisionWallCheck(d_t);
}

void CollisionManager:: collisionHoleCheck(float d_t){

	vector<SimulationObject* > temp = mSceneManager->getSimulationObjects();

	for (unsigned int i = 0; i < temp.size(); i++){
		if(temp[i] != mRocket){

		//Normales Minus hat bei Vektor3 Mist gebaut... deshalb der ugly code
			Vector3 diff_head = Vector3(	temp[i]->getPositionPointer()->getX() - mRocket-> getHeadPosition().getX(),
											temp[i]->getPositionPointer()->getY() - mRocket-> getHeadPosition().getY(),
											temp[i]->getPositionPointer()->getZ() - mRocket-> getHeadPosition().getZ());

			Vector3 diff_tail = Vector3(	temp[i]->getPositionPointer()->getX() - mRocket-> getTailPosition().getX(),
											temp[i]->getPositionPointer()->getY() - mRocket-> getTailPosition().getY(),
											temp[i]->getPositionPointer()->getZ() - mRocket-> getTailPosition().getZ());
			if (diff_head.length() < 0.4 || diff_tail.length() < 0.4){
				mRocket->crash();
			}
		}
	}
}

void CollisionManager:: collisionWallCheck(float d_t){
	vector<SimulationObject* > temp = mSceneManager->getSimulationObjects();

	for (unsigned int i = 0; i < temp.size(); i++){
		if(temp[i] == mRocket){
			//X
			if(mRocket -> getPositionPointer() -> getX() >= World_max_X || mRocket -> getPositionPointer() -> getX() <= World_min_X)
				mRocket -> getPhysics() -> setVelocity(Vector3(	-mRocket -> getPhysics() ->getVelocity().getX(),
																mRocket -> getPhysics() ->getVelocity().getY(),
																mRocket -> getPhysics() ->getVelocity().getZ()
																));
			//Y
			if(mRocket -> getPositionPointer() -> getY() >= World_max_Y || mRocket -> getPositionPointer() -> getY() <= World_min_Y)
				mRocket -> getPhysics() -> setVelocity(Vector3(	mRocket -> getPhysics() ->getVelocity().getX(),
																-mRocket -> getPhysics() ->getVelocity().getY(),
																mRocket -> getPhysics() ->getVelocity().getZ()
																));
			//Z
			if(mRocket -> getPositionPointer() -> getZ() >= World_max_Z || mRocket -> getPositionPointer() -> getZ() <= World_min_Z)
				mRocket -> getPhysics() -> setVelocity(Vector3(	mRocket -> getPhysics() ->getVelocity().getX(),
																mRocket -> getPhysics() ->getVelocity().getY(),
																-mRocket -> getPhysics() ->getVelocity().getZ()
																));


	}

}

}

void CollisionManager::collisionCheckAll(){
	vector<SimulationObject* > temp = mSceneManager->getSimulationObjects();
	for(unsigned int i = 0; i < temp.size(); i++){
		//cout<<"checking against wall :" <<i <<endl;
		collisionCheckObjectAgainstSceneWalls(temp[i]);
		for (unsigned int j =0; j < temp.size(); j++){
			//cout<<"checking against each other :" <<i<<", " <<j <<endl;
			collisionCheckObjectAgainstObject(temp[i],temp[j]);
		}
	}
}

void CollisionManager::collisionCheckObjectAgainstSceneWalls(SimulationObject* object){
	vector<Physics* > temp = object->getPhysicsList();
	for(unsigned int i = 0; i < temp.size();i++){
		collisionCheckPhysicsAgainstSceneWalls(temp[i]);
	}
}
void CollisionManager::collisionCheckPhysicsAgainstSceneWalls(Physics* physics){
	Vector3 n(0,0,0);
	Vector3 p = physics->getPosition();
	if (p.getX() > World_max_X) {	//rechts
		physics->setPosition(World_max_X-1.5, p.getY(), p.getZ());
		n.setX(-1.0);
		mCollisionForces.push_back(new ReflectiveCollision(physics, n));
	}
	if (p.getY() > World_max_Y) {	//oben
		physics->setPosition(p.getX(), World_max_Y-1.5, p.getZ());
		n.setY(-1.0);
		mCollisionForces.push_back(new ReflectiveCollision(physics, n));
	}
	if (p.getZ() > World_max_Z) {	//vorne
		physics->setPosition(p.getX(), p.getY(), World_max_Z-1.5);
		n.setZ(-1.0);
		mCollisionForces.push_back(new ReflectiveCollision(physics, n));
	}
	if (p.getX() < World_min_X) {	//links
		physics->setPosition(World_min_X+1.5, p.getY(), p.getZ());
		n.setX(1.0);
		mCollisionForces.push_back(new ReflectiveCollision(physics, n));
	}
	if (p.getY() < World_min_Y) {	//unten
		physics->setPosition(p.getX(), World_min_Y+1.5, p.getZ());
		n.setY(1.0);
		mCollisionForces.push_back(new ReflectiveCollision(physics, n));
	}
	if (p.getZ() < World_min_Z) {	//oben
		physics->setPosition(p.getX(), p.getY(), World_min_Z+1.5);
		n.setZ(1.0);
		mCollisionForces.push_back(new ReflectiveCollision(physics, n));
	}
}

void CollisionManager::collisionCheckObjectAgainstObject(SimulationObject* lhs,SimulationObject* rhs){
	vector<Physics* > lhs_phys = lhs->getPhysicsList();
	vector<Physics* > rhs_phys = rhs->getPhysicsList();
	//Alle MassePunkte auf Kollision testen
	for(unsigned int i = 0; i < lhs_phys.size();i++){
		for(unsigned int j=0; j < rhs_phys.size();j++){
			if (lhs_phys[i] != rhs_phys[j]){ //Unterschiedliche Massepunkte
				collisionCheckPhysicsAgainstPhysics(lhs_phys[i],rhs_phys[j]);
			}
		}
	}
}

void CollisionManager::collisionCheckPhysicsAgainstPhysics(Physics* lhs, Physics* rhs){
	Vector3 distance = rhs->getPosition() - lhs->getPosition();
	//Innerhalb der Kollisionsgrenze
	if (distance.length() < mMassPointEpsilon){
		float scalar = (lhs->getImpulse() * rhs->getImpulse());
		if (scalar < 0){	//Bewegen sich aufeinander zu --> Kollision
			mCollisionForces.push_back(new ElasticCollision(lhs,rhs,0.4));
		}
		//sonst nichts zu machen weil sie sich eh von einander weg bewegen
	}
}

void CollisionManager::applyAndEmptyCollisionForces(float d_t){
	for(unsigned int i = 0; i < mCollisionForces.size(); i++){
		mCollisionForces[i]->apply_fun(d_t);
	}
	mCollisionForces.clear();
}
