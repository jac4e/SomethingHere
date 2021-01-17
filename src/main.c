#include <stdio.h>

#include "draw_screen.h"
#include "tick_code.h"

int main(){
	struct display_data dat;
	struct tick_vars tick;

	// testing
	int dims[2] = {50, 20};
	char grid[dims[0] * dims[1]];

	// sets some defaults for screen state required
	// can be hidden in some init_dsp_data() 
	dat.grid_size[0] = dims[0];
	dat.grid_size[1] = dims[1];
	dat.types = (unsigned char *)&grid[0];

	// testing
	grid[0] = 0;
	grid[1] = 1;
	grid[2] = 2;

	init_screen(&dat);
	setup_clock(&tick);

	while(1){
		// compute new cycle time delta
		tick_delta(&tick);

		while(tick.delta > 0){
			// do per-tick things
			--tick.delta;
			++tick.ticks;
		}

		// update screen as frequently as possible
		update_screen(&dat);
		print_fps(&tick);
		++tick.frames;
	}

	return 0;
}
