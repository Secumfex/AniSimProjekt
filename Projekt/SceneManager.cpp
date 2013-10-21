#include <windows.h>
#include "SceneManager.h"
#include <GL/glut.h>

inline void SceneManager::initRocketSimulation(){
/////////////// Globale KrÃ¤fte     /////////////////////////////////////////////////////////////////////////

		//Erdanziehungskraft
//		SimpleForce* gravity = new SimpleForce(Vector3(0.0,-10.0,0.0));
//		mGlobalForceObjects.push_back(gravity);

		//Anziehungskraft untereinander
		GravitationalForce* gravitation = new GravitationalForce(1000.0,0.0,1.0);
		mGlobalForceObjects.push_back(gravitation);


/////////////// Simulations Objekte ////////////////////////////////////////////////////////////////////
		//TODO ObjectFactory bauen


		SimulationObject* black_hole0 = new BlackHole(400.0,Vector3(0.0,0.0,0.0));
		mSimulationObjects.push_back(black_hole0);

		mPlayerRocket = new RigidRocket(1.0,10.0,1 ,1);
		mPlayerRocket->getRigidBodyPointer()->setPosition(Vector3(.0,20.0,0.0));
		mSimulationObjects.push_back(mPlayerRocket);


		SimulationObject* black_hole1 = new BlackHole(1000.0,Vector3(25.0,8.0,0.0));
		mSimulationObjects.push_back(black_hole1);

//		SimulationObject* black_hole2 = new BlackHole(1000.0,Vector3(-5.0,12.0,0.0));
//		mSimulationObjects.push_back(black_hole2);

		SimulationObject* black_hole3 = new BlackHole(1000.0,Vector3(-5.0,-12.0,0.0));
		mSimulationObjects.push_back(black_hole3);

}

inline void SceneManager::initInterstellareZiegelsteinSimulation(){
//	SimpleForce* gravity = new SimpleForce(Vector3(0.0,-1.0,0.0));
//	mGlobalForceObjects.push_back(gravity);
	//Anziehungskraft untereinander
	GravitationalForce* gravitation = new GravitationalForce(100.0,0.0,1.0);
	mGlobalForceObjects.push_back(gravitation);


	//	InterstellarerZiegelstein* ziegelstein = new InterstellarerZiegelstein(2.0,0.5,1.0,1,Vector3(0,1.0,0.0));
//	mSimulationObjects.push_back(ziegelstein);
//
//	ziegelstein->getRigidBodyPointer()->setImpulse(Vector3(0.0,0.0,0.0));
//	ziegelstein->getRigidBodyPointer()->setAngularMomentum(Vector3(0.0,1.0,0.0));

	InterstellaresZweiMassePunkteObjekt* zweiMassePunkteObjekt = new InterstellaresZweiMassePunkteObjekt(0.5,0.5,1.0,Vector3(0.0,0.0,0.0));
	mSimulationObjects.push_back(zweiMassePunkteObjekt);

	zweiMassePunkteObjekt->getRigidBodyPointer()->setImpulse(Vector3(10.0,0.0,0.0));
	zweiMassePunkteObjekt->getRigidBodyPointer()->setAngularMomentum(Vector3(0.0,0.0,0.0));

	SimulationObject* black_hole0 = new BlackHole(1000.0,Vector3(0.0,6.0,0.0));
	mSimulationObjects.push_back(black_hole0);

}

void SceneManager::init(){
/*TODO Referenzen setzen
ObjectFactory  einfügen*/

	int simulation_index = 1;

	switch(simulation_index){
	case 1:
		initRocketSimulation();
		break;
	case 2:
		initInterstellareZiegelsteinSimulation();
		break;
	}
	
/////////////////////////////////////////////////////////////////////////////////////////
//Alle Objekte bei GlobalForces Anmelden
	for (unsigned int i = 0; i < mSimulationObjects.size(); i ++){
		for (unsigned int j = 0; j < mGlobalForceObjects.size();j++){
			registerSimulationObjectAtForce(mSimulationObjects[i],mGlobalForceObjects[j]);
		}
	}
}

void SceneManager::registerSimulationObjectAtForce(SimulationObject* simulationObject, Force* force){
	vector<Physics* > mp = simulationObject->getPhysicsList();
	for (unsigned int i = 0; i < mp.size(); i++ ){
		force->addInfluencedPhysics(mp[i]);
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

void SceneManager::update(float d_t){
/// ----Applying global forces...;
	applyGlobalForces(d_t);

//	----Applying forces...
	for (unsigned int i = 0; i < mForceObjects.size();i++){
		mForceObjects[i]->apply_fun(d_t);
	}

	for (unsigned int i = 0; i < mSimulationObjects.size(); i ++){
		drawAccumulatedForce(mSimulationObjects[i]);
		mSimulationObjects[i]->update(d_t);
	}
}

void SceneManager::applyGlobalForces(float d_t){
	for (unsigned int i = 0; i < mGlobalForceObjects.size(); i++){
		mGlobalForceObjects[i]->apply_fun(d_t);
	}
}

//Das is doof
RigidRocket* SceneManager::getPlayerRocket(){
	return mPlayerRocket;
}
