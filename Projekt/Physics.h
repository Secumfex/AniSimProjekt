#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"
#include <vector>

using namespace std;

//Forwarding Declaration von Forces, da Include nicht möglich
class Forces;

/*Klasse Physics aka. Massepunkt
 * Status zu jeder Zeit bestimmt durch:
 * 		[Position,Impuls]
 * Abgeleitet:
 * 		Velocity, i_Mass: Werden oft bei Berechnungen benötigt
 * ForceAkkumulator: in jedem Iterationsschritt sollen Wirkende Kräfte in ihm akkumuliert sein,
 * 					 beim Update des Massepuntkes wird die Kraft auf den Impuls angewendet und genullt
 * Mass: 	Masse, 0.0 bedeutet Statisches Objekt dessen Impuls nicht geändert werden kann*/

class Physics{

	private:
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
	Vector3 derivePosition(Vector3 pos, Vector3 p_dot, float d_t);
	//Ableitung bei Zeitschritt d_t (ausgehend von übergebenen Werten)
	Vector3 deriveImpulse(Vector3 imp, Vector3 i_dot, float d_t);

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
	void setImpulse(Vector3 impulse);
	void setVelocity(Vector3 velocity);
	void setMass(float mass);
	void setPosition(Vector3 position);
	void setPosition(float x, float y, float z);
	//Getter
	Vector3 getPosition() const;
	Vector3* getPositionPointer();
	Vector3 getVelocity() const;
	Vector3 getImpulse() const;
	float getMass() const;

	//Elastisches Abprallen an Oberflächennormalen (Kinematisch)
	void reflect(Vector3& normal);
	//Hilfsfunktionen ohne weitere Bedeutung
	void recomputeImpulse();
	void recomputeVelocity();

};

	/*Andere Hilfsfunktionen*/
////Zeichnet Richtungsvektor des input Massepunktes
//void drawDirection(const Physics& input);
////Zeichnet
//void drawInfluenceDirection(const Physics& target, const Physics& infl);
//float getDistance(const Physics& lhs, const Physics& rhs);

//Elastische Kollision von zwei Massepunkten << Alte Version, Lieber: Force-Klasse verwenden
void collide(Physics* lhs, const Vector3& lhsNormal, Physics* rhs, const Vector3& rhsNormal);

#endif
