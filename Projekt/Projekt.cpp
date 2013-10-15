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
	//Sonst potentielle NULL-Pointer Exception beim update
	rocketTracer = NULL;
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
}

//TODO INPUT / OUTPUT Manager
void Projekt::keyPressed(int key){
	if (key == ' '){
		if(mSceneManager.getPlayerRocket() != NULL){
			mSceneManager.getPlayerRocket()->launch();
		}
	}
	if(key == 'r'){
		mSceneManager.reset();
		if(rocketTracer!= NULL){
			rocketTracer = NULL;
		}
		if (mCamera != NULL){
			mCamera->setLookAt(Vector3(0,0,0));
			mCamera->setPosition(Vector3(0.0, 5.0, 5.0));
			mCamera->setDirection(Vector3(0.0, 0.0, -1.0));
		}
	}
	if(key == 'm'){
		mSceneManager.getPlayerRocket()->getPhysics()->switchIntegrationMode();
	}
	if(key == 't'){
		rocketTracer = new TimedPointTracer(mSceneManager.getPlayerRocket()->getPhysics()->getPositionPointer(),1000);
	}
	if(key == 'c'){
		if (mCamera != NULL){
			Vector3* rocketPos = mSceneManager.getPlayerRocket()->getPhysics()->getPositionPointer();
			mCamera->setLookAtPointer(rocketPos);
		}
	}
	if(key == 'b'){
		if (mCamera != NULL){
			mCamera->setLookAt(Vector3(0,0,0));
			mCamera->setPosition(Vector3(0.0, 5.0, 5.0));
			mCamera->setDirection(Vector3(0.0, 0.0, -1.0));
		}
	}
}

