#ifndef DRAW_SCREEN_H
#define DRAW_SCREEN_H

#ifdef __cplusplus
extern "C"{
#if 0
}
#endif
#endif

int init_screen(int dims[2]);
int update_screen(char **tiles, int dims[2]);

#ifdef __cplusplus
}
#endif

#endif // DRAW_SCREEN_H
