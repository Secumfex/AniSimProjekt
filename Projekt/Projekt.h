#ifndef PROJEKT_H
#define PROJEKT_H

#include <iostream>
#include "BasisApplication.h"
#include "SceneManager.h"

using namespace std;

class Projekt : public BasisApplication
{
	//---------------CONSTRUCTORS & DESTRUCTOR------------
	public:
		Projekt(void);
		~Projekt(void);

	//---------------MEMBER VARIABLES---------------------
	protected:
		SceneManager mSceneManager;

	//---------------MEMBER FUNCTIONS---------------------
	private:
		TimedPointTracer* rocketTracer;
	public:

		virtual void init(void);
		virtual void update(float d_t);
		virtual void draw(void);

		virtual void keyPressed(int key);
};
#endif
