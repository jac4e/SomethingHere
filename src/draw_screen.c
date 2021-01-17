#include <stdio.h>
#include <stdlib.h>

// #include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
// #include <GLFW/glfw3.h>

#include "draw_screen.h"

static char *test_argv = "test_name";
static int test_argc = 1;

int init_gl()
{
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
}

// set color before calling, so repeated calls
// dont require repeated glColor calls
// must be called with an active glBegin(GL_QUADS);
int draw_box(float pos[2], float side_len[2])
{
	glVertex2f(pos[0], pos[1]);
	glVertex2f(pos[0] + side_len[0], pos[1]);
	glVertex2f(pos[0] + side_len[0], pos[1] - side_len[1]);
	glVertex2f(pos[0], pos[1] - side_len[1]);

	printf("drawing box at: %f, %f\n", pos[0], pos[1]);

	return 0;
}

int compute_fracs(struct display_data *dat, float gap_pro)
{
	// x box and gap gl lengths
	dat->box_fracs[0] = 2.0F / (dat->grid_size[0] +
			gap_pro * (dat->grid_size[0] + 1 ));
	dat->gap_fracs[0] = gap_pro * dat->box_fracs[0];

	// y lengths
	dat->box_fracs[1] = 2.0F / (dat->grid_size[1] +
			gap_pro * (dat->grid_size[1] + 1 ));
	dat->gap_fracs[1] = gap_pro * dat->box_fracs[1];

	return 0;
}

int increment_pos(float pos[2], float inc[2])
{
	static float orig_x;

	// branch predictor will make this of negligable overhead
	if(!orig_x)
		orig_x = pos[0];


	return 0;
}

int tile_boxes(struct display_data *dat)
{
	int i;
	float pos[2] = {-1, -1};
	float inc[2];

	pos[0] += dat->gap_fracs[0];
	pos[1] += dat->gap_fracs[1];

	inc[0] = dat->gap_fracs[0] + dat->box_fracs[0];
	inc[1] = dat->gap_fracs[1] + dat->box_fracs[1];

	glColor3f(0.0F, 1.0F, 0.0F);
	glBegin(GL_QUADS);
	for(i = 0; i < dat->grid_size[0] * dat->grid_size[1]; ++i){
		draw_box(pos, dat->box_fracs);

		if((pos[0] + inc[0]) < 1){
			pos[0] += inc[0];
		}else{
			pos[0] = -1 + dat->gap_fracs[0];
			pos[1] += inc[1];
		}
	}
	glEnd();

	return 0;
}

void display_callback(void *display_data)
{
	// clear gl buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// draw something
	/*
	glBegin(GL_QUADS);
	glColor3f(0.0F, 1.0F, 0.0F);
	glVertex2f(-0.5F, -0.5F);
	glVertex2f(0.5F, -0.5F);
	glVertex2f(0.5F, 0.5F);
	glVertex2f(-0.5F, 0.5F);
	glEnd();
	*/

	tile_boxes(display_data);

	// render gl buffer
	glFlush();
	glutSwapBuffers();

	puts("display callback");
}

// called when window is resized
void reshape_callback(int x, int y, void *user_data)
{
	glViewport(0, 0, x, y);
	compute_fracs(user_data, 0.1);

	puts("reshape callback");
}

int init_screen(struct display_data *dat)
{
	glutInit(&test_argc, &test_argv);
	glutCreateWindow("something_here");

	// default to standard definition
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// create window and GL context
	glutFullScreen();
	init_gl();

	// set glut callbacks
	glutDisplayFuncUcall(&display_callback, dat);
	glutReshapeFuncUcall(&reshape_callback, dat);

	return 0;
}

int update_screen(char *tiles, int dims[2])
{
	// processes a single interation
	glutMainLoopEvent();
	return 0;
}
