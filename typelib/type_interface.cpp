#include "../include/definitions.h"
#include "../include/type_interface.h"
#include "includes.h"
#include "OutlineIterator.h"
#include "path.h"

vector<GlyphOutline*>* Test_API(wchar_t* utf32String)
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

	if (FT_New_Face(library, "/usr/share/fonts/truetype/arial.ttf", 0, &face))	///usr/share/fonts/truetype/ancient-scripts/Symbola_hint.ttf"
	{
		printf("ERROR: Failed to load font\n");
		return NULL;
	}

	FT_Set_Pixel_Sizes(face, 0, PIXEL_SCALE); // Set height to 48 pixels
	FT_Bool use_kerning = FT_HAS_KERNING( face );
	printf("Kerning: %s\n", use_kerning ? "Available" : "not Available");
	int len = wcslen(utf32String);
	FT_UInt old_glyph_index = 0;
	long advance_x = 0;
	for(int i=0; i<len; i++)
	{
		FT_ULong charCode = utf32String[i];	//0x1F600;
		printf("-->%c\n", utf32String[i]);
		FT_UInt glyph_index = FT_Get_Char_Index(face, charCode); // Get glyph index for 'A'

		if (glyph_index == 0)
		{
			printf("Emoji not found in the font!\n");
		}
		else
		{
			printf("Glyph index for: %u\n", glyph_index);
		}
		if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT))
		{
			printf("Failed to load glyph\n");
			return NULL;
		}
		
		FT_Vector kerningVec;
		kerningVec.x = 0;
		kerningVec.y = 0;
		if(old_glyph_index > 0)
		{
        	FT_Error error = FT_Get_Kerning(face, old_glyph_index,  glyph_index, FT_KERNING_DEFAULT, &kerningVec);
			if(error)
				printf("Error loading kerning\n");
			else
			{
				printf("Kerking pair got l%c, r%c	DIS: %ld\n", (char)old_glyph_index, (char)glyph_index,  kerningVec.x);
			}
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
		for(auto itPoly : *glyph)
		{
			for(int j=0; j<itPoly->size(); j+=2)
			{
				(*itPoly)[j] = (*itPoly)[j] + kerningVec.x + advance_x;
				//(*itPoly)[j + 1] = (*itPoly)[j + 1] + kerningVec.y;
			}
		}
		advance_x = face->glyph->advance.x;
		printf("kerning DIS: %ld\n", kerningVec.x >> 6);
		glyphCollection->push_back(glyph);
		old_glyph_index = glyph_index;
	}
	
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	scaleGlyph(glyphCollection);
	glyphCollection->push_back(glyph);
	return glyphCollection;
}
