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

// Computes the squared Euclidean distance
double distanceSquared(const FT_Vector& p1, const FT_Vector& p2) {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return dx * dx + dy * dy;
}

// Recursively subdivide based on curvature
void adaptiveBezierSubdivision(std::vector<double>* pointList, 
								const FT_Vector& p0, 
								const FT_Vector& p1, 
								const FT_Vector& p2, 
								double threshold)
{
	// Compute the midpoint of the curve
	FT_Vector mid;
	mid.x = (p0.x + 2 * p1.x + p2.x) / 4;
	mid.y = (p0.y + 2 * p1.y + p2.y) / 4;

	// Compute the midpoint of the chord
	FT_Vector chordMid;
	chordMid.x = (p0.x + p2.x) / 2;
	chordMid.y = (p0.y + p2.y) / 2;

	// Compute the deviation from the chord
	double deviationSq = distanceSquared(mid, chordMid);

	// If deviation is larger than the threshold, subdivide further
	if (deviationSq > threshold)
	{
		FT_Vector p01 = { (p0.x + p1.x) / 2, (p0.y + p1.y) / 2 };
		FT_Vector p12 = { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };
		FT_Vector p012 = { (p01.x + p12.x) / 2, (p01.y + p12.y) / 2 };

		// Recursively process first and second half of the curve
		adaptiveBezierSubdivision(pointList, p0, p01, p012, threshold);
		adaptiveBezierSubdivision(pointList, p012, p12, p2, threshold);
	}
	else
	{
		// Store the endpoint of this segment
		pointList->push_back(p2.x);
		pointList->push_back(p2.y);
	}
}

// Function to convert a Bézier curve into a list of vertices dynamically
void convertBezierToVerticesAdaptive(const FT_Vector& firstControlPoint, 
										const FT_Vector& onCurvePoint, 
										const FT_Vector& lastControlPoint, 
										double threshold, 
										vector<double>* pointList)
{
	// Always add the first point
	pointList->push_back(firstControlPoint.x);
	pointList->push_back(firstControlPoint.y);

	// Recursively generate the curve points
	adaptiveBezierSubdivision(pointList, firstControlPoint, onCurvePoint, lastControlPoint, threshold);

}


void midPoint(FT_Vector& p1, FT_Vector& p2, FT_Vector& pMid)
{
	pMid.x = (int)((double)(p1.x + p2.x) / 2.0 + 0.5);
	pMid.y = (int)((double)(p1.y + p2.y) / 2.0 + 0.5);
}

void scaleGlyph(vector<GlyphOutline*>* glyphCollection)
{
	for(auto itGlyph : *glyphCollection)
	{
		for(auto itPoly : *itGlyph)
		{
			std::for_each(itPoly->begin(), itPoly->end(), [](double &n) { n /= PIXEL_SCALE * 64; });
		}
	}
}
