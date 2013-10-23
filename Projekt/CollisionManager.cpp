/*
 * CollisionManager.cpp
 *
 *  Created on: 21.10.2013
 *      Author: Raphimulator
 */

#include <CollisionManager.h>
#include <vector>

using namespace std;

CollisionManager::CollisionManager() {
	// TODO Auto-generated constructor stub

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

