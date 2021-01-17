#include <stdio.h>

#include "draw_screen.h"

int main(){
	int dims[2] = {20,20};
	//initialize
	//run
	init_screen(dims);
	while(1)
		update_screen(NULL, NULL);
	//cleanup
	return 0;
}
