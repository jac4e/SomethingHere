#ifndef DRAW_SCREEN_H
#define DRAW_SCREEN_H

#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#include <GL/glut.h>

enum cell_types {
    CT_NOTHING,
    CT_AGENT,
    CT_WALL,
    CT_ENERGYCELL,
};

struct display_data {
    int grid_size[2];
    unsigned char *types;

    float box_fracs[2];
    float gap_fracs[2];

    GLsizei num_textures;
    int tex_res[2];
    GLuint *textures;
};

int init_dsp_data(struct display_data *dat, int dims[2]);
int init_screen(struct display_data *dat);
int update_screen(struct display_data *dat);

#ifdef __cplusplus
}
#endif
#endif  // DRAW_SCREEN_H
