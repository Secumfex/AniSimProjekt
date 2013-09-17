#ifndef HERMITE_SPLINE_CONTROL_POINT_H
#define HERMITE_SPLINE_CONTROL_POINT_H

#include "Vector3.h"


class HermiteSplineControlPoint
{
	public:
		HermiteSplineControlPoint(void);
		HermiteSplineControlPoint(Vector3 pos, Vector3 dir);
		virtual ~HermiteSplineControlPoint(void);

	private:
		Vector3 mPosition;
		Vector3 mDirection;



	public:
		inline void setPosition(Vector3 p){mPosition = p;}
		inline void setDirection(Vector3 d){mDirection = d;}

		inline Vector3 getPosition(void){return mPosition;}
		inline Vector3 getDirection(void){return mDirection;}
};
#endif