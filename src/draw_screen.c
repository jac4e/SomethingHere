#include <stdio.h>
#include <stdlib.h>

// #include <GL/glew.h>
#include <GL/glut.h>
// #include <GL/freeglut_ext.h>
// #include <GLFW/glfw3.h>

static char *test_argv = "test_name";
static int test_argc = 1;

void display_callback(void *user_data)
{
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw something
	glBegin(GL_QUADS);
	glColor3f(0.0F, 1.0F, 0.0F);
	glVertex2f(-0.5F, -0.5F);
	glVertex2f(0.5F, -0.5F);
	glVertex2f(0.5F, 0.5F);
	glVertex2f(-0.5F, 0.5F);
	glEnd();

	glFlush();
	glutSwapBuffers();

	puts("display callback");
}

// called when window is resized
void reshape_callback(int x, int y, void *user_data)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

int init_screen(int dims[2])
{
	GLenum err;

	// initializing glut parameters
	glutInit(&test_argc, &test_argv);
	glutCreateWindow("GLEW Test");

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1920, 1080);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// create window and GL context
	glutFullScreen();

	// openGL extension wrangler library
//	err = glewInit();
//	if(err != GLEW_OK){
		//printf("errorrrr\n");
			//return -1;
	//}
	//printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// set glut callbacks
	glutDisplayFuncUcall(&display_callback, NULL);
	glutReshapeFuncUcall(&reshape_callback, NULL);

	return 0;
}

int update_screen(char **tiles, int dims[2])
{
	// processes a single interation
	glutMainLoopEvent();
	return 0;
}
