#include <stdio.h>

#include "draw_screen.h"

int main(){
	int dims[2] = {20, 20};
	static char grid[20 * 20];
	struct display_data dat;

	// sets some defaults for screen state
	dat.grid_size[0] = dims[0];
	dat.grid_size[1] = dims[1];
	compute_fracs(&dat, 0.1);

	init_screen(&dat);

	while(1){
		update_screen(&grid[0], dims);
	}

	return 0;
}
