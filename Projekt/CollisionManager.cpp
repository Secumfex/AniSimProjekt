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

void CollisionManager:: collisionCheck(){

	vector<SimulationObject* > temp = mSceneManager->getSimulationObjects();

	for (unsigned int i = 0; i < temp.size(); i++){
		if(temp[i] != mRocket){

		//Normales Minus hat bei Vektor3 Mist gebaut... deshalb der ugly code
			Vector3 diff = Vector3(	temp[i]->getPositionPointer()->getX() - mRocket-> getPositionPointer()->getX(),
									temp[i]->getPositionPointer()->getY() - mRocket-> getPositionPointer()->getY(),
									temp[i]->getPositionPointer()->getZ() - mRocket-> getPositionPointer()->getZ());
//			cout << diff.getX() << "  "<< diff.getY() << "  "<< diff.getZ() << "  "<<endl;

			if (diff.length() < 1){
//				mRocket->crash();
				cout << "Es kracht"<< diff.length() << endl;
			}
		}
	}

}

