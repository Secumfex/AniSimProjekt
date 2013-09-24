#ifndef SIMULATIONOBJECTS_H
#define SIMULATIONOBJECTS_H

/**
 * Diese Datei enthält alle Deklarationen von SimulationObject-Klassen
 */

#include "Physics.h"

/**
 * Klasse SimulationObject : ein prinzipiell dynamisches Objekt in der Simulation, z.B. Rakete, Asteroiden oder Planeten
 * Physics: Massepunkt des Simulationsobjekts, sollte besser was anderes sein...
 */
class SimulationObject{
private:
	Physics mPhysics;
public:
	virtual void update(float d_t);
	virtual void draw() = 0;
	Physics* getPhysics();
};

/*
 * Klasse Rocket : die vom Spieler gesteuerte Rakete
 * Physics: 	Massepunkt der die Rakete repräsentiert TODO: sollte durch RigidObject ersetzt werden
 * Direction: 	Richtung der Rakete ( Erstmal == Impulsvektor )
 * fuel: 		verbleibender Treibstoff, bei 0.0 wird die Rakete nicht mehr beschleunigt.
 * mode: 		Modus der Rakete:
 * 					PRELAUNCH	:  Ausrichtung Möglich
 * 					LAUNCHED	:  Rakete beschleunigt bis fuel verbrannt, keine Ausrichtungen mehr möglich
 */class Rocket{
	 enum mode{PRELAUNCH,LAUNCHED};
 private:
	 Vector3 mDirection;
	 float fuel;
	 mode mMode;
 public:
	 Rocket();
	 ~Rocket();
	 virtual void update(float d_t);
	 virtual void draw();
 };

#endif
