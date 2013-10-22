#include "ParticleSystem.h"
#include <iostream>
using namespace std;

void ParticleSystem::addParticle(Physics* particle){
	mParticles.push_back(particle);
	for (unsigned int i = 0; i < mForces.size(); i ++){
		mForces[i]->addInfluencedPhysics(particle);
	}
}

void ParticleSystem::addParticles(vector<Physics* > particles){
	for (unsigned int i = 0; i < particles.size(); i ++){
		mParticles.push_back(particles[i]);
	}
}
vector<Physics* > ParticleSystem::getParticles(){
	return mParticles;
}

void ParticleSystem::clearAllParticles(){
	for(unsigned int i = 0; i < mForces.size();i++ ){
		mForces[i]->clearAllInfluencedPhysics();
	}
	mParticles.clear();

}
void ParticleSystem::applyForces(float d_t){
	for (unsigned int i = 0; i < mForces.size(); i++){
		mForces[i]->apply_fun(d_t);
	}
}

void ParticleSystem::addForce(Force* force){
	mForces.push_back(force);
	for (unsigned int i = 0; i < mParticles.size(); i++){
		force->addInfluencedPhysics(mParticles[i]);
	}
}

void ParticleSystem::update(float d_t){
	for (unsigned int i = 0; i < mParticles.size(); i++){
		mParticles[i]->update(d_t);
	}
}

ParticleEmitter::ParticleEmitter(int maxAmount, float particleLifeTime, Vector3* mEmissionForce){

}

void ParticleEmitter::update(float d_t){

}

void DampedSpringChain::addParticles(vector<Physics* > particles){
	for (unsigned int i = 0; i < particles.size(); i++){
		addParticle(particles[i]);
	}
	//cout<<"chain length: " <<mParticles.size()<<endl;
}

DampedSpringChain::DampedSpringChain(Physics* start, DampedSpring* dampedSpring){
	//mParticles.push_back(start);
	mDampedSpring = dampedSpring;
	mForces.push_back(mDampedSpring);
}

void DampedSpringChain::setDampedSpring(DampedSpring* dampedSpring){
	if (mDampedSpring != NULL){
		mForces.clear();
	}
	mForces.push_back(mDampedSpring);
	mDampedSpring = dampedSpring;

}

void DampedSpringChain::addParticle(Physics* particle){
	if (mParticles.size() >= 1){
		Physics* end = mParticles[mParticles.size()-1];
		mDampedSpring->addPhysicsPair(end,particle);
	}
	mParticles.push_back(particle);
}
