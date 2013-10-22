#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Physics.h"
#include "Forces.h"

class ParticleSystem{
protected:
	vector<Physics* > mParticles;	//Vektor aller Partikel des Partikelsystems
	vector<Force* > mForces;		//Vektor aller INNERHALB dieses Partikelsystems wirkenden Kräfte
public:
	virtual void addParticle(Physics* particle);
	virtual void addParticles(vector<Physics *> particles);

	//Löschen aller Referenzen in Forces und Particles
	virtual void clearAllParticles();
	
	void addForce(Force* force);

	/*Getter */
	vector<Physics* > getParticles();
	vector<Force* > getForces();

	virtual void applyForces(float d_t);
	virtual void update(float d_t);
};

class ParticleEmitter : public ParticleSystem{
protected:
	float mParticleLifeTime;
	int mMaxAmount;
	Vector3* mEmissionForce;
public:
	ParticleEmitter(int maxAmount = 100, float particleLifeTime = 0.0, Vector3* mEmissionForce = new Vector3(1,0,0));
	virtual void update(float d_t);
};

class DampedSpringChain : public ParticleSystem{
protected:
	DampedSpring* mDampedSpring;
public:
	DampedSpringChain(Physics* start= NULL, DampedSpring* dampedSpring = NULL);
	virtual void addParticle(Physics* particle);
	void setDampedSpring(DampedSpring* force);
	virtual void addParticles(vector<Physics *> particles);
};

#endif
