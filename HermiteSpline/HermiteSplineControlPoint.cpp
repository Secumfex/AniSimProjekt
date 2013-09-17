#include "HermiteSplineControlPoint.h"

HermiteSplineControlPoint::HermiteSplineControlPoint(void)
{
}


HermiteSplineControlPoint::HermiteSplineControlPoint(Vector3 pos, Vector3 dir){
	mPosition = pos;
	mDirection = dir;
}


HermiteSplineControlPoint::~HermiteSplineControlPoint(void)
{
}
