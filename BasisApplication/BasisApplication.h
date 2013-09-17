#ifndef BASIS_APPLICATION_H
#define BASIS_APPLICATION_H

#include <string>
using std::string;

#include <stdlib.h>
#include "GL/glut.h"

class BasisApplication
{
	//---------------CONSTRUCTORS & DESTRUCTOR------------
	public:
		BasisApplication(void);
		~BasisApplication(void);

	//---------------MEMBER VARIABLES---------------------
	protected:
		string mAppTitle;

	//---------------MEMBER FUNCTIONS---------------------
	private:
		void drawFloor(float xExt, float zExt, float xRes, float zRes);
		void drawCoordinateSystem(void);

	public:
		virtual void  init(void);
		virtual void update(float d_t);
		virtual void draw(void);

		inline void setAppTitle(string t){mAppTitle = t;}
		inline string getAppTitle(void){return mAppTitle;}

		virtual void keyPressed(int key){}


};
#endif