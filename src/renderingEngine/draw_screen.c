#include "draw_screen.h"

#include <GL/freeglut_ext.h>
#include <GL/glut.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "textures.h"

// when visual C allows compilation of strings longer than
// size_t = UINT16_MAX, **as gcc does**, then images can be
// compiled on vcc/windows
//
// defaulting to colored boxes for all platforms as a quick fix
// until we can implement a (better) image loading system
#define IMAGES_FIXED 0

static char *test_argv = "test_name";
static int test_argc = 1;

static int color_palette[5] = {
    0x073642,
    0xB58900,
    0xDC322F,
    0x859900,
};

int init_gl(struct display_data *dat) {
    int i;

    GLuint *textures;
    GLsizei n;

    int tex_res[2] = {1024, 1024};
    const void **texture_data;

    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

    return 0;
}

int compute_fracs(struct display_data *dat, float gap_pro) {
    // x box and gap gl lengths
    dat->box_fracs[0] = 2.0F / (dat->grid_size[0] +
                                gap_pro * (dat->grid_size[0] + 1));
    dat->gap_fracs[0] = gap_pro * dat->box_fracs[0];

    // y lengths
    dat->box_fracs[1] = 2.0F / (dat->grid_size[1] +
                                gap_pro * (dat->grid_size[1] + 1));
    dat->gap_fracs[1] = gap_pro * dat->box_fracs[1];

    return 0;
}

// does no texture loading init_gl() does texture loading
int init_dsp_data(struct display_data *dat, int dims[2]) {
    dat->grid_size[0] = dims[0];
    dat->grid_size[1] = dims[1];

    dat->types = malloc(sizeof *dat->types * (dims[0] * dims[1]));

    compute_fracs(dat, 0.1);

    return 0;
}

// set color before calling, so repeated calls
// dont require repeated glColor calls
// must be called with an active glBegin(GL_QUADS);
int draw_box(float pos[2], float side_len[2]) {
    glVertex2f(pos[0], pos[1]);
    glVertex2f(pos[0] + side_len[0], pos[1]);
    glVertex2f(pos[0] + side_len[0], pos[1] + side_len[1]);
    glVertex2f(pos[0], pos[1] + side_len[1]);

    return 0;
}

int draw_texture(float pos[2], float side_len[2], GLuint texture) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glVertex2f(pos[0], pos[1]);
    glVertex2f(pos[0] + side_len[0], pos[1]);
    glVertex2f(pos[0] + side_len[0], pos[1] + side_len[1]);
    glVertex2f(pos[0], pos[1] + side_len[1]);

    return 0;
}

int set_color(struct display_data *dat, int x, int y) {
    float r, g, b;
    unsigned char type;

    type = dat->types[y * dat->grid_size[0] + x];

    switch (type) {
        case CT_NOTHING:
            r = (unsigned char)(color_palette[0] >> 16) / (float)UCHAR_MAX;
            g = (unsigned char)(color_palette[0] >> 8) / (float)UCHAR_MAX;
            b = (unsigned char)(color_palette[0] >> 0) / (float)UCHAR_MAX;
            glColor3f(r, g, b);
            break;
        case CT_AGENT:
            r = (unsigned char)(color_palette[1] >> 16) / (float)UCHAR_MAX;
            g = (unsigned char)(color_palette[1] >> 8) / (float)UCHAR_MAX;
            b = (unsigned char)(color_palette[1] >> 0) / (float)UCHAR_MAX;
            glColor3f(r, g, b);
            break;
        case CT_WALL:
            r = (unsigned char)(color_palette[2] >> 16) / (float)UCHAR_MAX;
            g = (unsigned char)(color_palette[2] >> 8) / (float)UCHAR_MAX;
            b = (unsigned char)(color_palette[2] >> 0) / (float)UCHAR_MAX;
            glColor3f(r, g, b);
            break;
        default:
            r = (unsigned char)(color_palette[3] >> 16) / (float)UCHAR_MAX;
            g = (unsigned char)(color_palette[3] >> 8) / (float)UCHAR_MAX;
            b = (unsigned char)(color_palette[3] >> 0) / (float)UCHAR_MAX;

            r *= (float)type / UCHAR_MAX;
            g *= (float)type / UCHAR_MAX;
            b *= (float)type / UCHAR_MAX;

            glColor3f(r, g, b);
            break;
    }

    return 0;
}

int tile_boxes(struct display_data *dat) {
    int i, j;
    float pos[2] = {-1, -1};
    float inc[2];

    pos[0] += dat->gap_fracs[0];
    pos[1] += dat->gap_fracs[1];

    inc[0] = dat->gap_fracs[0] + dat->box_fracs[0];
    inc[1] = dat->gap_fracs[1] + dat->box_fracs[1];

    glBegin(GL_QUADS);
    for (i = 0; i < dat->grid_size[1]; ++i) {
        for (j = 0; j < dat->grid_size[0]; ++j) {
            set_color(dat, i, j);
            switch (dat->types[i * dat->grid_size[0] + j]) {
#ifdef IMAGES_FIXED
                case CT_NOTHING:
                    draw_box(pos, dat->box_fracs);
                    break;
                case CT_AGENT:
                    draw_texture(pos, dat->box_fracs, CT_AGENT);
                    break;
                case CT_WALL:
                    draw_texture(pos, dat->box_fracs, CT_WALL);
                    // pos resizing can be done here so fills
                    // entire cell
                    break;
                default:
                    // assume energy cell
                    draw_texture(pos, dat->box_fracs,
                                 CT_ENERGYCELL);
            }
#else
                draw_box(pos, dat->box_fracs);
#endif
            pos[0] += inc[0];
        }
        pos[1] += inc[1];
        pos[0] = -1 + dat->gap_fracs[0];
    }
    glEnd();

    return 0;
}

void display_callback(void *display_data) {
    // clear gl buffer
    glClear(GL_COLOR_BUFFER_BIT);

    tile_boxes(display_data);

    // render gl buffer
    glFlush();
    glutSwapBuffers();
}

// called when window is resized
void reshape_callback(int x, int y, void *user_data) {
    glViewport(0, 0, x, y);
    // p sure these never need to be recomputed they're relative
    //compute_fracs(user_data, 0.1);
}

int init_screen(struct display_data *dat) {
    glutInit(&test_argc, &test_argv);
    glutCreateWindow("something_here");

    // default to standard definition
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    // create window and GL context
    glutFullScreen();
    init_gl(dat);

    // set glut callbacks
    glutDisplayFuncUcall(&display_callback, dat);
    glutReshapeFuncUcall(&reshape_callback, dat);

    // initial fracs
    compute_fracs(dat, 0.1);

    return 0;
}

int update_screen(struct display_data *dat) {
    glClear(GL_COLOR_BUFFER_BIT);

    tile_boxes(dat);

    glFlush();
    glutSwapBuffers();

    // processes a single interation
    // nesessary to call resize callback
    glutMainLoopEvent();

    return 0;
}
