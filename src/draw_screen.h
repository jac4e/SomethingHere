#ifndef DRAW_SCREEN_H
#define DRAW_SCREEN_H

#ifdef __cplusplus
extern "C"{
#if 0
}
#endif
#endif

struct display_data
{
	int grid_size[2];
	unsigned char *types;

	float box_fracs[2];
	float gap_fracs[2];
};

int init_screen(struct display_data *dat);
int update_screen(struct display_data *dat);

#ifdef __cplusplus
}
#endif
#endif // DRAW_SCREEN_H
