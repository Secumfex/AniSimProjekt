/*
*MAIN() IS IN HERE
*/

#include <windows.h>
#include <iostream>
#include <stdio.h>

#include "GL/glut.h"

#include "Vector3.h"
#include "Camera.h"

#include "BasisApplication.h"
#include "Projekt.h"

#include <iostream>
#include <time.h>

using namespace std;

//---------------GLOBAL VARIABLES----------------------------------------
int wnd_width = 1024;
int wnd_height = 768;


GLfloat m_left = -0.5;
GLfloat m_right = 0.5;
GLfloat m_bottom = -0.5;
GLfloat m_top = 0.5;
GLfloat m_near = 0.1;
GLfloat m_far = 500.0;
GLfloat bckZ = -m_far + 5.0;

Vector3 camPos(0.0, 5.0, 50.0);
Vector3 camLookAt(0.0, 0.0, 0.0);
Vector3 camUpVector(0.0, 1.0, 0.0);

//float gLightPos [] = {-1.0, 1.0, 1.0, 0.0};
float gLightPos [] = {0.0, 5.0, 0.0, 1.0};
float gMatAmb [] = {0.6, 0.5, 0.0, 1.0};
float gMatDif [] = {0.8, 0.7, 0.0, 1.0};
float gMatSpec [] = {1.0, 1.0, 1.0, 1.0};

//timing
clock_t t;			  		//Variable für aktuelle Clocktime. Wenn Clock Time nötig, t = clock() setzten
double d_t = 0.016667;//nicht mehr nötig
clock_t update_start; 		//Clock Time des letzten Updatestarts
clock_t frame_last_swapped;	//Clock Time des letzten Buffer-Swaps

BasisApplication *gApplication;
Camera gCamera;

int gMouseXPos;
int gMouseYPos;
bool gMouseLButtonPressed = false;
bool gMouseRButtonPressed = false;
float gMouseOrbitAngle = 0.0f;

//---------FUNCTION PROTOTYPES -------------
void reshape(int w, int h);
//void drawFloor(float xExt, float zExt, float xRes, float zRes);

//init openGL
void initGL(void){

	glShadeModel (GL_SMOOTH);				
	glClearColor (1.0f, 1.0f, 1.0f, 1.0f);	
	//glClearDepth (1.0f);
	glClearDepth (m_far);
	glEnable (GL_DEPTH_TEST);				
	glDepthFunc (GL_LEQUAL);				
	//glCullFace (GL_BACK);					
	//glEnable(GL_CULL_FACE);                 
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHT0);
	

	reshape(wnd_width, wnd_height);
}

/*Aus Weihnachtsvorlesung geklaut*/
void print(char *string, float x, float y, float z){
	char *s;
	glColor3f(0.2,0.2,0.2);
	glRasterPos3f(x,y,z);
	for(s = string; *s; s++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*s);
	}
}

void drawFPS(float frametime){
	float fps = 0.0;
	if ( frametime > 0.0){
		fps = 1.0 / frametime;
	}
	char string[20];
	sprintf(string, "FPS: %2.2f", fps);
	print(string, 1.0, 0.0, 0.0);
}


//--------------------GLUT callback functions------------------------------------
void display(void){
	//-------------start taking time ---------------------
	/*CPU Clocks are used instead of GLUT_ELAPSED_TIME*/
	/*Aktuelle Clock-Time */
	t = clock();									//Clock Time: start der Display Methode
	
	float time_since_last_update_start = ((float)(t-update_start)/(float)CLOCKS_PER_SEC); //Zeit-Differenz
	if(time_since_last_update_start >= 0.01){		//Zeit seit letztem Update größer als 10 ms
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		gCamera.look();

		update_start = clock();						//Clock Time: Start des Updates

		gApplication->update(time_since_last_update_start);

		gApplication->draw();

		t = clock();								//Clock Time: Frame Bereit zum swap
		float frame_time = ((float)(t-frame_last_swapped)/(float)CLOCKS_PER_SEC);

		drawFPS(frame_time);

		glutSwapBuffers();
		frame_last_swapped = clock();				//Clock Time: Swap der Buffer
	}
	else{
		Sleep(0.0001);								//Warte für 0.1 ms
	}
}

void reshape (int w, int h){
	
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (float)w / (float)h,  m_near, m_far);

	glMatrixMode (GL_MODELVIEW);
}


//keyboard functions
void specialKeyboard(int key, int x, int y)
{

	gApplication->keyPressed(key);

	switch (key) {
		case GLUT_KEY_UP :
			break;
		case GLUT_KEY_DOWN :
			break;
		case GLUT_KEY_LEFT :
			break;
		case GLUT_KEY_RIGHT :
			break;    
		
		case GLUT_KEY_END:
			exit(0);
			break;

	}
}


void keyboard(unsigned char key, int x, int y)
{
	gApplication->keyPressed(key);

	switch(key){
		//escape key pressed
		case 27:
			exit(0);
			break;
	}
}


void mouseButtons(int button, int state, int x, int y){
	gMouseXPos = x;
	gMouseYPos = y;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		gMouseLButtonPressed = true;
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		gMouseLButtonPressed = false;
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		gMouseRButtonPressed = true;
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		gMouseRButtonPressed = false;
	}
}


void mouseMotion(int x, int y){

	int dX = x - gMouseXPos;
	int dY = y - gMouseYPos;

	float sensitivity =  0.1f;

	if(gMouseRButtonPressed){
		gCamera.setPosition(gCamera.getPosition() - dY * sensitivity * gCamera.getDirection());
	}

	if(gMouseLButtonPressed){
		gMouseOrbitAngle += dX * 0.1f * sensitivity;
		
		Vector3 p = gCamera.getPosition();
		p.setY(0.0f);

		float r = p.length();
		gCamera.setPosition(Vector3(sin(gMouseOrbitAngle) * r, gCamera.getPosition().getY() + dY * sensitivity, cos(gMouseOrbitAngle) * r));
		
	}

	gMouseXPos = x;
	gMouseYPos = y;

}



int main(int argc, char* argv[])
{
	

	//******************************************************
	//Hier werden die entsprechenden Übungen eingebunden. 

	gApplication = new Projekt();
	gApplication->setCameraPointer(&gCamera);
	
	
	//setup a GLUT window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(wnd_width, wnd_height);

	glutCreateWindow(gApplication->getAppTitle().c_str());
	initGL();

	gCamera.setPosition(Vector3(5.0f, 5.0f, 50.0f));
	gCamera.setLookAt(Vector3(0.0f, 0.0f, 0.0f));
	
	//registering callback functions
	glutReshapeFunc(reshape);	
	glutDisplayFunc(display);
	glutIdleFunc(display);   
	glutSpecialFunc(specialKeyboard); 
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButtons);
	glutMotionFunc(mouseMotion);

	gApplication->init();
	//timing init
	t = clock();
	update_start = clock(); //simulation time accumulator
	frame_last_swapped = clock();
	glutMainLoop();
	
	return 0;
}


