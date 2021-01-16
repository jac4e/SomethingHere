#include <stdio.h>
#include <stdlib.h>

//#include "../lib/glad/glad.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

static char *test_argv = "test_name";
static int test_argc = 1;

int init_screen(int dims[2])
{
	GLenum err;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1920, 1080);
	glutInit(&test_argc, &test_argv);
	glutCreateWindow("GLEW Test");

	err = glewInit();
	if(err != GLEW_OK){
		printf("errorrrr\n");
		return -1;
	}
	return 0;
}

int update_screen(char **tiles, int dims[2])
{
	return 0;
}
