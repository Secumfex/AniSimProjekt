#ifndef SIMULATIONOBJECTS_H
#define SIMULATIONOBJECTS_H

/**
 * Diese Datei enthält alle Deklarationen von SimulationObject-Klassen
 */

#include "Physics.h"
#include <GL/glut.h>

/**
 * Klasse SimulationObject : ein prinzipiell dynamisches Objekt in der Simulation, z.B. Rakete, Asteroiden oder Planeten
 * Physics: Massepunkt des Simulationsobjekts, sollte besser was anderes sein...
 */
class SimulationObject{
protected:
	Physics mPhysics;
public:
	SimulationObject(float mass = 0.0, Vector3 velocity = Vector3(0,0,0), Vector3 position = Vector3(0,0,0));
	virtual void update(float d_t);
	virtual void draw();

	//Setzen der Massepunkt-Membervariablen
	void setPhysicsMembers(float mass, Vector3 velocity, Vector3 position);

	Physics* getPhysics();
};

/*
 * Klasse Rocket : die vom Spieler gesteuerte Rakete
 * Physics: 	Massepunkt der die Rakete repräsentiert TODO: sollte durch RigidObject ersetzt werden
 * Direction: 	Richtung der Rakete ( Erstmal == Impulsvektor )
 * fuel: 		verbleibender Treibstoff, bei 0.0 wird die Rakete nicht mehr beschleunigt.
 * fuelpower:	Power-Faktor, guter Treibstoff bringt stärkere Beschleunigung
 * mode: 		Modus der Rakete:
 * 					PRELAUNCH	:  Ausrichtung Möglich
 * 					LAUNCHED	:  Rakete beschleunigt bis fuel verbrannt, keine Ausrichtungen mehr möglich
 */class Rocket : public SimulationObject{
	 enum mode{PRELAUNCH,LAUNCHED};
 private:
	 Vector3 mDirection;
	 float mFuel;
	 float mFuelPower;
	 mode mMode;
 public:
	 //Default Direction : negative Z-Achse
	 Rocket(float fuel = 1.0,float fuelPower = 1.0, Vector3 direction = Vector3(0.0,0.0,-1.0), Vector3 position = Vector3(0,0,0));
	 virtual void update(float d_t);
	 virtual void draw();

	 void launch();
	 float getFuel();
 };

#endif
