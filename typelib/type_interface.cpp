#include "../include/definitions.h"
#include "../include/type_interface.h"
#include "includes.h"
#include "OutlineIterator.h"
#include "path.h"

vector<GlyphOutline*>* Test_API()
{
	const double dt = 0.1;
	//printf("Hello World API");
	FT_Library library;
	FT_Face face;
	vector<GlyphPoly*>* glyph;
	vector<GlyphOutline*>* glyphCollection = new vector<GlyphOutline*> ();
	if (FT_Init_FreeType(&library)) {
		printf("ERROR: Could not initialize FreeType Library\n");
		return NULL;
	}

	if (FT_New_Face(library, "/usr/share/fonts/truetype/arial.ttf", 0, &face)) {
		printf("ERROR: Failed to load font\n");
		return NULL;
	}

	FT_Set_Pixel_Sizes(face, 0, PIXEL_SCALE); // Set height to 48 pixels

	FT_UInt glyph_index = FT_Get_Char_Index(face, /*0xc2*/'O'); // Get glyph index for 'A'
	if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
		printf("Failed to load glyph\n");
		return NULL;
	}
	
	printf("x_scake: %ld\n", face->size->metrics.x_scale);
	printf("Units per EM: %d\n", face->units_per_EM);

	if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE) {
		FT_Outline *outline = &face->glyph->outline;
		printf("Number of contours: %d\n", outline->n_contours);
		printf("Number of points: %d\n", outline->n_points);
		//int endIndex = 0;
		OutlineIterator* outlineIter = new OutlineIterator(outline);
		glyph = new vector<GlyphPoly*>();
		for(int i=0; i<outline->n_contours; i++)
		{
			outlineIter->setContour(i);
			FT_Vector point, nextPoint, nextNextPoint;
			char tag, nextTag, nextNextTag;
			bool loopBack = outlineIter->getNextPoint(point, tag);
			GlyphPoly* vertices = new GlyphPoly();
			while(!loopBack)
			{
				//printf("%ld, %ld	tags->%X\n", point.x, point.y, (uint8_t)tag);
				if((tag & 0x01) != 0)
				{
					loopBack |= outlineIter->getNextPoint(nextPoint, nextTag);
					if((nextTag & 0x01) != 0)
					{//add line segment from point->next point
						vertices->push_back(point.x);
						vertices->push_back(point.y);
						vertices->push_back(nextPoint.x);
						vertices->push_back(nextPoint.y);
						point = nextPoint;
						tag = nextTag;
					}
					else
					{//on,off
						outlineIter->peekNextPoint(nextNextPoint, nextNextTag);
						if((nextNextTag & 0x01) != 0)
						{//on, off, on
							loopBack |= outlineIter->getNextPoint(nextNextPoint, nextNextTag);
							convertBezierToVerticesAdaptive(point, nextPoint, nextNextPoint, dt, vertices);
							point = nextNextPoint;
							tag = nextNextTag;
						}
						else
						{//on, off, off
							FT_Vector pMid;
							midPoint(nextPoint, nextNextPoint, pMid);
							convertBezierToVerticesAdaptive(point, nextPoint, pMid, dt, vertices);
							point = pMid;
							tag = 0x01;
						}
					}
				}
				else
				{
					printf("\noff ???\n");
				}
			}
			glyph->push_back(vertices);
		}
		delete outlineIter;
	}
	
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	scaleGlyph(glyph);
	glyphCollection->push_back(glyph);
	return glyphCollection;
}
