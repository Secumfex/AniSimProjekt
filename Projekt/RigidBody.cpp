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
/*Renormalizer*/
void RigidBody::renormalize(){
	mQ = mQ.normalized();
	mR = mQ.computeRotationMatrix();
}

/*************Getter***********/
Vector3 RigidBody::getPosition(){
	return mCenterOfMass.getPosition();
}
float RigidBody::getMass(){
	return mCenterOfMass.getMass();
}
vector<Physics* > RigidBody::getMassPoints(){
	if(mMassPoints.size() == 0){
		vector<Physics* > mp;
		mp.push_back(&mCenterOfMass);
		return mp;
	}

	return mMassPoints;
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
Vector3 RigidBody::getAngularVelocity(){
	return mOmega;
}
Vector3 RigidBody::getAngularMomentum(){
	return mL;
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
	mQ = rotation.normalized();
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

	mCenterOfMass.setImpulse(mP);
}
void RigidBody::setVelocity(Vector3 velocity){
	mV = velocity;
	mP = mV * mMass;

	mCenterOfMass.setVelocity(velocity);
}
void RigidBody::setMass(float mass){
	mMass = mass;
	mCenterOfMass.setMass(mass);
}

/*********** Funktionalit√§t ***********/

void RigidBody::updateLinearForceAndTorque(float d_t){
	Vector3 forceAcc(0,0,0);
	Vector3 torqueAcc(0,0,0);
	Vector3 r(0,0,0);
	Vector3 f(0,0,0);
	Vector3 t(0,0,0);
	vector<Physics* > mp = getMassPoints();
	for (unsigned int i = 0; i < mp.size(); i++)
	{
		r = mp[i]->getPosition()-getPosition();
		f = mp[i]->getAccumulatedForce();

		t = r.cross(f);
		forceAcc+=f;
		torqueAcc+=t;

		mp[i]->clearAccumulatedForce();
	}
	mTorque = torqueAcc;
	mCenterOfMass.applyForce(forceAcc);
}

/**
 * Annahme: torque und force wurden f√ºr d_t bereits berechnet
 */
void RigidBody::update(float d_t){
/*******Eulerintegration***************/
	/////Update des Status
	//UpdatePosition
	//	Vector3 Xdot = mV*d_t;
	//	setPosition(mX+Xdot);

	updateLinearForceAndTorque(d_t);

	//UpdateRotation
	Quaternion OmegaQuat(0.0,mOmega);
	Quaternion Qdot = (OmegaQuat*mQ)*0.5*d_t;

	mQ += Qdot;
    mQ = mQ.normalized();


/*******Momente aktualisieren********/
	//Anwenden wirkender Kr√§fte auf Linear und Angular Momentum
//	Vector3 Pdot = mForce;
//
//	mP += Pdot;
//	mCenterOfMass.setImpulse(mP);
//
//	mForce = Vector3(0,0,0);

    mCenterOfMass.update(d_t);	//Linearen Teil von Center-Physik Updaten lassen

    setImpulse(mP = mCenterOfMass.getImpulse());
    setPosition(mCenterOfMass.getPosition());

    /*Angular Momentum aktualisieren*/
	Vector3 Ldot = mTorque;		//Moment‰nderung = Drehmoment
	mL += Ldot;					//Addiere Moment‰nderung

	mTorque = Vector3(0,0,0);

/*****Hilfsvariablen aktualisieren****/
	//Geschwindigkeit bereits ge‰ndert durch update

	//Update RotationMatrix
	mR = mQ.computeRotationMatrix();

	//Update aktuellen Tr‰gheitstensor I_inv
	Matrix3 R_trans = mR;
	R_trans.transpose();

	mIinv = mR * mIbody_inv * R_trans;
	//Update Omega
	mOmega = mIinv * mL;

	//printState();
}

RigidBlock::RigidBlock(float a, float b, float c, float mass,
	Vector3 position, Vector3 velocity,
	Quaternion rotation, Vector3 angularMomentum){
	//Status setzen
	setMass(mass);
	setPosition(position);
	setRotation(rotation);

	//Tr√§gheitstensor berechnen
	setDimensions(a,b,c);

	setVelocity(velocity);
	setAngularMomentum(angularMomentum);

	//printState();
}

void RigidBlock::setDimensions(float a,float b,float c){
	mA = a;
	mB = b;
	mC = c;
	float mass_factor = mMass / 12.0;
		Matrix3 I_body( b*b+c*c	,0		,0,
						0		,a*a+c*c,0,
						0		,0		,a*a+b*b);
	I_body *= mass_factor;
	setIbody(I_body);
}

RigidTwoMass::RigidTwoMass(float mass1, float mass2, float distance,Vector3 position, Vector3 velocity,
	Quaternion rotation, Vector3 angularMomentum){

	float dist_half = distance/2.0;
	float total_mass = mass1+mass2;
	float com_x 	 = -dist_half*(mass1 / total_mass) + dist_half*(mass2 / total_mass);

	float rel_x_m1 = - dist_half - com_x;
    float rel_x_m2 = dist_half - com_x;

   // cout << "com_x = " << com_x<< " , rel_x_m1 = " << rel_x_m1<< " , rel_x_m2 = " << rel_x_m2<<endl;

	mMassPoints.push_back(new RelativePhysics(mass1,Vector3(0,0,0),mCenterOfMass.getPositionPointer(),Vector3(rel_x_m1,0,0), &mQ));
	mMassPoints.push_back(new RelativePhysics(mass2,Vector3(0,0,0),mCenterOfMass.getPositionPointer(),Vector3(rel_x_m2,0,0), &mQ));

	float I_xx = 0;
	float I_yy = mass1 * ((rel_x_m1)*(rel_x_m1)) + mass2 * ((rel_x_m2)*(rel_x_m2));
	float I_zz = mass1 * ((rel_x_m1)*(rel_x_m1)) + mass2 * ((rel_x_m2)*(rel_x_m2));
	Matrix3 I_body( I_xx	,0	    ,	0,
					0		,I_yy 	,	0,
					0		,0		,	I_zz);
	initValues(total_mass, I_body, position, velocity, rotation, angularMomentum);
}

