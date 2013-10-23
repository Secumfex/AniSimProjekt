#include "Projekt.h"

Projekt::Projekt(){
	cout<<"keyboard settings: "<<endl<<
	      "  SPACE : launch missile"<<endl<<
		  "  r     : reset"<<endl<<
		  "  m     : switch integration mode"<<endl<<
	  	  "  t     : show rocket trace"<<endl<<
	  	  "  c     : camera look at rocket"<<endl<<
	  	  "  b     : camera look at origin"<<endl;
}

Projekt::~Projekt(){
	}

void Projekt::init(){
	mSceneManager.init();
	mController.setScene(&mSceneManager);
	mController.setCameraPointer(mCamera);
	rocketTracer = new TimedPointTracer(mSceneManager.getPlayerRocket()->getPositionPointer(),1000);
	mController.setTracer(rocketTracer);
	mController.resetObjectPointers();
	mController.setCollisionsManager(&mCollision);

	mCollision.setScene(&mSceneManager);
}

void Projekt::draw(){
	BasisApplication::draw();
	mSceneManager.draw();

	if(rocketTracer != NULL){
		drawTracer(rocketTracer);
	}
}

void Projekt::update(float d_t){
	BasisApplication::update(d_t);
	mSceneManager.update(d_t);

	/*Tracer*/
	if(rocketTracer != NULL){
		rocketTracer->trace(d_t);
	}

	mCollision.collisionCheck(d_t);
	if(mCollision.isCollision()){
//		cout << "Hallo Mama" <<endl;
				mCollision.explosion(d_t);
	}

	mCollision.collisionCheckAll();
	mCollision.applyAndEmptyCollisionForces(d_t);
}

//TODO INPUT / OUTPUT Manager
void Projekt::keyPressed(int key){
	mController.keyPressed(key);
}

