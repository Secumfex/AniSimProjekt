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
	collisiontime = 0.0;
}

bool CollisionManager:: isCollision (){
	return mRocket -> isCrashed();
}


void CollisionManager:: explosion (float d_t){

	collisiontime = collisiontime + d_t;

	if(3.0 > collisiontime){

	glPushMatrix();

	glColor3f(1.0, 0.9, 0.0);

	glTranslatef(collisionPoint.getX(), collisionPoint.getY(), collisionPoint.getZ());

	glutSolidTeapot(1.3 *collisiontime);
//	glutSolidSphere(1.3 *collisiontime , 4, 4);

	glPopMatrix();
	}
}

void CollisionManager:: collisionCheck(float d_t){

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
			if (diff_head.length() < 0.5 || diff_tail.length() < 0.5){
				mRocket->crash();
				collisionPoint = (mRocket -> getHeadPosition() + mRocket -> getTailPosition()) * 0.5;
//				collisionPoint = (*(mRocket->getPositionPointer()) + *(temp[i]->getPositionPointer())) * 0.5;
			}

			cout << collisionPoint.getY() << endl;
		}
	}

}

