#ifndef SIMULATIONOBJECTS_H
#define SIMULATIONOBJECTS_H

/**
 * Diese Datei enthält alle Deklarationen von SimulationObject-Klassen
 */

#include "RigidBody.h"
#include "Physics.h"
#include "ParticleSystem.h"
#include "UtilityClasses.h"
 
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

	virtual void switchIntegrationMode();

	//Liefert den einzigen Massepunkt zurück falls es eh nur den einen gibt
	virtual vector<Physics* > getPhysicsList();
	virtual Vector3* getPositionPointer();
	virtual Vector3 getPosition();

	virtual void setPosition(Vector3 pos);

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
	 enum mode{PRELAUNCH,LAUNCHED,CRASHED};
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

	 bool isLaunched();

	bool isCrashed();

	 void launch();
	 float getFuel();

	 void setDirection(Vector3 direction);
	 void setFuel(float fuel);
	 void setFuelPower(float fuelPower);

	 Vector3 getDirection();

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

class RigidSimulationObject : public SimulationObject{
protected:
	RigidBody* mRigidBody;
public:
	RigidSimulationObject(float mass = 1, Matrix3 Ibody = Matrix3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,1.0),
	Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0),
	Quaternion rotation = Quaternion(1,0,0,0), Vector3 angularMomentum = Vector3(0,0,0));

	RigidBody* getRigidBodyPointer();
	virtual Vector3* getPositionPointer();
	virtual Vector3 getPosition();
	virtual vector<Physics* > getPhysicsList();

	void drawAngularMomentum();
	void drawAngularVelocity();

	virtual void update(float d_t);
	virtual void draw();
	void rotate(Quaternion rot);

};

/*Ersatz für Rocket, jetzt mit sexy 2-Punkte-Rigid-Body*/
class RigidRocket : public RigidSimulationObject{
	enum mode{PRELAUNCH,LAUNCHED,CRASHED};
private:
	 Vector3 mDirection;
	 Quaternion* mRotation;
	 float mFuel;
	 float mFuelPower;
	 mode mMode;
	 Physics* mTail;
	 Physics* mHead;
	 float explosion_time;
	 void drawCrash();
	 void drawRocket();
	 void drawTail();

public:
	/*param:
	 * fuel : Menge des Spritts
	 * fuelPower : Kraft des Spritts
	 * mass_tail : Gewicht des Hinterteils bei leerem Tank
	 * mass_head : Gewicht des Vorderteils
	 * scale:		Skalierungsfaktor der Rakete
	 * position  : Startposition der Rakete*/
	RigidRocket(float fuel = 1.0, float fuelPower = 1.0, float mass_tail = 0.5,
			float mass_head = 0.5, Vector3 position = Vector3(0, 0, 0),
			float scale = 1.0);
	virtual void update(float d_t);
	virtual void draw();

	 Vector3 getHeadPosition();
	 Vector3 getTailPosition();

	 Physics* getTail();
	 Physics* getHead();

	bool isLaunched();

	bool isCrashed();

	void  crash();

	void launch();
	float getFuel();

	void setRotation(Quaternion rotation);
	void setFuel(float fuel);
	void setFuelPower(float fuelPower);

	Vector3 getDirection();
};

//Vorsicht: noch kein RigidSimulationObject!
class InterstellarerZiegelstein: public SimulationObject{
protected:
	RigidBlock mRigidBody;
	float mA,mB,mC;
public:

	InterstellarerZiegelstein(float a = 1.0, float b = 1.0, float c = 1.0, float mass = 1,
	Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0),
	Quaternion rotation = Quaternion(1,0,0,0), Vector3 angularMomentum = Vector3(0,0,0));

	RigidBody* getRigidBodyPointer();

	virtual void update(float d_t);
	virtual void draw();
	virtual void drawAngularMomentum();
	virtual void drawAngularVelocity();
};
//Objekt mit zwei Massepunkten aka Physi
class InterstellaresZweiMassePunkteObjekt : public RigidSimulationObject{

public:
	InterstellaresZweiMassePunkteObjekt(float mass1 = 0.5, float mass2 = 0.5, float distance = 1.0,
		Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0),
		Quaternion rotation = Quaternion(1,0,0,0), Vector3 angularMomentum = Vector3(0,0,0));

	virtual void update(float d_t);
	virtual void draw();
};

class ParticleCloud : public SimulationObject{
protected:
	ParticleSystem mParticleSystem;
public:
	ParticleCloud(int particleAmount = 100, float maxVelocity = 10.0, float maxPositionOffset = 40.0, Vector3 cloudCentrum = Vector3(0,0,0));
	virtual void createRandomParticles(int particleAmount = 100, float maxVelocity = 1.0, float maxPositionOffset = 10.0, Vector3 cloudCentrum = Vector3(0,0,0), float minPositionOffset = 0.0);

	virtual vector<Physics* > getPhysicsList();

	void forceParticlesToLoop(bool clockwise = true);

	virtual void draw();
	virtual void update(float d_t);
};

class ParticleRing : public ParticleCloud{
public:
	ParticleRing(int particleAmount = 100, float maxVelocity = 10.0,float maxRadius = 40.0, float minRadius = 30.0, Vector3 ringCentrum = Vector3(0,0,0));
};
/*Verwandte Funktionen*/
void drawAccumulatedForce(SimulationObject* pSim);

#endif
