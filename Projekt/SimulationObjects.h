#ifndef SIMULATIONOBJECTS_H
#define SIMULATIONOBJECTS_H

/**
 * Diese Datei enthält alle Deklarationen von SimulationObject-Klassen
 */

#include "RigidBody.h"
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
 * 					LAUNCHED	:  Rakete beschleunigt bis fuel verbrannt, keine Ausrichtungen mehr möglich*/
class Rocket : public SimulationObject{
	 enum mode{PRELAUNCH,LAUNCHED};
 private:
	 Vector3 mDirection;
	 float mFuel;
	 float mFuelPower;
	 mode mMode;
	 void drawRocket();
	 void drawTail();
 public:
	 //Default Direction : negative Z-Achse
	 Rocket(float fuel = 1.0,float fuelPower = 1.0, Vector3 direction = Vector3(0.0,0.0,-1.0), Vector3 position = Vector3(0,0,0));
	 virtual void update(float d_t);
	 virtual void draw();

	 void launch();
	 float getFuel();

	 void setDirection(Vector3 direction);
	 void setFuel(float fuel);
	 void setFuelPower(float fuelPower);

 };

/*
 * Klasse BlackHole
 * Objekt, das eine sehr, sehr hohe Masse besitzt aber sich nicht bewegt
 */
class BlackHole : public SimulationObject{
public:
	virtual void draw();
	virtual void update(float d_t);

	BlackHole(float mass = 1000.0, Vector3 position = Vector3(0,0,0));
};

class InterstellarerZiegelstein: public SimulationObject{
protected:
	RigidBlock mRigidBody;
	float mA,mB,mC;
public:

	InterstellarerZiegelstein(float a = 1.0, float b = 1.0, float c = 1.0,
			Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0),
			Quaternion rotation = Quaternion(0,0,0,1), Vector3 angularMomentum = Vector3(0,0,0));

	RigidBody* getRigidBodyPointer();

	virtual void update(float d_t);
	virtual void draw();
};

/*Verwandte Funktionen*/
void drawAccumulatedForce(SimulationObject* pSim);

#endif
