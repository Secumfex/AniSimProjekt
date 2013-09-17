#ifndef PROJEKT_H
#define PROJEKT_H

#include <iostream>
#include "BasisApplication.h"

using namespace std;

class Projekt : public BasisApplication
{
	//---------------CONSTRUCTORS & DESTRUCTOR------------
	public:
		Projekt(void);
		~Projekt(void);

	//---------------MEMBER VARIABLES---------------------
	protected:

	//---------------MEMBER FUNCTIONS---------------------
	private:
	public:
		virtual void init(void);
		virtual void update(float d_t);
		virtual void draw(void);
};
#endif
