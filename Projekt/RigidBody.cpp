/*
 * RigidBody.cpp
 *
 *  Created on: 15.10.2013
 *      Author: Arend
 */

#include "RigidBody.h"

RigidBody::RigidBody(float mass, Matrix3 Ibody ,
		  Vector3 position, Vector3 velocity,
		  Quaternion rotation , Vector3 angularMomentum ) {
	initValues( mass, Ibody ,
		  position, velocity,
		  rotation , angularMomentum);
}

void RigidBody::initValues(float mass, Matrix3 Ibody ,
			  Vector3 position, Vector3 velocity,
			  Quaternion rotation , Vector3 angularMomentum ){
		setPosition(position);
		setRotation(rotation);

		setMass(mass);
		setIbody(Ibody);
		setVelocity(velocity);

		setAngularMomentum(angularMomentum);
}
RigidBody::~RigidBody() {
	// TODO Auto-generated destructor stub
}
/*************Getter***********/
Vector3 RigidBody::getPosition(){
	return mCenterOfMass.getPosition();
}
Vector3 RigidBody::getImpulse(){
	return mCenterOfMass.getImpulse();
}
Vector3 RigidBody::getVelocity(){
	return mCenterOfMass.getVelocity();
}
Quaternion RigidBody::getRotationQuaternion(){
	return mQ;
}
Matrix3 RigidBody::getRotationMatrix(){
	return mR;
}
/*************Setter***********/
void RigidBody::setIbody(Matrix3 Ibody){
	mIbody = Ibody;
	Ibody.invert();
	mIbody_inv = Ibody;

	Matrix3 R_trans = mR;
	R_trans.transpose();
	mIinv =  mR * mIbody_inv * R_trans;
}
void RigidBody::setPosition(Vector3 position){
	mX = position;
	mCenterOfMass.setPosition(position);
}
void RigidBody::setPosition(float x, float y, float z){
	mX = Vector3(x,y,z);
	mCenterOfMass.setPosition(Vector3(x,y,z));
}
void RigidBody::setRotation(Quaternion rotation){
	mQ = rotation;

	Quaternion normQ = mQ.normalized();
	mR = normQ.computeRotationMatrix();
}

void RigidBody::setAngularMomentum(Vector3 angularMomentum){
	mL = angularMomentum;
	mOmega = mIinv * mL;
}

void RigidBody::setOmega(Vector3 omega){
	mOmega = omega;
}

void RigidBody::setImpulse(Vector3 impulse){
	mP = impulse;
	if(mMass != 0.0){
		mV = mP * ((1.0) / mMass);
	}
}
void RigidBody::setVelocity(Vector3 velocity){
	mV = velocity;
	mP = mV * mMass;
}
void RigidBody::setMass(float mass){
	mMass = mass;
	mCenterOfMass.setMass(mass);
}

/*********** Funktionalität ***********/

/**
 * Annahme: torque und force wurden für d_t bereits berechnet
 */
void RigidBody::update(float d_t){
/*******Eulerintegration***************/
	/////Update des Status
	//UpdatePosition
	Vector3 Xdot = mV*d_t;
	setPosition(mX+Xdot);

	//UpdateRotation
	Quaternion OmegaQuat(0.0,mOmega);
	Quaternion Qdot = (OmegaQuat*mQ)*0.5;
	//Qdot.debugPrintToCerr();
	mQ += Qdot;
    mQ = mQ.normalized();
	//mQ.debugPrintToCerr();


/*******Momente aktualisieren********/
	//Anwenden wirkender Kräfte auf Linear und Angular Momentum
	Vector3 Pdot = mForce;

	mP += Pdot;
	mCenterOfMass.setImpulse(mP);

	mForce = Vector3(0,0,0);

	Vector3 Ldot = mTorque;
	mL += Ldot;
	mTorque = Vector3(0,0,0);

/*****Hilfsvariablen aktualisieren****/
	//UpdateVelocity
	if (mMass != 0){
	mV = mP * ((1.0)/ mMass);}

	//Update RotationMatrix
	Quaternion normQ(mQ);
	normQ=normQ.normalized();
	mR = normQ.computeRotationMatrix();

	//Update I_inv
	Matrix3 R_trans = mR;
	R_trans.transpose();
	mIinv = mR * mIbody_inv * R_trans;
	//Update Omega
	mOmega = mIinv * mL;

	//printState();
}

RigidBlock::RigidBlock(float a, float b, float c,
	Vector3 position, Vector3 velocity,
	Quaternion rotation, Vector3 angularMomentum){

	//Status setzen
	setPosition(position);
	setRotation(rotation);

	//Trägheitstensor berechnen
	setDimensions(a,b,c);

	setVelocity(velocity);
	setAngularMomentum(angularMomentum);

	//printState();
}

void RigidBlock::setDimensions(float a,float b,float c){
	mA = a;
	mB = b;
	mC = c;

	setMass( a*b*c );
		Matrix3 I_body( b*b+c*c	,0		,0,
						0		,a*a+c*c,0,
						0		,0		,a*a+b*b);
	setIbody(I_body);

}
