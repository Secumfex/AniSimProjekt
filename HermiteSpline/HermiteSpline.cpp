#include "HermiteSpline.h"
#include <iostream>
using std::cout;
using std::endl;

#define ONE_THIRD (1.0f / 3.0f)
#define THRESH 0.001f

HermiteSpline::HermiteSpline(void)
{
}

HermiteSpline::~HermiteSpline(void)
{
}


void HermiteSpline::generateRenderVertices(void){

	vector<HermiteSplineControlPoint*>::iterator cpIt = mControlPoints.begin();
	while(cpIt != mControlPoints.end() - 1){
		sampleHermiteSpline(*cpIt, *(cpIt + 1), 32); 
		cpIt++;
	}

	
	
}


void HermiteSpline::sampleHermiteSpline(HermiteSplineControlPoint* c0, HermiteSplineControlPoint* c1, int numVerts){

	float tStart = 0.0f;
	float tEnd = 1.0f;
	
	float tStep = (tEnd - tStart) / (numVerts-1);
	float t = tStart;
	
	for(int i=0; i<numVerts; i++){
	
		Vector3 point, tangent;
		
		evaluateHermiteSpline(t, c0, c1, point, tangent);
		
		mVertices.push_back(point);

		t += tStep;
	}

	
}



void HermiteSpline::evaluateHermiteSpline(float t, HermiteSplineControlPoint* c0, HermiteSplineControlPoint* c1, Vector3 &point, Vector3 &tangent){
	vector<Vector3> bez;

	bez.push_back(c0->getPosition());
	bez.push_back(c0->getPosition() + ONE_THIRD * c0->getDirection());
	bez.push_back(c1->getPosition() - ONE_THIRD * c1->getDirection());
	bez.push_back(c1->getPosition());

	
	deCasteljau(t, bez, point, tangent);

}




void HermiteSpline::deCasteljau(float t, vector<Vector3> &cp, Vector3 &point, Vector3 &tangent){
	if(cp.size() == 2){
		point = ((1.0f - t) * cp[0] + t * cp[1]);
		tangent = cp[1] - cp[0];
		tangent.normalize();
	}
	else{
		vector<Vector3> ncp;
		vector<Vector3>::iterator cpIt = cp.begin();
		while(cpIt != cp.end()-1){
			ncp.push_back( (1.0f - t) * (*cpIt) + t * (*(cpIt + 1)) );
			cpIt++;
		}

		deCasteljau(t, ncp, point, tangent);
		ncp.clear();

	
	}
}


void HermiteSpline::generateArcLengthTable(int res){
	//********************************************************************************
	//hier muss die Erstellung der Bogenlängentabelle programmiert werden

	int numPatches = mControlPoints.size() - 1;
	int stepsPerPatch = res / numPatches;
	float u;
	float uStep = 1.0f / stepsPerPatch;
	float len = 0.0;

	Vector3 point, lastPoint;

	

	for(int i=0; i<numPatches; i++){

		u = 0.0f;
		for(int j=0; j<=stepsPerPatch; j++){

			if(i != 0 && j == 0){
				u += uStep;
				continue;
			}

			Vector3 t;
			evaluateHermiteSpline(u, mControlPoints[i], mControlPoints[i + 1], point, t);

			ArcLengthTableEntry ale;
			ale.u = u;
			ale.patchNumber = i;
			if(i == 0 && j == 0){
				ale.arcLength = 0.0f;
			}
			else{
				ale.arcLength = (point - lastPoint).length() + mArcLengthTable.back().arcLength;
			}
			
			lastPoint = point;
			mArcLengthTable.push_back(ale);
			
			u += uStep;

		}
	}

	//********************************************************************************

	//DEBUG
	for(int i=0; i<mArcLengthTable.size(); i++){
		cout<<"i: "<<i<<"  u: "<<mArcLengthTable[i].u<<"  Patch: "<<mArcLengthTable[i].patchNumber<<"   Length: "<<mArcLengthTable[i].arcLength<<endl; 
	}
}


void HermiteSpline::getParameterByArcLength(float length, float &u, int &patchNum){

	//********************************************************************************
	//hier wird der parameter u und der entsprechende Spline für eine gegebene Bogenlänge
	//in der Tabelle gesucht. Ein binäres Suchverfahren ist zu empfehlen. 

	if(length > mArcLengthTable.back().arcLength){ //given length exeeds path length, return -1 as error
		u = -1.0;
		patchNum = -1;
		return;
	}

	bool found = false;
	int index;
	int indexLow = 0;
	int indexHi = mArcLengthTable.size() - 1;

	while(!found){
		index = indexLow + 0.5 * (indexHi - indexLow);

		if(mArcLengthTable[index].arcLength == length){ //found
			u = mArcLengthTable[index].u;
			patchNum = mArcLengthTable[index].patchNumber;
			found = true;
		}

		if(mArcLengthTable[index].arcLength > length){
			if(index - 1 >= 0){
				if(mArcLengthTable[index-1].arcLength < length){

					float frac = (length - mArcLengthTable[index-1].arcLength) / (mArcLengthTable[index].arcLength - mArcLengthTable[index-1].arcLength);

					float interpolLow = mArcLengthTable[index - 1].u;
					/*
					Ueberprüfung: der kleinere Wert ist der letzte Wert des vorherigen Patch
					In der Bogenlängentabelle vgl.  Vorlesungsskript fehlt der Wert für u = 0
					Daher Wert für u=1 des vorherigen Patches nehmen.
					*/
					
					//if(interpolLow >= 1.0f){ 
					if(interpolLow >= 1.0f - THRESH && interpolLow <= 1.0f + THRESH){ 
						interpolLow = 0.0f;
					}
					float interpolHi =  mArcLengthTable[index].u;
					u = interpolLow + frac * (interpolHi - interpolLow);
					patchNum = mArcLengthTable[index].patchNumber;

					found = true;
				}
				else{
					indexHi = index - 1;
				}
			}
		}

		if(mArcLengthTable[index].arcLength < length){
			if(index + 1 < mArcLengthTable.size()){ //bounds check
				if(mArcLengthTable[index + 1].arcLength > length){

					float frac = (length - mArcLengthTable[index].arcLength) / (mArcLengthTable[index + 1].arcLength - mArcLengthTable[index].arcLength);

					float interpolLow = mArcLengthTable[index].u;
					//if(interpolLow >= 1.0f){
					if(interpolLow >= 1.0f - THRESH && interpolLow <= 1.0f + THRESH){ 
						interpolLow = 0.0f;
					}
					float interpolHi =  mArcLengthTable[index + 1].u;
					u = interpolLow + frac * (interpolHi - interpolLow);
					patchNum = mArcLengthTable[index + 1].patchNumber;
					
					found = true;
				}
				else{
					indexLow = index + 1;
				}
			}
		}
	}

	//********************************************************************************
}


void HermiteSpline::calculateFrenetFrame(float u, HermiteSplineControlPoint* c0, HermiteSplineControlPoint* c1, Vector3 &tangent, Vector3 &binormal, Vector3 &normal){
	//********************************************************************************
	//hier wird der Frenet-Frame berechnet und in den übergebenen Funktionsargumenten gespeichert
	//Sie brauchen noch die zweite Ableitung einer Bezier-Kurve...

	vector<Vector3> bez;

	bez.push_back(c0->getPosition());
	bez.push_back(c0->getPosition() + ONE_THIRD * c0->getDirection());
	bez.push_back(c1->getPosition() - ONE_THIRD * c1->getDirection());
	bez.push_back(c1->getPosition());
	
	Vector3 point;

	deCasteljau(u, bez, point, tangent);

	Vector3 secDer =   (6.0f - 6.0f * u) * bez[0] +
					 (18.0f * u - 12.0f) * bez[1] +
					  (6.0f - 18.0f * u) * bez[2] +
								6.0f * u * bez[3];

	binormal = secDer.cross(tangent);
	binormal.normalize();

	normal = binormal.cross(tangent);

}
