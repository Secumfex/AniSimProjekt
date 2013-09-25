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
		glutWireSphere(0.5,5,10);
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
	mPhysics.setPosition(position);
}

//Launch Rocket --> nur, wenn noch nicht gelaunched
void Rocket::launch(){
	if (mMode == PRELAUNCH){
		mMode = LAUNCHED;
		cout<<"LAUNCHED!"<<endl;
		mPhysics.applyForce(mDirection*mFuelPower);

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
			//Beschleunigungskraft abängig von FuelPower-Faktor
			Vector3 accelerationForce = mDirection*mFuelPower*d_t;
			mPhysics.applyForce(accelerationForce);

			//Verbrennung: 1 Unit / Sekunde oder so
			mFuel -= d_t;
		}

	//Massepunkt aktualisieren
	SimulationObject::update(d_t);

	//Falls In Bewegung: Richtung der Rakete an Bewegung anpassen
	mDirection = mPhysics.getImpulse();
	mDirection.normalize();

	//cout<<"fuel: " <<mFuel<<endl;

	}
}

//Solange nix besseres: Default draw methode + Richtung
void Rocket::draw(){

	//TODO hier stattdessen Modell zeichnen
	SimulationObject::draw();

	glPushMatrix();
		Vector3 pos = mPhysics.getPosition();
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_LINES);
			glVertex3f(pos.getX(), pos.getY(), pos.getZ());
			glVertex3f(pos.getX() + mDirection.getX(),pos.getY() + mDirection.getY(),pos.getZ() + mDirection.getZ());
		glEnd();
	glPopMatrix();
}
