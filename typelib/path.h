#ifndef PATH_H
#define PATH_H

#include "includes.h"

void convertBezierToVertices(const FT_Vector& firstControlPoint, 
								const FT_Vector& onCurvePoint, 
								const FT_Vector& lastControlPoint, 
								double dt, 
								vector<double>* pointList);
								
void convertBezierToVertices(const FT_Vector& firstControlPoint, 
								const FT_Vector& lastControlPoint, 
								double dt, 
								vector<double>* pointList);
#endif	//PATH_H
