#include "SceneManager.h"
#include <GL/glut.h>

void SceneManager::update(float d_t){
/// ----Applying global forces...;
//	applyGlobalForces(d_t);

//	----Applying forces...
//	for (unsigned int i = 0; i < mForceObjects.size();i++){
//		mForceObjects[i]->apply_fun(d_t);
//	}

// ----Updating all Objects
}

inline void SceneManager:: initRocketSimulation(){
//TODO/////////////Globale Kräfte/////////////////////////////////////////////////////////////////////////
//			SimpleForce* gravity = new SimpleForce(Vector3(0.0,-1,0.0));
//			mGlobalForceObjects.push_back(gravity);

}

void SceneManager::init(){
/*TODO Referenzen setzen
ObjectFactory Referenzen setzen*/

	int simulation_index = 1;

	switch(simulation_index){
	case 1:
		initRocketSimulation();
		break;
	}
	
//TODO///////////////////////////////////////////////////////////////////////////////////////
//	//Alle Objekte bei GlobalForces Anmelden
//	for (unsigned int i = 0; i < mPhysicsObjects.size(); i ++){
//		for (unsigned int j = 0; j < mGlobalForceObjects.size();j++){
//			mGlobalForceObjects[j]->addInfluencedPhysics(mPhysicsObjects[i]);
//		}
//	}
}

void SceneManager::drawWorldConstraints(){
	glPointSize(5.0);
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex3f(World_max_X,World_max_Y,World_max_Z);
		glVertex3f(World_max_X,World_min_Y,World_max_Z);
		glVertex3f(World_max_X,World_max_Y,World_min_Z);
		glVertex3f(World_max_X,World_min_Y,World_min_Z);
		glVertex3f(World_min_X,World_min_Y,World_max_Z);
		glVertex3f(World_min_X,World_min_Y,World_min_Z);
		glVertex3f(World_min_X,World_max_Y,World_min_Z);
		glVertex3f(World_min_X,World_max_Y,World_max_Z);
	glEnd();
	glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
}

void SceneManager::draw(){
	drawWorldConstraints();
	//TODO mObjectFactory.drawAllObjects();
}

//void SceneManager::applyGlobalForces(float d_t){
//	for (unsigned int i = 0; i < mGlobalForceObjects.size(); i++){
//		mGlobalForceObjects[i]->apply_fun(d_t);
//	}
//}
