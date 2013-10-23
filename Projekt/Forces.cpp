#include "Forces.h"

#include <iostream>
using namespace std;

/*Force Funktionalität*/
void Force::addInfluencedPhysics(Physics* physics){
	mInfluencedPhysics.push_back(physics);
}

void Force::addInfluencedPhysics(vector<Physics* > physics){
	for (unsigned int i = 0; i < physics.size(); i ++){
		addInfluencedPhysics(physics[i]);
	}
}
void Force::clearAllInfluencedPhysics(){
	mInfluencedPhysics.clear();
}


vector<Physics* > Force::getInfluencedPhysics(){
	return mInfluencedPhysics;
}
void Force::apply_fun(float d_t){
}

/*Unary Force Funktionalität*/
void UnaryForce::apply_fun(float d_t){
}

/*Binary Force Funktionalität*/
void BinaryForce::addPhysicsPair(Physics* lhs, Physics* rhs){
	mInfluencedPhysics.push_back(lhs);
	int lhs_i = mInfluencedPhysics.size()-1;
	mInfluencedPhysics.push_back(rhs);
	int rhs_i = mInfluencedPhysics.size()-1;
	mPairs.push_back(IntTupel(lhs_i,rhs_i));
}
void BinaryForce::apply_fun(float d_t){ }

/*SimpleForce Funktionalität*/
SimpleForce::SimpleForce(Vector3 force){
	mForce = force;
}
void SimpleForce::setForce(Vector3 force){
	mForce = force;
}
void SimpleForce::setForce(float x, float y, float z){
	mForce = Vector3(x,y,z);
}
void SimpleForce::setForce(Vector3 dir, float length){
	dir.normalize();
	mForce = dir*length;
}
void SimpleForce::apply_fun(float d_t){
	for (unsigned int i = 0; i < mInfluencedPhysics.size(); i++){
		mInfluencedPhysics[i]->applyForce(mForce*d_t);
	}
}
SimpleAcceleration::SimpleAcceleration(Vector3 force){
	mForce = force;
}
void SimpleAcceleration::apply_fun(float d_t){
	for (unsigned int i = 0; i < mInfluencedPhysics.size(); i++){
		float mass = mInfluencedPhysics[i]->getMass();
		mInfluencedPhysics[i]->applyForce(mForce*mass*d_t);
	}
}

/*ElasticCollision Funktionalität*/
ElasticCollision::ElasticCollision(Physics* lhs, Physics*rhs,float loss){
	addPhysicsPair(lhs,rhs);
	mRhs = rhs;
	mLhs = lhs;
	mLoss = loss;
	mLhsNormal = (rhs->getPosition()-lhs->getPosition());
	mLhsNormal.normalize();
	mRhsNormal = (-1.0)*mLhsNormal;
}
void ElasticCollision::apply_fun(float d_t) {
	//Bewegen sich von einander weg --> kein Abstoßen nötig
	if ((mLhs->getVelocity() - mRhs->getVelocity()) * mLhsNormal <= 0) {
		return;
	} else {
		mLhs->setImpulse(mLhs->getImpulse() * (1.0 - mLoss));
		mRhs->setImpulse(mRhs->getImpulse() * (1.0 - mLoss));
		Vector3 forceAgainstRhs = (mLhs->getImpulse() * (mRhsNormal))
				* mRhsNormal;
		Vector3 forceAgainstLhs = (mRhs->getImpulse() * (mLhsNormal))
				* mLhsNormal;
		Vector3 totalForceAgainstRhs = forceAgainstRhs - forceAgainstLhs;
		Vector3 totalForceAgainstLhs = forceAgainstLhs - forceAgainstRhs;

//		cout << "left impulse " << mLhs->getImpulse().length() << ", vel :"
//				<< mLhs->getVelocity().length() <<", force: " << totalForceAgainstLhs.length()<< endl;
//		cout << "right impulse " << mRhs->getImpulse().length() << ", vel :"
//				<< mRhs->getVelocity().length() <<", force: " << totalForceAgainstRhs.length()<< endl;

		mLhs->applyForce(totalForceAgainstLhs * (1.0));
		mRhs->applyForce(totalForceAgainstRhs * (1.0));
	}
}

/*ReflectiveCollision Funktionalität*/
ReflectiveCollision::ReflectiveCollision(Physics* target, Vector3& normal,float loss){
	mNormal = normal;
	addInfluencedPhysics(target);
	mTarget = target;
	mLoss = loss;
}
void ReflectiveCollision::apply_fun(float d_t){
	mTarget->setImpulse(mTarget->getImpulse()*(1.0 - mLoss));
	Vector3 imp = mTarget->getImpulse();

Vector3 forceAgainstNormal = (imp * (mNormal))*(mNormal);
Vector3 totalForceAgainstTarget = forceAgainstNormal*(-2.0);

mTarget->applyForce(totalForceAgainstTarget*(1.0));
}

/*ViscousDrag Funktionalität*/
ViscousDrag::ViscousDrag(float drag){
	setDrag(drag);
}
void ViscousDrag::setDrag(float drag){
	mDrag = drag;
}
float ViscousDrag::getDrag(){
	return mDrag;
}
void ViscousDrag::apply_fun(float d_t){
	for (unsigned int i = 0; i < mInfluencedPhysics.size();i++){
		Vector3 velocity = mInfluencedPhysics[i]->getVelocity();
		mInfluencedPhysics[i]->applyForce((-1.0)*mDrag*velocity*d_t);
	}
}

/*DampedSpring Funktionalität*/
DampedSpring::DampedSpring(Physics* lhs, Physics* rhs,float springConstant, float dampConstant, float restLength){
	if (lhs != NULL && rhs != NULL){
		addPhysicsPair(lhs,rhs);
	}
	setDampConstant(dampConstant);
	setSpringConstant(springConstant);
	setRestLength(restLength);
}
void DampedSpring::setDampConstant(float dampConstant){
	mDampConstant = dampConstant;
}
void DampedSpring::setSpringConstant(float springConstant){
	mSpringConstant = springConstant;
}
void DampedSpring::setRestLength(float restLength){
	mRestLength = restLength;
}
void DampedSpring::apply_fun(float d_t){
	for (unsigned int i = 0; i < mPairs.size(); i++){
		Physics* lhs = mInfluencedPhysics[mPairs[i].i];
		Physics* rhs = mInfluencedPhysics[mPairs[i].j];

		Vector3 pos_lhs = lhs->getPosition();
		Vector3 pos_rhs = rhs->getPosition();
		Vector3 pos_dif = (pos_rhs-pos_lhs);
		Vector3 vel_dif = rhs->getVelocity()-lhs->getVelocity();
		float pos_dif_length = pos_dif.length();

		//Tension --> "Gespanntheit" Berücksichtigt Länge der Feder in Ruheposition
		float tension = pos_dif_length-mRestLength;
		
		Vector3 f_b = pos_dif;
		f_b.normalize();
		f_b *= (mSpringConstant * tension + mDampConstant * ((vel_dif * pos_dif) / pos_dif_length))*d_t;
		Vector3 f_a = (-1.0) * f_b;
		
		lhs->applyForce(f_b);
		rhs->applyForce(f_a);
	}
}

GravitationalForce::GravitationalForce(float maxDistance, float minForce, float scale){
	setMaxDistance(maxDistance);
	setMinForce(minForce);
	setScale(scale);
}

void GravitationalForce::setMaxDistance(float maxDistance){
	mMaxDistance=maxDistance;
}
void GravitationalForce::setMinForce(float minForce){
	mMinForce=minForce;
}void GravitationalForce::setScale(float scale){
	mScale = scale;
}
float GravitationalForce::getMaxDistance(){
	return mMaxDistance;
}
float GravitationalForce::getMinForce(){
	return mMinForce;
}
float GravitationalForce::getScale(){
	return mScale;
}
void GravitationalForce::apply_fun(float d_t){
	for (unsigned int i = 0; i < mInfluencedPhysics.size()-1; i++){
		Physics* lhs = mInfluencedPhysics[i];
		for (unsigned int j = i+1 ; j < mInfluencedPhysics.size(); j++){
			Physics* rhs = mInfluencedPhysics[j];
			Vector3 distanceVector = rhs->getPosition()-lhs->getPosition();
			float distance = distanceVector.length();
			//Distanz in Ordnung
			if (distance < getMaxDistance() && distance >= 1.0){
				if(distance < 1){
					distance = 1;
				}
				distanceVector.normalize();
				//Anziehungskraft berechnen
				float force = mScale * (rhs->getMass()*lhs->getMass() / (distance*distance));
				//Anziehungskraft in Ordnung
				if (force > getMinForce()){
					Vector3 Rhs_forceVector = distanceVector;
					Rhs_forceVector *= force;
					//Anziehungskraft um d_t skaliert anwenden
					lhs->applyForce(Rhs_forceVector*d_t);
					Vector3 Lhs_forceVector = (-1.0) * distanceVector;
					Lhs_forceVector *= force;
					//Anziehungskraft um d_t skaliert anwenden
					rhs->applyForce(Lhs_forceVector*d_t);
				}
			}
		}
	}
}

DistancePointForce::DistancePointForce(float minDistance, float strength, Vector3 center, float maxDistance){
	mMinDistance = minDistance;
	mMaxDistance = maxDistance;
	mStrength = strength;
	mCenter = center;
}

void DistancePointForce::setMinDistance(float minDistance){
	mMinDistance = minDistance;
}
void DistancePointForce::setMaxDistance(float maxDistance){
	mMaxDistance = maxDistance;
}
void DistancePointForce::setStrength(float strength) {
	mStrength = strength;
}
void DistancePointForce::setCenter(Vector3 center){
	mCenter = center;
}
void DistancePointForce::apply_fun(float d_t){
	for (unsigned int i = 0; i <mInfluencedPhysics.size();i++){
		Vector3 distance = (mInfluencedPhysics[i]->getPosition()-mCenter);
		float dist = distance.length();
			if(dist > mMinDistance && dist < mMaxDistance){
				float difference = dist-mMinDistance;
				Vector3 dir = (-1.0) * distance;
				dir.normalize();
				mInfluencedPhysics[i]->applyForce(dir*mStrength*difference*d_t);
			}
	}
}
