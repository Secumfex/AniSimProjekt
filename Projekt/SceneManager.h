#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneConstraints.h"
#include "Physics.h"
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
/**TODO
 * 	vector<Force* > mForceObjects;
 *	vector<Force* > mGlobalForceObjects;
 *	ObjectFactory mObjectFactory;
 */

//TODO/* Wendet Globale Kräfte an (z.B. Erdanziehung, Drag)*/
//void applyGlobalForces(float d_t);

	//Laden der Raketensimulation
	inline void initRocketSimulation();

public:

	/*Init der Szene*/
	void init();
	/*Zeichnen der gesamten Szene */
	void draw();
	/*Update der gesamten Szene, dh. aller Objekte darin*/
	void update(float d_t);

	/*Init der Objekte*/
	void initObjects();
	/*Zeichnet die Kanten des Raumes*/
	void drawWorldConstraints();


};

#endif
