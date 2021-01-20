#include "tick_code.h"

#include <time.h>

int setup_clock(struct tick_vars *tick_vars_p);
int tick_delta(struct tick_vars *tick_vars_p);
int print_fps(struct tick_vars *tick_vars_p);
int timeval_subtract(struct timespec *result, 
		const struct timespec *x, 
		const struct timespec *y);