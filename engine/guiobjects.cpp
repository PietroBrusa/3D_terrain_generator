#include "engine.h"

#define EPSILON 0.00001f

// GLM:
#include <glm/gtc/type_ptr.hpp>

// FreeGlut:
#include "GL/freeglut.h"
#include "guiobjects.h"

void Eng::GUIObjects::start(glm::mat4 matrix)
{
	drawingView = matrix;

	// Set projection:
	/*glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(matrix));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

	// disable texture mapping
	glDisable(GL_TEXTURE_2D);

	// Disable light:
	glDisable(GL_LIGHTING);

	// Disable z buffer:
	glDisable(GL_DEPTH_TEST);*/
}

void Eng::GUIObjects::stop()
{
	// Enable z buffer:
	/*glEnable(GL_DEPTH_TEST);

	// Enable light:
	glEnable(GL_LIGHTING);*/
}

void Eng::GUIObjects::drawText(glm::vec2 pos, const unsigned char *text, glm::vec4 color)
{
	/*glColor4f(color.r, color.g, color.b, color.a);
	glRasterPos2f(pos.x, pos.y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, text);*/
}

void Eng::GUIObjects::drawRect(glm::vec2 pos, float width, float height, const glm::vec4 color)
{
	/*glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_TRIANGLE_STRIP);

	glVertex2f(pos.x, pos.y);				   // TOP    - LEFT
	glVertex2f(pos.x, pos.y - height);		   // BOTTOM - LEFT
	glVertex2f(pos.x + width, pos.y);		   // TOP    - RIGHT
	glVertex2f(pos.x + width, pos.y - height); // BOTTOM - RIGHT

	glEnd();*/
}

int Eng::GUIObjects::getWidth()
{
	float matrix_0_0 = drawingView[0][0]; // M[column][row]

	if (std::fabs(matrix_0_0) < EPSILON) {
		return 0;
	}

	float width = 2.0f / matrix_0_0;
	return (int)width;
}

int Eng::GUIObjects::getHeight()
{
	float matrix_1_1 = drawingView[1][1]; // M[column][row]

	if (std::fabs(matrix_1_1) < EPSILON) {
		return 0;
	}

	float height = 2.0f / matrix_1_1;
	return (int)height;
}
