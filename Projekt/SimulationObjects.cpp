#include <windows.h>
#include "SimulationObjects.h"
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;
 
SimulationObject::SimulationObject(float mass, Vector3 velocity,
		Vector3 position) {
	mPhysics.setMass(mass);
	mPhysics.setVelocity(velocity);
	mPhysics.setPosition(position);
}

Physics* SimulationObject::getPhysics(){
	return &mPhysics;
}
Vector3* SimulationObject::getPositionPointer(){
	return mPhysics.getPositionPointer();
}
Vector3 SimulationObject::getPosition(){
	return mPhysics.getPosition();
}

void SimulationObject::setPosition(Vector3 pos){
	mPhysics.setPosition(pos);
}

//liefert den einzigen Massepunkt zur�ck, falls es eh nur den einen gibt
vector<Physics*> SimulationObject::getPhysicsList(){
	vector<Physics* > result;
	result.push_back(&mPhysics);
	return result;
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

void SimulationObject::switchIntegrationMode(){
	vector<Physics* > pl = getPhysicsList();
	for (unsigned int i = 0 ; i < pl.size(); i++){
		pl[i]->switchIntegrationMode();
	}
}

Rocket::Rocket(float fuel, float fuelPower, Vector3 direction, Vector3 position){
	mFuel = fuel;
	mFuelPower = fuelPower;
	direction.normalize();
	mDirection = direction;
	mMode = PRELAUNCH;
					//Rakete ist schwerer je mehr Treibstoff sie besitzt
    mPhysics.setMass(mFuel+10.0);
    mPhysics.setVelocity(Vector3(0,0,0));
    mPhysics.setPosition(position);
}

bool Rocket::isLaunched(){
	return mMode == LAUNCHED;
}

bool Rocket:: isCrashed(){
	return mMode == CRASHED;
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
				//Hier verhindere ich dass weniger als 0 Treibstoff da ist
			mFuel = (mFuel<0.0)? 0.0 : mFuel;
				//Rakete verliert dadurch an Masse
			mPhysics.setMass(mPhysics.getMass()- d_t);
		}
		//mPhysics.getAccumulatedForce().debugPrintToCerr();
		//Massepunkt aktualisieren
		SimulationObject::update(d_t);

		// Sehr problematisch: Ab wann soll sich die Rakete der Bewegungsrichtung angleichen? Zu fr�h -> Rakete f�llt in Boden
		if(mPhysics.getVelocity().length() > 5.0){
			mDirection = mPhysics.getVelocity();
			mDirection.normalize();
		}
	}else{ //Update vor Start
		mPhysics.clearAccumulatedForce(); //Acc Force clearen, denn keine Kraft SOLL wirken
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

Vector3 Rocket::getDirection(){
	return mDirection;
}


//Solange nix besseres: Default draw methode + Richtung
void Rocket::draw(){

	// SimulationObject::draw();

	glPushMatrix();

	//Richtung aka Nase
		Vector3 pos = mPhysics.getPosition();
		Vector3 cross = mDirection.cross(Vector3(0, 1, 0));
		float angle = -acos((mDirection * Vector3(0, 1, 0))) * 180.0 / M_PI ;

		glColor3f(0.0,0.0,1.0);

		glTranslatef(pos.getX(),pos.getY(),pos.getZ());

		glRotatef (angle, cross.getX(), cross.getY(), cross.getZ());



		drawRocket();

	//Raketenschweif
		if(mFuel > 0 && mMode == LAUNCHED){
			drawTail();
		}
	glPopMatrix();
}

// Male/Importiere Rakete
void Rocket::drawRocket(){
	glPushMatrix();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5, 0.5, 0.7);
	glVertex3f(-0.25, -1.0, 0.25);
	glVertex3f(0.25, -1.0, 0.25);
	glVertex3f(-0.25, 0.8, 0.25);
	glVertex3f(0.25, 0.8, 0.25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5, 0.5, 0.7);
	glVertex3f(-0.25, -1.0, -0.25);
	glVertex3f(0.25, -1.0, -0.25);
	glVertex3f(-0.25, 0.8, -0.25);
	glVertex3f(0.25, 0.8, -0.25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.4, 0.4, 0.6);
	glVertex3f(0.25, -1.0, -0.25);
	glVertex3f(0.25, -1.0, 0.25);
	glVertex3f(0.25, 0.8, -0.25);
	glVertex3f(0.25, 0.8, 0.25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.4, 0.4, 0.6);
	glVertex3f(-0.25, -1.0, -0.25);
	glVertex3f(-0.25, -1.0, 0.25);
	glVertex3f(-0.25, 0.8, -0.25);
	glVertex3f(-0.25, 0.8, 0.25);
	glEnd();

	glColor3f(0.2, 0.2, 0.2);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.25, -1.0, -0.25);
		glVertex3f(-0.25, -1.0, 0.25);
		glVertex3f(0.25, -1.0, -0.25);
		glVertex3f(0.25, -1.0, 0.25);
		glEnd();

	glColor3f(0.8, 0.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.4, -1.0, 0.0);
	glVertex3f(-0.4, -1.0, 0.0);
	glVertex3f(-0.25, -0.8, 0.0);
	glVertex3f(0.25, -0.8, 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.0, -1.0, 0.4);
	glVertex3f(0.0, -1.0, -0.4);
	glVertex3f(0.0, -0.8, -0.25);
	glVertex3f(0.0, -0.8, 0.25);
	glEnd();

	glTranslated(0.0, 0.8, 0.0);
	glRotated(90, -1, 0, 0);
	glutSolidCone(0.35, 0.5, 6, 6);

	glPopMatrix();
}

void Rocket::drawTail(){

	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, -1.1, 0.0);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 6, 6);
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
	mPhysics.setMass(mass);
    mPhysics.setVelocity(Vector3(0,0,0));
    mPhysics.setPosition(position);
}

void drawAccumulatedForce(SimulationObject* pSim){
	Vector3 pos = pSim->getPhysics()->getPosition();
	Vector3 forceAcc = pSim->getPhysics()->getAccumulatedForce();
	glPushMatrix();
		glColor3f(0.7,0.5,0.5);
		glTranslatef(pos.getX(),pos.getY(),pos.getZ());
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(forceAcc.getX()*0.1,forceAcc.getY()*0.1,forceAcc.getZ()*0.1);
		glEnd();
	glPopMatrix();
}

void InterstellarerZiegelstein::draw(){
		float x = mA;
		float y = mB;
		float z = mC;

		glLineWidth(2.0);
		float currentColor[4];
		glGetFloatv(GL_CURRENT_COLOR,currentColor);
		glColor3f(0.8,0.2,0.3);
		//cout<<"I DONT KNOW WHY"<<endl;
		Vector3 pos = mRigidBody.getPosition();
		Quaternion rot = mRigidBody.getRotationQuaternion();
		//cout<<"Rotation: "<<r.getW() <<" , "<< r.getX() <<" , "<<  r.getY()<<" , "<<  r.getZ()<<endl;

		glPushMatrix();
			glTranslatef(pos.getX(),pos.getY(),pos.getZ());
			glRotatef(rot.getAngle(),rot.getX(),rot.getY(),rot.getZ());
			glScalef(x,y,z);
			glutWireCube(1.0);
		glPopMatrix();
		glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);

		drawAngularVelocity();
		drawAngularMomentum();
}
void InterstellarerZiegelstein::drawAngularMomentum(){
	Vector3 angular_momentum = mRigidBody.getAngularMomentum();
	Vector3 position = mRigidBody.getPosition();
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);
	glColor3f(0.0,0.2,0.8);
	glPushMatrix();
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(angular_momentum.getX(),angular_momentum.getY(),angular_momentum.getZ());
		glEnd();
	glPopMatrix();
	glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
}
void InterstellarerZiegelstein::drawAngularVelocity(){
	Vector3 angular_momentum = mRigidBody.getAngularMomentum();
	Vector3 position = mRigidBody.getPosition();
	drawVector3(angular_momentum,position,Vector3(0.0,0.5,0.8));
}
void InterstellarerZiegelstein::update(float d_t){
	mRigidBody.update(d_t);
}
InterstellarerZiegelstein::InterstellarerZiegelstein(float a, float b, float c, float mass,
	Vector3 position, Vector3 velocity,
	Quaternion rotation, Vector3 angularMomentum){
	mA = a;
	mB = b;
	mC = c;

	//Status setzen
	mRigidBody.setPosition(position);
	mRigidBody.setRotation(rotation);
	mRigidBody.setMass(mass);

	//Traegheitstensor berechnen
	mRigidBody.setDimensions(a, b, c);

	mRigidBody.setVelocity(velocity);
	mRigidBody.setAngularMomentum(angularMomentum);

}
RigidBody* InterstellarerZiegelstein::getRigidBodyPointer(){
	return &mRigidBody;
}
InterstellaresZweiMassePunkteObjekt::InterstellaresZweiMassePunkteObjekt(float mass1, float mass2, float distance, Vector3 position, Vector3 velocity, Quaternion rotation, Vector3 angularMomentum){
	mRigidBody = new RigidTwoMass(mass1,mass2,distance,position,velocity,rotation,angularMomentum);
}

void InterstellaresZweiMassePunkteObjekt::draw(){
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);

	vector<Physics* > mp = mRigidBody->getMassPoints();
	Quaternion rot = mRigidBody->getRotationQuaternion();
	Vector3 pos = mRigidBody->getPosition();
	glPushMatrix();
		glTranslatef(pos.getX(),pos.getY(),pos.getZ());
		glRotatef(rot.getAngle(),rot.getX(),rot.getY(),rot.getZ());
		glColor3f(0.4,1.0,0.4);
		glutWireSphere(mRigidBody->getMass()/4.0,10,5);
		glColor3f(1.0,0.0,0.4);
	glPopMatrix();
	for (unsigned int i = 0; i < mp.size(); i++){
		glPushMatrix();
			pos = mp[i]->getPosition();
			glTranslatef(pos.getX(), pos.getY(), pos.getZ());
			glRotatef(rot.getAngle(),rot.getX(),rot.getY(),rot.getZ());
			glutWireSphere(mp[i]->getMass()/4.0,10,5);
		glPopMatrix();
	}
	glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
}
void InterstellaresZweiMassePunkteObjekt::update(float d_t){
	RigidSimulationObject::update(d_t);
}
/*Wie ein SimulationObject aber eben mit einem RigidBody statt einem einzelnen Massepunkt (Physics)*/

RigidSimulationObject::RigidSimulationObject(float mass, Matrix3 Ibody,
		Vector3 position, Vector3 velocity,
	Quaternion rotation, Vector3 angularMomentum){
	mRigidBody = new RigidBody();
	mRigidBody->initValues(mass,Ibody,position,velocity,rotation,angularMomentum);
}

RigidBody* RigidSimulationObject::getRigidBodyPointer(){
	return mRigidBody;
}
Vector3* RigidSimulationObject::getPositionPointer(){
	return 	mRigidBody->getPositionPointer();
}
Vector3 RigidSimulationObject::getPosition(){
	return 	mRigidBody->getPosition();
}
vector<Physics* > RigidSimulationObject::getPhysicsList(){
	return mRigidBody->getMassPoints();
}
void RigidSimulationObject::update(float d_t){
	mRigidBody->update(d_t);
}

void RigidSimulationObject::draw(){
	Vector3 position = mRigidBody->getPosition();
	Quaternion rot = mRigidBody->getRotationQuaternion();
	glPushMatrix();
		glTranslatef(position.getX(),position.getY(),position.getZ());
		glRotatef(rot.getAngle(),rot.getX(),rot.getY(),rot.getZ());
		glutWireTeapot(mRigidBody->getMass()/2.0);
	glPopMatrix();
	drawAngularMomentum();
	drawAngularVelocity();
}
void RigidSimulationObject::drawAngularMomentum(){
	Vector3 angular_momentum = mRigidBody->getAngularMomentum();
	Vector3 position = mRigidBody->getPosition();
	drawVector3(angular_momentum,position,Vector3(0.0,0.2,0.8));
}
void RigidSimulationObject::drawAngularVelocity(){
	Vector3 angular_momentum = mRigidBody->getAngularMomentum();
	Vector3 position = mRigidBody->getPosition();
	drawVector3(angular_momentum,position,Vector3(0.0,0.5,0.8));
}



RigidRocket::RigidRocket(float fuel, float fuelPower, float mass_tail, float mass_head,  Vector3 position, float scale){
	explosion_time = 3.0;

	mFuel = fuel;
	mFuelPower = fuelPower;

	mMode = PRELAUNCH;

	//Rakete ist schwerer je mehr Treibstoff sie besitzt
	RigidTwoMass* twoMassBody = new RigidTwoMass(mass_tail+fuel,mass_head,1.5 * scale,position);
	mTail = twoMassBody->getLeftMassPoint();
	mHead = twoMassBody->getRightMassPoint();

	mDirection = mHead->getPosition()-mTail->getPosition(); //Richtung = Fu� nach Kopf
	mDirection.normalize();

	mRigidBody = twoMassBody;
	mRotation = mRigidBody->getRotationQuaternionPointer();

}

float RigidRocket::getFuel(){
	return mFuel;
}

bool RigidRocket::isLaunched(){
	return mMode == LAUNCHED;
}

bool RigidRocket:: isCrashed(){
	return mMode == CRASHED;
}

void RigidRocket:: crash(){
	mMode = CRASHED;
	mPhysics.setMass(0);
	mHead->setMass(0);
	mTail->setMass(0);
}

//Launch Rocket --> nur, wenn noch nicht gelaunched
void RigidRocket::launch(){
	if (mMode == PRELAUNCH){
		mMode = LAUNCHED;
		mRigidBody->clearForceAndTorque(); //Das ist n�tig falls Erdanziehungskraft vorhanden
	}
}

Vector3 RigidRocket:: getHeadPosition(){
	return mHead->getPosition();
}

Vector3 RigidRocket:: getTailPosition(){
	return mTail->getPosition();
}

Physics* RigidRocket:: getTail(){
	return mTail;
}
Physics* RigidRocket:: getHead(){
	return mHead;
}


/*
 * Wenn Modus PRELAUNCH: tue nichts
 * Wenn Modus LAUNCHED : Verbrenne Treibstoff, Beschleunige
 */
void RigidRocket::update(float d_t){
	if (mMode == LAUNCHED) {
		if (mFuel > 0.0) { //Solange Treibstoff vorhanden

			Vector3 accelerationForce = mDirection * mFuelPower; //Beschleunigungskraft ab�ngig von FuelPower-Faktor
			mTail->applyForce(accelerationForce * d_t);	//Kraft wirkt auf Hinterteil

			mFuel -= d_t; //Verbrennung: 1 Unit / Sekunde oder so
			mFuel = (mFuel < 0.0) ? 0.0 : mFuel; //Hier verhindere ich dass weniger als 0 Treibstoff da ist

			//TODO klappt noch nicht
			//mTail->setMass(mTail->getMass()- d_t); 	//Rakete verliert dadurch an Masse
		}

		//mPhysics.getAccumulatedForce().debugPrintToCerr();

		RigidSimulationObject::update(d_t); 		//Massepunkte aktualisieren


		//Richtung aktualisieren
		mDirection = mHead->getPosition()-mTail->getPosition();
		mDirection.normalize();

	} else { //Update vor Start
		mRigidBody->clearForceAndTorque(); //Acc Force clearen, denn keine Kraft SOLL wirken

		if(mMode == CRASHED)
			explosion_time-= d_t;
	}
}
void RigidSimulationObject::rotate(Quaternion rot){
	mRigidBody->rotate(rot);
}

void RigidRocket::setFuel(float fuel){
	mFuel = fuel;
}
void RigidRocket::setFuelPower(float fuelPower){
	mFuelPower = fuelPower;
}
Vector3 RigidRocket::getDirection(){
	return mDirection;
}
void RigidRocket::setRotation(Quaternion rotation){
	*mRotation = rotation;
}
void RigidRocket::draw(){

	// SimulationObject::draw();

	glPushMatrix();

	//Richtung aka Nase
		Vector3 pos = mRigidBody->getPosition();
		Quaternion rot = mRigidBody->getRotationQuaternion();
		glColor3f(0.0,0.0,1.0);

		glTranslatef(pos.getX(),pos.getY(),pos.getZ());

		glRotatef (rot.getAngle(), rot.getX(), rot.getY(), rot.getZ());

		glRotatef (-90,0,0,1);

		if(mMode == CRASHED){
			drawCrash();
		}else
			drawRocket();

	//Raketenschweif
		if(mFuel > 0 && mMode == LAUNCHED){
			drawTail();
		}
	glPopMatrix();
}

void RigidRocket::drawCrash(){
	glPushMatrix();

	glColor3f(1.0, 0.9, 0.0);
	if(explosion_time > 0)
	glutSolidSphere(1.3 * (3 - explosion_time) , 8, 8);

	glColor3f(0.2, 0.2, 0.2);

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-0.25, -1.0, 0.25);
			glVertex3f(0.25, -1.0, 0.25);
			glVertex3f(-0.25, 0.8, 0.25);
			glVertex3f(0.25, 0.8, 0.25);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-0.25, -1.0, -0.25);
			glVertex3f(0.25, -1.0, -0.25);
			glVertex3f(-0.25, 0.8, -0.25);
			glVertex3f(0.25, 0.8, -0.25);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.25, -1.0, -0.25);
			glVertex3f(0.25, -1.0, 0.25);
			glVertex3f(0.25, 0.8, -0.25);
			glVertex3f(0.25, 0.8, 0.25);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-0.25, -1.0, -0.25);
			glVertex3f(-0.25, -1.0, 0.25);
			glVertex3f(-0.25, 0.8, -0.25);
			glVertex3f(-0.25, 0.8, 0.25);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-0.25, -1.0, -0.25);
			glVertex3f(-0.25, -1.0, 0.25);
			glVertex3f(0.25, -1.0, -0.25);
			glVertex3f(0.25, -1.0, 0.25);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.4, -1.0, 0.0);
			glVertex3f(-0.4, -1.0, 0.0);
			glVertex3f(-0.25, -0.8, 0.0);
			glVertex3f(0.25, -0.8, 0.0);
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.0, -1.0, 0.4);
			glVertex3f(0.0, -1.0, -0.4);
			glVertex3f(0.0, -0.8, -0.25);
			glVertex3f(0.0, -0.8, 0.25);
		glEnd();

		glTranslated(0.0, 0.8, 0.0);
		glRotated(90, -1, 0, 0);
		glutSolidCone(0.35, 0.5, 6, 6);





		glPopMatrix();

}


// Male/Importiere Rakete
void RigidRocket::drawRocket(){
	glPushMatrix();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5, 0.5, 0.7);
	glVertex3f(-0.25, -1.0, 0.25);
	glVertex3f(0.25, -1.0, 0.25);
	glVertex3f(-0.25, 0.8, 0.25);
	glVertex3f(0.25, 0.8, 0.25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5, 0.5, 0.7);
	glVertex3f(-0.25, -1.0, -0.25);
	glVertex3f(0.25, -1.0, -0.25);
	glVertex3f(-0.25, 0.8, -0.25);
	glVertex3f(0.25, 0.8, -0.25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.4, 0.4, 0.6);
	glVertex3f(0.25, -1.0, -0.25);
	glVertex3f(0.25, -1.0, 0.25);
	glVertex3f(0.25, 0.8, -0.25);
	glVertex3f(0.25, 0.8, 0.25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.4, 0.4, 0.6);
	glVertex3f(-0.25, -1.0, -0.25);
	glVertex3f(-0.25, -1.0, 0.25);
	glVertex3f(-0.25, 0.8, -0.25);
	glVertex3f(-0.25, 0.8, 0.25);
	glEnd();

	glColor3f(0.2, 0.2, 0.2);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.25, -1.0, -0.25);
		glVertex3f(-0.25, -1.0, 0.25);
		glVertex3f(0.25, -1.0, -0.25);
		glVertex3f(0.25, -1.0, 0.25);
		glEnd();

	glColor3f(0.8, 0.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.4, -1.0, 0.0);
	glVertex3f(-0.4, -1.0, 0.0);
	glVertex3f(-0.25, -0.8, 0.0);
	glVertex3f(0.25, -0.8, 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.0, -1.0, 0.4);
	glVertex3f(0.0, -1.0, -0.4);
	glVertex3f(0.0, -0.8, -0.25);
	glVertex3f(0.0, -0.8, 0.25);
	glEnd();

	glTranslated(0.0, 0.8, 0.0);
	glRotated(90, -1, 0, 0);
	glutSolidCone(0.35, 0.5, 6, 6);

	glPopMatrix();
}

void RigidRocket::drawTail(){
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, -1.1, 0.0);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 6, 6);
}

ParticleCloud::ParticleCloud(int particleAmount, float maxVelocity, float maxPositionOffset, Vector3 cloudCentrum){
	setPosition(cloudCentrum);
	createRandomParticles(particleAmount, maxVelocity, maxPositionOffset,cloudCentrum);
	mParticleSystem.addForce(new DistancePointForce(maxPositionOffset,0.01,cloudCentrum));
}

void ParticleCloud::createRandomParticles(int particleAmount, float maxVelocity, float maxPositionOffset, Vector3 cloudCentrum, float minPositionOffset){
	mParticleSystem.clearAllParticles();
	for (int i = 0; i < particleAmount; i++){
			Vector3 pos = randomVector3(cloudCentrum,(((float)(i+1))/(float)particleAmount)*maxPositionOffset,(((float)(i+1))/(float)particleAmount)*maxPositionOffset,0.0,0.0,0.0,0.0);	//TODOnoch nur auf X und Y Achse
			float v = (((float) rand()) / ((float)RAND_MAX)-0.5)*2.0;
			Vector3 vel = randomVector3(Vector3(0,0,0),1.0,1.0,0.0);
			vel.normalize();
			vel*=v*maxVelocity;
			mParticleSystem.addParticle(new Physics(0.1,vel,pos));
		}
}

//Setzt den Impuls der Partikel orthogonal zum Mittelpunktsvektor
void ParticleCloud::forceParticlesToLoop(bool clockwise){
	vector<Physics* > p= mParticleSystem.getParticles();
	for (unsigned int i = 0; i <p.size(); i++ ){
		Vector3 CenterToParticle = p[i]->getPosition()-getPosition();
		CenterToParticle.normalize();
		Vector3 ortho(CenterToParticle.getY(),CenterToParticle.getX(),CenterToParticle.getZ());
		if (clockwise){
			ortho.setY(ortho.getY()*(-1));
		}
		else{
			ortho.setX(ortho.getX()*(-1));
		}
		float imp = p[i]->getImpulse().length();
		Vector3 newImp = imp*ortho;
		p[i]->setImpulse(newImp);
	}
}

void ParticleCloud::draw(){
	glColor3f(0.2,0.1,0.5);
	glPointSize(5.0);
	glBegin(GL_QUADS);

	vector<Physics* > particles = mParticleSystem.getParticles();
	for (unsigned int i = 0; i < particles.size();i++){
		Vector3 pos = particles[i]->getPosition();
		glVertex3f(pos.getX()+0.10, pos.getY()+0.10, pos.getZ());
		glVertex3f(pos.getX()-0.10, pos.getY()+0.10, pos.getZ());
		glVertex3f(pos.getX()-0.10, pos.getY()-0.10, pos.getZ());
		glVertex3f(pos.getX()+0.10, pos.getY()-0.10, pos.getZ());
		glVertex3f(pos.getX(), pos.getY()+0.10, pos.getZ()+0.10);
		glVertex3f(pos.getX(), pos.getY()+0.10, pos.getZ()-0.10);
		glVertex3f(pos.getX(), pos.getY()-0.10, pos.getZ()-0.10);
		glVertex3f(pos.getX(), pos.getY()-0.10, pos.getZ()+0.10);
	}
	glEnd();
}

void ParticleCloud::update(float d_t){
	mParticleSystem.applyForces(d_t);
	mParticleSystem.update(d_t);
}

vector<Physics* > ParticleCloud::getPhysicsList(){
	return mParticleSystem.getParticles();
}

ParticleRing::ParticleRing(int particleAmount, float maxVelocity, float maxRadius, float minRadius, Vector3 ringCentrum){
	setPosition(ringCentrum);
	ParticleCloud::createRandomParticles(particleAmount, maxVelocity, maxRadius,ringCentrum);
	//Kraft die au�erhalb des Rings nach Innen wirkt
	mParticleSystem.addForce(new DistancePointForce(maxRadius,0.001,ringCentrum));
	//Kraft die innerhalb des Rings nach Au�en wirkt
	mParticleSystem.addForce(new DistancePointForce(1.0,-0.001,ringCentrum,minRadius));
}
