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
	
#ifdef __unix__
	if(clock_getres(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_getres");
		return -1;
	}
	printf("clock resolution:\nsec:\t%ld\nnsec:\t%ld\n", \
			sysclock.tv_sec, \
			sysclock.tv_nsec);
	// not bothering to port clock_gotres() to windows its unnessecary
	if(clock_gettime(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
#elif defined(_WIN32) || defined(WIN32)
	if(clock_gettime_windows(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
#endif
	
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
	
#ifdef __unix__
	if(clock_gettime(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
#elif defined(_WIN32) || defined(WIN32)
	if(clock_gettime_windows(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
#endif
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
#ifdef __unix__
	if(clock_gettime(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
#elif defined (_WIN32) || defined(WIN32)
	if(clock_gettime_windows(CLOCK_REALTIME, &sysclock) < 0){
		perror("clock_gettime");
		return -1;
	}
#endif
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

// i stole this code from stack overflow at 1122
// see: https://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows
#if defined(_WIN32) || defined (WIN32)
LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime_windows(int X, struct timeval *tv)
{
    LARGE_INTEGER           t;
    FILETIME            f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}
#endif
