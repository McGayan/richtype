#ifndef OUTLINE_ITERATOR_H
#define OUTLINE_ITERATOR_H

#include "includes.h"

class OutlineIterator {
private:
    FT_Outline* outline;  // Pointer to the FreeType outline
    int contourIndex;     // Active contour index
    int startIndex;       // Starting index of the current contour
    int endIndex;         // Ending index of the current contour
    int currentIndex;     // Current index in the contour

public:
    // Constructor: Initialize with an FT_Outline pointer
    OutlineIterator(FT_Outline* outline);

    // Set the contour to iterate over
    void setContour(int index);

    // Retrieve the next point (loops automatically)
    bool getNextPoint(FT_Vector& point, char& tag);
};

#endif // OUTLINE_ITERATOR_H
