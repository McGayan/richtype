#include "path.h"


double quadraticBezier(double p0, double p1, double p2, double t)
{
	double oneMinusT = 1.0 - t;
	return oneMinusT * oneMinusT * p0 + 2 * oneMinusT * t * p1 + t * t * p2;
}

void convertBezierToVertices(const FT_Vector& firstControlPoint, 
								const FT_Vector& onCurvePoint, 
								const FT_Vector& lastControlPoint, 
								double dt, 
								vector<double>* pointList)
{
	if (dt <= 0.0 || dt > 1.0) dt = 0.1; // Ensure dt is within a reasonable range

	std::vector<double> vertices;
	for (double t = 0.0; t <= 1.0; t += dt)
	{
		double x = quadraticBezier(firstControlPoint.x, onCurvePoint.x, lastControlPoint.x, t);
		double y = quadraticBezier(firstControlPoint.y, onCurvePoint.y, lastControlPoint.y, t);
		pointList->push_back(x);
		pointList->push_back(y);
	}
}


void convertBezierToVertices(const FT_Vector& firstControlPoint, 
								const FT_Vector& lastControlPoint, 
								double dt, 
								vector<double>* pointList)
{
	FT_Vector onCurvePoint;
	onCurvePoint.x = (firstControlPoint.x + lastControlPoint.x) / 2.0 ;
	onCurvePoint.y = (firstControlPoint.y + lastControlPoint.y) / 2.0 ;
	convertBezierToVertices(firstControlPoint, onCurvePoint, lastControlPoint, dt, pointList);
}

void midPoint(FT_Vector& p1, FT_Vector& p2, FT_Vector& pMid)
{
	pMid.x = (int)((double)(p1.x + p2.x) / 2.0 + 0.5);
	pMid.y = (int)((double)(p1.y + p2.y) / 2.0 + 0.5);
}
