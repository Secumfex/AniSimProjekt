#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include <iostream>
#include <vector>
#include "Vector3.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "Physics.h"

using namespace std;
/**
 * Implementation nach Rigid Body Dynamics 1
 */
class RigidBody {
protected:

	vector<Physics> mMassPoints;
	Physics mCenterOfMass; //Lineare Kräfte können direkt angewandt werden

	//Konstanten (meistens)
	float mMass;
	Matrix3 mIbody;		//Trägheitstensor
	Matrix3 mIbody_inv;

	//Status Variablen
	Vector3 mX;		//Position
	Quaternion mQ;		//Rotation
	Vector3 mP;			//Impuls
	Vector3 mL;			//Drehimpuls (nicht zu verwechseln mit Drehmoment)

	//Ableitungen
	Matrix3 mIinv;		//Inverser Trägheitstensor
	Matrix3 mR;			//Rotation als Matrix
	Vector3 mV;			//Lineare Geschwindigkeit
	Vector3 mOmega;		//Winkelgeschwindigkeit?

	//Berechneter kram
	Vector3 mForce;		// Impuls auf shit
	Vector3 mTorque;	// Drehmoment <<-- Kraft einfluss auf Drehimpuls

public:
	 RigidBody(float mass = 1.0, Matrix3 Ibody = Matrix3(1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0),
			  Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0),
			  Quaternion rotation = Quaternion(), Vector3 angularMomentum = Vector3(0,0,0));
	virtual ~RigidBody();

	/*Getter und Setter*/

	Vector3 getPosition();
	Vector3 getImpulse();
	Vector3 getVelocity();
	Quaternion getRotationQuaternion();
	Matrix3 getRotationMatrix();

	void setMass(float mass);
	void setIbody(Matrix3 Ibody);
	void setPosition(Vector3 position);
	void setPosition(float x = 0.0, float y = 0.0, float z = 0.0);
	void setRotation(Quaternion rotation);
	void setAngularMomentum(Vector3 angularMomentum);
	void setOmega(Vector3 omega);
	void setImpulse(Vector3 impulse);
	void setVelocity(Vector3 velocity);

	void initValues(float mass = 1.0,
			Matrix3 Ibody = Matrix3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
					1.0), Vector3 position = Vector3(0, 0, 0),
			Vector3 velocity = Vector3(0, 0, 0), Quaternion rotation =
					Quaternion(), Vector3 angularMomentum = Vector3(0, 0, 0));

	/*Misc*/
	void applyForceAndTorque();
	void update(float d_t);
	void printState();

};

/**
 * Ein Stab mit 2 Massepunkten
 */
class RigidStick : public RigidBody{

};

//Ein Block mit SeitenlÃ¤ngen a, b, c
class RigidBlock : public RigidBody{
protected:
	float mA;
	float mB;
	float mC;
public:
	RigidBlock(float a = 1.0, float b = 1.0, float c = 1.0,
			Vector3 position = Vector3(0,0,0), Vector3 velocity = Vector3(0,0,0),
			Quaternion rotation = Quaternion(0,1,0,0), Vector3 angularMomentum = Vector3(0,0,0));
	void setDimensions(float a, float b, float c);
};

#endif /* RIGIDBODY_H_ */
