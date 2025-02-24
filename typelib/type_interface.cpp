#include "../include/definitions.h"
#include "../include/type_interface.h"
#include "includes.h"

void Test_API()
{
	//printf("Hello World API");
	FT_Library library;
	FT_Face face;

	if (FT_Init_FreeType(&library)) {
		printf("ERROR: Could not initialize FreeType Library\n");
		return;
	}

	if (FT_New_Face(library, "/usr/share/fonts/truetype/arial.ttf", 0, &face)) {
		printf("ERROR: Failed to load font\n");
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48); // Set character size

	if (FT_Load_Char(face, 'A', FT_LOAD_DEFAULT)) {
		printf("ERROR: Failed to load glyph\n");
		return;
	}
}
