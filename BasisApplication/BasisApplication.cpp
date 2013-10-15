#include <windows.h>
#include "BasisApplication.h"


BasisApplication::BasisApplication(void)
{
	mAppTitle = "AnSim SS 2010 Demo Framework";
	mCamera = NULL;
}


BasisApplication::~BasisApplication(void)
{
}


void BasisApplication::update(float d_t){
}


void BasisApplication::init(void){
}


void BasisApplication::draw(void){
	drawFloor(10.0f, 10.0f, 10, 10);
	drawCoordinateSystem();

}


void BasisApplication::drawFloor(float xExt, float zExt, float xRes, float zRes){
	float col0 [] = {0.4, 0.4, 0.4};
	float col1 [] = {0.6, 0.6, 0.6};
	float *col = NULL;

	
	float xStart = -0.5 * xExt;
	float zStart = -0.5 * zExt;
	float xStep = xExt / xRes;
	float zStep = zExt / zRes;

	
	for(int j=0; j<zRes; j++){
		for(int i=0; i<xRes; i++){
			if(j % 2 == 0){
				(i % 2 == 0) ? col = col0 : col = col1;
			}
			else{
				(i % 2 == 0) ? col = col1 : col = col0;
			}
			
			
			float xl = xStart + i * xStep;
			float xh = xStart + (i + 1) * xStep;
			float zl = zStart + j * zStep;
			float zh = zStart + (j + 1) * zStep;


			glColor3fv(col);
			glBegin(GL_QUADS);
			glVertex3f(xl, 0.0, zl);
			glVertex3f(xl, 0.0, zh);
			glVertex3f(xh, 0.0, zh);
			glVertex3f(xh, 0.0, zl);
			glEnd();

		}
	}
}


void BasisApplication::drawCoordinateSystem(void){
	
	glDepthFunc(GL_ALWAYS);
	glLineWidth(1.0);
    glPointSize(10.0);

    float w = 6.66f;
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(w, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, w, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, w);
    glEnd();

	glColor3f(1.0, 0.75, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();

	glDepthFunc(GL_LEQUAL);
	
}
