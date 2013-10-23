#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"
#include "Matrix3.h"
#include <vector>

using namespace std;

//Forwarding Declaration von Forces, da Include nicht möglich

/*Klasse Physics aka. Massepunkt
 * Status zu jeder Zeit bestimmt durch:
 * 		[Position,Impuls]
 * Abgeleitet:
 * 		Velocity, i_Mass: Werden oft bei Berechnungen benötigt
 * ForceAkkumulator: in jedem Iterationsschritt sollen Wirkende Kräfte in ihm akkumuliert sein,
 * 					 beim Update des Massepuntkes wird die Kraft auf den Impuls angewendet und genullt
 * Mass: 	Masse, 0.0 bedeutet Statisches Objekt dessen Impuls nicht geändert werden kann*/

class Physics{

	protected:
	Vector3 mPosition;
	Vector3 mImpulse;
	Vector3 mVelocity;
	Vector3 mForceAccumulator;

	float mMass;  //masse == 0 --> statisches Objekt
	float i_Mass; //inverted Mass

	//Je nachdem was man in update() möchte
	int mIntegrationMode;
	void eulerStep(float d_t);
	void rungeKutta(float d_t);

	//Anwenden des akkumulierten Kraftvektors auf Impuls
	void applyAccumulatedForce();

	//Ableitung bei Zeitschritt d_t (ausgehend von übergebenen Werten)
	Vector3 evaluatePosition(Vector3 pos, Vector3 p_dot, float d_t);
	//Ableitung bei Zeitschritt d_t (ausgehend von übergebenen Werten)
	Vector3 evaluateImpulse(Vector3 imp, Vector3 i_dot, float d_t);

	public:
	//Konstruktor
	Physics(float mass = 0.0, Vector3 velocity = Vector3(0,0,0), Vector3 position = Vector3(0,0,0));
	~Physics();

	//Kraft Anwenden und anschließend Position aktualisieren
	void update(float d_t);

	//Umschalten zwischen Euler und Runge-Kutta
	void switchIntegrationMode();

	//Anwenden eines Kraftvektors force, wird in ForceAkkumulator gespeist
	void applyForce(Vector3 force);

	//Nullen des Akkumulators
	void clearAccumulatedForce();

	//Setter
	virtual void setImpulse(Vector3 impulse);
	void setVelocity(Vector3 velocity);
	void setMass(float mass);
	virtual void setPosition(Vector3 position);
	virtual void setPosition(float x, float y, float z);
	//Getter
	virtual Vector3 getPosition() const;
	virtual Vector3* getPositionPointer();
	Vector3 getVelocity() const;
	virtual Vector3 getImpulse() const;
	virtual Vector3* getImpulsePointer();
	Vector3 getAccumulatedForce() const;

	float getMass() const;

	//Elastisches Abprallen an Oberflächennormalen (Kinematisch)
	void reflect(Vector3& normal);
	//Hilfsfunktionen ohne weitere Bedeutung
	void recomputeImpulse();
	void recomputeVelocity();
};

//Wie Physics, es wird aber davon ausgegangen dass sich die Achse um die sich der Massepunkt dreht wo anders befindet
class RelativePhysics : public Physics{
protected:
	Vector3* mCenterPosition;	//Position des Zentrum ein Pointer
	Vector3 mRelativePosition; 	//Relative Position zum Mittelpunkt unrotiert (mPosition)
	Quaternion* mRotation;		//Pointer zum Rotationsquaternion
	Vector3* mImpulseOverride;	//Pointer zu einem anderen Impuls
public:
	RelativePhysics(float mass = 0.0, Vector3 velocity = Vector3(0,0,0), Vector3* centerPosition = new Vector3(0,0,0), Vector3 relativePosition = Vector3(0,0,0), Quaternion* rotation = new Quaternion(0,0,1,0));

	//TODO WHYYYYYYYYYYYYYYYYYYYYYYYY
	Vector3 getPosition() const;
	Vector3 getRelativePosition() const;
	Vector3 getCenterPosition() const;

	Vector3  getImpulse() const;

	Vector3* getPositionPointer();
	Vector3* getCenterPositionPointer();
	Vector3* getRelativePositionPointer();


	void setPosition(Vector3 position);
	void setPosition(float x, float y, float z);
	void setCenterPositionPointer(Vector3* centerPosition);
	void setImpulseOverride(Vector3* impPointer);
	void setImpulse(Vector3 impulse);
	void setRelativePosition(Vector3 relativePosition);
	void setRelativePosition(float x, float y, float z);
	void setRotationPointer(Quaternion* rotation);
};


//Elastische Kollision von zwei Massepunkten << Alte Version, Lieber: Force-Klasse verwenden
void collide(Physics* lhs, const Vector3& lhsNormal, Physics* rhs, const Vector3& rhsNormal);

#endif
