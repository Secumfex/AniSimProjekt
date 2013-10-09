#include "Projekt.h"

Projekt::Projekt(){
	cout<<"keyboard settings: "<<endl<<
	      "  SPACE : launch missile"<<endl<<
		  "  r     : reset"<<endl<<
		  "  m     : switch integration mode"<<endl<<
	  	  "  t     : show rocket trace"<<endl;
}

Projekt::~Projekt(){
	}

void Projekt::init(){
	mSceneManager.init();



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

//TODO anders
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
	}
	if(key == 'm'){
		mSceneManager.getPlayerRocket()->getPhysics()->switchIntegrationMode();
	}
	if(key == 't'){
		rocketTracer = new TimedPointTracer(mSceneManager.getPlayerRocket()->getPhysics()->getPositionPointer());
	}
}

