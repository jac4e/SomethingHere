#include <stdio.h>

#include "ai/ai.h"
#include "renderingEngine/draw_screen.h"
#include "renderingEngine/textures.h"
#include "simulation/tick_code.h"

int main() {
    struct display_data dat;
    struct tick_vars tick;

    // testing
    int dims[2] = {50, 20};

    init_dsp_data(&dat, dims);

    init_screen(&dat);
    setup_clock(&tick);

    while (1) {
        // compute new cycle time delta
        tick_delta(&tick);

        while (tick.delta > 0) {
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
