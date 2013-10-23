/**
 * Allgemein Nützliche Klassen
 */
#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H
#include "Math.h"
#include "GL/glut.h"
#include <stdlib.h>
using namespace std;
/*
 * Klasse IntTupel fasst zwei Int-Werte zusammen.
 * Vglbar mit Tupel einer Relation
 */
class IntTupel{

public:
	int i;
	int j;
	IntTupel(int i, int j){
		this->i = i;
		this->j = j;
	}
};

/**
 * Klasse die die Werte eines Vector3 in bestimmten Zeitintervallen abspeichert
 * und gezeichnet werden kann
 */
class PointTracer{
protected:
	Vector3* mTarget;
	vector<Vector3> mPoints;
	unsigned int mLength;
	bool mEnabled;
public:
	PointTracer(Vector3* target = new Vector3(0, 0, 0), unsigned int length =
			100) {
		mTarget = target;
		mLength = length;
		mEnabled = false;
	}
	vector<Vector3>* getPointsPointer() {
		return &mPoints;
	}
	virtual void setTargetPointer(Vector3* target){
		mTarget = target;
	}
	virtual void trace() {
		if (mEnabled) {
			mPoints.push_back(
					Vector3(mTarget->getX(), mTarget->getY(), mTarget->getZ()));
			if (mPoints.size() > mLength) {
				mPoints.erase(mPoints.begin() + 0);
			}
		}
	}
	virtual void switchMode() {
		mEnabled = !mEnabled;
	}
	virtual void reset() {
		mPoints.clear();
	}
};

class TimedPointTracer : public PointTracer{
protected:
		float mInterval;
		float mTimer;

public:
		TimedPointTracer(Vector3* target = new Vector3(0,0,0),unsigned int length = 100, float interval = 0.1){
			mTarget = target;
			mLength = length;
			mInterval = interval;
			mTimer = 0.0;
			mEnabled = false;
		}
		void trace(float d_t){
			mTimer += d_t;
			if ((mTimer / mInterval) > 1.0 && mEnabled){
				mTimer-= mInterval;
				PointTracer::trace();
			}
		}
		void switchMode(){
			mTimer = 0.0;
			PointTracer::switchMode();
		}
		void reset(){
			mTimer = 0.0;
			PointTracer::reset();
		}
};

static void drawTracer(PointTracer* tracer){
	glPointSize(2.0);
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_POINTS);
	vector<Vector3>* trace = tracer->getPointsPointer();
	for (unsigned int i = 0; i < trace->size();i++){
		glVertex3f(trace->at(i).getX(),trace->at(i).getY(),trace->at(i).getZ());
	}
	glEnd();
}

static void drawVector3(Vector3 vector){
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(vector.getX(),vector.getY(),vector.getZ());
	glEnd();
}

static void drawVector3(Vector3 vector, Vector3 position){
	glPushMatrix();
		glTranslatef(position.getX(), position.getY(), position.getZ());
		drawVector3(vector);
	glPopMatrix();
}

static void drawVector3(Vector3 vector, Vector3 position, Vector3 color){
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR,currentColor);
		glColor3f(color.getX(),color.getY(),color.getZ());
		drawVector3(vector,position);
	glColor4f(currentColor[0],currentColor[1],currentColor[2],currentColor[3]);
}

static Vector3 randomVector3(Vector3 centrum =  Vector3(0,0,0), double max_x = 1.0,double max_y = 1.0, double max_z = 1.0, double min_x = 0.0,double min_y = 0.0,double min_z = 0.0){
	double 	r_x = (((rand() / ((double) RAND_MAX))-0.5)*2.0)*(max_x-min_x); //zwischen 0 und max_x-min_x;
		    r_x += r_x < 0 ? -min_x : min_x;
	double	r_y = (((rand() / ((double) RAND_MAX))-0.5)*2.0)*(max_y-min_y); //zwischen 0 und max_y
		   	r_y += r_y < 0 ? -min_y : min_y;
	double  r_z = (((rand() / ((double) RAND_MAX))-0.5)*2.0)*(max_z-min_z); //zwischen 0 und max_z
			r_z += r_z < 0 ? -min_z : min_z;
	return Vector3(centrum.getX()+r_x,centrum.getY()+r_y,centrum.getZ()+r_z);
}

#endif
