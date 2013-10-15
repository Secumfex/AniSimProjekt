#include <windows.h>
#include "SceneManager.h"
#include <GL/glut.h>

void SceneManager::update(float d_t){
/// ----Applying global forces...;
	applyGlobalForces(d_t);

//	----Applying forces...
	for (unsigned int i = 0; i < mForceObjects.size();i++){
		mForceObjects[i]->apply_fun(d_t);
	}

//TODO// ----Updating all Objects
	for (unsigned int i = 0; i < mSimulationObjects.size(); i ++){
		drawAccumulatedForce(mSimulationObjects[i]);
		mSimulationObjects[i]->update(d_t);
	}
}

inline void SceneManager:: initRocketSimulation(){
/////////////// Globale Kräfte     /////////////////////////////////////////////////////////////////////////

		//Erdanziehungskraft
//		SimpleForce* gravity = new SimpleForce(Vector3(0.0,-10.0,0.0));
//		mGlobalForceObjects.push_back(gravity);
		//Anziehungskraft untereinander
		GravitationalForce* gravitation = new GravitationalForce(100.0,0.0,5.0);
		mGlobalForceObjects.push_back(gravitation);


/////////////// Simulations Objekte ////////////////////////////////////////////////////////////////////
		//TODO ObjectFactory bauen

		mPlayerRocket = new Rocket(10.0,50.0,Vector3(1.0,-0.5,0.0));
		mSimulationObjects.push_back(mPlayerRocket);

		SimulationObject* black_hole1 = new BlackHole(1000.0,Vector3(5.0,8.0,0.0));
		mSimulationObjects.push_back(black_hole1);

		SimulationObject* black_hole2 = new BlackHole(1000.0,Vector3(-5.0,12.0,0.0));
		mSimulationObjects.push_back(black_hole2);

		SimulationObject* black_hole3 = new BlackHole(1000.0,Vector3(-5.0,-12.0,0.0));
		mSimulationObjects.push_back(black_hole3);

}

void SceneManager::init(){
/*TODO Referenzen setzen
ObjectFactory  einf�gen*/

	int simulation_index = 1;

	switch(simulation_index){
	case 1:
		initRocketSimulation();
		break;
	}
	
/////////////////////////////////////////////////////////////////////////////////////////
//Alle Objekte bei GlobalForces Anmelden
	for (unsigned int i = 0; i < mSimulationObjects.size(); i ++){
		for (unsigned int j = 0; j < mGlobalForceObjects.size();j++){
			mGlobalForceObjects[j]->addInfluencedPhysics(mSimulationObjects[i]->getPhysics());
		}
	}
}

void SceneManager::reset(){
	mSimulationObjects.clear();
	mGlobalForceObjects.clear();
	mPlayerRocket = NULL;
	init();
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
	float x = World_max_X;
	float y = World_max_Y;
	float z = World_max_Z;
			glBegin(GL_LINE_LOOP);
				glVertex3f(-x,World_min_Y,z); //0
				glVertex3f(x,World_min_Y,z); //1
				glVertex3f(x,y,z); //2
				glVertex3f(-x,y,z); //3
				glVertex3f(-x,World_min_Y,z); //4
				glVertex3f(-x,World_min_Y,-z); //5
				glVertex3f(x,World_min_Y,-z); //6
				glVertex3f(x,y,-z); //7
				glVertex3f(-x,y,-z); //8
				glVertex3f(-x,World_min_Y,-z); //9
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(-x,y,z); //10
				glVertex3f(-x,y,-z); //10
				glVertex3f(x,y,z); //11
				glVertex3f(x,y,-z); //11
				glVertex3f(x,World_min_Y,z); //12
				glVertex3f(x,World_min_Y,-z); //12
			glEnd();

	glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
}

void SceneManager::draw(){
	drawWorldConstraints();
	drawSimulationObjects();
}

void SceneManager::drawSimulationObjects(){
	for (unsigned int i = 0; i < mSimulationObjects.size(); i++){
		mSimulationObjects[i]->draw();
	}
}

void SceneManager::applyGlobalForces(float d_t){
	for (unsigned int i = 0; i < mGlobalForceObjects.size(); i++){
		mGlobalForceObjects[i]->apply_fun(d_t);
	}
}

//Das is doof
Rocket* SceneManager::getPlayerRocket(){
	return mPlayerRocket;
}
