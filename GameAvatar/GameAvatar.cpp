// GameAvatar.cpp : Defines the entry point for the console application.
//

#include "CEngine.h"
#include "GL/glut.h"

#define _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

int main(int argv, char** argc)
{

	// Use a single buffered window in RGB mode (as opposed to a double-buffered
	// window or color-index mode).
	glutInit(&argv, argc);
	// only one buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Position window at (80,80) and sets to it a title
	glutInitWindowPosition(200, 20);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Game Avatar - Vagner Landskron");

	// straight forward way to avoid Z-fighting
	//glDisable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.5f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	// disable CULL FACE - nice to have in a future version :)
	glDisable(GL_CULL_FACE);

	// prepares the PROJECTION MATRIX
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-(WINDOW_WIDTH / 2.0), (WINDOW_WIDTH / 2.0), -(WINDOW_HEIGHT / 2.0), (WINDOW_HEIGHT / 2.0), -100.0, 100.0);

	// preapres the MODELVIEW MATRIX
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Assign the clear screen color
	//Format (Red, Green, Blue, Alpha)
	//Values should remain normalized between 0 and 1
	//glClearColor(0.0, 0.0, 0.0, 0.0);

	CEngine mainApp;
	mainApp.ignition();
    return 0;
}

