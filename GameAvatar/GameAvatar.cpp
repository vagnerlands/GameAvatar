// GameAvatar.cpp : Defines the entry point for the console application.
//

#include "CEngine.h"
#include "GL/glut.h"

int main(int argv, char** argc)
{

	// Use a single buffered window in RGB mode (as opposed to a double-buffered
	// window or color-index mode).
	glutInit(&argv, argc);
	// only one buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Position window at (80,80) and sets to it a title
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Game Avatar - Vagner Landskron");

	// straight forward way to avoid Z-fighting
	glDisable(GL_DEPTH_TEST);

	// disable CULL FACE - nice to have in a future version :)
	glDisable(GL_CULL_FACE);

	// prepares the PROJECTION MATRIX
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(600.0 / 2.0), (600.0 / 2.0), -(600.0 / 2.0), (600.0 / 2.0), -1.0, 1.0);

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

