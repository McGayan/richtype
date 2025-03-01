#include "../include/definitions.h"
#include "../include/type_interface.h"
#include "includes.h"
#include "OutlineIterator.h"
#include "path.h"

vector<GlyphPoly*>* Test_API()
{
	const double dt = 0.01;
	//printf("Hello World API");
	FT_Library library;
	FT_Face face;
	vector<GlyphPoly*>* glyph;
	if (FT_Init_FreeType(&library)) {
		printf("ERROR: Could not initialize FreeType Library\n");
		return NULL;
	}

	if (FT_New_Face(library, "/usr/share/fonts/truetype/arial.ttf", 0, &face)) {
		printf("ERROR: Failed to load font\n");
		return NULL;
	}

	FT_Set_Pixel_Sizes(face, 0, 48); // Set height to 48 pixels

	FT_UInt glyph_index = FT_Get_Char_Index(face, 'o'); // Get glyph index for 'A'
	if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
		printf("Failed to load glyph\n");
		return NULL;
	}

	if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE) {
		FT_Outline *outline = &face->glyph->outline;
		printf("Number of contours: %d\n", outline->n_contours);
		printf("Number of points: %d\n", outline->n_points);
		int endIndex = 0;
		OutlineIterator* outlineIter = new OutlineIterator(outline);
		glyph = new vector<GlyphPoly*>();
		for(int i=0; i<outline->n_contours; i++)
		{
			//int startIndex = endIndex + 1;
			//endIndex = outline->contours[i];
			printf("C: %d----\n", i);
			outlineIter->setContour(i);
			FT_Vector point, nextPoint, nextNextPoint;
			char tag, nextTag, nextNextTag;
			bool loopBack = outlineIter->getNextPoint(point, tag);
			vector<double>* vertices = new vector<double>();
			while(!loopBack)
			{
				printf("%ld, %ld	tags->%X\n", point.x, point.y, (uint8_t)tag);
				if((tag | FT_CURVE_TAG_ON) == true)
				{
					loopBack |= outlineIter->getNextPoint(nextPoint, nextTag);
					if((nextTag | FT_CURVE_TAG_ON) == true)
					{//add line segment from point->next point
						vertices->push_back(point.x);
						vertices->push_back(point.y);
						vertices->push_back(nextPoint.x);
						vertices->push_back(nextPoint.y);
						
					}
					point = nextPoint;
					tag = nextTag;
				}
				else
				{
					loopBack |= outlineIter->getNextPoint(nextPoint, nextTag);
					if((nextTag | FT_CURVE_TAG_ON) == false)
					{//control, control	-> add a on curve point at the center and place a bezier
						convertBezierToVertices(point, nextPoint, dt, vertices);
						point = nextPoint;
						tag = nextTag;
					}
					else
					{
						loopBack |= outlineIter->getNextPoint(nextNextPoint, nextNextTag);
						if((nextNextTag | FT_CURVE_TAG_ON) == false)
						{//control, on, control	-> add bezier
							convertBezierToVertices(point, nextPoint, nextNextPoint, dt, vertices);
						}
						else
						{//control, on, on -> add polyline from next to nextnext
							vertices->push_back(nextPoint.x);
							vertices->push_back(nextPoint.y);
							vertices->push_back(nextNextPoint.x);
							vertices->push_back(nextNextPoint.y);
						}
						point = nextNextPoint;
						tag = nextNextTag;
					}
				}
				//loopBack |= outlineIter->getNextPoint(point, tag);
			}
			glyph->push_back(vertices);
			/*for(int j=startIndex; j<=endIndex; j++)
			{
				FT_Vector& point = outline->points[j];
				printf("(%d)	%ld, %ld	tags->%X\n", j, point.x, point.y, (uint8_t)(outline->tags[j]));
			}*/
		}
		delete outlineIter;
	}
	
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return glyph;
}
