#include "Physics.h"

	//Masse 0 -> solid
Physics::Physics(float mass, Vector3 velocity, Vector3 position){
	setMass(mass);
	setVelocity(velocity);
	setPosition(position);
	mIntegrationMode = 1;
}

Physics::~Physics(){
}

/*Hilfsfunktionen*/
void Physics::recomputeImpulse(){
	mImpulse = mMass * mVelocity;
}

void Physics::recomputeVelocity(){
	mVelocity = i_Mass*mImpulse;
}

/*Setter*/
void Physics::setVelocity(Vector3 velocity){
	if (mMass != 0){
		mVelocity = velocity;
		recomputeImpulse();
	}
}

void Physics::setImpulse(Vector3 impulse){
	if (mMass != 0){
		mImpulse = impulse;
		recomputeVelocity();
	}
}

void Physics::setMass(float mass){
	mMass = mass;
	if (mMass != 0.0){
		i_Mass = 1.0 / mMass;
	}
	else{
		i_Mass = 1.0;
	}
	recomputeImpulse();
}

void Physics::setPosition(Vector3 position){
	mPosition = position;
}

void Physics::setPosition(float x, float y, float z){
	mPosition.setX(x);
	mPosition.setY(y);
	mPosition.setZ(z);
}

/*Getter*/
float Physics::getMass() const{
	return mMass;
}

Vector3 Physics::getPosition() const{
	return mPosition;
}

Vector3* Physics::getPositionPointer(){
	Vector3* pointer = &mPosition;
	return pointer;
}


Vector3 Physics::getVelocity() const{
	return mVelocity;
}

Vector3 Physics::getImpulse() const{
	return mImpulse;
}
Vector3 Physics::getAccumulatedForce() const{
	return mForceAccumulator;
}

/*Kinematische Richtungsänderung ohne Impulserhaltungssatz
 *Richtungsvektor nach Anwendung genau an Normale normal reflektiert*/
void Physics::reflect(Vector3& normal){
	Vector3 v = getVelocity();
	Vector3 newDirection = -2*((v*normal)*normal) + v;
	setVelocity(newDirection);
}


///////////////////////////////// INTEGRATION ////////////////////////////
//////////////// RUNGE KUTTA ///////////////////
inline void Physics::rungeKutta(float d_t){
//	X[0] und X_dot ohne Zeit-Intervall (bzw. d_t = 1.0)
	Vector3 idot_0 = mForceAccumulator;	 //ist schon skaliert
	Vector3 pdot_0 = mVelocity;
	Vector3 p_0 = mPosition;
	Vector3 i_0 = mImpulse;

	//K1
	//X_dot mit Anfangswerten X = X_0
	Vector3 idot_1 = idot_0;			//Hier skaliert, dann muss sonst nix mehr skaliert werden
	Vector3 pdot_1 = pdot_0			* d_t;


	//K2
	//X_dot mit aktualisierten Werten X = X[0] + K1
    Vector3 i_2 = deriveImpulse( i_0, idot_1   , 0.5);//Neuer Impuls bei halbem Zeitschritt d_t
	Vector3 p_2 = derivePosition(p_0, pdot_1   , 0.5);//Neue Position bei halbem Zeitschritt d_t und Impuls i_0
    Vector3 idot_2 = (i_2-i_0);						//Impulsänderung    bei	T_0+0.5*d_t
    Vector3 pdot_2 = (p_2-p_0);						//Positionsänderung bei T_0+0.5*d_t

    //K3
    //X_dot mit aktualisierten Werten X = X[0] + K2
    Vector3 i_3 = deriveImpulse( i_0, idot_2, 0.5);	//Neuer Impuls bei halbem Zeitschritt d_t
	Vector3 p_3 = derivePosition(p_0, pdot_2, 0.5); //Neue Position bei halbem Zeitschritt d_t und Impuls i_0
    Vector3 idot_3 = (i_3-i_0);						//Impulsänderung    bei	T_0+0.5*d_t
    Vector3 pdot_3 = (p_3-p_0);						//Positionsänderung bei T_0+0.5*d_t

    //K4
    //X_dot mit aktualisierten Werten X = X[0] + K3
    Vector3 i_4 = deriveImpulse( i_0, idot_3, 1.0);	//Neuer Impuls bei halbem Zeitschritt d_t
	Vector3 p_4 = derivePosition(p_0, pdot_3, 1.0); //Neue Position bei halbem Zeitschritt d_t und Impuls i_0
    Vector3 idot_4 = (i_4-i_0);						//Impulsänderung    bei	T_0+0.5*d_t
    Vector3 pdot_4 = (p_4-p_0);						//Positionsänderung bei T_0+0.5*d_t

    //Endwerte setzen:
    Vector3 p_neu = p_0 + (pdot_1 + 2.0 * pdot_2 + 2.0 * pdot_3 + pdot_4) * ((1.0)/ 6.0);
    Vector3 i_neu = i_0 + (idot_1 + 2.0 * idot_2 + 2.0 * idot_3 + idot_4) * ((1.0)/ 6.0);
    setPosition(p_neu);
    setImpulse(i_neu);
    clearAccumulatedForce();
}

//liefert den Impuls bei Zeitschritt d_t von übergebenen Werten aus zurück
inline Vector3 Physics::deriveImpulse(Vector3 imp, Vector3 i_dot, float d_t){
	return  imp + i_dot * d_t;
}

//liefert die Positionbei Zeitschritt d_t von übergebenen Werten aus zurück
inline Vector3 Physics::derivePosition(Vector3 pos, Vector3 p_dot, float d_t){
	return pos + p_dot * d_t;
}
////////////////////////////////////////////////
//////////////// EULER STEP ////////////////////
inline void Physics::eulerStep(float d_t){
	applyAccumulatedForce();
	setPosition( mPosition + (mVelocity * d_t));
}

/*Kraft wird angewendet*/
inline void Physics::applyAccumulatedForce(){
	setImpulse(mImpulse+mForceAccumulator);
	clearAccumulatedForce();
}
///////////////////////////////////////////////
/*Update des Zustandes in Abhängigkeit von Zeitintervall d_t : Euler Integration oder Runge Kutta*/
void Physics::update(float d_t){
	switch(mIntegrationMode){
	case 0:
		eulerStep(d_t);
		break;
	case 1:
		rungeKutta(d_t);
		break;
	}
}


void Physics::switchIntegrationMode(){
	if(mIntegrationMode == 1){
		mIntegrationMode = 0;
		cout<<"switched to mode: Euler-Integration"<<endl;
	}
	else{
		mIntegrationMode = 1;
		cout<<"switched to mode: Runge-Kutta"<<endl;
	}
}

/////////////////////////////////////////////////////////////////////////


/*Akkumulator wird genullt*/
void Physics::clearAccumulatedForce(){
	mForceAccumulator = Vector3(0,0,0);
}

/*Force wird akkumuliert, es sei denn:
 *Masse = 0 --> nix passiert
 */void Physics::applyForce(Vector3 force){
	if (mMass != 0){
		mForceAccumulator+=force;
	}
}

/* Elastischer Stoß zweier Massepunkte
 * Input:
 * Linker  Massepunkt lhs
 * Rechter Massepunkt rhs
 * Linke  OberflächenNormale rhsNormal (in Richtung rechter Massepunkt)
 * Rechte OberflächenNormale lhsNormal (in Richtung linker  Massepunkt)
 * Nach Anwendung:
 * Kraft in lhs und rhs akkumuliert
 */
void collide(Physics* lhs, const Vector3& lhsNormal, Physics* rhs, const Vector3& rhsNormal){
	if ((lhs->getVelocity() - rhs->getVelocity()) * lhsNormal <= 0){
		return;
	} 
	else{
	 Vector3 forceAgainstRhs = (lhs->getImpulse() * (rhsNormal)) * rhsNormal;
	 Vector3 forceAgainstLhs = (rhs->getImpulse() * (lhsNormal)) * lhsNormal;
	 Vector3 totalForceAgainstRhs = forceAgainstRhs - forceAgainstLhs;
	 Vector3 totalForceAgainstLhs = forceAgainstLhs - forceAgainstRhs;
	 lhs->applyForce(totalForceAgainstLhs);
	 rhs->applyForce(totalForceAgainstRhs);
	}
}
RelativePhysics::RelativePhysics(float mass, Vector3 velocity, Vector3* centerPosition, Vector3 relativePosition, Quaternion* rotation){
	setMass(mass);
	setVelocity(velocity);
	setCenterPositionPointer(centerPosition);
	setRelativePosition(relativePosition);
	relativePosition.debugPrintToCerr();
	setRotationPointer(rotation);
	mIntegrationMode = 1;

}
void RelativePhysics::setRelativePosition(Vector3 relativePosition){
	mRelativePosition = relativePosition;
}
void RelativePhysics::setRelativePosition(float x, float y, float z){
	mRelativePosition = Vector3(x,y,z);
}
void RelativePhysics::setRotationPointer(Quaternion* rotation){
	mRotation = rotation;
}
Vector3 RelativePhysics::getRelativePosition() const{
	return mRelativePosition;
}
Vector3* RelativePhysics::getRelativePositionPointer(){
	return &mRelativePosition;
}
Vector3* RelativePhysics::getPositionPointer(){
	return mCenterPosition;
}
void RelativePhysics::setPosition(Vector3 position){
	*mCenterPosition = position;
}
void RelativePhysics::setPosition(float x, float y, float z){
	*mCenterPosition = Vector3(x,y,z);
}

void RelativePhysics::setCenterPositionPointer(Vector3* centerPosition){
	mCenterPosition=centerPosition;
}
Vector3* RelativePhysics::getCenterPositionPointer(){
	return mCenterPosition;
}
//Gibt statt der normalen Position die rotierte um relative position verschobene CenterPosition zurück
Vector3 RelativePhysics::getPosition() const{
	Matrix3 rot = mRotation->computeRotationMatrix();
	Vector3 res = rot*mRelativePosition;
	res += *mCenterPosition;
	return res;
}
