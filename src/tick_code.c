#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "tick_code.h"

#define TPS 60

// initializes tick_vars struct
int setup_clock(struct tick_vars *tick_vars)
{
	struct timespec sysclock;
	
	if(clock_getres(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_getres");
		return -1;
	}
	printf("clock resolution:\nsec:\t%ld\nnsec:\t%ld\n", \
			sysclock.tv_sec, \
			sysclock.tv_nsec);
	if(clock_gettime(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
	
	tick_vars->delta_timer = sysclock;
	tick_vars->fps_timer = sysclock;
	tick_vars->frames = 0;
	tick_vars->ticks = 0;
	tick_vars->debug = 0;
	
	return 0;
}

// updates target TPS difference in delta as the number of ticks to run
int tick_delta(struct tick_vars *tick_vars)
{
	struct timespec sysclock;
	struct timespec diff;
	
	if(clock_gettime(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
	if(timeval_subtract(&diff, &sysclock, &tick_vars->delta_timer) == 1){
		perror("tick_delta: negative time elapsed");
		return -1;
	}
	if(diff.tv_sec > 0){
		perror("tick taking too long");
		exit(-1);
	}

	tick_vars->delta += diff.tv_nsec / (1.0E9 / TPS);
	tick_vars->delta_timer = sysclock;
	
	return 0;
}

// checks to see if 1s has passed and if so prints the
// number of frames that have been rendered in that time
// and resets the frame counter
int print_fps(struct tick_vars *tick_vars)
{
	struct timespec sysclock;
	struct timespec diff;

	if(clock_gettime(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
	if(timeval_subtract(&diff, &sysclock, &tick_vars->fps_timer)){
		perror("print_fps: negative time elapsed");
		return -1;
	}
	if(diff.tv_sec > 0){
		printf("FPS:\t%d\n", tick_vars->frames);
		printf("TPS:\t%d\n", tick_vars->ticks);
		tick_vars->fps_timer = sysclock;
		tick_vars->frames = 0;
		tick_vars->ticks = 0;
	}
	return 0;
}

// from the GNU C Library manual, "Calculating Elapsed Time"
int timeval_subtract(struct timespec *result, 
		const struct timespec *x, 
		const struct timespec *y_orig)
{
	// local copy as code modifies y
	struct timespec y = *y_orig;
	// calculates any necessary carrys for the subtraction to work
	if(x->tv_nsec < y.tv_nsec){
		int num_sec = (y.tv_nsec - x->tv_nsec) / 1E9 + 1;
		y.tv_nsec -= 1E9 * num_sec;
		y.tv_sec += num_sec;
	}
	if(x->tv_nsec - y.tv_nsec > 1E9){
		int num_sec = (x->tv_nsec - y.tv_nsec) / 1E9;
		y.tv_nsec += 1E9 * num_sec;
		y.tv_sec -= num_sec;
	}
	result->tv_sec = x->tv_sec - y.tv_sec;
	result->tv_nsec = x->tv_nsec - y.tv_nsec;
	// fixes dumb diff-returns like (-1s):(1.01E9ns)
	while(result->tv_nsec >= 1E9){
		result->tv_nsec -= 1E9;
		++result->tv_sec;
	}
	// return 1 if the result is negative
	return x->tv_sec < y.tv_sec;
}
