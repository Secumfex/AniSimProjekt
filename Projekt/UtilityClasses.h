/**
 * Allgemein Nützliche Klassen
 */
#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H
#include "Math.h"
#include "GL/glut.h"
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
public:
	PointTracer(Vector3* target = new Vector3(0,0,0),unsigned int length = 100){
		mTarget = target;
		mLength = length;
	}
	vector<Vector3>* getPointsPointer(){
				return &mPoints;
			}
	virtual void trace(){
		mPoints.push_back(Vector3(mTarget->getX(), mTarget->getY(), mTarget->getZ()));

		if (mPoints.size() > mLength){
			mPoints.erase(mPoints.begin()+0);
		}
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
		}



		void trace(float d_t){
			mTimer += d_t;
			if ((mTimer / mInterval) > 1.0 ){
				mTimer-= mInterval;
				PointTracer::trace();
			}
		}
};

static void drawTracer(PointTracer* tracer){
	glBegin(GL_POINTS);
	vector<Vector3>* trace = tracer->getPointsPointer();
	for (unsigned int i = 0; i < trace->size();i++){
		glVertex3f(trace->at(i).getX(),trace->at(i).getY(),trace->at(i).getZ());
	}
	glEnd();
}

#endif
