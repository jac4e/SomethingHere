#ifndef TICKCODE_H
#define TICKCODE_H

#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#include <time.h>

struct tick_vars
{
	double delta;
	struct timespec delta_timer;
	struct timespec fps_timer;
	int ticks;
	int frames;
	int debug;
};

int setup_clock(struct tick_vars *tick_vars_p);
int tick_delta(struct tick_vars *tick_vars_p);
int print_fps(struct tick_vars *tick_vars_p);
int timeval_subtract(struct timespec *result, 
		const struct timespec *x, 
		const struct timespec *y);

#ifdef __cplusplus
}
#endif
#endif // TICKCODE_H
