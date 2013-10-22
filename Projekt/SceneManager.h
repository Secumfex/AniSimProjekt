#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneConstraints.h"
#include "SimulationObjects.h"
#include "ParticleSystem.h"
#include "Physics.h"
#include "Forces.h"
#include <vector>

using namespace std;
/**Klasse SceneManager:
 * verwaltet alle Objekte in der Szene
 * kann Objekte aus Factory anfordern oder löschen
 * kann Alle Objekte Updaten und Zeichnen
 * kann Globale Kräfte anwenden
 * */
class SceneManager{
private:
	vector<Physics* > mPhysicsObjects;
	vector<SimulationObject* > mSimulationObjects;
 	vector<Force* > mForceObjects;
	vector<Force* > mGlobalForceObjects;
/**TODO
*	ObjectFactory mObjectFactory;
*/

//TODO das würde ich lieber anders lösen KA

	RigidRocket* mPlayerRocket;
	ParticleSystem* mParticleSystem;

void applyGlobalForces(float d_t);

	//Laden der Raketensimulation
	inline void initRocketSimulation();
	inline void initInterstellareZiegelsteinSimulation();

public:
	/*Methode um ein SimulationsObject vollständig bei der Force anzumelden*/
	void registerSimulationObjectAtForce(SimulationObject* simulationObject, Force* force);

	RigidRocket* getPlayerRocket();

	/*Init der Szene*/
	void init();
	/*Reset der Szene*/
	void reset();

	/*Zeichnen der gesamten Szene */
	void draw();
	/*Update der gesamten Szene, dh. aller Objekte darin*/
	void update(float d_t);

	/*Init der Objekte*/
	void initObjects();
	/*Zeichnet die Kanten des Raumes*/
	void drawWorldConstraints();
	/*Zeichnet alle Simulations Objekte*/
	void drawSimulationObjects();

	/*Umschalten zwischen Euler Integration und Runge-Kutta*/
	void switchIntegrationMode();
};

#endif
