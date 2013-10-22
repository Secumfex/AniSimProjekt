#ifndef FORCES_H
#define FORCES_H

/**
 * Diese Datei enthält alle Deklarationen von Force-Klassen
 */

#include <vector>
#include "Vector3.h"
#include "Physics.h"
#include "UtilityClasses.h"

using namespace std;

//Forwarding Declaration von Physics, weil kein gegenseitiger include möglich ist (OOP Logik)
class Physics;

/**
 * Klasse Force:
 * Kraft-Klasse. Umfasst vor allem aber nicht nur Kraftdarstellungen als Vektor.
 * Jedesmal wenn apply_fun() aufgerufen wird, wird die Kraft
 * auf ihre definierte Art und Weise auf den Massepunkt angewendet.
 *
 * mInfluencedPhysics: Eine Kraftinstanz umfasst immer eine Liste der beeinflussten Massepunkten
 *
 * apply_fun(): Je nach Art der Kraft, wird diese Funktion anders ausgewertet
 * in der Regel wird Physics::applyForce(Vector3 force) ausgeführt, also ein Kraft-Vektor angewendet
 *
 */
class Force {
private:
protected:
	//Liste der beeinflussten Massepunkten
	vector<Physics* > mInfluencedPhysics;
public:
	//Hinzufügen eines beeinflussten Massepunkte
	void addInfluencedPhysics(Physics* physics);
	//Hinzufügen einer Liste von zu beinflussenden Massepunkten
	void addInfluencedPhysics(vector <Physics* > physics);
	//Löschen der Liste aller Beeinflussten Massepunkte
	virtual void clearAllInfluencedPhysics();
	//Getter der Liste der beeinflussten Massepunkten
	vector<Physics* > getInfluencedPhysics(); 
	//Anwenden der Force-Klasse auf ihre spezielle Weise
	virtual void apply_fun(float d_t);
};

//Wirkt auf jede beeinflusste Physik unabhängig von einander
class UnaryForce: public Force{
public:
	virtual void apply_fun(float d_t);
};

//Wirkt jeweils auf ein Massepunktpaar in Abhängigkeit von einander
class BinaryForce : public Force{
private:
protected:
	//Liste der Massepunktpaaren
	vector<IntTupel> mPairs;
public:
	//Hinzufügen eines Massepunktpaars
	void addPhysicsPair(Physics* lhs, Physics* rhs);
	virtual void apply_fun(float d_t);
};

/* einfacher Kraftvektor */
class SimpleForce : public UnaryForce{
protected:
	//Kraftvektor
	Vector3 mForce;
private:
public:	
	//Neuer Kraftvektor, default: Nullvektor
	SimpleForce(Vector3 force = Vector3(0,0,0));
	//Vektor dessen Länge der Kraft entspricht
	void setForce(Vector3 force);
	//Vektor dessen Länge der Kraft entspricht, Skalar
	void setForce(float x, float y, float z);
	//Richtung und Länge der Kraft
	void setForce(Vector3 dir, float length);
	//Anwenden der Kraft skaliert um d_t
	virtual void apply_fun(float d_t);
};


/*Wechselwirkende Kraft bei Elastischer Kollision zweier Massepunkte*/
class ElasticCollision : public BinaryForce{
private:
	Physics* mLhs;
	Physics* mRhs;

	//Normale berechnet durch Positionsdifferenz
	Vector3 mLhsNormal;
	//Normale berechnet durch Positionsdifferenz
	Vector3 mRhsNormal;
public:
	ElasticCollision(Physics* lhs, Physics* rhs);
	virtual void apply_fun(float d_t);
};

/*Reflektieren an einer Normalen*/
class ReflectiveCollision : public SimpleForce{
private:
	//Reflexionsnormale
	Vector3 mNormal;
	//Zu Reflektierender Massepunkt
	Physics* mTarget;
public:
	//Zu Reflektierender Massepunkt und Normale an der Reflektiert werden soll
	ReflectiveCollision(Physics* target, Vector3& normal);
	//Reflektiert bei voller Impulserhaltung den Massepunkt an der Normalen
	virtual void apply_fun(float d_t);
};

/*Strömungswiderstand (Luft-)
 * Verlangsamt bei jedem update die Geschwindigkeit des Massepunktes*/
class ViscousDrag : public UnaryForce{
private:
	//Stärke des Widerstandes
	float mDrag;
public:
	//Default: 1% Negative Beschleunigung
	ViscousDrag(float drag = 0.01);
	virtual void apply_fun(float d_t);
	float getDrag();
	void setDrag(float drag);
};

/*Federkraft zwischen zwei Physics
 * SpringConstant: Federkonstante (hoher Wert --> Hohe "Jigglyness")
 * DampConstant: Dämpfungskonstante
 * RestLength: Länge in Ruheposition*/
class DampedSpring : public BinaryForce{
private:
	//Federkonstante (hoher Wert --> Hohe "Jigglyness")
	float mSpringConstant;
	//Dämpfung
	float mDampConstant;
	//Länge der Feder in Ruhe-Position
	float mRestLength;
public:
	DampedSpring(Physics* lhs = NULL, Physics* rhs = NULL, float springConstant = 5.0, float dampConstant = 0.1, float restLength = 1.0);
	void setSpringConstant(float springConstant);
	void setDampConstant(float dampConstant);
	void setRestLength(float restLength);
	virtual void apply_fun(float d_t);
};

/**
 * Anziehungskraft zwischen Massepunkten
 * Alle beeinflussten Massepunkte ziehen sich gegenseitig an
 *
 * maxDistance: maximale Entfernung zu einander bei der die Anziehungskraft noch berechnet wird
 * minForce: 	minimale Kraftgröße, die angewendet wird. Sonst wird die Kraft verworfen (--> sehr leichte Partikel üben keine Anziehungskraft aus)
 * scale: 		Skalierungsfaktor, der Anziehungskräfte zusätzlich skaliert, muss je nach Simulation angepasst werden
 */
class GravitationalForce : public Force{
private:
	//Maximal zulässige Distanz zweier Massepunkte
	float mMaxDistance;
	//Minimal zulässiger Kraftwert der angewendet wird
	float mMinForce;
	//Skalierungsfaktor (je nach Simulation anpassen)
	float mScale;
public:
	GravitationalForce(float maxDistance = 10.0, float minForce = 0.000001, float scale = 1.0);
	void setMaxDistance(float maxDistance);
	void setMinForce(float minForce);
	void setScale(float minForce);
	float getMaxDistance();
	float getMinForce();
	float getScale();
	virtual void apply_fun(float d_t);
};

#endif
