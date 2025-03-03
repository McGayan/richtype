#include "GL/freeglut.h"
#include "GL/gl.h"
#include "../include/definitions.h"
#include "../include/type_interface.h"

#define GLUT_MOUSE_WHEEL_UP 3
#define GLUT_MOUSE_WHEEL_DOWN 4
#define WHEEL_ZOOM_DELTA 1.1f

vector<GlyphPoly*>* glyph;

using namespace std;

struct OutlinePolyline
{
	int VertCount;
	float* Vertices;
	
		void setVertices(vector<float>& vertList)
	{
		VertCount = vertList.size();
		Vertices = new float[VertCount];
		for(int i=0; i<VertCount; i+= 2)
		{
			Vertices[i] = vertList.at(i);
			Vertices[i+1] = vertList.at(i+1);
		}
		VertCount /= 2;
	}
	
	OutlinePolyline()
	{
		VertCount = 0;
		Vertices = NULL;
	}
	
	OutlinePolyline(vector<float>& vertList)
	{
		setVertices(vertList);
	}
	
	~OutlinePolyline()
	{
		if(Vertices != NULL)
		{
			delete[] Vertices;
		}
	}
	

};

CRectangle::CRectangle() {
		memset(pointData, 0, 8 * sizeof(double));
	}


vector<OutlinePolyline*> srcPolyList;
float* cbnBuffer = NULL;
int cbnBufferLength = 0;
float scale = 1;
float shiftX = 0;
float shiftY = 0;
CRectangle bRect;
CRectangle gridRect;
//vector<OutlinePolyline*> grid;
float gridSpacingX;
float gridSpacingY;

void GetBoundingBoxOfSrc(CRectangle* bRect)
{
	if(srcPolyList.size() > 0)
	{
		OutlinePolyline* firstPoly = srcPolyList.at(0);
		float x = firstPoly->Vertices[0];
		float y = firstPoly->Vertices[1];

		float llcX = x;
		float llcY = y;
		float urcX = x;
		float urcY = y;
		for(int polyIndex=0; polyIndex<srcPolyList.size(); polyIndex++)
		{
			OutlinePolyline* poly = srcPolyList.at(polyIndex);
			
			if(poly->VertCount >1)
			{

				
				for(int i=1; i<poly->VertCount; i++)
				{
					x = poly->Vertices[3*i];
					y = poly->Vertices[3*i + 1];
					if(x < llcX)
						llcX = x;
					else if(x > urcX)
						urcX = x;

					if(y < llcY)
						llcY = y;
					else if(y > urcY)
						urcY = y;
				}

			}
		}
		bRect->SetX0(llcX);
		bRect->SetY0(llcY);
		bRect->SetX1(urcX);
		bRect->SetY1(llcY);
		bRect->SetX2(urcX);
		bRect->SetY2(urcY);
		bRect->SetX3(llcX);
		bRect->SetY3(urcY);
	}
}

void SetGrid(float minX, float maxX, float minY, float maxY, float dx, float dy)
{
	gridRect.SetMinMax(minX, minY, maxX, maxY);
	gridSpacingX = dx;
	gridSpacingY = dy;
	
}

void ScreenToWorld(int srcX, int srcY, float* worldX, float* worldY)
{
	float windowWidth = (float)glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = (float)glutGet(GLUT_WINDOW_HEIGHT);
	float x = srcX * 2.0f / windowWidth - 1.0f;
	float y = (windowHeight - srcY) * 2.0f / windowHeight - 1.0f;
	//printf("x,y: %f, %f\n", x, y);
	*worldX = x / scale - shiftX;
	*worldY = y / scale - shiftY;
	//printf("World: %f, %f\n", *worldX, *worldY);
}

void InitScales()
{
	float llcX = gridRect.GetX0();
	float llcY = gridRect.GetY0();
	float urcX = gridRect.GetX2();
	float urcY = gridRect.GetY2();
	float imageWidth = urcX - llcX;
	float imageHeight = urcY - llcY;
	float maxCoord = 0.8f * 2.0f;
	scale = maxCoord / imageWidth;
	if(imageHeight > imageWidth)
	{
		scale = maxCoord / imageHeight;
	}
	shiftX = -llcX - imageWidth / 2.0f;
	shiftY = -llcY - imageHeight/2.0f;
	
	printf("scale: %f\n", scale);
	printf("shift: %f, %f\n", shiftX, shiftY);
	printf("src Width: %f, %f\n", imageWidth, imageHeight);
}

void DrawGrid()
{
	glColor3ub( 0, 80, 80 );
	float minX = (float)(gridRect.GetX0());
	float minY = (float)(gridRect.GetY0());
	float maxX = (float)(gridRect.GetX2());
	float maxY = (float)(gridRect.GetY2());
	
	glBegin(GL_LINES);
	for (float y = minY; y<=maxY; y+=gridSpacingY)
	{
		glVertex2f(minX, y);
		glVertex2f(maxX, y);
	}

	for (float x = minX; x<=maxX; x+=gridSpacingX)
	{
		glVertex2f(x, minY);
		glVertex2f(x, maxY);
	}
	
	glColor3ub( 0, 80*2, 80*2 );
	glVertex2f(minX, 0);
	glVertex2f(maxX, 0);
	glVertex2f(0, minY);
	glVertex2f(0, maxY);
		
	glEnd();
}

void DrawGlyph()
{
	glColor3ub( 200, 200, 0 );
	for(auto *poly : *glyph)
	{
		const int vertCount = (int)(poly->size() / 2);
		glBegin(GL_LINE_STRIP);
		for(int i=0; i<vertCount; i++)
		{
			glVertex2f(poly->at(2*i), poly->at(2*i + 1));
		}
		glVertex2f(poly->at(0), poly->at(1));	//closing segment
		glEnd();
	}
}

void Draw()
{
	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glOrtho( -6, 6, -6, 6, -1, 1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glScalef(scale, scale, 1);
	glTranslatef(shiftX, shiftY, 0);
	
	DrawGrid();
	DrawGlyph();
	glutSwapBuffers();
}

void OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

void OnMouse(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		;//printf("LEFT_DOWN: %d,%d\n", x, y);
	}
	else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
		float wx = 0;
		float wy = 0;
		ScreenToWorld(x, y, &wx, &wy);
	}
	else if(button == GLUT_MOUSE_WHEEL_UP)
	{
		float wx = 0;
		float wy = 0;
		ScreenToWorld(x, y, &wx, &wy);
		scale *= WHEEL_ZOOM_DELTA;
		shiftX = ((1 - WHEEL_ZOOM_DELTA) * wx + shiftX) / WHEEL_ZOOM_DELTA;
		shiftY = ((1 - WHEEL_ZOOM_DELTA) * wy + shiftY) / WHEEL_ZOOM_DELTA;
		
		glutPostRedisplay();
	}
	else if(button == GLUT_MOUSE_WHEEL_DOWN)
	{
		float wx = 0;
		float wy = 0;
		ScreenToWorld(x, y, &wx, &wy);
		scale /= WHEEL_ZOOM_DELTA;
		shiftX = (WHEEL_ZOOM_DELTA - 1) * wx + WHEEL_ZOOM_DELTA * shiftX;
		shiftY = (WHEEL_ZOOM_DELTA - 1) * wy + WHEEL_ZOOM_DELTA * shiftY;
		glutPostRedisplay();
	}
	else
	{
		InitScales();
		glutPostRedisplay();
	}
}


void InitGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hatching Sample");
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(Draw);
	glutMouseFunc(OnMouse);
	glutReshapeFunc(OnResize);
	glutMainLoop();
}

int main(int argc, char **argv)
{
	glyph = Test_API();
	//Test_Print();
	//printf("\n\n--------%d\n", glyph->size());
	SetGrid(-100, 1500, -100, 1500,50, 50);
	
	InitScales();
	InitGL(argc, argv);
}
