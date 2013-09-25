#include "Projekt.h"

Projekt::Projekt(){
	
}

Projekt::~Projekt(){

}

void Projekt::init(){
	mSceneManager.init();
}

void Projekt::draw(){
	BasisApplication::draw();
	mSceneManager.draw();
}

void Projekt::update(float d_t){
	BasisApplication::update(d_t);
	mSceneManager.update(d_t);
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
	}
}

