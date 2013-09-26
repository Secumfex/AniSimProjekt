#include "SimulationObjects.h"
#include <iostream>
using namespace std;

SimulationObject::SimulationObject(float mass, Vector3 velocity, Vector3 position){
	setPhysicsMembers(mass,velocity,position);
}

Physics* SimulationObject::getPhysics(){
	return &mPhysics;
}
void SimulationObject::setPhysicsMembers(float mass, Vector3 velocity, Vector3 position){
	mPhysics.setMass(mass);
	mPhysics.setVelocity(velocity);
	mPhysics.setPosition(position);
}

void SimulationObject::draw(){
	glPushMatrix();
		Vector3 pos = mPhysics.getPosition();
		glTranslatef(pos.getX(),pos.getY(),pos.getZ());
			glColor3f(1.0,0.0,0.0);
			glutWireSphere(0.25,5,10);
	glPopMatrix();
}

void SimulationObject::update(float d_t){
	mPhysics.update(d_t);
}

Rocket::Rocket(float fuel, float fuelPower, Vector3 direction, Vector3 position){
	mFuel = fuel;
	mFuelPower = fuelPower;
	direction.normalize();
	mDirection = direction;
	mMode = PRELAUNCH;
					//Rakete ist schwerer je mehr Treibstoff sie besitzt
    setPhysicsMembers(mFuel+10.0,Vector3(0,0,0),position);
}

//Launch Rocket --> nur, wenn noch nicht gelaunched
void Rocket::launch(){
	if (mMode == PRELAUNCH){
		mMode = LAUNCHED;
		mPhysics.clearAccumulatedForce(); //Das ist n�tig falls Erdanziehungskraft vorhanden
	}
}

/*
 * Wenn Modus PRELAUNCH: tue nichts
 * Wenn Modus LAUNCHED : Verbrenne Treibstoff, Beschleunige
 */
void Rocket::update(float d_t){
	if(mMode == LAUNCHED){
		//Solange Treibstoff vorhanden
		if (mFuel > 0.0){
			//Beschleunigungskraft ab�ngig von FuelPower-Faktor
			Vector3 accelerationForce = mDirection*mFuelPower;
			mPhysics.applyForce(accelerationForce*d_t);

			//Verbrennung: 1 Unit / Sekunde oder so
			mFuel -= d_t;
				//Rakete verliert dadurch an Masse
			mPhysics.setMass(mPhysics.getMass()- d_t);
		}

		//Massepunkt aktualisieren
		SimulationObject::update(d_t);

		//TODO Sehr problematisch: Ab wann soll sich die Rakete der Bewegungsrichtung angleichen? Zu fr�h -> Rakete f�llt in Boden
		if(mPhysics.getVelocity().length() > 5.0){
			mDirection = mPhysics.getVelocity();
			mDirection.normalize();
		}

	}
}

void Rocket::setFuel(float fuel){
	mFuel = fuel;
}
void Rocket::setFuelPower(float fuelPower){
	mFuelPower = fuelPower;
}
void Rocket::setDirection(Vector3 direction){
	direction.normalize();
	mDirection = direction;
}


//Solange nix besseres: Default draw methode + Richtung
void Rocket::draw(){

	//TODO hier stattdessen Modell zeichnen
	SimulationObject::draw();

	glPushMatrix();

	//Richtung aka Nase
		Vector3 pos = mPhysics.getPosition();
		glColor3f(0.0,0.0,1.0);

		glTranslatef(pos.getX(),pos.getY(),pos.getZ());
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(mDirection.getX(), mDirection.getY(),mDirection.getZ());
		glEnd();

	//Raketenschweif
		if(mFuel > 0 && mMode == LAUNCHED){
			glColor3f(1.0,0.7,0.0);
			glTranslatef(-mDirection.getX()*0.2,-mDirection.getY()*0.2,- mDirection.getZ()*0.2);
			glutWireSphere(0.1,4,5);
		}
	glPopMatrix();
}

void BlackHole::draw(){
	glPushMatrix();
		Vector3 pos = mPhysics.getPosition();
		glTranslatef(pos.getX(),pos.getY(),pos.getZ());
			glColor3f(0.0,0.0,0.0);
			glutSolidSphere(0.2,5,10);
			glutWireSphere(0.3,10,15);
	glPopMatrix();
}

void BlackHole::update(float d_t){
	//Tue nichts, bleib wie du bist
	mPhysics.clearAccumulatedForce();
}

BlackHole::BlackHole(float mass,Vector3 position){
	setPhysicsMembers(mass,Vector3(0,0,0),position);
}