#include "OutlineIterator.h"

OutlineIterator::OutlineIterator(FT_Outline* outline) : outline(outline), contourIndex(-1), startIndex(0), endIndex(0), currentIndex(0)
{
	
}

void OutlineIterator::setContour(int index)
{
	if (!outline || index < 0 || index >= outline->n_contours)
		return; // Invalid index

	contourIndex = index;
	startIndex = (index == 0) ? 0 : outline->contours[index - 1] + 1;
	endIndex = outline->contours[index];
	currentIndex = startIndex;
}


bool OutlineIterator::getNextPoint(FT_Vector& point, char& tag)
{
	if(!peekNextPoint(point, tag))
		return false;
	
	currentIndex++;
	// If we reached past the last point, reset to start
	if (currentIndex > endIndex)
	{
		currentIndex = startIndex;
		return true; // Indicate contour loop
	}

	return false; // Normal case
}

bool OutlineIterator::peekNextPoint(FT_Vector& point, char& tag)
{
	if (contourIndex < 0 || !outline) return false; // Invalid state

	point = outline->points[currentIndex]; // Get the current point
	tag = outline->tags[currentIndex];     // Get the tag value	
	return true;
}
