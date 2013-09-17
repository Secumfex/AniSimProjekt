#ifndef HERMITE_SPLINE_H
#define HERMITE_SPLINE_H

#include "HermiteSplineControlPoint.h"

#include "Vector3.h"


#include <vector>
using std::vector;

struct ArcLengthTableEntry{
	float u;
	int patchNumber;
	float arcLength;
};

class HermiteSpline
{

	//------------CONSTRUCTORS & DESTRUCTOR---------------------
	public:
		HermiteSpline(void);
		virtual ~HermiteSpline(void);


	//-------------MEMBER VARIABLES-------------------------------

	private:
		vector<HermiteSplineControlPoint*> mControlPoints;
		vector<Vector3> mVertices;

		vector<ArcLengthTableEntry> mArcLengthTable;
		
	
	//-------------MEMBER FUNCTIONS-------------------------------
	private:
		void deCasteljau(float t, vector<Vector3> &cp, Vector3 &point, Vector3 &tangent);
		void sampleHermiteSpline(HermiteSplineControlPoint* c0, HermiteSplineControlPoint* c1, int numVerts);
		
	public:
		void evaluateHermiteSpline(float u, HermiteSplineControlPoint* c0, HermiteSplineControlPoint* c1, Vector3 &point, Vector3 &tangent);
		void calculateFrenetFrame(float u, HermiteSplineControlPoint* c0, HermiteSplineControlPoint* c1, Vector3 &tangent, Vector3 &binormal, Vector3 &normal);

		inline void addControlPoint(HermiteSplineControlPoint* c){mControlPoints.push_back(c);}
		void generateRenderVertices(void);
		vector<HermiteSplineControlPoint*>* getControlPointsPtr(void){return &mControlPoints;}
		vector<Vector3>* getVerticesPtr(void){return &mVertices;}

		void generateArcLengthTable(int res);
		void getParameterByArcLength(float length, float &u, int &patchNum);

		 
};
#endif //HERMITE_SPLINE_H