#ifndef PATH_H
#define PATH_H

#include "includes.h"

void convertBezierToVertices(const FT_Vector& firstControlPoint, 
								const FT_Vector& onCurvePoint, 
								const FT_Vector& lastControlPoint, 
								double dt, 
								vector<double>* pointList);

void convertBezierToVerticesAdaptive(const FT_Vector& firstControlPoint, 
								const FT_Vector& onCurvePoint, 
								const FT_Vector& lastControlPoint, 
								double dt, 
								vector<double>* pointList);
								
void midPoint(FT_Vector& p1, FT_Vector& p2, FT_Vector& pMid);
#endif	//PATH_H
